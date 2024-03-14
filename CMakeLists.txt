cmake_minimum_required(VERSION 3.1...3.28)

project(
  Mafia
  VERSION 0.1.1
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_subdirectory(src/modules)

add_executable(main src/main.cc)
add_executable(test_validation src/test_validation.cc)
add_library(validation src/validation.cc src/validation.hh)
target_link_libraries(main PUBLIC validation)
target_link_libraries(validation PRIVATE modules)

target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}"
                                       "${PROJECT_SOURCE_DIR}/modules")

target_include_directories(
  test_validation PUBLIC "${PROJECT_BINARY_DIR}"
                         "${PROJECT_SOURCE_DIR}/modules")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
target_link_libraries(test_validation PRIVATE GTest::gtest_main)
gtest_discover_tests(test_validation)
