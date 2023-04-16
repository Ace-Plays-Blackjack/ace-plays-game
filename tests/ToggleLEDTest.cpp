/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <gtest/gtest.h>
#include "ToggleLED.h"

class ToggleLEDTest : public ::testing::Test {
protected:
    ToggleLED led;
};

TEST_F(ToggleLEDTest, Hit) {
    EXPECT_TRUE(led.flashled(HIT));
}

TEST_F(ToggleLEDTest, Stand) {
    EXPECT_TRUE(led.flashled(STAND));
}

TEST_F(ToggleLEDTest, Split) {
    EXPECT_TRUE(led.flashled(SPLIT));
}

TEST_F(ToggleLEDTest, Double) {
    EXPECT_TRUE(led.flashled(DOUBLE));
}

TEST_F(ToggleLEDTest, Stop) {
    EXPECT_TRUE(led.flashled(STOP));
}