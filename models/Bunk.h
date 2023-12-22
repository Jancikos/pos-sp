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

public:
    Bunk() = default;
    Bunk(int x, int y, Biotope* biotope) : x(x), y(y), biotope(biotope) {};
    int getX() const;
    int getY() const;
    Biotope* getBiotope() const;
    void setBiotope(Biotope* biotope);
    void setX(int x);
    void setY(int y);
    void print();
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

void Bunk::print() {
    std::cout << "Bunk: " << this->x << "x" << this->y << std::endl;
    if (this->biotope != nullptr) {
        this->biotope->print();
    } else {
        std::cout << "No biotope" << std::endl;
    }
}


#endif //POS_SP_BUNK_H
