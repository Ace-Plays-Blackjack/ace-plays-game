add_library(LCCV
    src/lccv.cpp
    src/libcamera_app.cpp
    src/libcamera_app_options.cpp
)

set_property (TARGET LCCV
    PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Add libcamera library
set(libcamera "/usr/include/libcamera")
set(libcamera_link "/usr/lib/arm-linux-gnueabihf/libcamera.so")
set(libcamera_link_base "/usr/lib/arm-linux-gnueabihf/libcamera-base.so")

target_include_directories(LCCV PUBLIC ${libcamera} include)

target_link_libraries(LCCV PUBLIC
${libcamera_link}  
${libcamera_link_base}
${OpenCV_LIBS}
)
message("**** Libcamera Path: ${libcamera} ****")
