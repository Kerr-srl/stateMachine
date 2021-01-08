
#ifndef SM_TEST_BASIC_SM_H_
#define SM_TEST_BASIC_SM_H_

#include "sm_state_machine.h"

#ifdef __cplusplus
extern "C" {
#endif

struct test_sm_state_data {
	struct test_sm_state_data_s1 {
		int a;
	} s1;
	struct test_sm_state_data_s2 {
		int b;
	} s2;
	struct test_sm_state_data_s3 {
		int c;
	} s3;
	struct test_sm_state_data_s4 {
		int d;
	} s4;
};

extern struct sm_state s1;
extern struct sm_state s2;
extern struct sm_state s3;
extern struct sm_state s4;
extern struct sm_state s_error;

enum sm_public_event {
	event_s1_to_s2,
	event_s2_to_s3,
	event_s3_to_s4,
	event_s4_to_s5,
};

void *test_sm_state_data_mapper(const struct sm_state *state,
								void *state_user_data);

void s_error_entry_action(void *sm_user_data,
						  const struct sm_state *current_state,
						  void *current_state_data,
						  const struct sm_event *event,
						  const struct sm_state *next_state,
						  void *next_state_data);
void s1_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data);
void s2_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data);
void s3_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data);
void s4_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data);
void s1_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data);
void s2_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data);
void s3_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data);
void s4_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data);
bool guard1(void *sm_user_data, void *condition, const struct sm_event *event);
bool guard2(void *sm_user_data, void *condition, const struct sm_event *event);
bool guard3(void *sm_user_data, void *condition, const struct sm_event *event);
bool guard4(void *sm_user_data, void *condition, const struct sm_event *event);

void trans_action1(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data);
void trans_action2(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data);
void trans_action3(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data);
void trans_action4(void *sm_user_data, const struct sm_state *current_state,
				   void *current_state_data, const struct sm_event *event,
				   const struct sm_state *next_state, void *next_state_data);

#ifdef __cplusplus
}
#endif

#endif /* ifndef SM_TEST_BASIC_SM_H_ */
