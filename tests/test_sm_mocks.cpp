#include "test_sm_mocks.hpp"

StateCallbackMocks mocks;

void s1_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData) {
	mocks.s1_entry_action(sm_user_data, oldStateData, event, newStateData);
}
void s2_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData) {
	mocks.s2_entry_action(sm_user_data, oldStateData, event, newStateData);
}
void s3_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData) {
	mocks.s3_entry_action(sm_user_data, oldStateData, event, newStateData);
}
void s4_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData) {
	mocks.s4_entry_action(sm_user_data, oldStateData, event, newStateData);
}
void s_error_entry_action(void *sm_user_data, void *oldStateData,
						  struct event *event, void *newStateData) {
	mocks.s_error_entry_action(sm_user_data, oldStateData, event, newStateData);
}
void s1_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData) {
	mocks.s1_exit_action(sm_user_data, oldStateData, event, newStateData);
}
void s2_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData) {
	mocks.s2_exit_action(sm_user_data, oldStateData, event, newStateData);
}
void s3_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData) {
	mocks.s3_exit_action(sm_user_data, oldStateData, event, newStateData);
}
void s4_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData) {
	mocks.s4_exit_action(sm_user_data, oldStateData, event, newStateData);
}
bool guard1(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard1(sm_user_data, condition, event);
}
bool guard2(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard2(sm_user_data, condition, event);
}
bool guard3(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard3(sm_user_data, condition, event);
}
bool guard4(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard4(sm_user_data, condition, event);
}

void trans_action1(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData) {
	mocks.trans_action1(sm_user_data, oldStateData, event, newStateData);
}
void trans_action2(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData) {
	mocks.trans_action2(sm_user_data, oldStateData, event, newStateData);
}
void trans_action3(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData) {
	mocks.trans_action3(sm_user_data, oldStateData, event, newStateData);
}
void trans_action4(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData) {
	mocks.trans_action4(sm_user_data, oldStateData, event, newStateData);
}
