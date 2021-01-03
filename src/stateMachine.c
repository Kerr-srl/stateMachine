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

#include "stateMachine.h"

#include <assert.h>

static void goToErrorState(struct stateMachine *stateMachine,
						   struct event *const event);
static struct transition *getTransition(struct stateMachine *stateMachine,
										struct state *state,
										struct event *const event);

void stateM_init(struct stateMachine *fsm, struct state *initialState,
				 struct state *errorState, struct state_machine_hooks *hooks,
				 void *user_data) {
	assert(initialState != NULL);
	assert(errorState != NULL);
	assert(fsm != NULL);
	assert(hooks != NULL);

	fsm->currentState = initialState;
	fsm->previousState = NULL;
	fsm->errorState = errorState;
	fsm->hooks = *hooks;
	fsm->user_data = user_data;
}

int stateM_handleEvent(struct stateMachine *fsm, struct event *event) {
	if (!fsm || !event)
		return stateM_errArg;

	if (!fsm->currentState) {
		goToErrorState(fsm, event);
		return stateM_errorStateReached;
	}

	if (!fsm->currentState->numTransitions)
		return stateM_noStateChange;

	struct state *nextState = fsm->currentState;
	do {
		struct transition *transition = getTransition(fsm, nextState, event);

		/* If there were no transitions for the given event for the current
		 * state, check if there are any transitions for any of the parent
		 * states (if any): */
		if (!transition) {
			nextState = nextState->parentState;
			continue;
		}

		/* A transition must have a next state defined. If the user has not
		 * defined the next state, go to error state: */
		if (!transition->nextState) {
			goToErrorState(fsm, event);
			return stateM_errorStateReached;
		}

		bool guard_rejected = false;
		if (transition->guard &&
			!transition->guard->fn(fsm->user_data, transition->condition,
								   event)) {
			guard_rejected = true;
		}
#if SM_STATE_MACHINE_ENABLE_LOG
		if (fsm->hooks.logger && fsm->hooks.logger->log_transition) {
			fsm->hooks.logger->log_transition(
				fsm, event, transition->guard, !guard_rejected,
				fsm->currentState, transition->action, transition->nextState);
		}
#endif
		if (guard_rejected) {
			continue;
		}

		nextState = transition->nextState;

		/* If the new state is a parent state, enter its entry state (if it has
		 * one). Step down through the whole family tree until a state without
		 * an entry state is found: */
		while (nextState->entryState)
			nextState = nextState->entryState;

		/* Run exit action only if the current state is left (only if it does
		 * not return to itself): */
		if (nextState != fsm->currentState && fsm->currentState->exit_action)
			fsm->currentState->exit_action->fn(fsm->user_data,
											   fsm->currentState->data, event,
											   nextState->data);

		/* Run transition action (if any): */
		if (transition->action) {
			assert(transition->action->fn);
			transition->action->fn(fsm->user_data, fsm->currentState->data,
								   event, nextState->data);
		}

		/* Call the new state's entry action if it has any (only if state does
		 * not return to itself): */
		if (nextState != fsm->currentState && nextState->entry_action) {
			assert(nextState->entry_action->fn);
			nextState->entry_action->fn(fsm->user_data, fsm->currentState->data,
										event, nextState->data);
		}

		fsm->previousState = fsm->currentState;
		fsm->currentState = nextState;

		/* If the state returned to itself: */
		if (fsm->currentState == fsm->previousState)
			return stateM_stateLoopSelf;

		if (fsm->currentState == fsm->errorState)
			return stateM_errorStateReached;

		/* If the new state is a final state, notify user that the state
		 * machine has stopped: */
		if (!fsm->currentState->numTransitions)
			return stateM_finalStateReached;

		return stateM_stateChanged;
	} while (nextState);

	return stateM_noStateChange;
}

struct state *stateM_currentState(struct stateMachine *fsm) {
	if (!fsm)
		return NULL;

	return fsm->currentState;
}

struct state *stateM_previousState(struct stateMachine *fsm) {
	if (!fsm)
		return NULL;

	return fsm->previousState;
}

static void goToErrorState(struct stateMachine *fsm,
						   struct event *const event) {
	fsm->previousState = fsm->currentState;
	fsm->currentState = fsm->errorState;

	if (fsm->currentState && fsm->currentState->entry_action) {
		fsm->currentState->entry_action->fn(fsm->user_data,
											fsm->previousState->data, event,
											fsm->currentState->data);
	}
}

static struct transition *getTransition(struct stateMachine *fsm,
										struct state *state,
										struct event *const event) {
	size_t i;

	for (i = 0; i < state->numTransitions; ++i) {
		struct transition *t = &state->transitions[i];

		/* A transition for the given event has been found: */
		if (t->eventType == event->type) {
			return t;
		}
	}

	/* No transitions found for given event for given state: */
	return NULL;
}

bool stateM_stopped(struct stateMachine *stateMachine) {
	if (!stateMachine)
		return true;

	return stateMachine->currentState->numTransitions == 0;
}
