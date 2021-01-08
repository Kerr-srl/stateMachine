/*
 * Copyright (c) 2013 Andreas Misje
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "sm_state_machine.h"

#include <assert.h>

static void go_to_error_state(struct sm_state_machine *sm_handle,
							  const struct sm_event *const event);
static struct sm_transition *
get_transition(const struct sm_state_machine *sm_handle,
			   const struct sm_state *state,
			   const struct sm_event *const event);
static void *get_state_data(const struct sm_state_machine *sm_handle,
							const struct sm_state *state);

void sm_state_machine_init(struct sm_state_machine *sm_handle, const char *name,
						   const struct sm_state *initial_state,
						   const struct sm_state *error_state,
						   struct sm_state_machine_hooks *hooks,
						   void *user_data, void *state_data) {
	assert(initial_state != NULL);
	assert(error_state != NULL);
	assert(sm_handle != NULL);
	assert(hooks != NULL);

#if SM_STATE_MACHINE_ENABLE_LOG
	sm_handle->name = name;
#endif

	sm_handle->current_state = initial_state;
	sm_handle->previous_state = NULL;
	sm_handle->error_state = error_state;
	sm_handle->hooks = *hooks;
	sm_handle->user_data = user_data;
	sm_handle->state_data = state_data;
}

int sm_state_machine_handle_event(struct sm_state_machine *sm_handle,
								  const struct sm_event *event) {
	if (!sm_handle || !event)
		return stateM_errArg;

	if (!sm_handle->current_state) {
		go_to_error_state(sm_handle, event);
		return stateM_errorStateReached;
	}

	if (!sm_handle->current_state->transitions ||
		!sm_handle->current_state->transitions->num_transitions)
		return stateM_noStateChange;

	const struct sm_state *next_state = sm_handle->current_state;
	do {
		struct sm_transition *transition =
			get_transition(sm_handle, next_state, event);

		/* If there were no transitions for the given event for the current
		 * state, check if there are any transitions for any of the parent
		 * states (if any): */
		if (!transition) {
			next_state = next_state->parent_state;
			continue;
		}

		/* A transition must have a next state defined. If the user has not
		 * defined the next state, go to error state: */
		if (!transition->next_state) {
			go_to_error_state(sm_handle, event);
			return stateM_errorStateReached;
		}

		bool guard_rejected = false;
		if (transition->guard &&
			!transition->guard->fn(
				sm_handle->user_data, sm_handle->current_state,
				get_state_data(sm_handle, sm_handle->current_state), event,
				transition->next_state,
				get_state_data(sm_handle, transition->next_state))) {
			guard_rejected = true;
		}
#if SM_STATE_MACHINE_ENABLE_LOG
		if (sm_handle->hooks.logger &&
			sm_handle->hooks.logger->log_transition) {
			sm_handle->hooks.logger->log_transition(
				sm_handle, event, transition->guard, !guard_rejected,
				sm_handle->current_state, transition->action,
				transition->next_state);
		}
#endif
		if (guard_rejected) {
			break;
		}

		next_state = transition->next_state;

		/* If the new state is a parent state, enter its entry state (if it has
		 * one). Step down through the whole family tree until a state without
		 * an entry state is found: */
		while (next_state->entry_state)
			next_state = next_state->entry_state;

		/* Run exit action only if the current state is left (only if it does
		 * not return to itself): */
		if (next_state != sm_handle->current_state &&
			sm_handle->current_state->exit_action)
			sm_handle->current_state->exit_action->fn(
				sm_handle->user_data, sm_handle->current_state,
				get_state_data(sm_handle, sm_handle->current_state), event,
				next_state, get_state_data(sm_handle, next_state));

		/* Run transition action (if any): */
		if (transition->action) {
			assert(transition->action->fn);
			transition->action->fn(
				sm_handle->user_data, sm_handle->current_state,
				get_state_data(sm_handle, sm_handle->current_state), event,
				next_state, get_state_data(sm_handle, next_state));
		}

		/* Call the new state's entry action if it has any (only if state does
		 * not return to itself): */
		if (next_state != sm_handle->current_state &&
			next_state->entry_action) {
			assert(next_state->entry_action->fn);
			next_state->entry_action->fn(
				sm_handle->user_data, sm_handle->current_state,
				get_state_data(sm_handle, sm_handle->current_state), event,
				next_state, get_state_data(sm_handle, next_state));
		}

		sm_handle->previous_state = sm_handle->current_state;
		sm_handle->current_state = next_state;

		/* If the state returned to itself: */
		if (sm_handle->current_state == sm_handle->previous_state)
			return stateM_stateLoopSelf;

		if (sm_handle->current_state == sm_handle->error_state)
			return stateM_errorStateReached;

		/* If the new state is a final state, notify user that the state
		 * machine has stopped: */
		if (sm_state_machine_stopped(sm_handle))
			return stateM_finalStateReached;

		return stateM_stateChanged;
	} while (next_state);

	return stateM_noStateChange;
}

const struct sm_state *
sm_state_machine_current_state(const struct sm_state_machine *sm_handle) {
	if (!sm_handle)
		return NULL;

	return sm_handle->current_state;
}

const struct sm_state *
sm_state_machine_previous_state(const struct sm_state_machine *sm_handle) {
	if (!sm_handle)
		return NULL;

	return sm_handle->previous_state;
}

static void go_to_error_state(struct sm_state_machine *sm_handle,
							  const struct sm_event *const event) {
	sm_handle->previous_state = sm_handle->current_state;
	sm_handle->current_state = sm_handle->error_state;

	if (sm_handle->current_state && sm_handle->current_state->entry_action) {
		sm_handle->current_state->entry_action->fn(
			sm_handle->user_data, sm_handle->previous_state,
			get_state_data(sm_handle, sm_handle->previous_state), event,
			sm_handle->current_state,
			get_state_data(sm_handle, sm_handle->current_state));
	}
}

static struct sm_transition *
get_transition(const struct sm_state_machine *sm_handle,
			   const struct sm_state *state,
			   const struct sm_event *const event) {
	if (state->transitions == NULL) {
		return NULL;
	}
	for (size_t i = 0; i < state->transitions->num_transitions; ++i) {
		struct sm_transition *t = &state->transitions->transitions[i];

		/* A transition for the given event has been found: */
		if (t->event_type == event->type) {
			return t;
		}
	}

	/* No transitions found for given event for given state: */
	return NULL;
}

bool sm_state_machine_stopped(struct sm_state_machine *sm_handle) {
	if (!sm_handle)
		return true;

	return !sm_handle->current_state->transitions ||
		   !sm_handle->current_state->transitions->num_transitions;
}

#if SM_STATE_MACHINE_ENABLE_LOG
const char *
sm_state_machine_get_name(const struct sm_state_machine *sm_handle) {
	assert(sm_handle != NULL);
	return sm_handle->name;
}
#endif

static void *get_state_data(const struct sm_state_machine *sm_handle,
							const struct sm_state *state) {
	if (sm_handle->hooks.state_data_mapper) {
		return sm_handle->hooks.state_data_mapper(state, sm_handle->state_data);
	}
	return NULL;
}
