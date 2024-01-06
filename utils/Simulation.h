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
    ~Simulation() {
        delete this->map;
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

    std::string toString();

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
        // pokial je to nulty krok, tak sa len vypise mapa
        if (this->time == 0) {
            this->makeFirstStep();
            continue;
        }

        // spravi sa krok a vypise sa mapa
        this->makeStep();

        bool continueUserEdit = true;
        do {
            // zobrazi sa menu aby si uzivatel mohol vybrat co chce robit
            option = mainMenu.getOptionCLI("What do you want to do?");
            switch (option) {
                case MainMenuOptions::ADD_FIRE:
                    // prida sa poziar manualne
                    std::cout << "Adding fire manually" << std::endl;
                    this->addFireManualy();
                    break;
                case MainMenuOptions::CHANGE_WIND:
                    // zmeni sa smer vetra manualne
                    std::cout << "Changing wind manually" << std::endl;
                    this->setWindTypeManually();
                    std::cout << "New actual wind: " << WindManager::getInstance()->getWindTypeTitle(this->windType) << std::endl;
                    break;
                case MainMenuOptions::MAKE_STEP:
                    // spravi sa krok
                    std::cout << "Making a step" << std::endl;
                    continueUserEdit = false;
                    break;
                case MainMenuOptions::MAKE_N_STEPS:
                    // spravi sa n krokov
                    std::cout << "Making n steps" << std::endl;
                    this->makeNSteps();
                    break;
                case MainMenuOptions::EXIT:
                    // ukonci sa simulacia
                    std::cout << "Exiting" << std::endl;
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
    // pokial je to prvy krok, tak sa inicializuje poziar
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
    // vypiseme aktualny cas, smer vetra a mapu
    std::cout << "Time: " << this->time << std::endl;
    std::cout << "Wind: " << WindManager::getInstance()->getWindTypeTitle(this->windType) << std::endl;
    this->map->print();
}

void Simulation::setWindTypeManually() {
    // zobrazi sa menu, aby si uzivatel mohol vybrat smer vetra
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
    // uzivatel si vyberie bunku, ktoru chce zapalit
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
    // pokial je smer vetra NONE, tak sa nemeni
    // vietor iny ako NONE ostava tri kola, potom sa moze zmenit
    if (this->windType != WindType::NONE) {
        if (this->time - this->lastWindChange < 3) {
            return;
        }
    }

    // s pravdepodobnostou 90% sa zmeni smer vetra na NONE
    static std::uniform_real_distribution<double> distWind(0, 1);
    if (distWind(this->rnd) < 0.9) {
        this->windType = WindType::NONE;
        return;
    }

    this->lastWindChange = this->time;
    static std::uniform_int_distribution<int> distWindType(1, 4);

    // zmeni sa smer vetra na nahodny
    int option = distWindType(this->rnd);
    this->windType = static_cast<WindType>(option);
}

void Simulation::makeNSteps() {
    // uzivatel si vyberie pocet krokov, ktore chce spravit
    int n = 0;
    std::cout << "Enter steps count: " << std::endl;
    std::cin >> n;
    std::cout <<  std::endl;


    for (int i = 0; i < n; i++) {
        this->makeStep();
        // sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

void Simulation::initFire() {
    // inicializacia poziaru
    std::uniform_int_distribution<int> distWidth(0, this->map->getWidth() - 1);
    std::uniform_int_distribution<int> distHeight(0, this->map->getHeight() - 1);

    // vyberie sa nahodna bunka, ktora sa zapali
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
    // warmup sluzi na to, aby preslo tolko krokov, kolko si uzivatel zadal
    // toto sa pouziva iba ked mal pouzivatel simulaciu ulozenu a chce v nej pokracovat
    this->time = 1;

    for (int i = 0; i < repCount; i++) {
        this->makeStep();
    }
}

SimulationCsvRecord Simulation::toCsvRecord() {
    // vrati zaznam simulacie
    return SimulationCsvRecord(this->nazov, this->seed, this->map->getWidth(), this->map->getHeight(), this->time);
}

std::string Simulation::toString() {
    // vrati string v tvare: nazov;seed;width;height;time
    std::string result = "";
    result += this->nazov + ";" + std::to_string(this->seed) + ";" + std::to_string(this->map->getWidth()) + ";" + std::to_string(this->map->getHeight()) + ";" + std::to_string(this->time);
    return result;
}


#endif //POS_SP_SIMULATION_H
