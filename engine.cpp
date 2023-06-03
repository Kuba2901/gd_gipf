#include "engine.h"

Engine::Engine() {
    // Create an instance of rules
    this->gameRules = new GameRules(this);     

    // Instantiate the board
    this->board = new BoardPieces(this);

    // Start processing commands
    processCommands();

    this->testingMoves = false;
}

void Engine::LOAD_GAME_BOARD() {
    // Delete previous values
    this->board->clearData();
    this->gameRules->clearData();

    // Load rules values
    this->gameRules->loadRules();

    // Set the currently moving player
    this->currentlyMoving = this->gameRules->getPlayer();

    // Set remaining pieces in the board
    this->board->setRemaining();

    // Scan and cache the board
    this->board->scanBoard();

    // Check the state of the board
    gameRules->checkBoardState(board->getBoard());

}

void Engine::PRINT_GAME_BOARD() {
    if (this->getRules()->getMapCorrect()) {
        this->board->printValues();
        this->board->printOriginalBoard();
        printf("\n");
    } else {
        printf("EMPTY_BOARD\n");
    }
}

void Engine::processCommands() {
    // Define a line buffor
    std::string line;

    while (getline(std::cin, line)) {
        if (line == "LOAD_GAME_BOARD") {
            LOAD_GAME_BOARD();
        } 
        if (line == "PRINT_GAME_BOARD") {
            PRINT_GAME_BOARD();
        } 
        if (line.find("DO_MOVE") != std::string::npos) {
            DO_MOVE(line);
        }
    }
}

void Engine::DO_MOVE(std::string args) {
    // Find DO_MOVE
    size_t movePos = args.find("DO_MOVE");

    // Get arguments
    std::string arguments = args.substr(movePos + 8);
    
    // Replace the - sign with ' '
    std::replace(arguments.begin(), arguments.end(), '-', ' ');

    // Create buffers
    std::string from, to, firstIndex, secondIndex;
    char color;

    // Extract the arguments
    std::stringstream ss(arguments);

    std::vector<std::string> tokens;
    
    /*std::getline(ss, from, '-');
    std::getline(ss, to, ' ');
    std::getline(ss, color, ':');
    std::getline(ss, firstIndex, ' ');
    std::getline(ss, secondIndex, ' ');*/

    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() > 2) {
        // Start testing state
        this->testingMoves = true;

        // The move is precise
        from = tokens[0], to = tokens[1];
        color = tokens[2][0];
        std::string firstIndex = tokens[3], secondIndex = tokens[4];

        if (color == 'b') color = 'B';
        else color = 'W';

        // 1. Create a copy of the board
        std::vector<std::vector<Point*>> boardCopy = this->board->getBoard();

        // 2a. Make the move
        this->board->makeMove(from, to);

        // Define code
        int moveCode = -1;

        // 3a. Check flat
        moveCode = this->board->verifyFlat(firstIndex, secondIndex, color);

        // The move was successful
        if (moveCode == 0) {
            this->board->findFlatlineCaptures();
        }

        // 3b. Check reverse diagonal 
        if (moveCode == -1) moveCode = this->board->verifyDiagonals(true, firstIndex, secondIndex, color);
        
        // The move was successful
        if (moveCode == 0) {
            this->board->reverseDiagonalCaptures();
        }
        

        // 3c. Check diagonal
        moveCode = this->board->verifyDiagonals(false, firstIndex, secondIndex, color);

        // The move was successful
        if (moveCode == 0) {
            this->board->diagonalCaptures();
        }

        // Revert board changes in case of error
        if (moveCode) this->board->setBoard(boardCopy);

        // Get all other possible captures
        getAllCaptures();

        // Print possible errors
        if (moveCode == 1)
            printf("WRONG_COLOR_OF_CHOSEN_ROW\n");

        else if (moveCode == 2)
            printf("WRONG_INDEX_OF_CHOSEN_ROW\n");

        // Print success if no error encountered
        if (moveCode == 0) printf("MOVE_COMMITED\n");

        // End testing
        this->testingMoves = false;
    }
    else {
        // The move is not precise
        from = tokens[0], to = tokens[1];

        // Classic execution
        // Make the move
        int succes = this->board->makeMove(from, to);

        // Indicate success
        if (!testingMoves && succes != -1) printf("MOVE_COMMITTED\n");

        printf("\n");

        // Check captures
        getAllCaptures();
    }

    // Change currently moving player
    changeCurrentlyMoving();
}

GameRules* Engine::getRules() {
    return this->gameRules;
}

char Engine::getCurrentlyMoving() {
    return this->currentlyMoving;
}

void Engine::changeCurrentlyMoving() {
    if (this->currentlyMoving == 'B') {
        this->currentlyMoving = 'W';
    } else {
        this->currentlyMoving = 'B';
    }
}

BoardPieces* Engine::getBoardPieces() {
    return this->board;
}

void Engine::getAllCaptures() {
    this->board->findFlatlineCaptures();
    this->board->diagonalCaptures();
    this->board->reverseDiagonalCaptures();
}