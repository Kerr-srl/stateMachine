#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_version_macros.hpp"
#include "catch2/trompeloeil.hpp"

#include "stateMachine.h"

#include <array>

using trompeloeil::_;
using trompeloeil::eq;
using trompeloeil::ne;
using trompeloeil::sequence;
using expectation = std::unique_ptr<trompeloeil::expectation>;

class StateCallbackMocks {
  public:
	MAKE_MOCK3(entry_action1,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(entry_action2,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(entry_action3,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(entry_action4,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(s_error_entry_action,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(exit_action1,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(exit_action2,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(exit_action3,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(exit_action4,
			   void(void *sm_user_data, void *stateData, struct event *event));
	MAKE_MOCK3(s_error_exit_action,
			   void(void *sm_user_data, void *stateData, struct event *event));

	MAKE_MOCK3(guard1,
			   bool(void *sm_user_data, void *condition, struct event *event));
	MAKE_MOCK3(guard2,
			   bool(void *sm_user_data, void *condition, struct event *event));
	MAKE_MOCK3(guard3,
			   bool(void *sm_user_data, void *condition, struct event *event));
	MAKE_MOCK3(guard4,
			   bool(void *sm_user_data, void *condition, struct event *event));

	MAKE_MOCK4(trans_action1, void(void *sm_user_data, void *oldStateData,
								   struct event *event, void *newStateData));
	MAKE_MOCK4(trans_action2, void(void *sm_user_data, void *oldStateData,
								   struct event *event, void *newStateData));
	MAKE_MOCK4(trans_action3, void(void *sm_user_data, void *oldStateData,
								   struct event *event, void *newStateData));
	MAKE_MOCK4(trans_action4, void(void *sm_user_data, void *oldStateData,
								   struct event *event, void *newStateData));
};
StateCallbackMocks mocks;

static void s1_entry_action(void *sm_user_data, void *stateData,
							struct event *event) {
	mocks.entry_action1(sm_user_data, stateData, event);
}
static void s2_entry_action(void *sm_user_data, void *stateData,
							struct event *event) {
	mocks.entry_action2(sm_user_data, stateData, event);
}
static void s3_entry_action(void *sm_user_data, void *stateData,
							struct event *event) {
	mocks.entry_action3(sm_user_data, stateData, event);
}
static void s4_entry_action(void *sm_user_data, void *stateData,
							struct event *event) {
	mocks.entry_action4(sm_user_data, stateData, event);
}
static void s_error_entry_action(void *sm_user_data, void *stateData,
								 struct event *event) {
	mocks.s_error_entry_action(sm_user_data, stateData, event);
}
static void s1_exit_action(void *sm_user_data, void *stateData,
						   struct event *event) {
	mocks.exit_action1(sm_user_data, stateData, event);
}
static void s2_exit_action(void *sm_user_data, void *stateData,
						   struct event *event) {
	mocks.exit_action2(sm_user_data, stateData, event);
}
static void s3_exit_action(void *sm_user_data, void *stateData,
						   struct event *event) {
	mocks.exit_action3(sm_user_data, stateData, event);
}
static void s4_exit_action(void *sm_user_data, void *stateData,
						   struct event *event) {
	mocks.exit_action4(sm_user_data, stateData, event);
}
static bool guard1(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard1(sm_user_data, condition, event);
}
static bool guard2(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard2(sm_user_data, condition, event);
}
static bool guard3(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard3(sm_user_data, condition, event);
}
static bool guard4(void *sm_user_data, void *condition, struct event *event) {
	return mocks.guard4(sm_user_data, condition, event);
}

static void trans_action1(void *sm_user_data, void *oldStateData,
						  struct event *event, void *newStateData) {
	mocks.trans_action1(sm_user_data, oldStateData, event, newStateData);
}
static void trans_action2(void *sm_user_data, void *oldStateData,
						  struct event *event, void *newStateData) {
	mocks.trans_action2(sm_user_data, oldStateData, event, newStateData);
}
static void trans_action3(void *sm_user_data, void *oldStateData,
						  struct event *event, void *newStateData) {
	mocks.trans_action3(sm_user_data, oldStateData, event, newStateData);
}
static void trans_action4(void *sm_user_data, void *oldStateData,
						  struct event *event, void *newStateData) {
	mocks.trans_action4(sm_user_data, oldStateData, event, newStateData);
}

enum eventTypes {
	Event_dummy,
};

static struct state s_error = {
	.entryAction = &s_error_entry_action,
};

TEST_CASE("Initialization") {
	SECTION("initialization shouldn't execute entry action") {
		stateMachine sm;
		state s_init = {
			.parentState = nullptr,
			.entryState = nullptr,
			.numTransitions = 0,
			.entryAction = s1_entry_action,
			.exitAction = s1_exit_action,
		};

		FORBID_CALL(mocks, entry_action1(_, _, _));
		FORBID_CALL(mocks, exit_action1(_, _, _));
		stateM_init(&sm, &s_init, &s_error, nullptr);
	}
}

TEST_CASE("State machine") {
	SECTION("state machine user data should be passed to all the state machine "
			"callbacks") {
		stateMachine sm;
		state s2 = {
			.parentState = nullptr,
			.entryState = nullptr,
			.numTransitions = 0,
			.entryAction = s2_entry_action,
			.exitAction = s2_exit_action,
		};

		std::array<transition, 1> s_init_transitions{
			{1, nullptr, guard1, trans_action1, &s2},
		};
		state s_init = {
			.parentState = nullptr,
			.entryState = nullptr,
			.transitions = s_init_transitions.data(),
			.numTransitions = 1,
			.entryAction = s1_entry_action,
			.exitAction = s1_exit_action,
		};

		void *fake_user_data = (void *)12;
		stateM_init(&sm, &s_init, &s_error, fake_user_data);

		struct event event;
		event.data = nullptr;
		event.type = 1;
		sequence seq;
		REQUIRE_CALL(mocks, guard1(fake_user_data, nullptr, &event))
			.IN_SEQUENCE(seq)
			.RETURN(true);
		REQUIRE_CALL(mocks, exit_action1(fake_user_data, nullptr, &event))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks,
					 trans_action1(fake_user_data, nullptr, &event, nullptr))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, entry_action2(fake_user_data, nullptr, &event))
			.IN_SEQUENCE(seq);
		stateM_handleEvent(&sm, &event);
	}
	SECTION("entry, transition and exit actions") {
		stateMachine sm;
		state s2 = {
			.parentState = nullptr,
			.entryState = nullptr,
			.numTransitions = 0,
			.entryAction = s2_entry_action,
			.exitAction = s2_exit_action,
		};

		std::array<transition, 1> s_init_transitions{
			{1, nullptr, nullptr, trans_action1, &s2},
		};
		state s_init = {
			.parentState = nullptr,
			.entryState = nullptr,
			.transitions = s_init_transitions.data(),
			.numTransitions = 1,
			.entryAction = s1_entry_action,
			.exitAction = s1_exit_action,
		};

		stateM_init(&sm, &s_init, &s_error, nullptr);

		struct event event;
		event.data = nullptr;
		event.type = 1;
		sequence seq;
		REQUIRE_CALL(mocks, exit_action1(nullptr, nullptr, &event))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, trans_action1(nullptr, nullptr, &event, nullptr))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, entry_action2(nullptr, nullptr, &event))
			.IN_SEQUENCE(seq);
		stateM_handleEvent(&sm, &event);
	}
}

TEST_CASE("Guards") {
}

TEST_CASE("Error state") {
}
