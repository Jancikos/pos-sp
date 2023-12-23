//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_SIMULATION_H
#define POS_SP_SIMULATION_H

#include "Map.h"
#include "Options.h"

class Simulation {
private:
    Map* map;
    long long time = 0;

public:
    Simulation(int width, int height) {
        this->map = new Map(width, height);
    }

    void run();

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
        this->makeStep();

        option = mainMenu.getOptionCLI("Co dalej?");

        switch (option) {
            case MainMenuOptions::MAKE_STEP:
                std::cout << "Making step" << std::endl;
                break;
            case MainMenuOptions::EXIT:
                std::cout << "Exiting" << std::endl;
                break;
            default:
                std::cout << "Neplatna volba" << std::endl;
                break;
        }


    } while(option != MainMenuOptions::EXIT);
}

void Simulation::makeStep() {
    std::cout << std::endl;

    std::cout << "Time: " << this->time << std::endl;
    this->map->print();
    this->time++;

    std::cout << std::endl;
}


#endif //POS_SP_SIMULATION_H
