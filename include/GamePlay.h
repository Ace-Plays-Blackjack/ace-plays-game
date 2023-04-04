#ifndef PLAY_GAME_H 
#define PLAY_GAME_H

#include "Card.h"
#include "CallbackLinker.h"

#define ACCUM_CNTR_THRESH 50
class GamePlay : public CallbackLinker
{
private:
    int accum_cntr = 0;
    std::vector<int> card_accum;
    void accumulator();
    std::vector<int> convertStr2Int(std::vector<cv::String> &card_names);

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
        if (card_names[i] == "Ace"){
            /* Ace has value of 11 */
            cards_int.push_back(11);
        }
        else if (card_names[i] == "Two"){
            cards_int.push_back(2);
        }
        else if (card_names[i] == "Three"){
            cards_int.push_back(3);
        }
        else if (card_names[i] == "Four"){
            cards_int.push_back(4);
        }
        else if (card_names[i] == "Five"){
            cards_int.push_back(5);
        }
        else if (card_names[i] == "Six"){
            cards_int.push_back(6);
        }
        else if (card_names[i] == "Seven"){
            cards_int.push_back(7);
        }
        else if (card_names[i] == "Eight"){
            cards_int.push_back(8);
        }
        else if (card_names[i] == "Nine"){
            cards_int.push_back(9);
        }
        else if (card_names[i] == "Ten"){
            cards_int.push_back(10);
        }
        else if (card_names[i] == "Jack"){
            /* Jack has value of 10 */
            cards_int.push_back(10);
        }
        else if (card_names[i] == "Queen"){
            /* Queen has value of 10 */
            cards_int.push_back(10);
        }
        else if (card_names[i] == "King"){
            /* King has value of 10 */
            cards_int.push_back(10);
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