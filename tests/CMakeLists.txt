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
