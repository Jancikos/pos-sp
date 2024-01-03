//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_WINDMANAGER_H
#define POS_SP_WINDMANAGER_H

#include <iostream>
#include "../models/Wind.h"

enum  WindType {
    NONE,
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class WindManager {
protected:
    inline static WindManager* instance;
    Wind** windTypes;

    // singleton
    WindManager();
    ~WindManager();
    WindManager(WindManager &other) = delete;
    void operator=(const WindManager&) = delete;

    void initializeWindTypes();
public:
    static WindManager* getInstance();

    Wind** getWinds();
    Wind* getWind(WindType windType);

    double probabilityOfSpreadingFire(WindType windType, int xOffset, int yOffset);

    void printWinds();

    std::string getWindTypeTitle(WindType type);
};

WindManager::WindManager() {
    initializeWindTypes();
}

WindManager *WindManager::getInstance() {
    // vrati instanciu, ak neexistuje, vytvori ju
    if (WindManager::instance == nullptr) {
        WindManager::instance = new WindManager();
    }
    return WindManager::instance;
}

void WindManager::initializeWindTypes() {
    // vytvori sa pole vetrov, ktore sa pouzivaju v programe
    windTypes = new Wind*[5];

    windTypes[WindType::NONE] = new Wind("None", 0, 0);
    windTypes[WindType::NORTH] = new Wind("North", 0, -1);
    windTypes[WindType::EAST] = new Wind("East", 1, 0);
    windTypes[WindType::SOUTH] = new Wind("South", 0, 1);
    windTypes[WindType::WEST] = new Wind("West", -1, 0);
}

Wind** WindManager::getWinds() {
    return windTypes;
}

Wind* WindManager::getWind(WindType windType) {
    return windTypes[windType];
}

void WindManager::printWinds() {
    for (int i = 0; i < 5; i++) {
        std::cout << i << " - " << windTypes[i]->getTitle() << std::endl;
    }
}

WindManager::~WindManager() {
    delete[] windTypes;
    WindManager::instance = nullptr;
}

double WindManager::probabilityOfSpreadingFire(WindType windType, int xOffset, int yOffset) {
    // ak nie je vietor, tak je pravdepodobnosť 20%
    if (windType == WindType::NONE) {
        return 0.2;
    }

    auto wind = *this->getWind(windType);

    // ak sa oheň šíri v smere vetra, tak je pravdepodobnosť 90%
    if (wind.getXOffSet() == xOffset && wind.getYOffSet() == yOffset) {
        return 0.9;
    }

    // ak sa oheň nešíri v smere vetra, tak je pravdepodobnosť 2%
    return 0.02;
}

std::string WindManager::getWindTypeTitle(WindType type) {
    return this->getWind(type)->getTitle();
}


#endif //POS_SP_WINDMANAGER_H
