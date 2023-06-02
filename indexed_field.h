#pragma once
#include <string>
#include <sstream>


struct IndexedField {
    int x, y;
    std::string index;

    IndexedField(int x_, int y_, char c, int num) {
        this->x = x_;
        this->y = y_;

        std::ostringstream oss;
        oss << c << num;
        std::string var = oss.str();

        this->index = var;
    }
};