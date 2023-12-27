//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_MAP_H
#define POS_SP_MAP_H

#include <chrono>
#include "../models/Cell.h"
#include "BiotopeManager.h"
#include "random"
#include "../models/Wind.h"
#include "WindManager.h"

class Map {
private:
    int width, height;
    Cell** cells;

public:
    Map(int width, int height) : width(width), height(height) { };
    void initializeBunks(std::default_random_engine &rnd);
    int getWidth() const;
    int getHeight() const;
    Cell** getCells() const;
    void print();

    void spreadFire(WindType windType, std::default_random_engine &rnd);

    bool isOutOfMap(int x, int y);
};

void Map::initializeBunks(std::default_random_engine &rnd) {
    this->cells = new Cell*[this->width];

    auto* biotopManager = BiotopeManager::getInstance();

    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int x = 0; x < this->width; x++) {
        this->cells[x] = new Cell[this->height];
        for (int y = 0; y < this->height; y++) {
            auto& cell = this->cells[x][y];

            cell.setX(x);
            cell.setY(y);

            auto num = dist(rnd);
            if (num < 0.6) {
                cell.setBiotope(biotopManager->getBiotop(Biotopes::MEADOW));
            } else if (num < 0.8) {
                cell.setBiotope(biotopManager->getBiotop(Biotopes::FOREST));
            } else if (num < 0.95) {
                cell.setBiotope(biotopManager->getBiotop(Biotopes::WATER));
            } else {
                cell.setBiotope(biotopManager->getBiotop(Biotopes::STONE));
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

Cell** Map::getCells() const {
    return this->cells;
}

void Map::print() {
    std::cout << "Map: " << this->width << "x" << this->height << std::endl;
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            auto& cell = this->cells[x][y];
            cell.printMap();
        }
        std::cout << std::endl;
    }
}

void Map::spreadFire(WindType windType, std::default_random_engine &rnd) {
    std::vector<Cell*> bunksOnFire;
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            auto& cell = this->cells[x][y];

            if (cell.isOnFire()) {
                bunksOnFire.push_back(&cell);
            }
        }
    }

    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto windManager = WindManager::getInstance();

    for (auto* cell : bunksOnFire) {
        for (int xOffset = -1; xOffset <= 1; xOffset++) {
            for (int yOffset = -1; yOffset <= 1; yOffset++) {
                if (xOffset == 0 && yOffset == 0) {
                    continue;
                }
                if (xOffset != 0 && yOffset != 0) {
                    continue;
                }
                int x = cell->getX() + xOffset;
                int y = cell->getY() + yOffset;
                if (this->isOutOfMap(x, y)) {
                    continue;
                }

                auto& bunkXY = this->cells[x][y];

                // if cell is burnt, skip it
                if (bunkXY.isBurnt()) {
                    continue;
                }

                // cell is not flammable
                if (!bunkXY.getBiotope()->isFlammable()) {
                    continue;
                }

                double pOfFireSpread = windManager->probabilityOfSpreadingFire(windType, xOffset, yOffset);

                // cell is not on fire
                if (dist(rnd) > pOfFireSpread) {
                    continue;
                }

                bunkXY.setIsOnFire(true);
            }
        }
    }
}

bool Map::isOutOfMap(int x, int y) {
    return x < 0 || x >= this->width || y < 0 || y >= this->height;
}

#endif //POS_SP_MAP_H
