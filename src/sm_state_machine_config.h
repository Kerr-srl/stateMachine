/**
 * \verbatim
 *                              _  __
 *                             | |/ /
 *                             | ' / ___ _ __ _ __
 *                             |  < / _ \ '__| '__|
 *                             | . \  __/ |  | |
 *                             |_|\_\___|_|  |_|
 * \endverbatim
 * \file		sm_state_machine_config.h
 *
 * \brief		state machine compile time configuration
 *
 * \copyright	Copyright 2021 Kerr s.r.l. - All Rights Reserved.
 */
#ifndef SM_STATE_MACHINE_CONFIG_H_
#define SM_STATE_MACHINE_CONFIG_H_

#ifndef SM_STATE_MACHINE_ENABLE_LOG
/**
 * Whether to enable log functionality
 */
#define SM_STATE_MACHINE_ENABLE_LOG 1u
#endif

#ifndef SM_STATE_MACHINE_OPTIMIZE_RAM
/**
 * Whether to enable RAM optimization by constructing transition functions
 * (instead of transition tables) for each state.
 *
 * Users must use \ref SM_STATE_MACHINE_TRANSITION_DEF_START
 * "SM_STATE_MACHINE_TRANSITION_DEF_*" macros to make this feature work.
 */
#define SM_STATE_MACHINE_OPTIMIZE_RAM 0u
#endif

#endif /* ifndef SM_STATE_MACHINE_CONFIG_H_ */
