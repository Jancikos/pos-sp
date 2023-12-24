//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_SIMULATION_H
#define POS_SP_SIMULATION_H

#include "Map.h"
#include "Options.h"
#include "WindManager.h"
#include <random>
#include <chrono>

class Simulation {
private:
    WindType windType;
    Map *map;
    long long time = 0;

public:
    Simulation(int width, int height) {
        this->map = new Map(width, height);
        this->windType = WindType::NONE;
    }

    void run();

    void makeFirstStep();

    void makeStep();

    enum MainMenuOptions {
        MAKE_STEP,
        ADD_FIRE,
        CHANGE_WIND,
        EXIT
    };

    void print();

    void setWindTypeManually();

    void setWindType();

    void addFireManualy();
};

void Simulation::run() {
    int option = 0;
    Options mainMenu;
    mainMenu.addOption(MainMenuOptions::MAKE_STEP, "Make step");
    mainMenu.addOption(MainMenuOptions::ADD_FIRE, "Add fire");
    mainMenu.addOption(MainMenuOptions::CHANGE_WIND, "Change wind");
    mainMenu.addOption(MainMenuOptions::EXIT, "Exit");

    do {
        if (this->time == 0) {
            this->makeFirstStep();
            continue;
        }

        this->makeStep();

        bool continueUserEdit = true;
        do {
            option = mainMenu.getOptionCLI("Co dalej?");
            switch (option) {
                case MainMenuOptions::ADD_FIRE:
                    std::cout << "Adding fire" << std::endl;
                    this->addFireManualy();
                    break;
                case MainMenuOptions::CHANGE_WIND:
                    std::cout << "Changing wind" << std::endl;
                    this->setWindTypeManually();
                    std::cout << "New actual wind: " << WindManager::getInstance()->getWindTypeTitle(this->windType) << std::endl;
                    break;
                case MainMenuOptions::MAKE_STEP:
                    std::cout << "Making step" << std::endl;
                    continueUserEdit = false;
                    break;
                case MainMenuOptions::EXIT:
                    std::cout << "Exiting" << std::endl;
                    continueUserEdit = false;
                    break;
                default:
                    std::cout << "Neplatna volba" << std::endl;
                    break;
            }
        } while (continueUserEdit);
    } while (option != MainMenuOptions::EXIT);
}


void Simulation::makeStep() {
    std::cout << std::endl;

    //      zmenime smer vetra
    this->setWindType();

//      rozsirime poziar
    this->map->spreadFire(this->windType);

//      vypiseme mapu
    this->print();
    this->time++;

    std::cout << std::endl;
}

void Simulation::makeFirstStep() {
//    inicializacia poziaru
    std::uniform_int_distribution<int> distWidth(0, this->map->getWidth() - 1);
    std::uniform_int_distribution<int> distHeight(0, this->map->getHeight() - 1);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rnd(seed);

    do {
        int x = distWidth(rnd);
        int y = distHeight(rnd);

        if (!this->map->getCells()[x][y].getBiotope()->isFlammable()) {
            continue;
        }

        this->map->getCells()[x][y].setIsOnFire(true);
        break;
    } while (true);

//    vypiseme mapu
    this->print();

    this->time++;
    std::cout << std::endl;
}

void Simulation::print() {
    std::cout << "Time: " << this->time << std::endl;
    std::cout << "Wind: " << WindManager::getInstance()->getWindTypeTitle(this->windType) << std::endl;
    this->map->print();
}

void Simulation::setWindTypeManually() {
    auto* windManager = WindManager::getInstance();
    Options windMenu;
    windMenu.addOption(WindType::NONE, windManager->getWindTypeTitle(WindType::NONE));
    windMenu.addOption(WindType::NORTH, windManager->getWindTypeTitle(WindType::NORTH));
    windMenu.addOption(WindType::EAST, windManager->getWindTypeTitle(WindType::EAST));
    windMenu.addOption(WindType::SOUTH, windManager->getWindTypeTitle(WindType::SOUTH));
    windMenu.addOption(WindType::WEST, windManager->getWindTypeTitle(WindType::WEST));

    std::cout << "Actual wind: " << windManager->getWindTypeTitle(this->windType) << std::endl;
    int option = windMenu.getOptionCLI("Zvol smer vetra:");
    this->windType = static_cast<WindType>(option);
}

void Simulation::addFireManualy() {
    do {
        int x = 0, y = 0;
        std::cout << "Zadaj suradnice bunky, kde chces pridat ohen: " << std::endl;
        std::cin >> x >> y;

        if (this->map->isOutOfMap(x, y)) {
            std::cout << "Zadane suradnice su mimo mapy" << std::endl;
            continue;
        }

        if (!this->map->getCells()[x][y].getBiotope()->isFlammable()) {
            std::cout << "Bunka nemoze horiet" << std::endl;
            continue;
        }

        if (this->map->getCells()[x][y].isOnFire()) {
            std::cout << "Bunka je uz hori" << std::endl;
            continue;
        }

        this->map->getCells()[x][y].setIsOnFire(true);
        break;
    } while (true);
}

void Simulation::setWindType() {

// TODO - implementovat zmenu smeru vetra

}

#endif //POS_SP_SIMULATION_H
