include(CTest)

include(FetchContent)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.0.0-preview3)

FetchContent_GetProperties(Catch2)
if(NOT catch2_POPULATED)
	FetchContent_Populate(Catch2)
	add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
	list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
	include(Catch)
endif()

FetchContent_Declare(
	trompeloeil
	GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git
	GIT_TAG  v39)
FetchContent_MakeAvailable(trompeloeil)

set(TARGET_NAME ${PROJECT_NAME}-tests)
add_executable(${TARGET_NAME} 
	basic.cpp
	multi-instance.cpp
	)
target_link_libraries(${TARGET_NAME} 
	PRIVATE 
	Catch2::Catch2WithMain
	state-machine::state-machine
	trompeloeil
	)
target_compile_features(${TARGET_NAME}
	PRIVATE
	cxx_std_17
	)

catch_discover_tests(${TARGET_NAME} EXTRA_ARGS --use-colour yes)

if (${STATE_MACHINE_COVERAGE})
	if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
	endif()
	add_custom_target("${PROJECT_NAME}-coverage"
		COMMAND
		lcov --rc lcov_branch_coverage=1 -c -i -d . -o coverage-base.info
		COMMAND
		ctest -V
		COMMAND
		lcov --rc lcov_branch_coverage=1 -c -d . -o coverage-run.info
		COMMAND
		lcov --rc lcov_branch_coverage=1 -a coverage-base.info -a coverage-run.info -o coverage.info > /dev/null
		COMMAND
		lcov --rc lcov_branch_coverage=1 --remove coverage.info "/usr/*"
		"*/test/*" "*/src/**/*.mock.*" "*/src/**/*.test.*" "*/third-party/*" -o
		coverage.info > /dev/null
		COMMAND
		genhtml --rc lcov_branch_coverage=1 coverage.info -o ${PROJECT_BINARY_DIR}/coverage/
		COMMENT "Coverage report"
		VERBATIM
		WORKING_DIRECTORY
		${PROJECT_BINARY_DIR}
		)
endif()
