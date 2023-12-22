#include <iostream>
#include "utils/BiotopeManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl << std::endl;

    auto* biotopMnaager = BiotopeManager::getInstance();
    biotopMnaager->printBiotops();

    std::cout << "Biotop forest este raz: " << std::endl;
    auto* biotopForest = biotopMnaager->getBiotop(Biotopes::FOREST);
    biotopForest->print();

    return 0;
}
