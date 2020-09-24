#include "Pack.h"
#include "Player.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class Game {
public:
    Game(istream &packName, Player * playersIn[4])
            : teamZeroScore(0), teamOneScore(0), lastWinner(1),
            teamZeroTrickCount(0), teamOneTrickCount(0), Dealer(0),
            numHand(0), whoOrdered(0), trumpSuit(""), ledSuit("") {
            
            Pack packIn(packName);
            pack = packIn;
                
            players[0] = playersIn[0];
            players[1] = playersIn[1];
            players[2] = playersIn[2];
            players[3] = playersIn[3];
            
        };
    
    void Shuffle() {
        pack.shuffle();
        //pack.reset();
    }
    
    void Deal(){
        
        for (int i = 0; i < 3; ++i) {
            players[(Dealer + 1)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; ++i) {
            players[(Dealer + 2)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; ++i) {
            players[(Dealer + 3)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; ++i) {
            players[(Dealer)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; ++i) {
            players[(Dealer + 1)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; ++i) {
            players[(Dealer + 2)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; ++i) {
            players[(Dealer + 3)%4]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; ++i) {
            players[(Dealer)%4]->add_card(pack.deal_one());
        }
        
    }
    
    void MakeTrump() {
        
        int orderer = -1;
        Card upcard = pack.deal_one();
        trumpSuit = "";
        bool orderup = false;
        cout << "Hand " << numHand << endl;
        cout << players[Dealer % 4]->get_name() << " deals" << endl;
        cout << upcard << " turned up" << endl;
        
        /////////////ROUND ONE////////////////////
        for (int i = 1; i <= 4; ++i) {
            bool isDealer = false;
            if (i == 4) {
                isDealer = true;
            }
            if (orderup == false) {
                orderup = players[(Dealer + i) % 4]->make_trump(upcard,
                                                                isDealer,
                                                                1,
                                                                trumpSuit);
                if (orderup == true) {
                    cout << players[(Dealer + i) % 4]->get_name()
                    << " orders up " << trumpSuit << endl;
                    
                    orderer = (Dealer + i) % 4;
                }
                else {
                    cout << players[(Dealer + i) % 4]->get_name() << " passes"
                    << endl;
                }
            }
            
        }
        
        if (orderup == true) {
            players[Dealer % 4]->add_and_discard(upcard);
        }
        
        ///////////////ROUND TWO/////////////////////////
        for (int i = 1; i <= 4; ++i) {
            bool isDealer = false;
            if (i == 4) {
                isDealer = true;
            }
            if (orderup == false) {
                orderup = players[(Dealer + i) % 4]->make_trump(upcard,
                                                                isDealer,
                                                                2,
                                                                trumpSuit);
                if (orderup == true) {
                    cout << players[(Dealer + i) % 4]->get_name()
                    << " orders up " << trumpSuit << endl;
                    
                    orderer = (Dealer + i) % 4;
                }
                else {
                    cout << players[(Dealer + i) % 4]->get_name()
                    << " passes" << endl;
                }
            }
        }
        
        if (orderer % 2 == 0) {
            whoOrdered = 0;
        }
        
        else {
            whoOrdered = 1;
        }
        return;
    }
    
    void playTrick() {
        
        
        Card led_card = players[lastWinner % 4]->lead_card(trumpSuit);
        cout << led_card << " led by " << players[lastWinner % 4]->get_name()
                                                    << endl;
        Card win = led_card;
        string winner = players[(lastWinner) % 4]->get_name();
        int newWinner = (lastWinner) % 4;
        
        Card play_card_1 =
                players[(lastWinner + 1) % 4]->play_card(led_card, trumpSuit);
        if (Card_less(win, play_card_1, led_card, trumpSuit)) {
            win = play_card_1;
            winner = players[(lastWinner + 1) % 4]->get_name();
            newWinner = (lastWinner + 1) % 4;
        }
        
        
        cout << play_card_1 << " played by " <<
            players[(lastWinner + 1) % 4]->get_name() << endl;
        
        Card play_card_2 =
            players[(lastWinner + 2) % 4]->play_card(led_card, trumpSuit);
        
        cout << play_card_2 << " played by " <<
            players[(lastWinner + 2) % 4]->get_name() << endl;
        
        if (Card_less(win, play_card_2, led_card, trumpSuit)) {
            win = play_card_2;
            winner = players[(lastWinner + 2) % 4]->get_name();
            newWinner = (lastWinner + 2) % 4;
        }
        
        Card play_card_3 =
            players[(lastWinner + 3) % 4]->play_card(led_card, trumpSuit);
        
        cout << play_card_3 << " played by " <<
            players[(lastWinner + 3) % 4]->get_name() << endl;
        
        if (Card_less(win, play_card_3, led_card, trumpSuit)) {
            win = play_card_3;
            winner = players[(lastWinner + 3) % 4]->get_name();
            newWinner = (lastWinner + 3) % 4;
        }
        
        lastWinner = newWinner;
        cout << winner << " takes the trick " << endl;
        
        if (lastWinner % 2 == 0) {
            teamZeroTrickCount++;
        }
        else {
            teamOneTrickCount++;
        }
        
    }
    
    void announceHandWinner() {
        
        if (whoOrdered == 0) {
            if (teamZeroTrickCount > teamOneTrickCount) {

                cout << players[0]->get_name() << " and "
                    << players[2]->get_name() << " win the hand" << endl;
                if (teamZeroTrickCount == 3 || teamZeroTrickCount == 4) {
                    
                    teamZeroScore++;
                    
                }
                else if (teamZeroTrickCount == 5) {
                    teamZeroScore += 2;
                    cout << "march!" << endl;
                }
            }
            else {
                cout << players[1]->get_name() << " and "
                    << players[3]->get_name() << " win the hand" << endl;
                teamOneScore += 2;
                cout << "euchered!" << endl;
            }
        }
        else {
            if (teamOneTrickCount > teamZeroTrickCount) {
                cout << players[1]->get_name() << " and "
                    << players[3]->get_name() << " win the hand" << endl;
                
                if (teamOneTrickCount == 3 || teamOneTrickCount == 4) {
                    teamOneScore++;
                }
                else if (teamOneTrickCount == 5) {
                    teamOneScore += 2;
                    cout << "march!" << endl;
                }
            }
            else {
                cout << players[0]->get_name() << " and "
                << players[2]->get_name() << " win the hand" << endl;
                teamZeroScore += 2;
                cout << "euchered!" << endl;
            }
        }
        cout << players[0]->get_name() << " and " << players[2]->get_name()
                << " have " << teamZeroScore << " points" << endl;
        cout << players[1]->get_name() << " and " << players[3]->get_name()
        << " have " << teamOneScore << " points" << endl << endl;;
        Dealer++;
        numHand++;
        lastWinner = Dealer + 1;
        pack.reset();
        teamOneTrickCount = 0;
        teamZeroTrickCount = 0;
    }
    
    int getZeroScore() {
        return teamZeroScore;
    }

    int getOneScore() {
        return teamOneScore;
    }
    
    void announceWinner() {
        if (teamZeroScore > teamOneScore) {
            cout << players[0]->get_name() << " and " << players[2]->get_name()
            << " win!" << endl;
        }
        
        else {
            cout << players[1]->get_name() << " and " << players[3]->get_name()
            << " win!" << endl;
        }
    }

private:
    Player *players[4];
    Pack pack;
    int teamZeroScore;
    int teamOneScore;
    int lastWinner;
    int teamZeroTrickCount;
    int teamOneTrickCount;
    int Dealer;
    int numHand;
    bool whoOrdered;
    string trumpSuit;
    string ledSuit;
};

///////////////////////////////  DRIVER FUNCTION  //////////////////////////////

void Print_Error();
bool Check_Points(int &pointsIn);
bool Check_Shuffle(string &shuffle);
bool Check_Types(string &playerZeroType, string &playerOneType,
                 string &playerTwoType, string &playerThreeType);

int main (int argc, char ** argv) {
   
    //Checks number of arguments
    if (argc != 12) {
        Print_Error();
        return 0;
    }
    
    //Sets all variables equal to respective C-line arguments
    string fileName = argv[1];
    string shuffle = argv[2];
    int pointsIn = atoi(argv[3]);
    string playerZeroName = argv[4];
    string playerZeroType = argv[5];
    string playerOneName = argv[6];
    string playerOneType = argv[7];
    string playerTwoName = argv[8];
    string playerTwoType = argv[9];
    string playerThreeName = argv[10];
    string playerThreeType = argv[11];
    
    //Checks in file can be opened
    ifstream packIn;
    packIn.open(fileName);
    if(!packIn.is_open()) {
        cout << "Error opening " << fileName << endl;
        return 1;
    }
    
    if (Check_Points(pointsIn) || Check_Shuffle(shuffle) ||
        Check_Types(playerZeroType, playerOneType,
                    playerTwoType, playerThreeType)) {
        
        Print_Error();
        return 1;
            
    }
    
/////////////////////////////////  DECLARE GAME  ///////////////////////////////
    
    Player * player0 = Player_factory(playerZeroName, playerZeroType);
    Player * player1 = Player_factory(playerOneName, playerOneType);
    Player * player2 = Player_factory(playerTwoName, playerTwoType);
    Player * player3 = Player_factory(playerThreeName, playerThreeType);

    Player * playersIn[4] = {player0, player1, player2, player3};
    
    Game game(packIn, playersIn);
    
    
///////////////////////////////////  PLAY GAME  ////////////////////////////////
    
    while (pointsIn > game.getOneScore() && pointsIn > game.getZeroScore()) {
        //SHUFFLE
        if (shuffle == "shuffle") {
            game.Shuffle();
        }
        
        //DEAL
        game.Deal();
        
        game.MakeTrump();
        cout << endl;
        
        for (int i = 0; i < 5; ++i) {
            game.playTrick();
            cout << endl;
        }
        game.announceHandWinner();
    }
    
    game.announceWinner();
    
}




void Print_Error() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
    << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
    << "NAME4 TYPE4" << endl;
}

bool Check_Points(int &pointsIn) {
    
    if (!(pointsIn >= 1 && pointsIn <= 100)) {
        return 1;
    }
    return 0;
    
}

bool Check_Shuffle(string &shuffle) {
    
    if (!(shuffle == "shuffle" || shuffle == "noshuffle")) {
        return 1;
    }
    
    return 0;
}

bool Check_Types(string &playerZeroType, string &playerOneType,
                 string &playerTwoType, string &playerThreeType) {
    
    if ((playerOneType != "Human" && playerOneType != "Simple") ||
        (playerTwoType != "Human" && playerTwoType != "Simple") ||
        (playerThreeType != "Human" && playerThreeType != "Simple") ||
        (playerZeroType != "Human" && playerZeroType != "Simple")) {
        
        return 1;
        
    }
    
    return 0;
    
}
