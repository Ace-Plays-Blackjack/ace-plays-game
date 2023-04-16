cmake_minimum_required(VERSION 3.0.0)
project(ACE_PLAYS VERSION 1.0.0)
option(NEW_CAM_STACK "Select new or old CAM stack: NEW==ON, OLD==OFF" ON)

add_definitions(-DCMAKE_NEW_CAM_STACK_${NEW_CAM_STACK})

include_directories(include)

add_executable(${PROJECT_NAME}  
    src/main.cpp
    src/Camera.cpp
    src/ToggleLED.cpp
    src/DetectCard.cpp
    src/StrategyEngine.cpp
    src/CardDiscriminator.cpp
    src/GamePlay.cpp
)

set_property (TARGET ${PROJECT_NAME} 
    PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
if(NOT NEW_CAM_STACK)
include(CPack)
endif(NOT NEW_CAM_STACK)

# Remove this later
#~ set(OpenCV_DIR "${OPENCV_DIR}/x64/vc15/")

# Add OpenCV library
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})

#Include Google test
option(PACKAGE_TESTS "Build the tests" ON)

if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    #add_subdirectory(tests)

endif()

# Find the path to the pigpio includes.
find_path(pigpio_INCLUDE_DIR 
	NAMES pigpio.h pigpiod_if.h pigpiod_if2.h
	HINTS /usr/local/include)
	
# Find the pigpio libraries.
find_library(pigpio_LIBRARY 
	NAMES libpigpio.so
	HINTS /usr/local/lib)
find_library(pigpiod_if_LIBRARY 
	NAMES libpigpiod_if.so
	HINTS /usr/local/lib)
find_library(pigpiod_if2_LIBRARY 
	NAMES libpigpiod_if2.so
	HINTS /usr/local/lib)
    
# Set the pigpio variables to plural form to make them accessible for 
# the paramount cmake modules.
set(pigpio_INCLUDE_DIRS ${pigpio_INCLUDE_DIR})
set(pigpio_INCLUDES     ${pigpio_INCLUDE_DIR})

# Handle REQUIRED, QUIET, and version arguments 
# and set the <packagename>_FOUND variable.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pigpio 
    DEFAULT_MSG 
    pigpio_INCLUDE_DIR pigpio_LIBRARY pigpiod_if_LIBRARY pigpiod_if2_LIBRARY)

target_link_libraries( ${PROJECT_NAME} PUBLIC ${pigpio_LIBRARY})

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

