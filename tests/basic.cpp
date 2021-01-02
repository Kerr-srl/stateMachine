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
	MAKE_MOCK2(entry_action1, void(void *stateData, struct event *event));
	MAKE_MOCK2(entry_action2, void(void *stateData, struct event *event));
	MAKE_MOCK2(entry_action3, void(void *stateData, struct event *event));
	MAKE_MOCK2(entry_action4, void(void *stateData, struct event *event));
	MAKE_MOCK2(s_error_entry_action,
			   void(void *stateData, struct event *event));
	MAKE_MOCK2(exit_action1, void(void *stateData, struct event *event));
	MAKE_MOCK2(exit_action2, void(void *stateData, struct event *event));
	MAKE_MOCK2(exit_action3, void(void *stateData, struct event *event));
	MAKE_MOCK2(exit_action4, void(void *stateData, struct event *event));
	MAKE_MOCK2(s_error_exit_action, void(void *stateData, struct event *event));

	MAKE_MOCK2(guard1, void(void *condition, struct event *event));
	MAKE_MOCK2(guard2, void(void *condition, struct event *event));
	MAKE_MOCK2(guard3, void(void *condition, struct event *event));
	MAKE_MOCK2(guard4, void(void *condition, struct event *event));

	MAKE_MOCK3(trans_action1, void(void *oldStateData, struct event *event,
								   void *newStateData));
	MAKE_MOCK3(trans_action2, void(void *oldStateData, struct event *event,
								   void *newStateData));
	MAKE_MOCK3(trans_action3, void(void *oldStateData, struct event *event,
								   void *newStateData));
	MAKE_MOCK3(trans_action4, void(void *oldStateData, struct event *event,
								   void *newStateData));
};
StateCallbackMocks mocks;

static void s1_entry_action(void *stateData, struct event *event) {
	mocks.entry_action1(stateData, event);
}
static void s2_entry_action(void *stateData, struct event *event) {
	mocks.entry_action2(stateData, event);
}
static void s3_entry_action(void *stateData, struct event *event) {
	mocks.entry_action3(stateData, event);
}
static void s4_entry_action(void *stateData, struct event *event) {
	mocks.entry_action4(stateData, event);
}
static void s_error_entry_action(void *stateData, struct event *event) {
	mocks.s_error_entry_action(stateData, event);
}
static void s1_exit_action(void *stateData, struct event *event) {
	mocks.exit_action1(stateData, event);
}
static void s2_exit_action(void *stateData, struct event *event) {
	mocks.exit_action2(stateData, event);
}
static void s3_exit_action(void *stateData, struct event *event) {
	mocks.exit_action3(stateData, event);
}
static void s4_exit_action(void *stateData, struct event *event) {
	mocks.exit_action4(stateData, event);
}
static void guard1(void *condition, struct event *event) {
	mocks.guard1(condition, event);
}
static void guard2(void *condition, struct event *event) {
	mocks.guard2(condition, event);
}
static void guard3(void *condition, struct event *event) {
	mocks.guard3(condition, event);
}
static void guard4(void *condition, struct event *event) {
	mocks.guard4(condition, event);
}

static void trans_action1(void *oldStateData, struct event *event,
						  void *newStateData) {
	mocks.trans_action1(oldStateData, event, newStateData);
}
static void trans_action2(void *oldStateData, struct event *event,
						  void *newStateData) {
	mocks.trans_action2(oldStateData, event, newStateData);
}
static void trans_action3(void *oldStateData, struct event *event,
						  void *newStateData) {
	mocks.trans_action3(oldStateData, event, newStateData);
}
static void trans_action4(void *oldStateData, struct event *event,
						  void *newStateData) {
	mocks.trans_action4(oldStateData, event, newStateData);
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
			.parentState = NULL,
			.entryState = NULL,
			.numTransitions = 0,
			.entryAction = s1_entry_action,
			.exitAction = s1_exit_action,
		};

		FORBID_CALL(mocks, entry_action1(_, _));
		FORBID_CALL(mocks, exit_action1(_, _));
		stateM_init(&sm, &s_init, &s_error);
	}
}

TEST_CASE("State machine") {
	SECTION("entry, transition and exit actions") {
		stateMachine sm;
		state s2 = {
			.parentState = NULL,
			.entryState = NULL,
			.numTransitions = 0,
			.entryAction = s2_entry_action,
			.exitAction = s2_exit_action,
		};

		std::array<transition, 1> s_init_transitions{
			{1, NULL, NULL, trans_action1, &s2},
		};
		state s_init = {
			.parentState = NULL,
			.entryState = NULL,
			.transitions = s_init_transitions.data(),
			.numTransitions = 1,
			.entryAction = s1_entry_action,
			.exitAction = s1_exit_action,
		};

		stateM_init(&sm, &s_init, &s_error);

		struct event event;
		event.data = NULL;
		event.type = 1;
		sequence seq;
		REQUIRE_CALL(mocks, exit_action1(_, _)).IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, trans_action1(_, _, _)).IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, entry_action2(_, _)).IN_SEQUENCE(seq);
		stateM_handleEvent(&sm, &event);
	}
}

TEST_CASE("Guards") {
}

TEST_CASE("Error state") {
}
