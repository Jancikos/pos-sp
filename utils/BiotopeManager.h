//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_BIOTOPEMANAGER_H
#define POS_SP_BIOTOPEMANAGER_H

#include "../models/Biotope.h"

enum Biotopes {
    MEADOW,
    FOREST,
    WATER,
    STONE,
    COUNT // this is not a biotop, but a number of windTypes, so it should be the last
};

class BiotopeManager {
protected:
    inline static BiotopeManager* instance;
    Biotope** biotypes;

    //   singleton
    BiotopeManager();
    ~BiotopeManager();
    BiotopeManager(BiotopeManager &other) = delete;
    void operator=(const BiotopeManager&) = delete;

    void initializeBiotypes();
public:
    static BiotopeManager* getInstance();

    Biotope** getBiotops();
    Biotope* getBiotop(Biotopes biotopEnum);

    void printBiotops();
};

BiotopeManager::BiotopeManager() {
    initializeBiotypes();
}

BiotopeManager *BiotopeManager::getInstance() {
    if (BiotopeManager::instance == nullptr) {
        BiotopeManager::instance = new BiotopeManager();
    }
    return BiotopeManager::instance;
}

void BiotopeManager::initializeBiotypes() {
    biotypes = new Biotope*[Biotopes::COUNT];

    biotypes[Biotopes::MEADOW] = new Biotope("Meadow", "█", true);
    biotypes[Biotopes::FOREST] = new Biotope("Forest", "█", true);
    biotypes[Biotopes::WATER] = new Biotope("Water", "█", false);
    biotypes[Biotopes::STONE] = new Biotope("Stone", "█", false);
}

Biotope** BiotopeManager::getBiotops() {
    return biotypes;
}

Biotope* BiotopeManager::getBiotop(Biotopes biotopEnum) {
    return biotypes[biotopEnum];
}

void BiotopeManager::printBiotops() {
    for (int i = 0; i < Biotopes::COUNT; i++) {
        biotypes[i]->print();
    }
}

BiotopeManager::~BiotopeManager() {
    delete[] biotypes;
    BiotopeManager::instance = nullptr;
}


#endif //POS_SP_BIOTOPEMANAGER_H
