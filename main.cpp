#include <iostream>
#include "utils/BiotopeManager.h"
#include "utils/Map.h"
#include "utils/Simulation.h"

int main() {
    std::cout << "Hello, simulation poziaru!" << std::endl << std::endl;

//    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    unsigned seed = 1608314400;
    int width = 65;
    int height = 12;
    std::string nazov;
    int pocetIteracii = 0;

    Options nacitanie;
    std::cout << "Chcete nacitat mapu zo suboru?" << std::endl;
    nacitanie.addOption(1, "ano");
    nacitanie.addOption(2, "nie");
    int odpoved = nacitanie.getOptionCLI("Zadajte cislo moznosti: ");
    switch (odpoved) {
        case 1:
            // tu sa nacita mapa zo suboru
            break;
        case 2:
            // tu sa vygeneruje nova mapa
            //    std::cout << "Zadaj sirku a vysku mapy: " << std::endl;
            //    std::cin >> width >> height;
            break;
        default:
            std::cout << "Zadali ste zlu moznost" << std::endl;
            break;
    }

    Simulation simulation(nazov, width, height, seed, pocetIteracii);
    simulation.run();

    return 0;
}
