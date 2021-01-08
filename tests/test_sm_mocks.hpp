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
	MAKE_MOCK6(s_error_exit_action,
			   void(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data));

	MAKE_MOCK3(guard1, bool(void *sm_user_data, void *condition,
							const struct sm_event *event));
	MAKE_MOCK3(guard2, bool(void *sm_user_data, void *condition,
							const struct sm_event *event));
	MAKE_MOCK3(guard3, bool(void *sm_user_data, void *condition,
							const struct sm_event *event));
	MAKE_MOCK3(guard4, bool(void *sm_user_data, void *condition,
							const struct sm_event *event));

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
