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
/**
 * \verbatim
 *                              _  __
 *                             | |/ /
 *                             | ' / ___ _ __ _ __
 *                             |  < / _ \ '__| '__|
 *                             | . \  __/ |  | |
 *                             |_|\_\___|_|  |_|
 * \endverbatim
 * \file		sm_state_machine.h
 *
 * \brief		state machine - interface
 *
 * \copyright	Copyright 2021 Kerr s.r.l. - All Rights Reserved.
 */

/**
 * \mainpage %sm_state_machine
 *
 * %sm_state_machine is a feature-rich, yet simple finite state machine
 * implementation. It supports grouped states, guarded transitions, events
 * with payload, entry and exit actions, transition actions and access to
 * user-defined state data from all actions.
 *
 * The user must build the state machine by linking together states and
 * transitions arrays with pointers. A pointer to an initial state and an
 * error state is given to sm_state_machine_init() to initialise a state machine
 * object. The state machine is run by passing events to it with the function
 * sm_state_machine_handle_event(). The return value of
 * sm_state_machine_handle_event() will give an indication to what has happened.
 *
 * \image html stateMachine.svg "Illustrating a stateMachine"
 */

/**
 * \defgroup sm_state_machine State machine
 *
 * \author Andreas Misje
 * \date 27.03.13
 *
 * \author Lijun Chen
 * \date 2021-01-01
 *
 * \brief Hierarchical State Machine
 *
 * A finite state machine implementation that supports nested states, guards
 * and entry/exit routines. All state machine data is stored in separate
 * objects, and the state machine must be built by the user. States are
 * connected using pointers, and all data can be stored on either the stack,
 * heap or both.
 */

/**
 * \addtogroup sm_state_machine
 * @{
 *
 * \file
 * \example stateMachineExample.c Simple example of how to create a state
 * machine
 * \example nestedTest.c Simple example testing the behaviour of nested
 * parent states
 */

#ifndef SM_STATE_MACHINE_H_
#define SM_STATE_MACHINE_H_

#include "sm_state_machine_config.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sm_state;
struct sm_state_machine;

/**
 * \brief Event
 *
 * Events trigger transitions from a state to another. Event types are defined
 * by the user. Any event may optionally contain a \ref #event::data
 * "payload".
 *
 * \sa sm_state
 * \sa sm_transition
 */
struct sm_event {
	/**
	 * \brief Type of event. Defined by user.
	 */
	int type;
	/**
	 * \brief Event payload.
	 *
	 * How this is used is entirely up to the user. This data
	 * is always passed together with #type in order to make it possible to
	 * always cast the data correctly.
	 */
	void *data;
};

/**
 * \brief Check if data passed with event fulfils a condition
 *
 * A transition may be conditional. If so, this function, if non-NULL,
 * will be called. Its first argument will be supplied with #condition,
 * which can be compared against the \ref event::data "payload" in the
 * #event. The user may choose to use this argument or not. Only if the
 * result is true, the transition will take place.
 *
 * \param [in] sm_user_data the user data passed in #sm_state_machine_init
 * will be passed
 * \param event the event passed to the state machine.
 *
 * \returns true if the event's data fulfils the condition, otherwise
 * false.
 */
typedef bool (*sm_guard_fn)(void *sm_user_data,
							const struct sm_state *current_state,
							void *current_state_data,
							const struct sm_event *event,
							const struct sm_state *next_state,
							void *next_state_data);
/**
 * \brief Guard
 *
 * Guards check if a transition is allowed at the moment
 */
struct sm_guard {
#if SM_STATE_MACHINE_ENABLE_LOG
	/**
	 * \brief Name that can be used for logging and debugging
	 */
	const char *name;
#endif
	sm_guard_fn fn;
};

/**
 * \brief Utility macro that you can use to define a guard object, in order to
 * handle the conditional sm_guard::name field
 *
 * \param [in] _fn_ The function to be executed as guard
 *
 * The stringified name of the function will be used for the sm_guard::name
 * field
 */
#if SM_STATE_MACHINE_ENABLE_LOG
#define SM_STATE_MACHINE_GUARD(_fn_)                                           \
	(struct sm_guard) {                                                        \
		.name = #_fn_, .fn = _fn_                                              \
	}
#else
#define SM_STATE_MACHINE_GUARD(_fn_)                                           \
	(struct sm_guard) {                                                        \
		.fn = _fn_                                                             \
	}
#endif

/**
 * \brief Function containing tasks to be performed during the
 * transition.
 *
 * The function could represent:
 *
 * * a transition action
 * * an entry action
 * * an exit action
 *
 * \param [in] sm_user_data the user data passed in #sm_state_machine_init
 * \param [in] current_state
 * \parblock
 * Depending on the type of action this function is representing, this
 * parameter could have the following values and meanings:
 *
 * * transition action: previous state
 * * entry action: previous state
 * * exit action: current state
 * \endparblock
 * \param [in] current_state_data instance specific, state local data of \p
 * current_state (see \ref sm_state_machine_hooks::state_data_mapper)
 * \param [in] event the event passed to the state machine, due to which this
 * action is being executed.
 * \param [in] new_state
 * \parblock
 * Depending on the type of action this function is representing, this
 * parameter could have the following values and meanings:
 *
 * * transition action: next state
 * * entry action: current state
 * * exit action: next state
 * \endparblock
 * \param [in] current_state_data instance specific, state local data of \p
 * next_state (see \ref sm_state_machine_hooks::state_data_mapper)
 */
typedef void (*sm_action_fn)(void *sm_user_data,
							 const struct sm_state *current_state,
							 void *current_state_data,
							 const struct sm_event *event,
							 const struct sm_state *new_state,
							 void *new_state_data);
/**
 * \brief Action
 *
 * Functions that are executed on state change
 */
struct sm_action {
#if SM_STATE_MACHINE_ENABLE_LOG
	/**
	 * \brief Name that can be used for logging and debugging
	 */
	const char *name;
#endif
	/**
	 * \brief Function to be executed
	 */
	sm_action_fn fn;
};

/**
 * \brief Utility macro that you can use to define an action object, in order
 * to handle the conditional sm_guard::name field
 *
 * \param [in] _fn_ The function to be executed as action
 *
 * The stringified name of the function will be used for the sm_action::name
 * field
 */
#if SM_STATE_MACHINE_ENABLE_LOG
#define SM_STATE_MACHINE_ACTION(_fn_)                                          \
	(struct sm_action) {                                                       \
		.name = #_fn_, .fn = _fn_                                              \
	}
#else
#define SM_STATE_MACHINE_ACTION(_fn_)                                          \
	(struct sm_action) {                                                       \
		.fn = _fn_                                                             \
	}
#endif

/**
 * \brief Transition between a state and another state
 *
 * All states that are not final must have at least one transition. The
 * transition may be guarded or not. Transitions are triggered by events. If
 * a state has more than one transition with the same type of event (and the
 * same condition), the first transition in the array will be run. An
 * unconditional transition placed last in the transition array of a state can
 * act as a "catch-all". A transition may optionally run an #action.
 *
 * It is perfectly valid for a transition to return to the state it belongs to.
 * Such a transition will not call the state's \ref sm_state::entry_action
 * "entry action" or \ref sm_state::exit_action "exit action". If there are no
 * transitions for the current event, the state's parent will be handed the
 * event.
 *
 * \sa sm_event
 * \sa sm_state
 */
struct sm_transition {
	/**
	 * \brief The event that will trigger this transition.
	 */
	int event_type;
	/**
	 * \brief The guard to be executed. If it returns false, the transition is
	 * aborted.
	 *
	 * May be NULL.
	 */
	struct sm_guard *guard;
	/**
	 * \brief The action to be executed as a side effect of the transition.
	 * This is called only if #guard returns true.
	 *
	 * May be NULL.
	 */
	struct sm_action *action;
	/**
	 * \brief The next state
	 *
	 * This must point to the next state that will be entered. It cannot be
	 * NULL.
	 */
	struct sm_state *next_state;
};

/**
 * This struct represents the transitions that are defined for a single state.
 */
struct sm_state_transitions {
#if SM_STATE_MACHINE_OPTIMIZE_RAM
	/**
	 * If #SM_STATE_MACHINE_OPTIMIZE_RAM is enabled, the transitions defined
	 * with the \ref SM_STATE_MACHINE_TRANSITION_DEF_START
	 * "SM_STATE_MACHINE_TRANSITION_DEF_*" macros will be glued into single
	 * function.
	 *
	 * This is the pointer to that function.
	 */
	int (*handle_event)(struct sm_state_machine *state_machine,
						const struct sm_event *event);
#else
	/**
	 * \brief An array of transitions for the state.
	 */
	struct sm_transition *transitions;
	/**
	 * \brief Number of transitions in the #transitions array.
	 */
	size_t num_transitions;
#endif
};

#if SM_STATE_MACHINE_OPTIMIZE_RAM
int sm_state_machine_transition_def_helper_handle_event(
	struct sm_state_machine *sm_handle, const struct sm_event *event,
	sm_guard_fn guard, sm_action_fn transition_action,
	const struct sm_state *next_state);
int sm_state_machine_transition_def_helper_handle_event_ex(
	struct sm_state_machine *sm_handle, const struct sm_event *event,
	struct sm_guard *guard, struct sm_action *transition_action,
	const struct sm_state *next_state);
int sm_state_machine_transition_def_helper_parent_handle_event(
	struct sm_state_machine *sm_handle, const struct sm_event *event);
#define SM_STATE_MACHINE_TRANSITION_DEF_START(_state_name_)                    \
	int _state_name_##_transition_fn(struct sm_state_machine *sm_handle,       \
									 const struct sm_event *event) {
#define SM_STATE_MACHINE_TRANSITION_ADD(_event_, _guard_, _action_,            \
										_next_state_)                          \
	if (event->type == _event_) {                                              \
		return sm_state_machine_transition_def_helper_handle_event(            \
			sm_handle, event, _guard_, _action_, _next_state_);                \
	}

#define SM_STATE_MACHINE_TRANSITION_ADD_EX(_event_, _guard_, _action_,         \
										   _next_state_)                       \
	if (event->type == _event_) {                                              \
		return sm_state_machine_transition_def_helper_handle_event_ex(         \
			sm_handle, event, (_guard_), (_action_), _next_state_);            \
	}

#define SM_STATE_MACHINE_TRANSITION_DEF_END(_state_name_)                      \
	return sm_state_machine_transition_def_helper_parent_handle_event(         \
		sm_handle, event);                                                     \
	}                                                                          \
	struct sm_state_transitions _state_name_##_transition = {                  \
		.handle_event = _state_name_##_transition_fn,                          \
	};
#define SM_STATE_MACHINE_TRANSITION_GET(_state_name_) _state_name_##_transition
#else
/**
 * \brief Start the definition of transitions for a state
 */
#define SM_STATE_MACHINE_TRANSITION_DEF_START(_state_name_)                    \
	struct sm_transition _state_name_##_transition_array[] = {
/**                                                                            \
 * \brief Add a transition to a state using shorthand notation: i.e. you just
 * have to pass the functions as guard and action, instead of using
 * #SM_STATE_MACHINE_GUARD and #SM_STATE_MACHINE_ACTION
 *
 * \param [in] event event
 * \param [in] guard guard function (type #sm_action_fn)
 * \param [in] action action function (type #sm_action_fn)
 * \param [in] next_state next state (type #sm_state *)
 */
#define SM_STATE_MACHINE_TRANSITION_ADD(_event_, _guard_, _action_,            \
										_next_state_)                          \
	{_event_, _guard_ == NULL ? NULL : &SM_STATE_MACHINE_GUARD(_guard_),       \
	 _action_ == NULL ? NULL : &SM_STATE_MACHINE_ACTION(_action_),             \
	 _next_state_},

/**                                                                            \
 * \brief Add a transition to a state.
 *
 * \param [in] event event
 * \param [in] guard guard function (type #sm_action *)
 * \param [in] action action function (type #sm_action *)
 * \param [in] next_state next state (type #sm_state *)
 */
#define SM_STATE_MACHINE_TRANSITION_ADD_EX(_event_, _guard_, _action_,         \
										   _next_state_)                       \
	{_event_, _guard_, _action_, _next_state_},
/**
 * \brief End the transition definition of a state
 */
#define SM_STATE_MACHINE_TRANSITION_DEF_END(_state_name_)                      \
	}                                                                          \
	;                                                                          \
	struct sm_state_transitions _state_name_##_transition = {                  \
		.transitions = _state_name_##_transition_array,                        \
		.num_transitions = sizeof(_state_name_##_transition_array) /           \
						   sizeof(struct sm_transition),                       \
	};
/**
 * \brief Get the state transition defined with \ref
 * SM_STATE_MACHINE_TRANSITION_DEF_START "SM_STATE_MACHINE_TRANSITION_DEF_*"
 * macros
 */
#define SM_STATE_MACHINE_TRANSITION_GET(_state_name_) _state_name_##_transition
#endif

/**
 * \brief State
 *
 * The current state in a state machine moves to a new state when one of the
 * #transitions in the current state triggers on an event.
 *
 * An optional \ref #exit_action "exit action" is called when the state is
 * left, and an \ref #entryAction "entry action" is called when the state
 * machine enters a new state.
 *
 * If a state returns to itself, neither #exit_action nor #entry_action
 * will be called.
 *
 * An optional \ref sm_transition::action "transition action" is called in
 * either case.
 *
 * ### Hierarchical state machine ###
 *
 * States may be organised in a hierarchy by setting \ref #parent_state
 * "parent states". When a group/parent state is entered, the state machine is
 * redirected to the group state's \ref #entry_state "entry state" (if
 * non-NULL).
 * If an event does not trigger a transition in a state and if the
 * state has a parent state, the event will be passed to the parent state.
 * This behaviour is repeated for all parents. Thus all children of a state
 * have a set of common #transitions. A parent state's entry_action will not
 * be called if an event is passed on to a child state.
 *
 * \note If #entry_state is defined for a group state, the group state's
 * #entry_action will still be called. As one would expect, all the \ref
 * entry_action "entry actions" s along the parent child chain are called.
 *
 * \warning The state machine cannot detect cycles in parent chains and
 * children chains. If such cycles are present, stateM_handleEvent() will
 * never finish due to never-ending loops.
 *
 * ### Final state ###
 * A final state is a state that terminates the state machine. A state is
 * considered as a final state if its #numTransitions is 0:
 * ~~~{.c}
 * struct state finalState = {
 *    .transitions = NULL,
 *    .numTransitions = 0,
 * }
 * ~~~
 * The error state used by the state machine to indicate errors should be a
 * final state. Any calls to sm_state_machine_handle_event() when the current
 * state is a final state will return #stateM_noStateChange.
 *
 * \sa sm_event
 * \sa sm_transition
 */
struct sm_state {
#if SM_STATE_MACHINE_ENABLE_LOG
	/**
	 * \brief Name that can be used for logging and debugging
	 */
	const char *name;
#endif
	/**
	 * \brief If the state has a parent state, this pointer must be non-NULL.
	 */
	struct sm_state *parent_state;
	/**
	 * \brief If this state is a parent state, this pointer may point to a
	 * child state that serves as an entry point.
	 */
	struct sm_state *entry_state;
	/**
	 * \brief Transitions defined for the current state
	 */
	struct sm_state_transitions *transitions;
	/**
	 * \brief This action is executed whenever the state is being entered. May
	 * be NULL.
	 *
	 * \note If a state returns to itself through a transition (either directly
	 * or through a parent/group sate), its #entry_action will not be called.
	 *
	 * \note A group/parent state with its #entry_state defined will not have
	 * its #entry_action called.
	 */
	struct sm_action *entry_action;
	/**
	 * \brief This action is called whenever the state is being left. May be
	 * NULL.
	 *
	 * \note If a state returns to itself through a transition (either directly
	 * or through a parent/group sate), its #exit_action will not be called.
	 */
	struct sm_action *exit_action;
};

/**
 * \brief Utility macro that you can use to conditionally add the name field in
 * a #sm_state object
 *
 * \param [in] _state_name_ a token what will be stringified and used for the
 * sm_state::name field
 */
#if SM_STATE_MACHINE_ENABLE_LOG
#define SM_STATE_MACHINE_STATE_NAME(_state_name_) .name = #_state_name_
#else
#define SM_STATE_MACHINE_STATE_NAME(_state_name_) .parent_state = NULL
#endif

/**
 * \brief State machine hooks
 *
 * Set of hooks that the application should provide to this library
 */
struct sm_state_machine_hooks {
#if SM_STATE_MACHINE_ENABLE_LOG
	struct sm_state_machine_logger {
		void (*log_attempt_transition)(
			const struct sm_state_machine *state_machine,
			const char *state_machine_name, const struct sm_event *event,
			const char *event_name, const struct sm_guard *guard,
			const struct sm_state *current_state,
			const struct sm_action *transition_action,
			const struct sm_state *next_state);
	} * logger;
#endif
	const char *(*stringify_event)(const struct sm_event *event);
	/**
	 * In order to support multiple instances of same state machine
	 */
	void *(*state_data_mapper)(const struct sm_state *state,
							   void *state_user_data);
};

/**
 * \brief State machine
 *
 * Treat this struct as an opaque type. Don't manipulate the
 * members directly.
 */
struct sm_state_machine {
#if SM_STATE_MACHINE_ENABLE_LOG
	/**
	 * \brief Name that can be used for logging and debugging
	 */
	const char *name;
#endif
	/** \brief Pointer to the current state */
	const struct sm_state *current_state;
	/**
	 * \brief Pointer to previous state
	 *
	 * The previous state is stored for convenience in case the user needs to
	 * keep track of previous states.
	 */
	const struct sm_state *previous_state;
	/**
	 * \brief Pointer to a state that will be entered whenever an error occurs
	 * in the state machine.
	 *
	 * See #stateM_errorStateReached for when the state machine enters the
	 * error state.
	 */
	const struct sm_state *error_state;
	/**
	 * \brief Application hooks
	 */
	struct sm_state_machine_hooks hooks;
	/**
	 * \brief Data that will be available for each state::entryAction "entry
	 * action" and state::exit_action "exit action", and in any \ref
	 * transition::action "transition action"
	 */
	void *user_data;
	/**
	 * \brief See
	 */
	void *state_data;
};

/**
 * \brief Initialise the state machine
 *
 * This function initialises the supplied state_machine and sets the current
 * state to \pn{initialState}. No actions are performed until
 * stateM_handleEvent() is called. It is safe to call this function numerous
 * times, for instance in order to reset/restart the state machine if a final
 * state has been reached.
 *
 * \note The \ref #state::entryAction "entry action" for \pn{initialState}
 * will not be called.
 *
 * \note If \pn{initialState} is a parent state with its \ref
 * state::entry_state "entry_state" defined, it will not be entered. The user
 * must explicitly set the initial state.
 *
 * \param [in] state_machine the state machine to initialise.
 * \param [in] state_machine_name the name of the state machine. May be NULL.
 * \param [in] initial_state the initial state of the state machine.
 * \param [in] error_state pointer to a state that acts a final state and
 * notifies the system/user that an error has occurred.
 * \param [in] hooks struct of pointers to the state machine hooks
 * \param [in] user_data pointer to user defined data that will be passed in
 * every action and guard
 * \param [in] state_data pointer to user defined state data. Portion of the
 * data will be passed to every action and guard.
 */
void sm_state_machine_init(struct sm_state_machine *state_machine,
						   const char *state_machine_name,
						   const struct sm_state *initial_state,
						   const struct sm_state *error_state,
						   struct sm_state_machine_hooks *hooks,
						   void *user_data, void *state_data);

/**
 * \brief stateM_handleEvent() return values
 */
enum stateM_handleEventRetVals {
	/** \brief Erroneous arguments were passed */
	stateM_errArg = -2,
	/**
	 * \brief The error state was reached
	 *
	 * This value is returned either when the state machine enters the error
	 * state itself as a result of an error, or when the error state is the
	 * next state as a result of a successful transition.
	 *
	 * The state machine enters the state machine if any of the following
	 * happens:
	 * - The current state is NULL
	 * - A transition for the current event did not define the next state
	 */
	stateM_errorStateReached,
	/** \brief The current state changed into a non-final state */
	stateM_stateChanged,
	/**
	 * \brief The state changed back to itself
	 *
	 * The state can return to itself either directly or indirectly. An
	 * indirect path may inlude a transition from a parent state and the use of
	 * \ref state::entry_state "entry_states".
	 */
	stateM_stateLoopSelf,
	/**
	 * \brief The current state did not change on the given event
	 *
	 * If any event passed to the state machine should result in a state
	 * change, this return value should be considered as an error.
	 */
	stateM_noStateChange,
	/** \brief A final state (any but the error state) was reached */
	stateM_finalStateReached,
};

/**
 * \brief Utility macro that reduces the boilerplate code required for ref
 * sm_state_machine_hooks::state_data_mapper.
 */
#define SM_STATE_MACHINE_STATE_DATA_MAP_FN_DEF_START(                          \
	_map_fn_name_, _global_state_data_struct_name_)                            \
	void *_map_fn_name_(const struct sm_state *state, void *state_user_data) { \
		struct _global_state_data_struct_name_ *state_data =                   \
			(struct _global_state_data_struct_name_ *)state_user_data;         \
/**                                                                            \
 */
#define SM_STATE_MACHINE_STATE_DATA_MAP_FN_ADD(_state_name_)                   \
	if ((&(_state_name_)) == state)                                            \
		return &state_data->_state_name_;
/**                                                                            \
 */
#define SM_STATE_MACHINE_STATE_DATA_MAP_FN_ADD_EX(_state_name_, _field_)       \
	if ((&(_state_name_)) == state)                                            \
		return &state_data->_field_;
/**
 */
#define SM_STATE_MACHINE_STATE_DATA_MAP_FN_DEF_END()                           \
	return NULL;                                                               \
	}

/**
 * \brief Pass an event to the state machine
 *
 * The event will be passed to the current state, and possibly to the current
 * state's parent states (if any). If the event triggers a transition, a new
 * state will be entered. If the transition has an \ref transition::action
 * "action" defined, it will be called. If the transition is to a state other
 * than the current state, the current state's \ref state::exit_action
 * "exit action" is called (if defined). Likewise, if the state is a new
 * state, the new state's \ref state::entryAction "entry action" is called (if
 * defined).
 *
 * The returned value is negative if an error occurs.
 *
 * \param state_machine the state machine to pass an event to.
 * \param event the event to be handled.
 *
 * \return #stateM_handleEventRetVals
 */
int sm_state_machine_handle_event(struct sm_state_machine *state_machine,
								  const struct sm_event *event);

/**
 * \brief Get the current state
 *
 * \param state_machine the state machine to get the current state from.
 *
 * \retval a pointer to the current state.
 * \retval NULL if \pn{state_machine} is NULL.
 */
const struct sm_state *
sm_state_machine_current_state(const struct sm_state_machine *state_machine);

/**
 * \brief Get the previous state
 *
 * \param state_machine the state machine to get the previous state from.
 *
 * \retval the previous state.
 * \retval NULL if \pn{state_machine} is NULL.
 * \retval NULL if there has not yet been any transitions.
 */
const struct sm_state *
sm_state_machine_previous_state(const struct sm_state_machine *state_machine);

/**
 * \brief Check if the state machine has stopped
 *
 * \param state_machine the state machine to test.
 *
 * \retval true if the state machine has reached a final state.
 * \retval false if \pn{state_machine} is NULL or if the current state is not a
 * final state.
 */
bool sm_state_machine_stopped(struct sm_state_machine *state_machine);

#if SM_STATE_MACHINE_ENABLE_LOG
/**
 * \brief Return the name assigned to the state machine during initialization
 *
 * \param state_machine -
 *
 * \returns the name passed during initialization. May be NULL.
 */
const char *
sm_state_machine_get_name(const struct sm_state_machine *state_machine);
#endif

#ifdef __cplusplus
}
#endif

#endif // SM_STATE_MACHINE_H_

/**
 * @}
 */
