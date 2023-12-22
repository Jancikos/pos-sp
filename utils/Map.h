//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_MAP_H
#define POS_SP_MAP_H

#include "../models/Bunk.h"
#include "BiotopeManager.h"
#include "random"

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

    static std::default_random_engine rnd;
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int x = 0; x < width; x++) {
        this->bunks[x] = new Bunk[height];
        for (int y = 0; y < height; y++) {
            auto& bunk = this->bunks[x][y];

            bunk.setX(x);
            bunk.setY(y);

            if (dist(rnd) < 0.6) {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::MEADOW));
            } else if (dist(rnd) < 0.8) {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::FOREST));
            } else if (dist(rnd) < 0.95) {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::WATER));
            } else {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::STONE));
            }
        }
    }
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

Bunk* Map::getBunks() const {
    return *bunks;
}

void Map::print() {
    std::cout << "Map: " << width << "x" << height << std::endl;
    for (int y = 0; y < height; y++) {
        std::cout << "| ";
        for (int x = 0; x < width; x++) {
            auto& bunk = this->bunks[x][y];
            std::cout << bunk.getBiotope()->getCode() << " ";
        }
        std::cout << "|" << std::endl;
    }
}

#endif //POS_SP_MAP_H
