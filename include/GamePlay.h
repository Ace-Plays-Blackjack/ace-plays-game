#ifndef PLAY_GAME_H 
#define PLAY_GAME_H

#include "Card.h"
#include "CallbackLinker.h"

#define ACCUM_CNTR_THRESH 50
class GamePlay
{
private:
    int accum_cntr = 0;
    std::vector<int> card_accum;
    void accumulator();
    std::vector<int> convertStr2Int();

public:
    GamePlay(/* args */);
    ~GamePlay();
    void nextCallback(cv::Mat &nextFrame){}; // Unused
    /* Overlead the nextCallback function */
    void nextCallback(Card_params &Card_params);
};

GamePlay::GamePlay(/* args */){
}

std::vector<int> GamePlay::convertStr2Int(std::vector<cv::String> &card_names){
    std::vector<int> cards_int;

    for (int i = 0; i < card_names.size(); i++)
    {
        switch (card_names[i]){
            case "Ace":
                /* Ace has value of 11 */
                cards_int.push_back(11);
                break;
            case "Two":
                cards_int.push_back(2);
                break;
            case "Three":
                cards_int.push_back(3);
                break;
            case "Four":
                cards_int.push_back(4);
                break;
            case "Five":
                cards_int.push_back(5);
                break;
            case "Six":
                cards_int.push_back(6);
                break;
            case "Seven":
                cards_int.push_back(7);
                break;
            case "Eight":
                cards_int.push_back(8);
                break;
            case "Nine":
                cards_int.push_back(9);
                break;
            case "Ten":
                cards_int.push_back(10);
                break;
            case "Jack":
                /* Jack has value of 10 */
                cards_int.push_back(10);
                break;
            case "Queen":
                /* Queen has value of 10 */
                cards_int.push_back(10);
                break;
            case "King":
                /* King has value of 10 */
                cards_int.push_back(10);
                break;
            
            default:
                break;
        }
    std::cout << "Int Card " + std::to_string(i) + ":" << cards_int[i] << std::endl;
    }
    return cards_int;
}

void GamePlay::accumulator(){

}

void GamePlay::nextCallback(Card_params &Card_params){
    /* Increment counter everytime new detections arrive */
    accum_cntr ++;
    /* Received Card_params need to be accumulated */
    std::vector<cv::String> card_names = Card_params.card_names;
    /* Convert all detections to their Blackjack value */
    std::vector<int> cards_names_int = convertStr2Int(card_names);

    /* Accumulate results */
    // accumulator();

    /* If counter expires, check most prominent result */
    if (accum_cntr == ACCUM_CNTR_THRESH){
        /* Print out the detected value once */

        /* Reset counter */
        accum_cntr = 0;
    }

}


GamePlay::~GamePlay(){
}



#endif /* PLAY_GAME_H */