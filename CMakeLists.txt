cmake_minimum_required(VERSION 3.9)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
	option(STATE_MACHINE_DOCS "Generate html documentation" ON)
	option(STATE_MACHINE_TEST "Build tests" ON)
	option(STATE_MACHINE_COVERAGE "Test coverage" ON)
	option(STATE_MACHINE_EXAMPLE "Build examples" ON)
else()
	option(STATE_MACHINE_DOCS "Generate html documentation" OFF)
	option(STATE_MACHINE_TEST "Build tests" OFF)
	option(STATE_MACHINE_COVERAGE "Test coverage" OFF)
	option(STATE_MACHINE_EXAMPLE "Build examples" OFF)
endif()
option(FETCHCONTENT_QUIET "Disable logs of FetchContent" OFF)


project(state-machine VERSION 0.1.0 
	LANGUAGES C CXX
	)

add_subdirectory(doc)
add_subdirectory(examples)
if (STATE_MACHINE_TEST)
	enable_testing()
	add_subdirectory(tests)
else()
	add_subdirectory(src)
endif()
