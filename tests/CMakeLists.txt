cmake_minimum_required(VERSION 3.0)
project(ACE_PLAYS VERSION 1.0.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
set(OpenCV_DIR "../opencv_build")

find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

include_directories(../src)
include_directories(../include)
include_directories(../LCCV)
include_directories(${OpenCV_INCLUDE_DIRS})

enable_testing()

add_executable(StrategyEngineTest StrategyEngineTest.cpp ../src/StrategyEngine.cpp)
target_link_libraries(StrategyEngine_test gtest gtest_main)

add_executable(LedsTest LedsTest.cpp ../src/Leds.cpp)
target_link_libraries(LedsTest ${OpenCV_LIBS} gtest gtest_main pigpio)

add_executable(GamePlayTest GamePlayTest.cpp ../src/GamePlay.cpp ../src/StrategyEngine.cpp ../src/Leds.cpp ../src/DetectCard.cpp ../src/CardDiscriminator.cpp)
target_link_libraries(GamePlayTest ${OpenCV_LIBS} gtest gtest_main pigpio)

