/*
 * Copyright (c) 2013 Andreas Misje
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "state_machine.h"
#include <stdint.h>
#include <stdio.h>

/* This simple example checks keyboad input against the two allowed strings
 * "ha\n" and "hi\n". If an unrecognised character is read, a group state will
 * handle this by printing a message and returning to the idle state. If the
 * character '!' is encountered, a "reset" message is printed, and the group
 * state's entry state will be entered (the idle state).
 *
 *                   print 'reset'
 *       o      +---------------------+
 *       |      |                     | '!'
 *       |      v     group state     |
 * +-----v-----------------------------------+----+
 * |  +------+  'h'  +---+  'a'  +---+  '\n'      |
 * +->| idle | ----> | h | ----> | a | ---------+ |
 * |  +------+       +---+\      +---+          | |
 * |   ^ ^ ^               \'i'  +---+  '\n'    | |
 * |   | | |                \--> | i | ------+  | |
 * |   | | |                     +---+       |  | |
 * +---|-|-|----------------+----------------|--|-+
 *     | | |                |                |  |
 *     | | |                | '[^hai!\n]'    |  |
 *     | | | print unrecog. |                |  |
 *     | | +----------------+   print 'hi'   |  |
 *     | +-----------------------------------+  |
 *     |               print 'ha'               |
 *     +----------------------------------------+
 */

/* Types of events */
enum eventType {
	Event_keyboard,
};

/* Compare keyboard character from transition's condition variable against
 * data in event. */
static bool compareKeyboardChar(void *ch, struct sm_event *event);

static void printRecognisedChar(void *stateData, struct sm_event *event);
static void printUnrecognisedChar(void *oldStateData, struct sm_event *event,
								  void *newStateData);
static void printReset(void *oldStateData, struct sm_event *event,
					   void *newStateData);
static void printHiMsg(void *oldStateData, struct sm_event *event,
					   void *newStateData);
static void printHaMsg(void *oldStateData, struct sm_event *event,
					   void *newStateData);
static void printErrMsg(void *stateData, struct sm_event *event);
static void printEnterMsg(void *stateData, struct sm_event *event);
static void printExitMsg(void *stateData, struct sm_event *event);

/* Forward declaration of states so that they can be defined in an logical
 * order: */
static struct sm_state checkCharsGroupState, idleState, hState, iState, aState;

/* All the following states (apart from the error state) are children of this
 * group state. This way, any unrecognised character will be handled by this
 * state's transition, eliminating the need for adding the same transition to
 * all the children states. */
static struct sm_state checkCharsGroupState = {
	.parent_state = NULL,
	/* The entry state is defined in order to demontrate that the 'reset'
	 * transtition, going to this group state, will be 'redirected' to the
	 * 'idle' state (the transition could of course go directly to the 'idle'
	 * state): */
	.entry_state = &idleState,
	.transitions =
		(struct transition[]){
			{
				Event_keyboard,
				(void *)(intptr_t)'!',
				&compareKeyboardChar,
				&printReset,
				&idleState,
			},
			{
				Event_keyboard,
				NULL,
				NULL,
				&printUnrecognisedChar,
				&idleState,
			},
		},
	.num_transitions = 2,
	.data = "group",
	.entry_action = &printEnterMsg,
	.exit_action = &printExitMsg,
};

static struct sm_state idleState = {
	.parent_state = &checkCharsGroupState,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){
			{Event_keyboard, (void *)(intptr_t)'h', &compareKeyboardChar, NULL,
			 &hState},
		},
	.num_transitions = 1,
	.data = "idle",
	.entry_action = &printEnterMsg,
	.exit_action = &printExitMsg,
};

static struct sm_state hState = {
	.parent_state = &checkCharsGroupState,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){
			{Event_keyboard, (void *)(intptr_t)'a', &compareKeyboardChar, NULL,
			 &aState},
			{Event_keyboard, (void *)(intptr_t)'i', &compareKeyboardChar, NULL,
			 &iState},
		},
	.num_transitions = 2,
	.data = "H",
	.entry_action = &printRecognisedChar,
	.exit_action = &printExitMsg,
};

static struct sm_state iState = {
	.parent_state = &checkCharsGroupState,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){{Event_keyboard, (void *)(intptr_t)'\n',
							   &compareKeyboardChar, &printHiMsg, &idleState}},
	.num_transitions = 1,
	.data = "I",
	.entry_action = &printRecognisedChar,
	.exit_action = &printExitMsg,
};

static struct sm_state aState = {
	.parent_state = &checkCharsGroupState,
	.entry_state = NULL,
	.transitions =
		(struct transition[]){{Event_keyboard, (void *)(intptr_t)'\n',
							   &compareKeyboardChar, &printHaMsg, &idleState}},
	.num_transitions = 1,
	.data = "A",
	.entry_action = &printRecognisedChar,
	.exit_action = &printExitMsg};

static struct sm_state errorState = {.entry_action = &printErrMsg};

int main() {
	struct sm_state_machine m;
	stateM_init(&m, &idleState, &errorState);

	int ch;
	while ((ch = getc(stdin)) != EOF)
		sm_state_machine_handle_event(
			&m, &(struct sm_event){Event_keyboard, (void *)(intptr_t)ch});

	return 0;
}

static bool compareKeyboardChar(void *ch, struct sm_event *event) {
	if (event->type != Event_keyboard)
		return false;

	return (intptr_t)ch == (intptr_t)event->data;
}

static void printRecognisedChar(void *stateData, struct sm_event *event) {
	printEnterMsg(stateData, event);
	printf("parsed: %c\n", (char)(intptr_t)event->data);
}

static void printUnrecognisedChar(void *oldStateData, struct sm_event *event,
								  void *newStateData) {
	printf("unrecognised character: %c\n", (char)(intptr_t)event->data);
}

static void printReset(void *oldStateData, struct sm_event *event,
					   void *newStateData) {
	puts("Resetting");
}

static void printHiMsg(void *oldStateData, struct sm_event *event,
					   void *newStateData) {
	puts("Hi!");
}

static void printHaMsg(void *oldStateData, struct sm_event *event,
					   void *newStateData) {
	puts("Ha-ha");
}

static void printErrMsg(void *stateData, struct sm_event *event) {
	puts("ENTERED ERROR STATE!");
}

static void printEnterMsg(void *stateData, struct sm_event *event) {
	printf("Entering %s state\n", (char *)stateData);
}

static void printExitMsg(void *stateData, struct sm_event *event) {
	printf("Exiting %s state\n", (char *)stateData);
}
