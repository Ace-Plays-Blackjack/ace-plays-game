/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PLAY_GAME_H 
#define PLAY_GAME_H

#include "Leds.h"
#include "StrategyEngine.h"
#include "Card.h"
#include "CallbackLinker.h"

#define ACCUM_CNTR_THRESH 4
class GamePlay : public CallbackLinker
{
private:
    /* y increments from left to right (width) */
    /* x increments from top to bottom (height) */
    double frame_w, frame_h = 0;
    double frame_w_midpoint, frame_h_midpoint = 0;
    bool gameStarted = false;
    int num_dealer_cards = 0;
    int num_player_cards = 0;
    int total_cards = 0;
    int prev_total_cards = 0;
    ToggleLED leds;
    StrategyEngine game_engine;

    struct Hand{
        std::vector<int> cards;
        std::vector<cv::Point_<int>> card_midpoint;
    };
    Hand playersHand;
    Hand dealersHand;
    /* Helps find index of which card belongs to who */
    std::vector<bool> whos_hand; /* true == player, false == dealer*/

    int accum_cntr = 0;
    std::vector<std::vector<int>> card_accum;
    std::vector<std::vector<cv::Point_<int>>> card_midpoint_accum;

    void accumulator(std::vector<int> &cards_names_int, std::vector<cv::Point_<int>> &cards_centre_pts);
    std::vector<int> convertStr2Int(std::vector<cv::String> &card_names);
    void whosHand(cv::Point_<int> &card_midpoint);
    void clear_whosHand();
    // void identifyHand();
    void play_game(std::vector<int> cards_played, std::vector<cv::Point_<int>> cards_centre_pts);
    void game_reset();

public:
    GamePlay(double res_w, double res_h);
    ~GamePlay();
    void nextCallback(AcePlaysUtils &callbackData);
};

#endif /* PLAY_GAME_H */