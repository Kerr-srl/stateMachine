/**
 * \verbatim
 *                              _  __
 *                             | |/ /
 *                             | ' / ___ _ __ _ __
 *                             |  < / _ \ '__| '__|
 *                             | . \  __/ |  | |
 *                             |_|\_\___|_|  |_|
 * \endverbatim
 * \file		test_sm.h
 *
 * \brief		State machine used by unit tests - interface
 *
 * \copyright	Copyright 2020 Kerr s.r.l. - All Rights Reserved.
 */
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
	struct test_sm_state_data_s5 {
		int e;
	} s5;
	struct test_sm_state_data_s5_child {
		int e;
	} s5_child;
};

extern struct sm_state s1;
extern struct sm_state s2;
extern struct sm_state s3;
extern struct sm_state s4;
extern struct sm_state s5;
extern struct sm_state s5_child;
extern struct sm_state s5_child_child;
extern struct sm_state s6;
extern struct sm_state s6_child;
extern struct sm_state s6_child_child;
extern struct sm_state s_error;

enum sm_public_event {
	event_s1_to_s2,
	event_s1_to_s5,
	event_s1_to_s_guard,
	event_s2_to_s3,
	event_s3_to_s4,
	event_s5_child_child_to_s6_child_child,
	event_chain_s1_s2,
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
void s5_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data);
void s5_child_entry_action(void *sm_user_data,
						   const struct sm_state *current_state,
						   void *current_state_data,
						   const struct sm_event *event,
						   const struct sm_state *next_state,
						   void *next_state_data);
void s5_child_child_entry_action(void *sm_user_data,
								 const struct sm_state *current_state,
								 void *current_state_data,
								 const struct sm_event *event,
								 const struct sm_state *next_state,
								 void *next_state_data);
void s6_entry_action(void *sm_user_data, const struct sm_state *current_state,
					 void *current_state_data, const struct sm_event *event,
					 const struct sm_state *next_state, void *next_state_data);
void s6_child_entry_action(void *sm_user_data,
						   const struct sm_state *current_state,
						   void *current_state_data,
						   const struct sm_event *event,
						   const struct sm_state *next_state,
						   void *next_state_data);
void s6_child_child_entry_action(void *sm_user_data,
								 const struct sm_state *current_state,
								 void *current_state_data,
								 const struct sm_event *event,
								 const struct sm_state *next_state,
								 void *next_state_data);
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
void s5_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data);
void s5_child_exit_action(void *sm_user_data,
						  const struct sm_state *current_state,
						  void *current_state_data,
						  const struct sm_event *event,
						  const struct sm_state *next_state,
						  void *next_state_data);
void s5_child_child_exit_action(void *sm_user_data,
								const struct sm_state *current_state,
								void *current_state_data,
								const struct sm_event *event,
								const struct sm_state *next_state,
								void *next_state_data);
void s6_exit_action(void *sm_user_data, const struct sm_state *current_state,
					void *current_state_data, const struct sm_event *event,
					const struct sm_state *next_state, void *next_state_data);
void s6_child_exit_action(void *sm_user_data,
						  const struct sm_state *current_state,
						  void *current_state_data,
						  const struct sm_event *event,
						  const struct sm_state *next_state,
						  void *next_state_data);
void s6_child_child_exit_action(void *sm_user_data,
								const struct sm_state *current_state,
								void *current_state_data,
								const struct sm_event *event,
								const struct sm_state *next_state,
								void *next_state_data);
bool guard1(void *sm_user_data, const struct sm_state *current_state,
			void *current_state_data, const struct sm_event *event,
			const struct sm_state *next_state, void *next_state_data);
bool guard2(void *sm_user_data, const struct sm_state *current_state,
			void *current_state_data, const struct sm_event *event,
			const struct sm_state *next_state, void *next_state_data);
bool guard3(void *sm_user_data, const struct sm_state *current_state,
			void *current_state_data, const struct sm_event *event,
			const struct sm_state *next_state, void *next_state_data);
bool guard4(void *sm_user_data, const struct sm_state *current_state,
			void *current_state_data, const struct sm_event *event,
			const struct sm_state *next_state, void *next_state_data);

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
