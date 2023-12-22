//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_MAP_H
#define POS_SP_MAP_H

#include "../models/Bunk.h"
#include "BiotopeManager.h"

class Map {
private:
    int width, height;
    Bunk** bunks;

public:
    Map(int width, int height) : width(width), height(height) {
        this->initializeBunks();
    };
    void initializeBunks();
    int getWidth() const;
    int getHeight() const;
    Bunk* getBunks() const;
    void print();
};

void Map::initializeBunks() {
    this->bunks = new Bunk*[width];

    auto* biotopManager = BiotopeManager::getInstance();

    for (int x = 0; x < width; x++) {
        this->bunks[x] = new Bunk[height];
        for (int y = 0; y < height; y++) {
            auto& bunk = this->bunks[x][y];

            bunk.setX(x);
            bunk.setY(y);
            bunk.setBiotope(BiotopeManager::getInstance()->getBiotop(Biotopes::MEADOW));
        }
    }
}

#endif //POS_SP_MAP_H
