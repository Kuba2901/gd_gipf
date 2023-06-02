#include "rules.h"

// The default game rules constructor 
GameRules::GameRules(Engine *engine) {
    this->engine = engine;
}

void GameRules::clearData() {
    this->S = 0;
    this->K = 0;
    this->GW = 0;
    this->GB = 0;
    this->whiteOnBoard = 0;
    this->blackOnBoard = 0;
    this->player = 0;
}

void GameRules::loadRules() {
    int S_, K_, GW_, GB_;
    std::cin >> S_ >> K_ >> GW_ >> GB_;
    // printf("S: %d, K: %d, GW: %d, Gb: %d\n", S_, K_, GW_, GB_);

    // Assign values
    this->S = S_;
    this->K = K_;
    this->GW = GW_;
    this->GB = GB_;

    // Count the pieces on the board
    getOnBoard();
}

/*
@returns
0 - false
1 - true
*/
int GameRules::checkRules() {
    return checkK() && checkGW() && checkGB();
}

int GameRules::checkK() {
    return (K > 2 && K < (2*S-1));
}

int GameRules::checkGW() {
    return GW > 3;
}

int GameRules::checkGB() {
    return GB > 3;
}

int GameRules::getS() {
    return this->S;
}

int GameRules::getK() {
    return this->K;
}

int GameRules::getGW() {
    return this->GW;
}

int GameRules::getGB() {
    return this->GB;
}

void GameRules::setS(int s) {
    this->S = s;
}

void GameRules::setK(int k) {
    this->K = k;
}

void GameRules::setGW(int gw) {
    this->GW = gw;
}

void GameRules::setGB(int gb) {
    this->GB = gb;
}

void GameRules::getOnBoard() {
    int white, black;
    char player_;

    std::cin >> white >> black >> player_;

    this->whiteOnBoard = GW - white;
    this->blackOnBoard = GB - black;
    this->player = player_;
}

/*
@returns
0 - BOARD_STATE_OK
1 - WRONG_WHITE_PAWNS_NUMBER - zła liczba białych pionów
2 - WRONG_BLACK_PAWNS_NUMBER - zła liczba czarnych pionów
3 - WRONG_BOARD_ROW_LENGTH - zła długość wiersza planszy
*/
int GameRules::checkBoardState(std::vector<std::vector<Point *>> boardPieces) {
    // Pawn counters
    int counterWhite = 0;
    int counterBlack = 0;
    
    // TODO: Change later (might not work)
    int _goal = getUnderscoreGoal();
    int _counter = 0;

    // Count elements
    for (auto pointRow : boardPieces)
    {
        for (auto point : pointRow)
        {
            if (point->c == 'W') {
                counterWhite++;
            } else if (point->c == 'B') {
                counterBlack++;
            } else if (point->c == '_') {
                _counter++;
            }
        }
        
    }
    
    // Decrement the goal
    _goal -= counterWhite + counterBlack;

    int incorrectRows = this->engine->getBoardPieces()->boardCorrect();


    // Return the message
    if (counterWhite > this->whiteOnBoard) {
        // std::cout << "White on board: "  << this->whiteOnBoard << ", counter: " << counterWhite  << std::endl;
        std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
        this->mapCorrect = false;
        return 1;
    } else if (counterBlack > this->blackOnBoard) {
        // std::cout << "White on board: "  << this->blackOnBoard << ", counter: " << counterWhite  << std::endl;
        std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
        this->mapCorrect = false;
        return 2;
    } 
    // Check the len' of each side TODO: 
    else if (_goal != _counter) {
        // std::cout << "GOAL: "  << _goal << ", counter: " << _counter << std::endl;
        std::cout << "WRONG_BOARD_ROW_LENGTH\n";
        this->mapCorrect = false;
        return 3;
    }
    // Check if contains any n rows
    else if (incorrectRows > 0) {
        incorrectRows == 1 ? std::cout << "ERROR_FOUND_" << incorrectRows << "_ROW_OF_LENGTH_K " << std::endl : std::cout << "ERROR_FOUND_" << incorrectRows << "_ROWS_OF_LENGTH_K " << std::endl;
        printf("\n");
        this->mapCorrect = false;
        return 0;
    }
    else {
        std::cout << "BOARD_STATE_OK\n" << std::endl;
        this->mapCorrect = true;
        return 0;
    }
    
    
}

int GameRules::getUnderscoreGoal() {
    int g = 0;
    int i = S - 2;

    while (i >= 0) {
        int curr = S + i;

        // *2 because of hexagon's symmetry
        g += 2*curr;

        // Decrement the iterator
        i--;
    }

    g += 2 * S - 1;
    return g;
}

bool GameRules::getMapCorrect() {
    return this->mapCorrect;
}

void GameRules::printRules() {
    int delWhite = GW - whiteOnBoard;
    int delBlack = GB - blackOnBoard;

    printf("%d %d %d %d\n", S, K, GW, GB);
    printf("%d %d %c\n", delWhite, delBlack, player);
}

int GameRules::getBlackReserve() {
    return this->GB - this->blackOnBoard;
}

int GameRules::getWhiteReserve() {
    return this->GW - this->whiteOnBoard;
}

char GameRules::getPlayer() {
    return this->player;
}