cmake_minimum_required(VERSION 3.0.0)
project(ACE_PLAYS VERSION 1.0.0)
option(NEW_CAM_STACK "Select new or old CAM stack: NEW==ON, OLD==OFF" ON)

add_definitions(-DCMAKE_NEW_CAM_STACK_${NEW_CAM_STACK})

include(CTest)
enable_testing()

include_directories(include)

if(NEW_CAM_STACK)
include_directories(LCCV/include)
endif(NEW_CAM_STACK)

add_executable(${PROJECT_NAME}  
    src/main.cpp
    src/camera.cpp
    src/leds.cpp
    src/DetectCard.cpp
    src/StrategyEngine.cpp
    src/GamePlay.cpp
)
if(NEW_CAM_STACK)
target_sources(${PROJECT_NAME} PRIVATE
    LCCV/src/lccv.cpp
    LCCV/src/libcamera_app.cpp
    LCCV/src/libcamera_app_options.cpp
)
endif(NEW_CAM_STACK)


set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17

)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Remove this later
set(OpenCV_DIR "${OPENCV_DIR}/x64/vc15/")

# Add OpenCV library
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${pigpio_LIBRARY} ${OpenCV_LIBS} )

if(NEW_CAM_STACK)
# Add libcamera library
set(libcamera "/usr/include/libcamera")
set(libcamera_link "/usr/lib/arm-linux-gnueabihf/libcamera.so")
set(libcamera_link_base "/usr/lib/arm-linux-gnueabihf/libcamera-base.so")

include_directories(${libcamera})

target_link_libraries(${PROJECT_NAME} 
${libcamera_link}  
${libcamera_link_base}
)
endif(NEW_CAM_STACK)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

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


message("**** OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS} ****")
# message("OpenCV_LIBS: ${OpenCV_LIBS}")
message("**** Libcamera Path: ${libcamera} ****")
message("**** New CAM stack option: -DCMAKE_NEW_CAM_STACK_${NEW_CAM_STACK} ****")
# Clean cache to always load the latest NEW_CAM_STACK option 
unset(NEW_CAM_STACK CACHE)