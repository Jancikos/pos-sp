#include <iostream>
#include "utils/BiotopeManager.h"
#include "utils/Map.h"
#include "utils/Simulation.h"
#include "utils/SimulationCsvLoader.h"

// todo UI prelozit do ANJ
int main() {
    std::cout << "Hello, simulation poziaru!" << std::endl << std::endl;

    SimulationCsvLoader loader("/home/kostor/sp/simulations.csv");
//    SimulationCsvLoader loader("./data/simulations.csv");
    SimulationCsvRecord simulationCsvRecord;

    Options nacitanie;
    std::cout << "Chcete nacitat mapu zo suboru?" << std::endl;
    nacitanie.addOption(1, "ano");
    nacitanie.addOption(2, "nie");
    int result = nacitanie.getOptionCLI("Zadajte cislo moznosti: ");
    switch (result) {
        case 1: {
            // ulozene simulacie
            for (auto const& [key, simulationCsvRecord] : loader.getSimulationRecords())
            {
                std::cout << key << std::endl;
            }

            // tu sa nacita simulacia zo suboru
            std::string simTitle = Helper::readLineFromConsole("Enter sim title: ");
            simulationCsvRecord = loader.getByTitle(simTitle);
            break;
        }
        case 2:
            // tu sa vytvori nova simulacia

            // todo zadavat to z konzoly
            simulationCsvRecord.setTitle("test");
            simulationCsvRecord.setSeed(std::chrono::system_clock::now().time_since_epoch().count());
            simulationCsvRecord.setWidth(65);
            simulationCsvRecord.setHeight(12);


            //    std::cout << "Zadaj sirku a vysku mapy: " << std::endl;
            //    std::cin >> width >> height;
            //    std::cout << "Zadaj nazov mapy: " << std::endl;
            //    std::cin >> nazov;
            //    seed = std::chrono::system_clock::now().time_since_epoch().count();
            break;
    }

    Simulation simulation(simulationCsvRecord);
    simulation.run();

    // ask for save
    Options options;
    options.addOption(1, "ano");
    options.addOption(2, "nie");
    int saveResult = options.getOptionCLI("Chcete ulozit simulaciu do suboru? ");

    if (saveResult == 1) {
        loader.addSimulationRecord(simulation.toCsvRecord());
        loader.save();
    }


    return 0;
}
