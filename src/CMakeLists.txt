set(TARGET_NAME ${PROJECT_NAME})

add_library(${TARGET_NAME} STATIC "")
add_library(${TARGET_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})
target_sources(${TARGET_NAME}
	PRIVATE
	stateMachine.c
	)

target_include_directories(${TARGET_NAME}
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	)

if (${STATE_MACHINE_COVERAGE})
	target_compile_options(${TARGET_NAME}
		PRIVATE
		--coverage
		)
	target_link_libraries(${TARGET_NAME}
		PRIVATE
		gcov
		)
endif()
