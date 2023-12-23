//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_SIMULATION_H
#define POS_SP_SIMULATION_H

#include "Map.h"

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
};

void Simulation::run() {
    while (true) {
        this->makeStep();

        // enter 0 to stop
        std::cout << "Enter 0 to stop: (ine cislo pre pokracovanie) ";
        int input;
        std::cin >> input;
        if (input == 0) {
            break;
        }
    }
}

void Simulation::makeStep() {
    std::cout << std::endl;

    std::cout << "Time: " << this->time << std::endl;
    this->map->print();
    this->time++;

    std::cout << std::endl;
}


#endif //POS_SP_SIMULATION_H
