//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_CELL_H
#define POS_SP_CELL_H

#include "Biotope.h"

class Cell {
protected:
    int x = 0, y = 0;
    Biotope* biotope = nullptr;
    bool onFire = false;
    bool burnt = false;
    int onFireTimes = 0;

public:
    Cell() = default;
    Cell(int x, int y, Biotope* biotope) : x(x), y(y), biotope(biotope) {};
    int getX() const;
    int getY() const;
    Biotope* getBiotope() const;
    void setBiotope(Biotope* biotope);
    void setX(int x);
    void setY(int y);
    void setIsOnFire(bool isOnFire);
    bool isOnFire() const;
    void setIsBurnt(bool isBurnt);
    bool isBurnt() const;
    int getOnFireTimes() const;
    void setOnFireTimes(int fireTimes);
    void printMap();
};

int Cell::getX() const {
    return this->x;
}

int Cell::getY() const {
    return this->y;
}

Biotope* Cell::getBiotope() const {
    return this->biotope;
}

void Cell::setBiotope(Biotope* biotope) {
    this->biotope = biotope;
}

void Cell::setX(int x) {
    this->x = x;
}

void Cell::setY(int y) {
    this->y = y;
}

void Cell::printMap() {
    std::string biotopeCode = this->biotope->getCode();

    if (this->burnt) {
        // print it in dark purple
        std::cout << "\033[38;5;53m" << biotopeCode << "\033[0m";
        return;
    }

    if (this->onFire) {
        // print it in red
        std::cout << "\033[1;31m" << biotopeCode << "\033[0m";
        return;
    }

    std::cout << "\033[" << this->biotope->getColor() << "m" << biotopeCode << "\033[0m";
}

void Cell::setIsOnFire(bool isOnFire) {
    this->onFire = isOnFire;
}

bool Cell::isOnFire() const {
    return this->onFire;
}

void Cell::setIsBurnt(bool isBurnt) {
    this->burnt = isBurnt;
}

bool Cell::isBurnt() const {
    return this->burnt;
}

int Cell::getOnFireTimes() const {
    return this->onFireTimes;
}

void Cell::setOnFireTimes(int fireTimes) {
    this->onFireTimes = fireTimes;
}


#endif //POS_SP_CELL_H
