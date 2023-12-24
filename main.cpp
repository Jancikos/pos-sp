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
//    std::cout << "Zadaj sirku a vysku mapy: " << std::endl;
//    std::cin >> width >> height;

    Simulation simulation(width, height, seed);
    simulation.run();

    return 0;
}
