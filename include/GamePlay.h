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