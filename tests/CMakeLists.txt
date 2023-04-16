cmake_minimum_required(VERSION 3.0)
project(ACE_PLAYS VERSION 1.0.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_definitions(-DCMAKE_NEW_CAM_STACK_${NEW_CAM_STACK})

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
set(OpenCV_DIR "../opencv_build")

enable_testing()
find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

link_directories( ${OpenCV_LIB_DIR} )

include_directories(../src)
include_directories(../include)
include_directories(../LCCV)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include LCCV library
if(NEW_CAM_STACK)

	add_subdirectory(LCCV)
	target_link_libraries(${PROJECT_NAME} PUBLIC LCCV)

endif(NEW_CAM_STACK)

message("**** OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS} ****")
# message("OpenCV_LIBS: ${OpenCV_LIBS}")
message("**** New CAM stack option: -DCMAKE_NEW_CAM_STACK_${NEW_CAM_STACK} ****")

# Add CMAKE flag
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# Clean cache to always load the latest NEW_CAM_STACK option 
unset(NEW_CAM_STACK CACHE)

add_executable(StrategyEngineTest StrategyEngineTest.cpp ../src/StrategyEngine.cpp)
target_link_libraries(StrategyEngineTest gtest gtest_main)

add_executable(ToggleLEDTest ToggleLEDTest.cpp ../src/ToggleLED.cpp)
target_link_libraries(ToggleLEDTest ${OpenCV_LIBS} gtest gtest_main pigpio)

add_executable(GamePlayTest GamePlayTest.cpp ../src/GamePlay.cpp ../src/StrategyEngine.cpp ../src/ToggleLED.cpp ../src/DetectCard.cpp ../src/CardDiscriminator.cpp)
target_link_libraries(GamePlayTest ${OpenCV_LIBS} gtest gtest_main pigpio)

add_executable(CardDiscriminatorTest CardDiscriminatorTest.cpp ../src/Camera.cpp ../src/CardDiscriminator.cpp)
target_link_libraries(CardDiscriminatorTest ${OpenCV_LIBS} gtest gtest_main)

add_executable(CameraTest CameraTest.cpp ../src/Camera.cpp ../src/CardDiscriminator.cpp)
target_link_libraries(CameraTest ${OpenCV_LIBS} gtest gtest_main ${LCCV_LIBS})
