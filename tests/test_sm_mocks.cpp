#include "test_sm_mocks.hpp"

StateCallbackMocks mocks;

void s1_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data) {
	mocks.s1_entry_action(sm_user_data, current_state, current_state_data,
						  event, next_state, next_state_data);
}
void s2_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data) {
	mocks.s2_entry_action(sm_user_data, current_state, current_state_data,
						  event, next_state, next_state_data);
}
void s3_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data) {
	mocks.s3_entry_action(sm_user_data, current_state, current_state_data,
						  event, next_state, next_state_data);
}
void s4_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data) {
	mocks.s4_entry_action(sm_user_data, current_state, current_state_data,
						  event, next_state, next_state_data);
}
void s_error_entry_action(void *sm_user_data,
						  const struct sm_state *current_state,
						  void *current_state_data,
						  const struct sm_event *event,
						  const struct sm_state *next_state,
						  void *next_state_data) {
	mocks.s_error_entry_action(sm_user_data, current_state, current_state_data,
							   event, next_state, next_state_data);
}
void s1_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data) {
	mocks.s1_exit_action(sm_user_data, current_state, current_state_data, event,
						 next_state, next_state_data);
}
void s2_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data) {
	mocks.s2_exit_action(sm_user_data, current_state, current_state_data, event,
						 next_state, next_state_data);
}
void s3_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data) {
	mocks.s3_exit_action(sm_user_data, current_state, current_state_data, event,
						 next_state, next_state_data);
}
void s4_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data) {
	mocks.s4_exit_action(sm_user_data, current_state, current_state_data, event,
						 next_state, next_state_data);
}
bool guard1(void *sm_user_data, void *condition, const struct sm_event *event) {
	return mocks.guard1(sm_user_data, condition, event);
}
bool guard2(void *sm_user_data, void *condition, const struct sm_event *event) {
	return mocks.guard2(sm_user_data, condition, event);
}
bool guard3(void *sm_user_data, void *condition, const struct sm_event *event) {
	return mocks.guard3(sm_user_data, condition, event);
}
bool guard4(void *sm_user_data, void *condition, const struct sm_event *event) {
	return mocks.guard4(sm_user_data, condition, event);
}

void trans_action1(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data) {
	mocks.trans_action1(sm_user_data, current_state, current_state_data, event,
						next_state, next_state_data);
}
void trans_action2(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data) {
	mocks.trans_action2(sm_user_data, current_state, current_state_data, event,
						next_state, next_state_data);
}
void trans_action3(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data) {
	mocks.trans_action3(sm_user_data, current_state, current_state_data, event,
						next_state, next_state_data);
}
void trans_action4(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data) {
	mocks.trans_action4(sm_user_data, current_state, current_state_data, event,
						next_state, next_state_data);
}
