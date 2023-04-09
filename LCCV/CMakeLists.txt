include_directories(include)

add_executable(${CMAKE_PROJECT_NAME}
    src/lccv.cpp
    src/libcamera_app.cpp
    src/libcamera_app_options.cpp
)
# Add libcamera library
set(libcamera "/usr/include/libcamera")
set(libcamera_link "/usr/lib/arm-linux-gnueabihf/libcamera.so")
set(libcamera_link_base "/usr/lib/arm-linux-gnueabihf/libcamera-base.so")

include_directories(${libcamera})

target_link_libraries(${CMAKE_PROJECT_NAME} 
${libcamera_link}  
${libcamera_link_base}
)
message("**** Libcamera Path: ${libcamera} ****")