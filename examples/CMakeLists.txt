if (NOT STATE_MACHINE_EXAMPLE)
	return()
endif()

add_executable(${PROJECT_NAME}-example
	stateMachineExample.c
	)
target_link_libraries(${PROJECT_NAME}-example
	PRIVATE
	state-machine::state-machine
	)
