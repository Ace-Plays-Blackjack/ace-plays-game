project(ACE_PLAYS VERSION 1.0.0)
include_directories(include)

target_sources(${PROJECT_NAME} PRIVATE
    src/lccv.cpp
    src/libcamera_app.cpp
    src/libcamera_app_options.cpp
)
# Add libcamera library
set(libcamera "/usr/include/libcamera")
set(libcamera_link "/usr/lib/arm-linux-gnueabihf/libcamera.so")
set(libcamera_link_base "/usr/lib/arm-linux-gnueabihf/libcamera-base.so")

include_directories(${libcamera})

target_link_libraries(${PROJECT_NAME} 
${libcamera_link}  
${libcamera_link_base}
)
message("**** Libcamera Path: ${libcamera} ****")