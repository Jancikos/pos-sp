//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_BUNK_H
#define POS_SP_BUNK_H

#include "Biotope.h"

class Bunk {
protected:
    int x = 0, y = 0;
    Biotope* biotope = nullptr;
    bool isOnFire = false;

public:
    Bunk() = default;
    Bunk(int x, int y, Biotope* biotope) : x(x), y(y), biotope(biotope) {};
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

int Bunk::getX() const {
    return this->x;
}

int Bunk::getY() const {
    return this->y;
}

Biotope* Bunk::getBiotope() const {
    return this->biotope;
}

void Bunk::setBiotope(Biotope* biotope) {
    this->biotope = biotope;
}

void Bunk::setX(int x) {
    this->x = x;
}

void Bunk::setY(int y) {
    this->y = y;
}

void Bunk::printMap() {
    char biotopeCode = this->biotope->getCode();

    if (this->isOnFire) {
        // print it in red
        std::cout << "\033[1;31m" << biotopeCode << "\033[0m";
        return;
    }

    std::cout << biotopeCode;
}

void Bunk::setIsOnFire(bool isOnFire) {
    this->isOnFire = isOnFire;
}

bool Bunk::getIsOnFire() const {
    return this->isOnFire;
}


#endif //POS_SP_BUNK_H
