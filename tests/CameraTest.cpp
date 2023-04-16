/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <gtest/gtest.h>
#include <thread>
#include "Camera.h"
#include "CallbackLinker.h"

class TestCallbackLinker : public CallbackLinker
{
public:
    void nextCallback(AcePlaysUtils &callbackData) override {}
};

class CameraTest : public ::testing::Test
{
protected:
    CameraTest() {}
    Camera cam;
    TestCallbackLinker callback;

};

TEST(CameraTest, TestConstructor) {
    // Test with new camera stack
    Camera camera1(0, 640, 480);
    EXPECT_EQ(camera1.activeCapture.get(cv::CAP_PROP_FRAME_WIDTH), 640);
    EXPECT_EQ(camera1.activeCapture.get(cv::CAP_PROP_FRAME_HEIGHT), 480);
    EXPECT_EQ(camera1.activeCapture.get(cv::CAP_PROP_FPS), 30);

    // Test with old camera stack
    Camera camera2(0, 640, 480);
    EXPECT_EQ(camera2.activeCapture.get(cv::CAP_PROP_FRAME_WIDTH), 640);
    EXPECT_EQ(camera2.activeCapture.get(cv::CAP_PROP_FRAME_HEIGHT), 480);
    EXPECT_EQ(camera2.activeCapture.get(cv::CAP_PROP_FPS), 30);
}

TEST(CameraTest, Recording) {
    
    Camera cam(0, 640, 480);
    TestCallbackLinker cbLinker;
    cam.registerCallback(&cbLinker);
    cam.startRecording();
    #if NEW_CAM_STACK
    {
        // Wait for 10 seconds
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    #else
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    #endif
    cam.stopRecording();
    cam.unregisterCallback();
    EXPECT_EQ(cam.getErr(), NO_ERROR);
}












