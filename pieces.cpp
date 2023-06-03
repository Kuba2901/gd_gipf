#include "pieces.h"

BoardPieces::BoardPieces(Engine *engine_): originalBoard(), boardPieces() {
    this->engine = engine_;
}

void BoardPieces::decrementWhite() {
    this->remainingWhite--;
}

void BoardPieces::decrementBlack() {
    this->remainingBlack--;
}

int BoardPieces::getRemainingWhite() {
    return this->remainingWhite;
}

int BoardPieces::getRemainingBlack() {
    return this->remainingBlack;
}

std::vector<std::vector<Point*>> BoardPieces::getBoard() {
    return this->boardPieces;
}

void BoardPieces::scanBoard() {
    int S = engine->getRules()->getS();

    // Create line buffer
    std::string line;

    // Get board dimensions
    int height = 2*(S); //2*(S-1);
    int width = 3*(S-1); // 3*(S-1);

    for (int y = 0; y < height; y++) {
        std::vector<Point*> row;

        // Get current line
        getline(std::cin, line);

        printf("BOARD READING LINE: %s\n", line.c_str());

        if (!line.empty() && line[0] != '\r') {
            for (int x = 0; x < line.length(); ++x) {
                // Adding +1 for later '+'
                row.push_back(new Point(x, y + 1, line[x]));
            }

            boardPieces.push_back(row);
        }
    }

    // Copy the original board to a different structure
    this->originalBoard = boardPieces;

    if (boardPieces.size() > 0) {
        addFreeFields(S);
        reindexFields(S);
    }
}

int BoardPieces::trimBackWhitespaces(int y) {
    int iter = 0;

    for (int i = boardPieces[y].size() - 1; i > 0 ; --i) {
        Point *curr = boardPieces[y][i];

        if (curr->c != ' ' && curr->c != '\r') {
            return iter;
        }

        iter++;
    }

    return NULL;
}

void BoardPieces::removeEmpty() {
    for (size_t y = 0; y < boardPieces.size(); y++)
    {
        if (boardPieces[y].size() == 0) {
            // printf("LINE EMPTY: %d", y);
        }
     }
    
}

void BoardPieces::printBoard() {
    for (auto pointRow : boardPieces)
    {
        for (auto point : pointRow)
        {
            printf("%c", point->c);
        }

        printf("\n");
        
    }    
}


void BoardPieces::printHashMap() {
    for (auto kv : boardMap)
    {
        std::cout << kv.first;
    }
    
}

void BoardPieces::addFreeFields(int S) {
    // Add the '+' signs to the top and bottom of the board
    std::vector<Point *> top;
    std::vector<Point *> bottom;

    for (size_t y = 0; y < boardPieces.size(); y++)
    {

        for (size_t x = 0; x < boardPieces[y].size() - 1; x++)
        {
            // Inserting to the front
            Point* curr = boardPieces[y][x];

            // The first element is not empty
            if (x == 0 && curr->c != ' ') {
                boardPieces[y].insert(boardPieces[y].begin(), new Point(0, y, '+'));
                boardPieces[y].insert(boardPieces[y].begin() + 1, new Point(1, y, ' '));
                break;
            } 
            else if (curr->c != ' ' && boardPieces[y][x+1]) {
                boardPieces[y].insert(boardPieces[y].begin() + x, new Point(x, y, '+'));
                boardPieces[y].insert(boardPieces[y].begin() + x + 1, new Point(x, y, ' '));
                break;
            }
        }

        // Push '+' sign to the end
        boardPieces[y].insert(boardPieces[y].end() - findRowEnd(boardPieces[y]), new Point(boardPieces[y].size() - 2, y, ' '));
        boardPieces[y].insert(boardPieces[y].end() - findRowEnd(boardPieces[y]) + 1, new Point(boardPieces[y].size() - 2, y, '+'));
    }

    if (boardPieces.size() > 0) {
        // Iterate over the top
        for (size_t x = 0; x < boardPieces[0].size() - 1; x++)
        {
            if (boardPieces[0][x + 1]->c != ' ' && boardPieces[0][x + 1]->c != '\r' && x >= S) {
                top.push_back(new Point(x, 0, '+'));
            } else {
                top.push_back(new Point(x, 0, ' '));
            }
        }


        // Iterate over the bottom
        for (size_t x = 0; x < boardPieces[boardPieces.size() - 1].size() - 1; x++)
        {
            if (boardPieces[boardPieces.size() - 1][x + 1]->c != ' ' && boardPieces[boardPieces.size() - 1][x + 1]->c != '\r' && x >= S) {
                bottom.push_back(new Point(x, boardPieces.size() + 1, '+'));
            } else {
                bottom.push_back(new Point(x ,boardPieces.size() + 1, ' '));
            }
        }

        // Push rows to the board
        boardPieces.insert(boardPieces.begin(), top);
        boardPieces.push_back(bottom);
    }

    // Repair indexes
    shiftIndexes();
}

int BoardPieces::findRowEnd(std::vector<Point *> row) {
    int iter = 0;

    for (int i = row.size() - 1; i >= 0; i--) {
        if (row[i]->c != ' ' && row[i]->c != '\r') {
            return iter;
        }

        iter++;
    }

    return iter;
}

// Shift indexes while inserting '+' signs
void BoardPieces::shiftIndexes() {
    for (size_t y = 0; y < boardPieces.size(); y++)
    {
        for (size_t x = 0; x < boardPieces[y].size(); x++)
        {
            boardPieces[y][x]->x = x;
            boardPieces[y][x]->y = y;
        }
    }
}

void BoardPieces::reindexFields(int S) {
    char c = 'a';

    int yCounter = 0;
    int xCounter = 0;
    int wordCounter = 0;

    for (int y = 0; y < boardPieces.size(); y++)
    {
        // Second half
        if (y > S) {
            wordCounter++;
        }

        for (size_t i = 0; i < boardPieces[y].size(); i++)
        {
            std::vector<Point *> pointRow = boardPieces[y];
            Point *curr = pointRow[i];

            // Before the line
            if (i <= 3*S-yCounter) {
                if (curr->c != ' ') {
                    // Start counting
                    if (xCounter == 0) {
                        xCounter = (y <= S - 1) ? (i + 1): 1;
                    } 
                    // Increment the counter
                    else {
                        xCounter++;
                    }

                    char character = c + wordCounter;
                    int index = xCounter;

                    std::string key;
                    key += character;
                    key += std::to_string(index);

                    insertToTheMap(key, curr);

                    // printf("%c%d", character, index);
                    
                    c++;

                } else  {
                    // Print out the space
                    // printf("%c", curr->c);
                }
                
            } 
            // After the line
            else {
                // Check the character
                if (curr->c == 'B' || curr->c == 'W' || curr->c == '_' || curr->c == '+') {
                    char character = c + wordCounter;
                    int index = xCounter;

                    std::string key;
                    key += character;
                    key += std::to_string(index);

                    insertToTheMap(key, curr);

                    // printf("%c%d", character, index);
                    c++;

                } else {
                    // Print out the space
                    // printf("%c", curr->c);
                }
            }
        }

        // printf("\n");

        yCounter++;
        xCounter = 0;

        // Reset the character
        c = 'a';
    }
}


void BoardPieces::insertToTheMap(std::string key, Point* val) {
    boardMap[key] = val;
}

void BoardPieces::printField(std::string key) {
    bool exists = boardMap.count(key);

    if (exists) {
        Point* curr = boardMap[key];
        // std::cout << "FOUND: " << curr->c << std::endl;
    } else {
        // std::cout << "DOES not EXIST" << std::endl;
    }
}

bool BoardPieces::entryExists(std::string key) {
    // printf("ENTRY %s %s\n", key.c_str(), boardMap.count(key) > 0 ? "exists": "doesn't exist");
    return boardMap.count(key);
}

bool BoardPieces::compareIndexes(std::string key1, std::string key2) {
    if (entryExists(key1) && entryExists(key2)) {
        // Get indexes from the keys
        int index1 = stoi(key1.substr(1));
        int index2 = stoi(key2.substr(1));

        return abs(index1 - index2) <= 1;
    }

    return false;
}

bool BoardPieces::isEdge(std::string key1) {
    int S = engine->getRules()->getS();
    // Get information from string 1
    char c1 = key1[0];
    int index1 = stoi(key1.substr(1));

    // Last letter
    char last = 'a' + 2*S;
    int lastMaxIndex = S + 1;

    // Top edge
    int constValue = ('a' - (S + 1));
    int currValue = c1 - index1;

    // Check letter
    if ((c1 == 'a' || c1 == last) && (index1 > 0 && index1 <= S+1)) {
        // printf("A OR LAST");
        return true;
    }

    // Index equal to 1
    if (index1 == 1) {
        // printf("INDEX EQUAL TO 1");
        return true;
    }

    // Delta equal to constValue
    if (currValue == constValue) {
        // printf("DELTA EQUAL CONSTVALUE");
        return true;
    } 

    // Right top corner - multiple comparisons
    int letterDelta = last - c1;
    int indexDelta = index1 - lastMaxIndex;

    if (letterDelta == indexDelta) {
        // printf("TOP RIGHT");
        return true;
    }

    // printf("CENTER, constValue: %d", constValue);
    return false;
}

bool BoardPieces::isCorner(std::string key) {
    // Get S
    int S = engine->getRules()->getS();

    // Get information from the string key
    char c1 = key[0];
    int index = stoi(key.substr(1));

    // Determine the max index for the first and last letter
    int flMaxIndex = S+1;

    // Determine the last letter
    char last = 'a' + 2*S;

    // Determine the max index on the board
    int boardMaxIndex = 2*S+1;

    // Determine longest diagonal letter
    char longestDiagonalLetter = 'a' + S;

    // Print data
    // printf("LONGEST DIAGONAL LETTER: %c", longestDiagonalLetter);

    // CORNER CHECKING
    // Check if it's the first or last letter and the index is 1 or S+1
    if ((c1 == 'a' || c1 == last) && (index == 1 || index == flMaxIndex)) {
        return true;
    }

    // Check if it's a corner on the longest diagonal
    if (c1 == longestDiagonalLetter && (index == 1 || index == boardMaxIndex)) {
        return true;
    }

    return false;
}

bool BoardPieces::isCornerMoveLegal(std::string key1, std::string key2) {
    // Define map points
    MapPoint *mp1 = new MapPoint(key1);
    MapPoint *mp2 = new MapPoint(key2);

    // Define index delta
    int indexDelta = abs(mp1->index - mp2->index);
    int charDelta = abs(mp1->c - mp2->c);

    // Check if the two points are next to each other
    if (indexDelta <= 1) {
        // Move a -> b
        if (mp1->c == 'a' && mp2->c == 'b') {
            return true;
        }

        // Move on the longest diagonal
        if (mp1->c == mp2->c) {
            return true;
        }

        // Move from the last letter to the one before
        if (mp1->c - mp2->c == 1) {
            return true;
        }
    }

    // No condition was fulfilled
    return false;
}

// The point is not a corner
bool BoardPieces::isMoveLegal(std::string key1, std::string key2) {
    int S = engine->getRules()->getS();

    // Check if values are in the board
    if (!entryExists(key1) || !entryExists(key2)) {
        // printf("ERROR: Entries don't exist");
        return false;
    }

    // Define map points
    MapPoint *mp1 = new MapPoint(key1);
    MapPoint *mp2 = new MapPoint(key2);

    // Check whether the first field is on the edge
    if (!isEdge(key1)) {
        // printf("ERROR: The first field is not on the edge\n");
        return false;
    }

    // Check whether the second field is not on the edge
    if (isEdge(key2)) {
        // printf("ERROR: The second field is on the edge\n");
        return false;
    }

    // Check if the first field is a corner
    if (isCorner(key1)) {

        // Check if the corner move is legal
        if (isCornerMoveLegal(key1, key2)) {
            // TODO: Add map checking
            // printf("SUCCESS: Corner move is legal\n");
            return true;
        }

        // printf("ERROR: Corner move is not legal\n");
        return false;

    }

    

    // printf("ERROR: Conditions were not met\n");
    return false;
}

bool BoardPieces::isFieldAdjacent(std::string key1, std::string key2) {
    // Define map points
    MapPoint *mp1 = new MapPoint(key1);
    MapPoint *mp2 = new MapPoint(key2);

    // Define index delta
    int indexDelta = abs(mp1->index - mp2->index);
    int charDelta = abs(mp1->c - mp2->c);

    // Check non-corner move conditions
    if (indexDelta <= 1 && charDelta <= 1) {
        // printf("SUCCESS: Move legal\n");
        return true;
    }

    return false;
}

void BoardPieces::makeMove(std::string begin, std::string dest) {
    // If any error was encountered - stop
    if (detectMovementErrors(begin, dest)) {
        return;
    }

    // TODO: Implement unknown direction error

    // Determine direction if no error was encountered
    MapPoint *start = new MapPoint(begin);
    MapPoint *end = new MapPoint(dest);

    // 1. Flat line ->
    if ((end->index - start->index == 1) && (end->c - start->c == 1)) {
        // Push to the right;
        pushFlatRow(true, begin);
        return;
    }
    // 2. Flat line <-
    else if ((end->index == start->index) && (end->c - start->c == -1)) {
        // Push to the left;
        pushFlatRow(false, begin);
        return;
    }
    // 3. Diagonal top-right
    else if ((end->c == start->c) && (end->index - start->index == 1)) {
        pushDiagonal(true, true, begin);
        return;
    }
    // 4. Diagonal bottom-left
    else if ((end->c == start->c) && (end->index - start->index == -1)) {
        pushDiagonal(true, false, begin);
        return;
    }
    // 5. Diagonal top-left
    else if ((end->c - start->c == -1) && (end->index - start->index == 1)) {
        pushDiagonal(false, true, begin);
        return;
    }
    // 6. Diagonal bottom-right
    else if ((end->c - start->c == 1) && (end->index == start->index)) {
        pushDiagonal(false, false, begin);
        return;
    }
}

// Printing out the indexes
void BoardPieces::printIndexes() {
    for (auto pointRow : boardPieces)
    {
        for (auto point : pointRow)
        {
            printf("(%d,%d) ", point->x, point->y);
        }

        printf("\n");
    }
    
}

void BoardPieces::clearData() {
    std::vector<std::vector<Point*>> boardPieces;
    this->boardPieces.clear();
    this->remainingBlack = 0;
    this->remainingBlack = 0;
    this->boardMap.clear();
}

void BoardPieces::printOriginalBoard() {
    for (auto pointRow : originalBoard)
    {
        for (auto point : pointRow)
        {
            printf("%c", point->c);
        }

        printf("\n");
        
    }    
}

void BoardPieces::findFlatlineCaptures() {
    // Define the number needed to capture
    int K = this->engine->getRules()->getK();

    // Determine vector height
    int height = boardPieces.size();

    // Define how many pawns in a row for each player
    int whiteInRow = 0;
    int blackInRow = 0;

    // Define vectors of points to be captured
    std::vector<Point *> captured;

    // Find <-> this way (starting from 1 and ending at -1 because of edges)
    for (size_t y = 0; y < height; y++)
    {
        // Determine row width
        int width = boardPieces[y].size();

        // Same here
        for (size_t x = 0; x < width; x++)
        {
            // Define the current element
            Point* curr = boardPieces[y][x];

            if (curr->c == 'W') {
                // printf("WHITE FOUND\n");
                whiteInRow++;
                blackInRow = 0;
            } 
            else if (curr->c == 'B') {
                // printf("BLACK FOUND\n");
                whiteInRow = 0;
                blackInRow++;
            } else if (curr->c == '_') {
                // printf("RESETING COUNTER\n");
                blackInRow = 0;
                whiteInRow = 0;
            }

            // Check status (TODO: Change from 4 to K)
            if (whiteInRow == K) {
                // 4 whites in a row (capture everying in this row)
                // TODO: White captures everything in that row
                // printf("POSSIBLE CAPTURE OF BLACKS IN ROW: %d\n", y);

                // Find all pawns adjacent to the white_row
                findInLine(this->engine->getCurrentlyMoving(), x, y, &captured);

                // ? Reseting counters
                whiteInRow = 0;
                blackInRow = 0;

                // TODO: Check this break;
                // break;

            }
            else if (blackInRow == K) {
                // 4 blacks in a row (capture everying in this row)
                // TODO: Black captures everything in that row
                // printf("POSSIBLE CAPTURE OF WHITES IN ROW: %d\n", y);

                // Find all pawns adjacent to the white_row
                findInLine(this->engine->getCurrentlyMoving(), x, y, &captured);

                // ? Reseting counters
                whiteInRow = 0;
                blackInRow = 0;

                // TODO: Check this break;
                // break;
            }
        }

        // Remove captured pieces from the board
        removeCapturedFromBoard(&captured);
        
        // Reset counters and vectors
        whiteInRow = 0;
        blackInRow = 0;
        captured.clear();
    }
    
}

void BoardPieces::removeCapturedFromBoard(std::vector<Point *> *captured) {
    int K = this->engine->getRules()->getK();

    // Define who's turn it is
    char turn = 0;

    // Create counters
    int whiteCounter = 0;
    int blackCounter = 0;

    // Check whether vectors are not empty and remove pieces from the board
    for (size_t i = 0; i < captured->size(); i++)
    {
        // Determine the current point
        Point *curr = captured->at(i);

        // Determine whose piece it is
        if (curr->c == 'B') 
            blackCounter++;
        else if (curr->c == 'W')
            whiteCounter++;

        if (whiteCounter == K && turn == 0) {
            turn = 'W';
        } else if (blackCounter == K && turn == 0) {
            turn = 'B';
        }

        // Remove from the board
        curr->c = '_';

        // printf("[C] %s CAPTURED!\n", curr->c == 'B' ? "BLACK": "WHITE");
    }

    // Increment values for players
    if (turn == 'B') {
        // Increment values for the black player
        // printf("BLACK REMAINING BEFORE CHANGES: %d\n", remainingBlack);
        this->remainingBlack += blackCounter;
        // printf("BLACK REMAINING AFTER CHANGES: %d\n", remainingBlack);
        
    } else {
        // Increment values for the white player
        // printf("WHiTE REMAINING BEFORE CHANGES: %d\n", remainingWhite);
        this->remainingWhite += whiteCounter;
        // printf("WHiTE REMAINING AFTER CHANGES: %d\n", remainingWhite);
    }
    
}


void BoardPieces::updateRemaining(int white, int black) {
    this->remainingWhite += white;
    this->remainingBlack += black;
}

void BoardPieces::printRemaining() {
    printf("[R] Remaining pieces: W(%d): B(%d)\n", this->remainingWhite, this->remainingBlack);
}

void BoardPieces::setRemaining() {
    this->remainingBlack = engine->getRules()->getBlackReserve();
    this->remainingWhite = engine->getRules()->getWhiteReserve();
}

void BoardPieces::findInLine(char c, int x, int y, std::vector<Point *> *vec) {
    // Get the other char
    char otherChar = c == 'B' ? 'W' : 'B';

    // Define starting indexes
    int lineX = x;
    int lineY = y;

    while (boardPieces[lineY][lineX]->c != '+') {
        Point *curr = boardPieces[lineY][lineX];

        // A player piece was encountered
        if (curr->c == c || curr->c == otherChar) {
            vec->push_back(curr);
        } 
        // Empty field encountered
        else if (curr->c == '_') {
            break;
        }

        // Decrement iterators
        lineX++;
    }

    // Reset iterators
    lineX = x;

    // Go backwards
    while (boardPieces[lineY][lineX]->c != '+') {
        Point *curr = boardPieces[lineY][lineX];

        // A player piece was encountered
        if (curr->c == c || curr->c == otherChar) {
            vec->push_back(curr);
        } 
        // Empty field encountered
        else if (curr->c == '_') {
            break;
        }

        // Decrement iterators
        lineX--;
    }
}


void BoardPieces::captureDiagonal(int x, int y, std::vector<Point *> *vec) {
    // Define diagonal values
    int diagonalX = x;
    int diagonalY = y;

    // Go down-right
    while (boardPieces[diagonalY][diagonalX]->c != '+') {
        // Get the current point
        Point *curr = boardPieces[diagonalY][diagonalX];

        // check whether a piece was encountered
        if (curr->c == 'W' || curr->c == 'B') {
            vec->push_back(curr);
        }
        // break on '_' encountered
        else if (curr->c == '_') {
            break;
        }

        // Move down-right
        diagonalX++;
        diagonalY++;
    }

    // Reset values
    diagonalX = x;
    diagonalY = y;

    // Go top-left
    while (boardPieces[diagonalY][diagonalX]->c != '+') {
        // Get the current point
        Point *curr = boardPieces[diagonalY][diagonalX];

        // check whether a piece was encountered
        if (curr->c == 'W' || curr->c == 'B') {
            vec->push_back(curr);
        }
        // break on '_' encountered
        else if (curr->c == '_') {
            break;
        }

        // Move down-left
        diagonalX--;
        diagonalY--;
    }

    // Remove captured from the board
    removeCapturedFromBoard(vec);
}


void BoardPieces::diagonalCaptures() {
    // THIS ORIENTATION - /
    int height = boardPieces.size();

    // Define the number needed to capture
    int K = this->engine->getRules()->getK();

    // Define vectors of points to be captured
    std::vector<Point *> captured;

    // Iterate from the top (1 and -1 to ignore the '+' rows)
    for (size_t y = 1; y < height - 1; y++)
    {
        // Get the starting point
        int start = findFirstDiagonalIndex(y);

        // Get the width of the row
        int width = boardPieces[y].size();

        // Iterate from left to right
        for (size_t x = start; x < width; x++)
        {
            // Ignore spaces
            if (boardPieces[y][x]->c == ' ') continue;

            // Define diagonal values
            int diagonalX = x;  
            int diagonalY = y;

            // Define how many pawns in a row for each player
            int whiteInRow = 0;
            int blackInRow = 0;

            while (diagonalY < height && diagonalX < boardPieces[diagonalY].size() && boardPieces[diagonalY][diagonalX]->c != '+') {
                // Get the point
                Point *curr = boardPieces[diagonalY][diagonalX];

                // Increment point with a certain character
                if (curr->c == 'B') {
                    blackInRow++;
                    whiteInRow = 0;
                } 
                else if (curr->c == 'W') {
                    whiteInRow++;
                    blackInRow = 0;
                }
                else if (curr->c == '_') {
                    blackInRow = 0;
                    whiteInRow = 0;
                }

                // Check states
                if (whiteInRow == K) {
                    // White has reached the K
                    // TODO: Find everything connected in this diagonal
                    captureDiagonal(diagonalX, diagonalY, &captured);

                    // Reset the counters
                    whiteInRow = 0;
                    blackInRow = 0;
                }
                else if (blackInRow == K) {
                    // Black has reached the K
                    // TODO: Find everything connected in this diagonals
                    captureDiagonal(diagonalX, diagonalY, &captured);

                    // Reset the counters
                    whiteInRow = 0;
                    blackInRow = 0;
                }

                // Move down-left
                diagonalX++;
                diagonalY++;
            }

            // Clear the vector
            captured.clear();
        }
    }
}

int BoardPieces::findFirstDiagonalIndex(int y) {
    if (!boardPieces.empty() && boardPieces[y].empty()) {
        for (size_t x = 0; x < boardPieces[y].size(); x++)
        {
            Point *curr = boardPieces[y][x];
            if (curr == nullptr) {
                printf("NULLPTR\n");
            }

            if (curr->c == '+') {
                return x+1;
            }
        }
    }

    return 100000;
}

int BoardPieces::findLastDiagonalIndex(int y) {
    for (size_t x = findFirstDiagonalIndex(y); x < boardPieces[y].size(); x++)
    {
        Point *curr = boardPieces[y][x];

        if (curr->c == '+') {
            return x;
        }
    }

    return -10;

}

void BoardPieces::reverseDiagonalCaptures() {
    // THIS ORIENTATION - /
    int height = boardPieces.size();

    // Define the number needed to capture
    int K = this->engine->getRules()->getK();

    // Define vectors of points to be captured
    std::vector<Point *> captured;

    // Iterate from the top (1 and -1 to ignore the '+' rows)
    for (size_t y = 1; y < height - 1; y++)
    {
        // Get the starting point
        int start = findFirstDiagonalIndex(y);

        // Get the width of the row
        int width = boardPieces[y].size();

        // Iterate from left to right
        for (size_t x = start; x < width; x++)
        {
            // Ignore spaces
            if (boardPieces[y][x]->c == ' ') continue;

            // Define diagonal values
            int diagonalX = x;  
            int diagonalY = y;

            // diagonalY >= 0 &&

            // Define how many pawns in a row for each player
            int whiteInRow = 0;
            int blackInRow = 0;

            // Define rules
            while (diagonalY > 0 &&  diagonalY < height && diagonalX < boardPieces[diagonalY].size() && boardPieces[diagonalY][diagonalX]->c != '+') {
                // Get the point
                Point *curr = boardPieces[diagonalY][diagonalX];

                // Increment point with a certain character
                if (curr->c == 'B') {
                    blackInRow++;
                    whiteInRow = 0;
                } 
                else if (curr->c == 'W') {
                    whiteInRow++;
                    blackInRow = 0;
                }
                else if (curr->c == '_') {
                    blackInRow = 0;
                    whiteInRow = 0;
                }

                // Check states
                if (whiteInRow == K) {
                    // White has reached the K
                    // TODO: Find everything connected in this diagonal
                    captureReverseDiagonal(diagonalX, diagonalY, &captured);

                    // Reset the counters
                    whiteInRow = 0;
                    blackInRow = 0;
                }
                else if (blackInRow == K) {
                    // Black has reached the K
                    // TODO: Find everything connected in this diagonals
                    captureReverseDiagonal(diagonalX, diagonalY, &captured);

                    // Reset the counters
                    whiteInRow = 0;
                    blackInRow = 0;
                }

                // Move top-right
                diagonalX++;
                diagonalY--;
            }   

            // Clear the vector
            captured.clear();
        }
    }
}

void BoardPieces::captureReverseDiagonal(int x, int y, std::vector<Point *> *vec) {
    // Define diagonal values
    int diagonalX = x;
    int diagonalY = y;

    // Go top-right
    while (boardPieces[diagonalY][diagonalX]->c != '+') {
        // Get the current point
        Point *curr = boardPieces[diagonalY][diagonalX];

        // check whether a piece was encountered
        if (curr->c == 'W' || curr->c == 'B') {
            vec->push_back(curr);
        }
        // break on '_' encountered
        else if (curr->c == '_') {
            break;
        }

        // Move top-right
        diagonalX++;
        diagonalY--;
    }

    // Reset values
    diagonalX = x;
    diagonalY = y;

    // Go down-left
    while (boardPieces[diagonalY][diagonalX]->c != '+') {
        // Get the current point
        Point *curr = boardPieces[diagonalY][diagonalX];

        // check whether a piece was encountered
        if (curr->c == 'W' || curr->c == 'B') {
            vec->push_back(curr);
        }
        // break on '_' encountered
        else if (curr->c == '_') {
            break;
        }

        // Move down-left
        diagonalX--;
        diagonalY++;
    }

    // Remove captured from the board
    removeCapturedFromBoard(vec);
}


void BoardPieces::pushFlatRow(bool right, std::string key) {
    // Get x and y values
    Point *pt = boardMap[key];
    int x = pt->x;
    int y = pt->y;

    // Define a temporary row to store items to be shifted
    std::vector<Point *> tempRow;

    // Check which direction
    if (right) {
        // Get the row width (-1 to avoid the '+' sign)
        int width = boardPieces[y].size() - 1;

        for (size_t i = x + 1; i < width; i++)
        {
            // Get the current point
            Point *curr = boardPieces[y][i];

            // Ignore spaces
            if (curr->c == ' ') continue;

            // Get points to be shifted
            tempRow.push_back(curr);

            // Break on empty space
            if (curr->c == '_') {
                break;
            }

            // If the + sign was encountered, the row is full
            if (curr->c == '+') {
                printf("BAD_MOVE_ROW_IS_FULL\n");

                // TODO: Repair
                // Manually change player
                this->engine->changeCurrentlyMoving();

                return;
            }
        }
    }
    // To the left
    else {
        // Find items to be shifted
        for (int i = x - 1; i >= 0; i--)
        {
            // Get the current point
            Point *curr = boardPieces[y][i];

            // Ignore spaces
            if (curr->c == ' ') continue;

            // Get points to be shifted
            tempRow.push_back(curr);

            // Break on empty space
            if (curr->c == '_') {
                break;
            }

            // If the + sign was encountered, the row is full
            if (curr->c == '+') {
                printf("BAD_MOVE_ROW_IS_FULL\n");

                // TODO: Repair
                // Manually change player
                this->engine->changeCurrentlyMoving();

                return;
            }
        }
    }

    // Shift all elements to the right
    int tempRowLen = tempRow.size();

    // Shift elements to the right
    for (int i = tempRowLen - 1; i > 0; i--) {
        tempRow[i]->c = tempRow[i - 1]->c;
    }

    // Assign the first element as current players piece
    if (tempRowLen > 0) tempRow[0]->c = engine->getCurrentlyMoving();

    // Decrement remaining pieces
    decrementRemaining();

    // Indicate success
    printf("MOVE_COMMITTED\n");
}

void BoardPieces::pushDiagonal(bool reverse, bool top, std::string key) {
    // Get x and y values
    Point *pt = boardMap[key];

    // Get the x and y values
    int x = pt->x;
    int y = pt->y;

    // Change the starting position
    // 1. Top-right
    if (reverse && top) {
        x++;
        y--;
    } 
    // 2. Bottom-left
    else if (reverse && !top) {
        x--;
        y++;
    }
    // 3. Top-left
    else if (!reverse && top) {
        x--;
        y--;
    }
    // 4. Bottom-right
    else if (!reverse && !top) {
        x++;
        y++;
    }

    // Define board height
    int height = boardPieces.size();

    // Define a vector to store points to be shifted
    std::vector<Point *> pts;

    // Check if in bounds
    while (y < height && x < boardPieces[y].size()) {
        // Current point on the map
        Point *curr = boardPieces[y][x];

        // To be sure - ignore spaces
        if (curr->c == ' ') continue;

        // Get points to be shifted
        pts.push_back(curr);

        // Break on empty space
        if (curr->c == '_') {
            break;
        }

        // If the + sign was encountered, the row is full
        if (curr->c == '+') {
            printf("BAD_MOVE_ROW_IS_FULL\n");

            // TODO: Repair
            // Manually change player
            this->engine->changeCurrentlyMoving();

            return;
        }

        // Determine which kind of diagonal
        if (!reverse) {
            // determine direction
            if (top) {
                // Decrement the iterators
                y--;
                x--;
            } 
            // going down
            else {
                // Increment the iterators
                y++;
                x++;
            }
        } else {
            // determine direction
            if (top) {
                // Move top-right
                x++;
                y--;
            } 
            // going down
            else {
                // Move down-left
                x--;
                y++;
            }

        }
    }

    // Shift all elements to the right
    int ptsLen = pts.size();

    // Shift elements to the right
    for (int i = ptsLen - 1; i > 0; i--) {
        pts[i]->c = pts[i - 1]->c;
    }

    // Assign the first element as current players piece
    pts[0]->c = engine->getCurrentlyMoving();

    // Decrement remaining pieces
    decrementRemaining();

    // Indicate success
    printf("MOVE_COMMITTED\n");
}

void BoardPieces::printPointLocation(std::string key) {
    if (entryExists(key)) {
        Point *pt = boardMap[key];

        printf("(%d, %d) - %c\n", pt->x, pt->y, pt->c);
        return;
    }

    printf("POINT DOES NOT EXIST");
}

void BoardPieces::decrementRemaining() {
    if (this->engine->getCurrentlyMoving() == 'B') {
        decrementBlack();
    } else {
        decrementWhite();
    }
}

void BoardPieces::printValues() {
    GameRules *rules = this->engine->getRules();
    int S = rules->getS();
    int K = rules->getK();
    int GW = rules->getGW();
    int GB = rules->getGB();

    printf("%d %d %d %d\n", S, K, GW, GB);
    printf("%d %d %c\n", remainingWhite, remainingBlack, this->engine->getCurrentlyMoving());
}

int BoardPieces::boardCorrect() {
    // Sum all rows
    int sum = findKElementsDiagonal() + findKElementsFlat() + findKElementsReverseDiagonal();
    return sum;

}

int BoardPieces::findKElementsReverseDiagonal() {
    // THIS ORIENTATION - /
    int height = boardPieces.size();

    // Define the number needed to capture
    int K = this->engine->getRules()->getK();

    // Define how many pawns in a row for each player
    int whiteInRow = 0;
    int blackInRow = 0;

    // Vector of ignored points
    std::vector<Point *> ignoredPoints;

    if (height > 0) {
        // Iterate over the board
        for (size_t y = 1; y < height - 1; y++)
        {
            for (int x = findLastDiagonalIndex(y) - 1; x >= 0; x--)
            {
                // Break if an x was encountered
                if (boardPieces[y][x]->c == '+') break;

                // Ignore spaces
                if (boardPieces[y][x]->c == ' ') continue;

                // Define diagonal iterators
                int diagX = x;
                int diagY = y;

                // Iterate while in bounds
                while (diagY < height && diagX < boardPieces[diagY].size() && boardPieces[diagY][diagX]->c != '+') {
                    // Get the current point
                    Point *curr = boardPieces[diagY][diagX];

                    // Break if the current point has already been marked
                    if (pointIgnored(curr, ignoredPoints)) {
                        break;
                    }

                    if (curr->c == 'B') {
                        blackInRow++;
                        whiteInRow = 0;
                    }
                    else if (curr->c == 'W') {
                        whiteInRow++;
                        blackInRow = 0;
                    }
                    else if (curr->c == '_') {
                        blackInRow = 0;
                        whiteInRow = 0;
                    }

                    // Check state
                    if (blackInRow == K || whiteInRow == K) {
                        // Add final point to ignored
                        ignoredPoints.push_back(curr);
                    }

                    // Move down-left
                    diagX--;
                    diagY++;
                }

                whiteInRow = 0;
                blackInRow = 0;
            }

        }
    }
    
    return ignoredPoints.size();
}

int BoardPieces::findKElementsDiagonal() {
    int height = boardPieces.size();

    // Define the number needed to capture
    int K = this->engine->getRules()->getK();

    // Counters
    int whiteInRow = 0;
    int blackInRow = 0;

    // Ignored points vector
    std::vector<Point *> ignoredPoints;

    if (height > 0) {
        // Iterate over the board
        for (size_t y = 1; y < boardPieces.size() - 1; y++) {
            // Find the first index
            for (size_t x = findFirstDiagonalIndex(y); x < boardPieces[y].size() - 1; x++)
            {
                // Ignore spaces
                if (boardPieces[y][x]->c == ' ') continue;

                // Define diagonal iterators
                int diagX = x;
                int diagY = y;

                // Iterate while in bounds
                while (diagY < height && diagX < boardPieces[diagY].size() && boardPieces[diagY][diagX]->c != '+') {
                    // Get the current point
                    Point *curr = boardPieces[diagY][diagX];

                    // Break if the current point has already been marked
                    if (pointIgnored(curr, ignoredPoints)) {
                        break;
                    }

                    if (curr->c == 'B') {
                        blackInRow++;
                        whiteInRow = 0;
                    }
                    else if (curr->c == 'W') {
                        whiteInRow++;
                        blackInRow = 0;
                    }
                    else if (curr->c == '_') {
                        blackInRow = 0;
                        whiteInRow = 0;
                    }

                    // Check state
                    if (blackInRow == K || whiteInRow == K) {
                        // Add final point to ignored
                        ignoredPoints.push_back(curr);
                    }

                    // Move down-right
                    diagX++;
                    diagY++;
                }

                whiteInRow = 0;
                blackInRow = 0;
            }
        }
    }

    return ignoredPoints.size();
}

int BoardPieces::findKElementsFlat() {
    // Define the number needed to capture
    int K = this->engine->getRules()->getK();

    // Determine vector height
    int height = boardPieces.size();
    int rowCount = 0;

    if (height > 0) {
        // Find <-> this way (starting from 1 and ending at -1 because of edges)
        for (size_t y = 0; y < height; y++)
        {
            // Determine row width
            int width = boardPieces[y].size();

            // Define how many pawns in a row for each player
            int whiteInRow = 0;
            int blackInRow = 0;

            // Same here
            for (size_t x = 0; x < width; x++)
            {
                // Define the current element
                Point* curr = boardPieces[y][x];

                if (curr->c == 'W') {
                    // printf("WHITE FOUND\n");
                    whiteInRow++;
                    blackInRow = 0;
                }
                else if (curr->c == 'B') {
                    // printf("BLACK FOUND\n");
                    whiteInRow = 0;
                    blackInRow++;
                } else if (curr->c == '_') {
                    // printf("RESETING COUNTER\n");
                    blackInRow = 0;
                    whiteInRow = 0;
                }

                // Check status (TODO: Change from 4 to K)
                if (whiteInRow == K || blackInRow == K) {
                    // printf("FOUND FLAT K\n");
                    // printf("(%d, %d)\n", x, y);
                    rowCount++;

                    whiteInRow = 0;
                    blackInRow = 0;

                    break;
                }
            }
        }
    }

    return rowCount;
}


bool BoardPieces::pointIgnored(Point *pt, std::vector<Point *> ignoredPoints) {
    for (auto point : ignoredPoints)
    {
        if (point->x == pt->x && point->y == pt->y) return true;
    }

    return false;   
}

int BoardPieces::countRepetitions(std::vector<std::vector<Point *>> rows) {
    // Remove repetitions
    int repeatedRows = 0;

    if (rows.size() > 0) {
        // Iterate over vector of vectors
        for (size_t i = 0; i < rows.size() - 1; i++)
        {
            // Get the row
            std::vector<Point *> row = rows[i];
            std::vector<Point *> nextRow = rows[i + 1];

            // Iterate over the row
            for (size_t j = 0; j < row.size(); j++)
            {
                // Flag
                bool repetitionFound = false;

                // Get the point
                Point *curr = row[j];

                // Iterate over points in the next row
                for (size_t k = 0; k < nextRow.size(); k++)
                {
                    // Get the point in the other row
                    Point *nextCurr = nextRow[k];

                    // Compare points
                    if (curr->x == nextCurr->x && curr->y == nextCurr->y) {
                        repetitionFound = true;
                        break;
                    }
                }

                if (repetitionFound) {
                    repeatedRows++;
                    break;
                }
                
            }
            
        }
    }    


    return repeatedRows;
}

bool BoardPieces::detectMovementErrors(std::string begin, std::string dest) {
    // The first field out of bounds
    if (!entryExists(begin)) {
        printf("BAD_MOVE_%s_IS_WRONG_INDEX\n", begin.c_str());
        return true;
    }
    //The second field is out of bounds
    if (!entryExists(dest)) {
        printf("BAD_MOVE_%s_IS_WRONG_INDEX\n", dest.c_str());
        return true;
    }
    // The first field is not on edge
    if (!isEdge(begin)) {
        printf("BAD_MOVE_%s_IS_WRONG_STARTING_FIELD\n", begin.c_str());
        return true;
    }
    // The second field is not adjacent
    if (!isFieldAdjacent(begin, dest)) {
        printf("UNKNOWN_MOVE_DIRECTION\n");
        return true;
    }
    // The destination field is incorrect
    if (isEdge(dest)) {
        printf("BAD_MOVE_%s_IS_WRONG_DESTINATION_FIELD\n", dest.c_str());
        return true;
    }

    return false;
}