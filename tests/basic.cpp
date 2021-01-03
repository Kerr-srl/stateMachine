#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_version_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/trompeloeil.hpp"

#include "sm_state_machine.h"
#include "test_sm.h"
#include "test_sm_mocks.hpp"

#include <array>

using trompeloeil::_;
using trompeloeil::eq;
using trompeloeil::ne;
using trompeloeil::sequence;
using expectation = std::unique_ptr<trompeloeil::expectation>;

TEST_CASE("Initialization") {
	SECTION("initialization shouldn't execute entry action") {
		sm_state_machine sm;
		FORBID_CALL(mocks, s1_entry_action(_, _, _, _));
		FORBID_CALL(mocks, s1_exit_action(_, _, _, _));
		sm_state_machine_hooks hooks;
		sm_state_machine_init(&sm, &s1, &s_error, &hooks, nullptr);
	}
}

TEST_CASE("State machine") {
	void *fake_user_data = GENERATE(values<void *>({nullptr, (void *)12}));
	sm_state_machine sm;
	sm_state_machine_hooks hooks;
	sm_state_machine_init(&sm, &s1, &s_error, &hooks, fake_user_data);

	SECTION("guard, entry, transition and exit actions"
			"callbacks") {
		struct sm_event event;
		event.data = nullptr;
		event.type = event_s1_to_s2;
		sequence seq;
		REQUIRE_CALL(mocks, guard1(fake_user_data, nullptr, &event))
			.IN_SEQUENCE(seq)
			.RETURN(true);
		REQUIRE_CALL(mocks,
					 s1_exit_action(fake_user_data, nullptr, &event, nullptr))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks,
					 trans_action1(fake_user_data, nullptr, &event, nullptr))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks,
					 s2_entry_action(fake_user_data, nullptr, &event, nullptr))
			.IN_SEQUENCE(seq);
		sm_state_machine_handle_event(&sm, &event);
	}
}

TEST_CASE("Guards") {
}

TEST_CASE("Error state") {
}
