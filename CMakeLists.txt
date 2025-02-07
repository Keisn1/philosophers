set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
project(philosophers LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(philosophers)

option(BUILD_TEST "Build tests" ON)
option(BUILD_PHILO "Build philo" OFF)

file(GLOB_RECURSE PHILO_SOURCES "philo/*.c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -fsanitize=address")

if(BUILD_TEST)
  file(GLOB UNITTEST_FILES "tests/unittests/*.cpp")
  set(UNITTEST_EXECUTABLE "run_unittests")

  add_executable(${UNITTEST_EXECUTABLE} ${PHILO_SOURCES} ${UNITTEST_FILES})
  target_link_libraries(${UNITTEST_EXECUTABLE} gtest gtest_main pthread gmock)

  target_include_directories(${UNITTEST_EXECUTABLE} PRIVATE "philo")
  include(GoogleTest)
  gtest_discover_tests(${UNITTEST_EXECUTABLE})
endif()

file(GLOB PHILO_SOURCES "philo/*.c")
if(BUILD_PHILO)
  set(PHILO "philo")

  add_executable(${PHILO} ${PHILO_SOURCES})
  target_include_directories(${PHILO} PRIVATE philo)
endif()
