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
		sm_state_machine_hooks hooks = {};
		FORBID_CALL(mocks, s1_entry_action(_, _, _, _, _, _));
		FORBID_CALL(mocks, s1_exit_action(_, _, _, _, _, _));
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks, nullptr,
							  nullptr);
	}
}

TEST_CASE("State machine") {
	void *fake_user_data = GENERATE(values<void *>({nullptr, (void *)12}));

	SECTION("guard, entry, transition and exit actions"
			"callbacks") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {};
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, nullptr);

		struct sm_event event;
		event.data = nullptr;
		event.type = event_s1_to_s2;
		sequence seq;
		REQUIRE_CALL(mocks,
					 guard1(fake_user_data, &s1, nullptr, &event, &s2, nullptr))
			.IN_SEQUENCE(seq)
			.RETURN(true);
		REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, nullptr, &event,
										   &s2, nullptr))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, trans_action1(fake_user_data, &s1, nullptr, &event,
										  &s2, nullptr))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, s2_entry_action(fake_user_data, &s1, nullptr,
											&event, &s2, nullptr))
			.IN_SEQUENCE(seq);
		sm_state_machine_handle_event(&sm, &event);
	}

	SECTION("transition should be stopped if guard returns false") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {};
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, nullptr);

		struct sm_event event;
		event.data = nullptr;
		event.type = event_s1_to_s2;
		REQUIRE_CALL(mocks,
					 guard1(fake_user_data, &s1, nullptr, &event, &s2, nullptr))
			.RETURN(false);
		FORBID_CALL(mocks, s1_exit_action(_, _, _, _, _, _));
		FORBID_CALL(mocks, trans_action1(_, _, _, _, _, _));
		FORBID_CALL(mocks, s2_entry_action(_, _, _, _, _, _));
		sm_state_machine_handle_event(&sm, &event);
	}

	SECTION("single instance - state data") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {.state_data_mapper =
											test_sm_state_data_mapper};
		test_sm_state_data data;
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, &data);

		struct sm_event event;
		event.data = nullptr;
		event.type = event_s1_to_s2;
		sequence seq;
		REQUIRE_CALL(
			mocks, guard1(fake_user_data, &s1, &data.s1, &event, &s2, &data.s2))
			.IN_SEQUENCE(seq)
			.RETURN(true);
		REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, &data.s1,
										   &event, &s2, &data.s2))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, trans_action1(fake_user_data, &s1, &data.s1, &event,
										  &s2, &data.s2))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, s2_entry_action(fake_user_data, &s1, &data.s1,
											&event, &s2, &data.s2))
			.IN_SEQUENCE(seq);
		sm_state_machine_handle_event(&sm, &event);
	}

	SECTION("multiple instances - state data") {
		sm_state_machine_hooks hooks = {.state_data_mapper =
											test_sm_state_data_mapper};
		sm_state_machine sm_1;
		test_sm_state_data data_1;
		sm_state_machine_init(&sm_1, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, &data_1);

		sm_state_machine sm_2;
		test_sm_state_data data_2;
		sm_state_machine_init(&sm_2, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, &data_2);

		{
			struct sm_event event;
			event.data = nullptr;
			event.type = event_s1_to_s2;
			sequence seq;
			REQUIRE_CALL(mocks, guard1(fake_user_data, &s1, &data_1.s1, &event,
									   &s2, &data_1.s2))
				.IN_SEQUENCE(seq)
				.RETURN(true);
			REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, &data_1.s1,
											   &event, &s2, &data_1.s2))
				.IN_SEQUENCE(seq);
			REQUIRE_CALL(mocks, trans_action1(fake_user_data, &s1, &data_1.s1,
											  &event, &s2, &data_1.s2))
				.IN_SEQUENCE(seq);
			REQUIRE_CALL(mocks, s2_entry_action(fake_user_data, &s1, &data_1.s1,
												&event, &s2, &data_1.s2))
				.IN_SEQUENCE(seq);
			sm_state_machine_handle_event(&sm_1, &event);
		}

		{
			struct sm_event event;
			event.data = nullptr;
			event.type = event_s1_to_s2;
			sequence seq;
			REQUIRE_CALL(mocks, guard1(fake_user_data, &s1, &data_2.s1, &event,
									   &s2, &data_2.s2))
				.IN_SEQUENCE(seq)
				.RETURN(true);
			REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, &data_2.s1,
											   &event, &s2, &data_2.s2))
				.IN_SEQUENCE(seq);
			REQUIRE_CALL(mocks, trans_action1(fake_user_data, &s1, &data_2.s1,
											  &event, &s2, &data_2.s2))
				.IN_SEQUENCE(seq);
			REQUIRE_CALL(mocks, s2_entry_action(fake_user_data, &s1, &data_2.s1,
												&event, &s2, &data_2.s2))
				.IN_SEQUENCE(seq);
			sm_state_machine_handle_event(&sm_2, &event);
		}
	}
}

TEST_CASE("Guards") {
}

TEST_CASE("Error state") {
}
