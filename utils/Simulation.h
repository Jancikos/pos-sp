//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_SIMULATION_H
#define POS_SP_SIMULATION_H

#include "Map.h"
#include "Options.h"
#include "WindManager.h"
#include "../models/SimulationCsvRecord.h"
#include <random>
#include <chrono>
#include <thread>

class Simulation {
private:
    std::default_random_engine rnd;
    unsigned long seed;
    WindType windType;
    Map *map;
    long long time = 0;
    long long lastWindChange = 0;
    std::string nazov;

    void warmup(int repCount);
public:
    Simulation(SimulationCsvRecord record) : Simulation(record.getTitle(), record.getWidth(), record.getHeight(), record.getSeed(), record.getTime()) { };

    Simulation(std::string nazov, int width, int height, unsigned long seed, int pocetIteracii) : rnd(seed) {
        this->nazov = nazov;
        this->seed = seed;
        this->windType = WindType::NONE;
        this->map = new Map(width, height);
        this->map->initializeBunks(this->rnd);

        if (pocetIteracii > 0) {
            this->warmup(pocetIteracii);
        }
    }

    void run();

    void makeFirstStep();

    void makeStep();

    enum MainMenuOptions {
        MAKE_STEP,
        MAKE_N_STEPS,
        ADD_FIRE,
        CHANGE_WIND,
        EXIT
    };

    void print();

    void setWindTypeManually();

    void changeWindType();

    void addFireManualy();

    void makeNSteps();

    void initFire();

    SimulationCsvRecord toCsvRecord();
};

void Simulation::run() {
    int option = 0;
    Options mainMenu;
    mainMenu.addOption(MainMenuOptions::MAKE_STEP, "Make step");
    mainMenu.addOption(MainMenuOptions::MAKE_N_STEPS, "Make n steps");
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
            option = mainMenu.getOptionCLI("What do you want to do?");
            switch (option) {
                case MainMenuOptions::ADD_FIRE:
                    std::cout << "Add fire manually" << std::endl;
                    this->addFireManualy();
                    break;
                case MainMenuOptions::CHANGE_WIND:
                    std::cout << "Change wind manually" << std::endl;
                    this->setWindTypeManually();
                    std::cout << "New actual wind: " << WindManager::getInstance()->getWindTypeTitle(this->windType) << std::endl;
                    break;
                case MainMenuOptions::MAKE_STEP:
                    std::cout << "Make a step" << std::endl;
                    continueUserEdit = false;
                    break;
                case MainMenuOptions::MAKE_N_STEPS:
                    std::cout << "Make n steps" << std::endl;
                    this->makeNSteps();
                    break;
                case MainMenuOptions::EXIT:
                    std::cout << "Exit" << std::endl;
                    continueUserEdit = false;
                    break;
                default:
                    std::cout << "Invalid option" << std::endl;
                    break;
            }
        } while (continueUserEdit);
    } while (option != MainMenuOptions::EXIT);
}


void Simulation::makeStep() {
    std::cout << std::endl;

    // zmenime smer vetra
    this->changeWindType();

    // rozsirime poziar
    if (this->time == 1) {
        this->initFire();
    } else {
        this->map->spreadFire(this->windType, this->rnd);
    }

    // bunky, ktore horia viac ako 10 kol, sa spalia
    this->map->changeOnFireToBurnt(this->rnd);

    // spalene bunky sa zmenia na luku
    this->map->changeBurntToMeadow(this->rnd);

    // luka sa zmeni na les
    this->map->changeMeadowToForest(this->rnd);

    // vypiseme mapu
    this->print();
    this->time++;

    std::cout << std::endl;
}

void Simulation::makeFirstStep() {
    // vypiseme mapu
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
    int option = windMenu.getOptionCLI("Choose new wind type");
    this->lastWindChange = this->time;
    this->windType = static_cast<WindType>(option);
}

void Simulation::addFireManualy() {
    do {
        int x = 0, y = 0;
        std::cout << "Enter x and y coordinates for cell you want to set on fire: " << std::endl;
        std::cin >> x >> y;

        if (this->map->isOutOfMap(x, y)) {
            std::cout << "Entered coordinates are out of map" << std::endl;
            continue;
        }

        if (!this->map->getCells()[x][y].getBiotope()->isFlammable()) {
            std::cout << "Cell is not flammable" << std::endl;
            continue;
        }

        if (this->map->getCells()[x][y].isOnFire()) {
            std::cout << "Cell is already on fire" << std::endl;
            continue;
        }

        this->map->getCells()[x][y].setIsOnFire(true);
        break;
    } while (true);
}

void Simulation::changeWindType() {
    if (this->windType != WindType::NONE) {
        if (this->time - this->lastWindChange < 3) {
            return;
        }
    }
    static std::uniform_real_distribution<double> distWind(0, 1);
    if (distWind(this->rnd) < 0.9) {
        this->windType = WindType::NONE;
        return;
    }

    this->lastWindChange = this->time;
    static std::uniform_int_distribution<int> distWindType(1, 4);

    int option = distWindType(this->rnd);
    this->windType = static_cast<WindType>(option);
}

void Simulation::makeNSteps() {
    int n = 0;
    std::cout << "Enter steps count: " << std::endl;
    std::cin >> n;
    std::cout <<  std::endl;


    for (int i = 0; i < n; i++) {
        this->makeStep();
//        sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

void Simulation::initFire() {
    //    inicializacia poziaru
    std::uniform_int_distribution<int> distWidth(0, this->map->getWidth() - 1);
    std::uniform_int_distribution<int> distHeight(0, this->map->getHeight() - 1);

    do {
        int x = distWidth(this->rnd);
        int y = distHeight(this->rnd);

        if (!this->map->getCells()[x][y].getBiotope()->isFlammable()) {
            continue;
        }

        this->map->getCells()[x][y].setIsOnFire(true);
        break;
    } while (true);
}

void Simulation::warmup(int repCount) {
    this->time = 1;
    this->initFire();

    for (int i = 0; i < repCount; i++) {
        this->makeStep();
    }
}

SimulationCsvRecord Simulation::toCsvRecord() {
    return SimulationCsvRecord(this->nazov, this->seed, this->map->getWidth(), this->map->getHeight(), this->time);
}

#endif //POS_SP_SIMULATION_H
