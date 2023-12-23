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
    bool isOnFire = false;

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
    bool getIsOnFire() const;
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

    if (this->isOnFire) {
        // print it in red
        std::cout << "\033[1;31m" << biotopeCode << "\033[0m";
        return;
    }

    if (this->biotope->getTitle() == "Water") {
        // print it in blue
        std::cout << "\033[1;34m" << biotopeCode << "\033[0m";
        return;
    }

    if (this->biotope->getTitle() == "Stone") {
        // print it in light gray
        std::cout << "\033[1;37m" << biotopeCode << "\033[0m";
        return;
    }

    if (this->biotope->getTitle() == "Forest") {
        // print it in dark green
        std::cout << "\033[1;32m" << biotopeCode << "\033[0m";
        return;
    }

    if (this->biotope->getTitle() == "Meadow") {
        // print it in light green
        std::cout << "\033[1;92m" << biotopeCode << "\033[0m";
        return;
    }

    std::cout << biotopeCode;
}

void Cell::setIsOnFire(bool isOnFire) {
    this->isOnFire = isOnFire;
}

bool Cell::getIsOnFire() const {
    return this->isOnFire;
}


#endif //POS_SP_CELL_H
