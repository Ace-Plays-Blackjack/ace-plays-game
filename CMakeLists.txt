cmake_minimum_required(VERSION 3.0.0)
project(ACE_PLAYS VERSION 1.0.0)
option(NEW_CAM_STACK "Select new or old CAM stack: NEW==ON, OLD==OFF" OFF)

include(CTest)
enable_testing()

include_directories(include)

if(NEW_CAM_STACK)
include_directories(LCCV/include)
endif(NEW_CAM_STACK)

add_executable(${PROJECT_NAME}  
    src/main.cpp
    src/camera.cpp
    src/DetectCard.cpp
)
if(NEW_CAM_STACK)
add_executable(${PROJECT_NAME}  
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
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS}  )
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
# message("OpenCV_LIBS: ${OpenCV_LIBS}")

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
message("Libcamera Path: ${libcamera}")
endif(NEW_CAM_STACK)
# Clean cache to always load the latest NEW_CAM_STACK option 
unset(NEW_CAM_STACK CACHE)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
