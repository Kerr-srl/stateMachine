/**
 * \verbatim
 *                              _  __
 *                             | |/ /
 *                             | ' / ___ _ __ _ __
 *                             |  < / _ \ '__| '__|
 *                             | . \  __/ |  | |
 *                             |_|\_\___|_|  |_|
 * \endverbatim
 * \file		test.cpp
 *
 * \brief		Unit tests
 *
 * \copyright	Copyright 2021 Kerr s.r.l. - All Rights Reserved.
 */
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

#define SETUP_LOOSE_MOCK_DEFAULT()                                             \
	ALLOW_CALL(mocks, s1_entry_action(_, _, _, _, _, _));                      \
	ALLOW_CALL(mocks, s2_entry_action(_, _, _, _, _, _));                      \
	ALLOW_CALL(mocks, s3_entry_action(_, _, _, _, _, _));                      \
	ALLOW_CALL(mocks, s4_entry_action(_, _, _, _, _, _));                      \
	ALLOW_CALL(mocks, s5_entry_action(_, _, _, _, _, _));                      \
	ALLOW_CALL(mocks, s5_child_entry_action(_, _, _, _, _, _));                \
	ALLOW_CALL(mocks, s5_child_child_entry_action(_, _, _, _, _, _));          \
	ALLOW_CALL(mocks, s6_entry_action(_, _, _, _, _, _));                      \
	ALLOW_CALL(mocks, s6_child_entry_action(_, _, _, _, _, _));                \
	ALLOW_CALL(mocks, s6_child_child_entry_action(_, _, _, _, _, _));          \
	ALLOW_CALL(mocks, s1_exit_action(_, _, _, _, _, _));                       \
	ALLOW_CALL(mocks, s2_exit_action(_, _, _, _, _, _));                       \
	ALLOW_CALL(mocks, s3_exit_action(_, _, _, _, _, _));                       \
	ALLOW_CALL(mocks, s4_exit_action(_, _, _, _, _, _));                       \
	ALLOW_CALL(mocks, s5_exit_action(_, _, _, _, _, _));                       \
	ALLOW_CALL(mocks, s5_child_exit_action(_, _, _, _, _, _));                 \
	ALLOW_CALL(mocks, s5_child_child_exit_action(_, _, _, _, _, _));           \
	ALLOW_CALL(mocks, s6_exit_action(_, _, _, _, _, _));                       \
	ALLOW_CALL(mocks, s6_child_exit_action(_, _, _, _, _, _));                 \
	ALLOW_CALL(mocks, s6_child_child_exit_action(_, _, _, _, _, _));           \
	ALLOW_CALL(mocks, guard1(_, _, _, _, _, _)).RETURN(true);                  \
	ALLOW_CALL(mocks, guard2(_, _, _, _, _, _)).RETURN(true);                  \
	ALLOW_CALL(mocks, guard3(_, _, _, _, _, _)).RETURN(true);                  \
	ALLOW_CALL(mocks, guard4(_, _, _, _, _, _)).RETURN(true);                  \
	ALLOW_CALL(mocks, trans_action1(_, _, _, _, _, _));                        \
	ALLOW_CALL(mocks, trans_action2(_, _, _, _, _, _));                        \
	ALLOW_CALL(mocks, trans_action3(_, _, _, _, _, _));                        \
	ALLOW_CALL(mocks, trans_action4(_, _, _, _, _, _));

TEST_CASE("Initialization") {
	SETUP_LOOSE_MOCK_DEFAULT();

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
	SETUP_LOOSE_MOCK_DEFAULT();

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

	SECTION("same event, multiple guards") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {};
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, nullptr);

		SECTION("guard") {
			struct sm_event event;
			event.data = nullptr;
			event.type = event_s1_to_s_guard;
			sequence seq;
			REQUIRE_CALL(mocks, guard1(fake_user_data, &s1, nullptr, &event,
									   &s1, nullptr))
				.IN_SEQUENCE(seq)
				.RETURN(false);
			REQUIRE_CALL(mocks, guard2(fake_user_data, &s1, nullptr, &event,
									   &s2, nullptr))
				.IN_SEQUENCE(seq)
				.RETURN(false);
			REQUIRE_CALL(mocks, guard3(fake_user_data, &s1, nullptr, &event,
									   &s3, nullptr))
				.IN_SEQUENCE(seq)
				.RETURN(true);
			REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, nullptr,
											   &event, &s3, nullptr))

				.IN_SEQUENCE(seq);
			REQUIRE_CALL(mocks, s3_entry_action(fake_user_data, &s1, nullptr,
												&event, &s3, nullptr))
				.IN_SEQUENCE(seq);
			sm_state_machine_handle_event(&sm, &event);
		}

		SECTION("catch all") {
			struct sm_event event;
			event.data = nullptr;
			event.type = event_s1_to_s_guard;
			sequence seq;
			REQUIRE_CALL(mocks, guard1(fake_user_data, &s1, nullptr, &event,
									   &s1, nullptr))
				.IN_SEQUENCE(seq)
				.RETURN(false);
			REQUIRE_CALL(mocks, guard2(fake_user_data, &s1, nullptr, &event,
									   &s2, nullptr))
				.IN_SEQUENCE(seq)
				.RETURN(false);
			REQUIRE_CALL(mocks, guard3(fake_user_data, &s1, nullptr, &event,
									   &s3, nullptr))
				.IN_SEQUENCE(seq)
				.RETURN(false);
			REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, nullptr,
											   &event, &s4, nullptr))
				.IN_SEQUENCE(seq);
			REQUIRE_CALL(mocks, s4_entry_action(fake_user_data, &s1, nullptr,
												&event, &s4, nullptr))
				.IN_SEQUENCE(seq);
			sm_state_machine_handle_event(&sm, &event);
		}
	}

	SECTION("single instance - state data") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {
			.state_data_mapper = test_sm_state_data_mapper,
		};
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

	SECTION("an event should be used to perform one transition") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {
			.state_data_mapper = test_sm_state_data_mapper,
		};
		test_sm_state_data data;
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, &data);

		struct sm_event event;
		event.data = nullptr;
		event.type = event_chain_s1_s2;
		sequence seq;
		REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, &data.s1,
										   &event, &s2, &data.s2))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, trans_action1(fake_user_data, &s1, &data.s1, &event,
										  &s2, &data.s2))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, s2_entry_action(fake_user_data, &s1, &data.s1,
											&event, &s2, &data.s2))
			.IN_SEQUENCE(seq);
		FORBID_CALL(mocks, s2_exit_action(fake_user_data, &s2, &data.s2, &event,
										  &s3, &data.s3));
		FORBID_CALL(mocks, trans_action2(fake_user_data, &s2, &data.s2, &event,
										 &s3, &data.s3));
		FORBID_CALL(mocks, s3_entry_action(fake_user_data, &s2, &data.s2,
										   &event, &s3, &data.s3));
		sm_state_machine_handle_event(&sm, &event);
	}

	SECTION("an event should be used to perform one transition") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {
			.state_data_mapper = test_sm_state_data_mapper,
		};
		test_sm_state_data data;
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, &data);

		struct sm_event event;
		event.data = nullptr;
		event.type = event_chain_s1_s2;
		sequence seq;
		REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, &data.s1,
										   &event, &s2, &data.s2))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, trans_action1(fake_user_data, &s1, &data.s1, &event,
										  &s2, &data.s2))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, s2_entry_action(fake_user_data, &s1, &data.s1,
											&event, &s2, &data.s2))
			.IN_SEQUENCE(seq);
		FORBID_CALL(mocks, s2_exit_action(fake_user_data, &s2, &data.s2, &event,
										  &s3, &data.s3));
		FORBID_CALL(mocks, trans_action2(fake_user_data, &s2, &data.s2, &event,
										 &s3, &data.s3));
		FORBID_CALL(mocks, s3_entry_action(fake_user_data, &s2, &data.s2,
										   &event, &s3, &data.s3));
		sm_state_machine_handle_event(&sm, &event);
	}

	SECTION("entry_state chain") {
		sm_state_machine sm;
		sm_state_machine_hooks hooks = {
			.state_data_mapper = test_sm_state_data_mapper,
		};
		test_sm_state_data data;
		sm_state_machine_init(&sm, nullptr, &s1, &s_error, &hooks,
							  fake_user_data, &data);

		struct sm_event event;
		event.data = nullptr;
		event.type = event_s1_to_s5;
		sequence seq;
		REQUIRE_CALL(mocks, s1_exit_action(fake_user_data, &s1, &data.s1,
										   &event, &s5, &data.s5))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, s5_entry_action(fake_user_data, &s1, &data.s1,
											&event, &s5, &data.s5))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks,
					 s5_child_entry_action(fake_user_data, &s1, &data.s1,
										   &event, &s5_child, &data.s5_child))
			.IN_SEQUENCE(seq);
		REQUIRE_CALL(mocks, s5_child_child_entry_action(
								fake_user_data, &s1, &data.s1, &event,
								&s5_child_child, nullptr))
			.IN_SEQUENCE(seq);
		sm_state_machine_handle_event(&sm, &event);
	}
}
