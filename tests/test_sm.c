#include "test_sm.h"

struct state s1 = {
	.parentState = NULL,
	.entryState = NULL,
	.transitions =
		(struct transition[]){
			{event_s1_to_s2, NULL, &SM_STATE_MACHINE_GUARD(guard1),
			 &SM_STATE_MACHINE_ACTION(trans_action1), &s2},
		},
	.numTransitions = 1,
	.entry_action = &SM_STATE_MACHINE_ACTION(s1_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s1_exit_action),
};
struct state s2 = {
	.parentState = NULL,
	.entryState = NULL,
	.transitions =
		(struct transition[]){
			{event_s2_to_s3, NULL, NULL, NULL, &s3},
		},
	.numTransitions = 1,
	.entry_action = &SM_STATE_MACHINE_ACTION(s2_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s2_exit_action),
};

struct state s3 = {
	.parentState = NULL,
	.entryState = NULL,
	.transitions =
		(struct transition[]){
			{event_s3_to_s4, NULL, NULL, NULL, &s4},
		},
	.numTransitions = 1,
	.entry_action = &SM_STATE_MACHINE_ACTION(s3_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s3_exit_action),
};

struct state s4 = {
	.parentState = NULL,
	.entryState = NULL,
	.transitions = NULL,
	.numTransitions = 0,
	.entry_action = &SM_STATE_MACHINE_ACTION(s4_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s4_exit_action),
};
struct state s_error = {
	.entry_action = &SM_STATE_MACHINE_ACTION(s_error_entry_action),
};
