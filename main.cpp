#include <iostream>
#include "utils/BiotopeManager.h"
#include "utils/Map.h"
#include "utils/Simulation.h"

int main() {
    std::cout << "Hello, simulation poziaru!" << std::endl << std::endl;

    int width = 10;
    int height = 5;
//    std::cout << "Zadaj sirku a vysku mapy: " << std::endl;
//    std::cin >> width >> height;

    Simulation simulation(width, height);
    simulation.run();

    return 0;
}
