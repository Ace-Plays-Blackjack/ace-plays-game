cmake_minimum_required(VERSION 3.0.0)
project(OPENCV_TEST VERSION 0.1.0)

include(CTest)
enable_testing()


include_directories(include)
add_executable(OPENCV_TEST 
    src/main.cpp
    src/camera.cpp
)
set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS}  )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
