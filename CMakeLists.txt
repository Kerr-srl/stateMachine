cmake_minimum_required(VERSION 3.9)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
	option(STATE_MACHINE_DOCS "Generate html documentation" ON)
	option(STATE_MACHINE_EXAMPLE "Build examples" ON)
else()
	option(STATE_MACHINE_DOCS "Generate html documentation" OFF)
	option(STATE_MACHINE_EXAMPLE "Build examples" OFF)
endif()

project(state-machine VERSION 0.1.0 
	LANGUAGES C
	)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(examples)
