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
