#pragma once
#include <iostream>
#include <string>

struct MapPoint {
    char c;
    int index;

    MapPoint(std::string key) {
        this->c = key[0];
        this->index = stoi(key.substr(1));
    }
};