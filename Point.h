#pragma once

struct Point {
    int x, y;
    char c;

    Point() {
        this->x = 0;
        this->y = 0;
    }

    Point(int x_, int y_) {
        this->x = x_;
        this->y = y_;
        this->c = ' ';
    }

    Point(int x_, int y_, char c_) {
        this->x = x_;
        this->y = y_;
        this->c = c_;
    }

    Point(const Point& other) {
        this->x = other.x;
        this->y = other.y;
        this->c = other.c;
    }


    bool operator==(const Point& other) {
        return (x == other.x && y == other.y);
    }

};