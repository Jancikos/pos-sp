//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_BUNK_H
#define POS_SP_BUNK_H

#include "Biotope.h"

class Bunk {
protected:
    int x, y = 0;
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


#endif //POS_SP_BUNK_H
