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
    Bunk** getBunks() const;
    void print();

    void spreadFire();

    bool isOutOfMap(int x, int y);
};

void Map::initializeBunks() {
    this->bunks = new Bunk*[this->width];

    auto* biotopManager = BiotopeManager::getInstance();

    static std::default_random_engine rnd;
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int x = 0; x < this->width; x++) {
        this->bunks[x] = new Bunk[this->height];
        for (int y = 0; y < this->height; y++) {
            auto& bunk = this->bunks[x][y];

            bunk.setX(x);
            bunk.setY(y);

            auto num = dist(rnd);
            if (num < 0.6) {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::MEADOW));
            } else if (num < 0.8) {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::FOREST));
            } else if (num < 0.95) {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::WATER));
            } else {
                bunk.setBiotope(biotopManager->getBiotop(Biotopes::STONE));
            }
        }
    }
}

int Map::getWidth() const {
    return this->width;
}

int Map::getHeight() const {
    return this->height;
}

Bunk** Map::getBunks() const {
    return this->bunks;
}

void Map::print() {
    std::cout << "Map: " << this->width << "x" << this->height << std::endl;
    for (int y = 0; y < this->height; y++) {
        std::cout << "| ";
        for (int x = 0; x < this->width; x++) {
            auto& bunk = this->bunks[x][y];
            bunk.printMap();
            std::cout << " ";
        }
        std::cout << "|" << std::endl;
    }
}

void Map::spreadFire() {
    std::vector<Bunk*> bunksOnFire;
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            auto& bunk = this->bunks[x][y];
            if (bunk.getIsOnFire()) {
                bunksOnFire.push_back(&bunk);
            }
        }
    }

    for (auto* bunk : bunksOnFire) {
        for (int xOffset = -1; xOffset <= 1; xOffset++) {
            for (int yOffset = -1; yOffset <= 1; yOffset++) {
                if (xOffset == 0 && yOffset == 0) {
                    continue;
                }
                if (xOffset != 0 && yOffset != 0) {
                    continue;
                }
                int x = bunk->getX() + xOffset;
                int y = bunk->getY() + yOffset;
                if (this->isOutOfMap(x, y)) {
                    continue;
                }

                auto& bunkXY = this->bunks[x][y];
                bunkXY.setIsOnFire(true);
            }
        }
    }
}

bool Map::isOutOfMap(int x, int y) {
    return x < 0 || x >= this->width || y < 0 || y >= this->height;
}

#endif //POS_SP_MAP_H
