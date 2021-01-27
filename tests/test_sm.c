/**
 * \verbatim
 *                              _  __
 *                             | |/ /
 *                             | ' / ___ _ __ _ __
 *                             |  < / _ \ '__| '__|
 *                             | . \  __/ |  | |
 *                             |_|\_\___|_|  |_|
 * \endverbatim
 * \file		test_sm.c
 *
 * \brief		State machine used by unit tests - implementation
 *
 * \copyright	Copyright 2021 Kerr s.r.l. - All Rights Reserved.
 */
#include "test_sm.h"

static struct sm_action trans_action1_action =
	SM_STATE_MACHINE_ACTION(trans_action1);
SM_STATE_MACHINE_TRANSITION_DEF_START(s1)
SM_STATE_MACHINE_TRANSITION_ADD_EX(event_s1_to_s2,
								   &SM_STATE_MACHINE_GUARD(guard1),
								   &trans_action1_action, &s2)
SM_STATE_MACHINE_TRANSITION_ADD_EX(event_chain_s1_s2, NULL,
								   &trans_action1_action, &s2)
SM_STATE_MACHINE_TRANSITION_DEF_END(s1)
struct sm_state s1 = {
	SM_STATE_MACHINE_STATE_NAME(s1),
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions = &SM_STATE_MACHINE_TRANSITION_GET(s1),
	.entry_action = &SM_STATE_MACHINE_ACTION(s1_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s1_exit_action),
};

SM_STATE_MACHINE_TRANSITION_DEF_START(s2)
SM_STATE_MACHINE_TRANSITION_ADD(event_s2_to_s3, NULL, NULL, &s3)
SM_STATE_MACHINE_TRANSITION_ADD(event_chain_s1_s2, NULL, trans_action2, &s3)
SM_STATE_MACHINE_TRANSITION_DEF_END(s2)
struct sm_state s2 = {
	SM_STATE_MACHINE_STATE_NAME(s2),
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions = &SM_STATE_MACHINE_TRANSITION_GET(s2),
	.entry_action = &SM_STATE_MACHINE_ACTION(s2_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s2_exit_action),
};

SM_STATE_MACHINE_TRANSITION_DEF_START(s3)
SM_STATE_MACHINE_TRANSITION_ADD(event_s3_to_s4, NULL, NULL, &s4)
SM_STATE_MACHINE_TRANSITION_DEF_END(s3)
struct sm_state s3 = {
	SM_STATE_MACHINE_STATE_NAME(s3),
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions = &SM_STATE_MACHINE_TRANSITION_GET(s3),
	.entry_action = &SM_STATE_MACHINE_ACTION(s3_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s3_exit_action),
};

struct sm_state s4 = {
	SM_STATE_MACHINE_STATE_NAME(s4),
	.parent_state = NULL,
	.entry_state = NULL,
	.transitions = NULL,
	.entry_action = &SM_STATE_MACHINE_ACTION(s4_entry_action),
	.exit_action = &SM_STATE_MACHINE_ACTION(s4_exit_action),
};
struct sm_state s_error = {
	SM_STATE_MACHINE_STATE_NAME(s_error),
	.entry_action = &SM_STATE_MACHINE_ACTION(s_error_entry_action),
};

void *test_sm_state_data_mapper(const struct sm_state *state,
								void *state_user_data);

SM_STATE_MACHINE_STATE_DATA_MAP_FN_DEF_START(test_sm_state_data_mapper,
											 test_sm_state_data)
SM_STATE_MACHINE_STATE_DATA_MAP_FN_ADD(s1)
SM_STATE_MACHINE_STATE_DATA_MAP_FN_ADD(s2)
SM_STATE_MACHINE_STATE_DATA_MAP_FN_ADD(s3)
SM_STATE_MACHINE_STATE_DATA_MAP_FN_ADD(s4)
SM_STATE_MACHINE_STATE_DATA_MAP_FN_DEF_END()
