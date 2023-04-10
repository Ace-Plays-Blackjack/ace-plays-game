#include "GamePlay.h"

GamePlay::GamePlay(double res_w, double res_h):leds(),game_engine(){
    /* Get resolution of frame */
    frame_w = res_w;
    frame_h = res_h;
    frame_w_midpoint = (int)(frame_w/2);
    frame_h_midpoint = (int)(frame_h/2);
    /* Reset LEDs at startup */
    leds.flashled(STOP);
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
    } 
    return cards_int;
}


void GamePlay::clear_whosHand(){
    /* Clear the whos_hand vector when done */
    // if (!whos_hand.empty()){
    whos_hand.clear();
    num_dealer_cards = 0;
    num_player_cards = 0;
}

void GamePlay::whosHand(cv::Point_<int> &card_midpoint){
    /* Camera view is upside down, hence top of frame will be 
     * for the player (top of frame has smaller y value)*/
    if (card_midpoint.y > frame_h_midpoint){
        num_dealer_cards ++;
        whos_hand.push_back(true);
    }
    else if(card_midpoint.y < frame_h_midpoint){
        num_player_cards ++;
        whos_hand.push_back(false);
    }
}

void GamePlay::accumulator(std::vector<int> &cards_names_int, std::vector<cv::Point_<int>> &cards_centre_pts){
    
    if (accum_cntr == ACCUM_CNTR_THRESH){
        /* Reset counter */
        accum_cntr = 0;
        /* Decide on card value */
        // identifyHand();

        if (!card_accum.empty()){
            card_accum.clear();
        }
        if (!card_midpoint_accum.empty()){
            card_midpoint_accum.clear();
        }
    }
    else{
        card_accum.push_back(cards_names_int);
        card_midpoint_accum.push_back(cards_centre_pts);
        /* Increment counter everytime new detections arrive */
        accum_cntr ++;
    }
}

void GamePlay::game_reset(){
    gameStarted = false;
    total_cards = 0;
    prev_total_cards = 0;
    clear_whosHand();
    dealersHand.cards.clear();
    dealersHand.card_midpoint.clear();
    playersHand.cards.clear();
    playersHand.card_midpoint.clear();
    leds.flashled(STOP);
}

void GamePlay::play_game(std::vector<int> cards_played, std::vector<cv::Point_<int>> cards_centre_pts){

    /* Get how many new cards have been played */
    int new_cards_played = total_cards - prev_total_cards;
    /* If 3, then we started the game. Clear the hand. */
    if (new_cards_played == 3){
        clear_whosHand();
    }

    /* At the game start, prev_total_cards == 0 and new_cards_played == 3 */
    /* When new card is played, prev_total_cards == 3 and total_cards == 4*/
    for (int i = prev_total_cards; i < total_cards; i++){
        whosHand(cards_centre_pts[i]);
        /* If true, card in dealer's hand */
        if (whos_hand[i]){
            dealersHand.cards.push_back(cards_played[i]);
            dealersHand.card_midpoint.push_back(cards_centre_pts[i]);
        }
        else if (!whos_hand[i]){
            playersHand.cards.push_back(cards_played[i]);
            playersHand.card_midpoint.push_back(cards_centre_pts[i]);
        }
    }

    /* Demonstration of Strategy Engine */
    /* Dealer only plays one card */
    decisions choice = game_engine.getchoice(dealersHand.cards[0], playersHand.cards);
    /* Demonstration of LED Toggling*/
    leds.flashled(choice);
    /* New card has been played, hence make prev equal to current */
    prev_total_cards = total_cards;
}


void GamePlay::nextCallback(AcePlaysUtils &callbackData){
    Card_params Card_params = callbackData.cardParams;
    /* Received Card_params need to be accumulated */
    std::vector<cv::String> card_names = Card_params.card_names;
    /* Cards centerpoints index position corresponds to each card */
    std::vector<cv::Point_<int>> cards_centre_pts = Card_params.centre_pts; 
    /* Convert all detections to their Blackjack value */
    std::vector<int> cards_names_int = convertStr2Int(card_names);
    
    /* Game starts when 3 cards are present in the field of view 
     * 1 for the dealer and 2 for the player. If new game hasn't started: */
    if (!gameStarted && cards_names_int.size() == cards_centre_pts.size() && cards_names_int.size() == 3){
        for (int i = 0; i < cards_names_int.size(); i++)
        {          
            whosHand(cards_centre_pts[i]);
        }
        /* Check if 1 card in dealers side and 2 cards in players side */
        if (num_dealer_cards == 1 && num_player_cards == 2){
            gameStarted = true;
            total_cards = 3;
        }
        else if (num_dealer_cards > 1 && num_player_cards > 2){
            clear_whosHand();
            gameStarted = false;
        }
    }
    

    int font_size = 1;//Declaring the font size//
    cv::Scalar font_Color_GS(0, 0, 255);//Declaring the color of the font//
    int font_weight = 2;//Declaring the font weight//
    int x_offset = 30;
    
    /* By this point it is verified that the game has started */
    if (gameStarted){
        cv::putText(Card_params.currentFrame, "Game Started!",cv::Point(25,25),cv::FONT_HERSHEY_COMPLEX, 1,font_Color_GS, font_weight);
        /* Accumulate results */
        // accumulator(cards_names_int, cards_centre_pts);

        /* If new cards played, then play the game */
        if(cards_names_int.size() > total_cards){
            total_cards++;
        }
        else if(cards_names_int.size() == 0){
            /* Game Reset */
            game_reset();
        }
        /* This check also verifies that ocassionally missed detections */
        /* will not trigger wrong counting */
        if(total_cards > prev_total_cards){
            play_game(cards_names_int, cards_centre_pts);
        }
    }

    try
    {
        /* Print out the detected value once */
        for (int i = 0; i < cards_names_int.size(); i++){
            cv::Point text_position((cards_centre_pts[i].x - x_offset), cards_centre_pts[i].y);
            cv::String text_show;
            text_show = card_names[i];
            cv::Scalar font_Color(255, 0, 0);//Declaring the color of the font//

            /* If accum_cntr has been reset */
            if (accum_cntr == 0){
                /* Change card color to green if the result is locked */
                font_Color = {0,255,0};

            }
            
            cv::putText(Card_params.currentFrame, text_show, text_position,cv::FONT_HERSHEY_COMPLEX, font_size,font_Color, font_weight);
        
        }
        cv::imshow("Frame", Card_params.currentFrame);

    }
    catch (cv::Exception& e)
    {
        const char* err_msg = e.what();
        std::cout << "Exception caught: cv::imshow:\n" << err_msg << std::endl;
    }

}


GamePlay::~GamePlay(){
}
