#include "test_sm.h"

struct sm_state s1 = {
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){
			{event_s1_to_s2, NULL, &SM_STATE_MACHINE_GUARD(guard1),
			 &SM_STATE_MACHINE_ACTION(trans_action1), &s2},
		},
	.num_transitions = 1,
	.entry_action = &SM_STATE_MACHINE_ACTION(s1_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s1_exit_action),
};
struct sm_state s2 = {
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){
			{event_s2_to_s3, NULL, NULL, NULL, &s3},
		},
	.num_transitions = 1,
	.entry_action = &SM_STATE_MACHINE_ACTION(s2_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s2_exit_action),
};

struct sm_state s3 = {
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){
			{event_s3_to_s4, NULL, NULL, NULL, &s4},
		},
	.num_transitions = 1,
	.entry_action = &SM_STATE_MACHINE_ACTION(s3_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s3_exit_action),
};

struct sm_state s4 = {
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions = NULL,
	.num_transitions = 0,
	.entry_action = &SM_STATE_MACHINE_ACTION(s4_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s4_exit_action),
};
struct sm_state s_error = {
	.entry_action = &SM_STATE_MACHINE_ACTION(s_error_entry_action),
};
