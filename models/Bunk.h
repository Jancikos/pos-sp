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
    return x;
}

int Bunk::getY() const {
    return y;
}

Biotope* Bunk::getBiotope() const {
    return biotope;
}

void Bunk::setBiotope(Biotope* biotope) {
    Bunk::biotope = biotope;
}

void Bunk::setX(int x) {
    Bunk::x = x;
}

void Bunk::setY(int y) {
    Bunk::y = y;
}

void Bunk::print() {
    std::cout << "Bunk: " << x << "x" << y << std::endl;
    if (biotope != nullptr) {
        biotope->print();
    }
}


#endif //POS_SP_BUNK_H
