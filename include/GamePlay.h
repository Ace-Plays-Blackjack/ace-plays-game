#ifndef PLAY_GAME_H 
#define PLAY_GAME_H

#include "Card.h"
#include "CallbackLinker.h"

class GamePlay
{
private:
    int accum_cntr = 0;
    std::vector<int> card_accum;
    void accumulator();
public:
    GamePlay(/* args */);
    ~GamePlay();
    void nextCallback(cv::Mat &nextFrame){}; // Unused
    /* Overlead the nextCallback function */
    void nextCallback(Card_params &Card_params);
};

GamePlay::GamePlay(/* args */){
}

void GamePlay::accumulator(){

}

void GamePlay::nextCallback(Card_params &Card_params){
    /* Increment counter everytime new detections arrive */
    accum_cntr ++;
    /* Received Card_params need to be accumulated */
    std::vector<cv::String> card_names = Card_params.card_names;



}


GamePlay::~GamePlay(){
}



#endif /* PLAY_GAME_H */