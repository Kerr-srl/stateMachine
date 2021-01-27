/**
 * \verbatim
 *                              _  __
 *                             | |/ /
 *                             | ' / ___ _ __ _ __
 *                             |  < / _ \ '__| '__|
 *                             | . \  __/ |  | |
 *                             |_|\_\___|_|  |_|
 * \endverbatim
 * \file		test_sm_mocks.hpp
 *
 * \brief		State machine callback mocks for unit tests - interface
 *
 * \copyright	Copyright 2021 Kerr s.r.l. - All Rights Reserved.
 */
#ifndef SM_TEST_SM_MOCKS_HPP_
#define SM_TEST_SM_MOCKS_HPP_

#include "test_sm.h"

#include "trompeloeil.hpp"

struct StateCallbackMocks {
	MAKE_MOCK6(s1_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s2_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s3_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s4_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s5_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s5_child_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s5_child_child_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s6_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s6_child_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s6_child_child_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s_error_entry_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s1_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s2_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s3_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s4_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s5_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s5_child_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s5_child_child_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s6_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s6_child_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s6_child_child_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(s_error_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));

	MAKE_MOCK6(guard1,
			   bool(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(guard2,
			   bool(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(guard3,
			   bool(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(guard4,
			   bool(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));

	MAKE_MOCK6(trans_action1,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(trans_action2,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(trans_action3,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
	MAKE_MOCK6(trans_action4,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));
};

extern StateCallbackMocks mocks;

#endif /* ifndef SM_TEST_SM_MOCKS_HPP_ */
