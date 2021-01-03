
#ifndef SM_TEST_BASIC_SM_H_
#define SM_TEST_BASIC_SM_H_

#include "stateMachine.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct state s1;
extern struct state s2;
extern struct state s3;
extern struct state s4;
extern struct state s_error;

enum sm_public_event {
	event_s1_to_s2,
	event_s2_to_s3,
	event_s3_to_s4,
	event_s4_to_s5,
};

void s_error_entry_action(void *sm_user_data, void *oldStateData,
						  struct event *event, void *newStateData);
void s1_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData);
void s2_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData);
void s3_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData);
void s4_entry_action(void *sm_user_data, void *oldStateData,
					 struct event *event, void *newStateData);
void s1_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData);
void s2_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData);
void s3_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData);
void s4_exit_action(void *sm_user_data, void *oldStateData, struct event *event,
					void *newStateData);
bool guard1(void *sm_user_data, void *condition, struct event *event);
bool guard2(void *sm_user_data, void *condition, struct event *event);
bool guard3(void *sm_user_data, void *condition, struct event *event);
bool guard4(void *sm_user_data, void *condition, struct event *event);

void trans_action1(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData);
void trans_action2(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData);
void trans_action3(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData);
void trans_action4(void *sm_user_data, void *oldStateData, struct event *event,
				   void *newStateData);

#ifdef __cplusplus
}
#endif

#endif /* ifndef SM_TEST_BASIC_SM_H_ */
