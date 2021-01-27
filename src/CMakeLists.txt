set(MAIN_TARGET_NAME ${PROJECT_NAME})

add_library(${MAIN_TARGET_NAME} STATIC "")
add_library(${PROJECT_NAME}::${MAIN_TARGET_NAME} ALIAS ${MAIN_TARGET_NAME})
target_sources(${MAIN_TARGET_NAME}
	PRIVATE
	sm_state_machine.c
	)

target_include_directories(${MAIN_TARGET_NAME}
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	)

if (${STATE_MACHINE_COVERAGE})
	target_compile_options(${MAIN_TARGET_NAME}
		PRIVATE
		--coverage
		)
	target_link_libraries(${MAIN_TARGET_NAME}
		PRIVATE
		gcov
		)
endif()

if (TARGET ${PROJECT_NAME}::config)
	target_link_libraries(${MAIN_TARGET_NAME}
		PUBLIC
		${PROJECT_NAME}::config
		)
endif()

set(UTILS_TARGET_NAME utils)
add_library(${UTILS_TARGET_NAME} STATIC "")
add_library(${PROJECT_NAME}::${UTILS_TARGET_NAME} ALIAS ${UTILS_TARGET_NAME})
target_sources(${UTILS_TARGET_NAME}
	PRIVATE
	sm_utils.cpp
	)
