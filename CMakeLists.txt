cmake_minimum_required(VERSION 3.16.0)
project(Mastermind VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(mastermind source/main.cpp source/game.cpp)

add_executable(mastermindCheat source/main.cpp source/game.cpp)

target_compile_options(mastermindCheat PUBLIC -DCHEAT_MODE)

add_executable(mastermindTest source/test.cpp source/game.cpp)
target_link_libraries(mastermindTest pthread gtest)

include(GoogleTest)
gtest_discover_tests(mastermindTest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
