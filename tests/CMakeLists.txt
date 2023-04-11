cmake_minimum_required(VERSION 3.0)
project(ACE_PLAYS VERSION 1.0.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
set(OpenCV_DIR "../opencv_build")

find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

include_directories(../src)
include_directories(../include)
include_directories(../LCCV)

enable_testing()

add_executable(StrategyEngine_test StrategyEngine_test.cpp ../src/StrategyEngine.cpp)
target_link_libraries(StrategyEngine_test gtest gtest_main)

add_executable(Leds_test Leds_test.cpp ../src/leds.cpp)
target_link_libraries(Leds_test gtest gtest_main pigpio)

