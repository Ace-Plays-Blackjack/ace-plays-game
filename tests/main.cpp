#include <gtest/gtest.h>
#include "Card.h"
#include "Camera.h"
#include "CardDiscriminator.h"
#include "DetectCard.h"
#include "GamePlay.h"
#include "Leds.h"
#include "StrategyEngine.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
