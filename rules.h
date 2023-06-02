#pragma once
#include <iostream>
#include <vector>
#include "Point.h"
#include "engine.h"

class Engine;


class GameRules {
    // The board size expressed as the number of hexagonal board fields on each side of the board
    int S;

    // The number of player pieces that trigger the collection of pieces 
    int K;

    // The number of pieces belonging to the white player
    int GW;

    // The number of pieces belonging to the black player
    int GB;

    // Determine how many player pieces are on board
    int whiteOnBoard;
    int blackOnBoard;

    // Determine whose turn it is 
    char player;

    // Determine whether the map is correct
    bool mapCorrect;

    // Engine ptr
    Engine *engine;

    public:
        // The default game rules constructor 
        GameRules(Engine *engine);

        virtual void clearData();

        virtual void loadRules();

        /*
        @returns
        0 - false
        1 - true
        */
        virtual int checkRules();

        virtual int checkK();

        virtual int checkGW();

        virtual int checkGB();

        virtual int getS();

        virtual int getK();

        virtual int getGW();

        virtual int getGB();

        virtual char getPlayer();

        virtual void setS(int s);

        virtual void setK(int k);

        virtual void setGW(int gw);

        virtual void setGB(int gb);

        virtual void getOnBoard();

        virtual int getWhiteReserve();
        
        virtual int getBlackReserve();

        /*
        @returns
        0 - BOARD_STATE_OK
        1 - WRONG_WHITE_PAWNS_NUMBER - zła liczba białych pionów
        2 - WRONG_BLACK_PAWNS_NUMBER - zła liczba czarnych pionów
        3 - WRONG_BOARD_ROW_LENGTH - zła długość wiersza planszy
        */
        virtual int checkBoardState(std::vector<std::vector<Point *>> boardPieces);

        virtual int getUnderscoreGoal();

        virtual bool getMapCorrect();

        virtual void printRules();
};