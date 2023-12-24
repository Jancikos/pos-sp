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


#endif //POS_SP_CELL_H
