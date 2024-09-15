#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "pieces.h"
#include "rules.h"
#include "Point.h"

class BoardPieces;
class GameRules;

class Engine {
    // Keeping track of the current cursor position
    Point *cursorPos;

    /*
    @brief The indicator of whose turn it is
    @returns
    'W' - white 
    'B' - black
    */
    char currentlyMoving;

    // Game rules
    GameRules *gameRules;

    // Keep track of the board structure
    BoardPieces *board;

    // Keep track whether a test move is in progress
    bool testingMoves;
    bool errorEncountered;


    public:
        Engine();
        
        virtual void LOAD_GAME_BOARD();

        virtual void PRINT_GAME_BOARD();

        virtual void processCommands();

        virtual void DO_MOVE(std::string args);

        virtual GameRules* getRules();

        virtual char getCurrentlyMoving();

        virtual void changeCurrentlyMoving();

        virtual BoardPieces* getBoardPieces();

        virtual void getAllCaptures();

        virtual void setTesting(bool testing_);

        virtual void setError(bool error_);

};