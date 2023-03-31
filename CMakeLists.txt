cmake_minimum_required(VERSION 3.0.0)
project(OPENCV_TEST VERSION 0.1.0)

include(CTest)
enable_testing()


include_directories(include)
add_executable(OPENCV_TEST 
    src/main.cpp
    src/camera.cpp
    src/leds.cpp
    src/DetectCard.cpp
)
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
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${pigpio_LIBRARY} ${OpenCV_LIBS}  )
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
