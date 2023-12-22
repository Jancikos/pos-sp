#include <iostream>
#include "utils/BiotopeManager.h"
#include "utils/Map.h"

int main() {
    std::cout << "Hello, World!" << std::endl << std::endl;

    auto* biotopMnaager = BiotopeManager::getInstance();
    biotopMnaager->printBiotops();

    std::cout << "Biotop forest este raz: " << std::endl;
    auto* biotopForest = biotopMnaager->getBiotop(Biotopes::FOREST);
    biotopForest->print();

    int width;
    int height;
    std::cout << "Zadaj sirku a vysku mapy: " << std::endl;
    std::cin >> width >> height;

    auto* map = new Map(width, height);
    map->print();

    return 0;
}
