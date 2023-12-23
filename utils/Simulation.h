//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_SIMULATION_H
#define POS_SP_SIMULATION_H

#include "Map.h"
#include "Options.h"

class Simulation {
private:
    Map *map;
    long long time = 0;

public:
    Simulation(int width, int height) {
        this->map = new Map(width, height);
    }

    void run();

    void makeFirstStep();

    void makeStep();

    enum MainMenuOptions {
        MAKE_STEP = 1,
        EXIT = 2
    };
};

void Simulation::run() {
    int option = 0;
    Options mainMenu;
    mainMenu.addOption(MainMenuOptions::MAKE_STEP, "Make step");
    mainMenu.addOption(MainMenuOptions::EXIT, "Exit");

    do {
        if (this->time == 0) {
            this->makeFirstStep();
        }

        this->makeStep();

        option = mainMenu.getOptionCLI("Co dalej?");
        bool continueUserEdit = false;
        do {
            switch (option) {
                case MainMenuOptions::MAKE_STEP:
                    std::cout << "Making step" << std::endl;
                    continueUserEdit = true;
                    break;
                case MainMenuOptions::EXIT:
                    std::cout << "Exiting" << std::endl;
                    continueUserEdit = true;
                    break;
                default:
                    std::cout << "Neplatna volba" << std::endl;
                    break;
            }
        } while (continueUserEdit);
    }


}

while(option != MainMenuOptions::EXIT);
}

void Simulation::makeStep() {
    std::cout << std::endl;

//      rozsirime poziar


//      vypiseme mapu
    std::cout << "Time: " << this->time << std::endl;
    this->map->print();
    this->time++;

    std::cout << std::endl;
}

void Simulation::makeFirstStep() {

//    inicializacia poziaru
    // todo - zmenit na nahodne bunky v nasom rozsahu
    this->map->getBunks()[1][1].setIsOnFire(true);
}


#endif //POS_SP_SIMULATION_H
