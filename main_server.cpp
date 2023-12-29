#include <iostream>
#include "utils/BiotopeManager.h"
#include "utils/Map.h"
#include "utils/Simulation.h"
#include "utils/SimulationCsvLoader.h"
#include "utils/MySocketServer.h"


int main() {
    return MySocketServer::run(2956);


    SimulationCsvLoader loader("/home/kostor/sp/simulations.csv");
//    SimulationCsvLoader loader("./data/simulations.csv");
    SimulationCsvRecord simulationCsvRecord;

    Options nacitanie;
    std::cout << "Do you want to load map from server?" << std::endl;
    nacitanie.addOption(1, "yes");
    nacitanie.addOption(2, "no");
    int result = nacitanie.getOptionCLI("Enter number of your choice: ");
    switch (result) {
        case 1: {
            // ulozene simulacie
            for (auto const& [key, simulationCsvRecord] : loader.getSimulationRecords())
            {
                std::cout << key << std::endl;
            }

            // tu sa nacita simulacia zo suboru
            std::string simTitle = Helper::readLineFromConsole("Enter simulation title: ");
            simulationCsvRecord = loader.getByTitle(simTitle);
            break;
        }
        case 2:
            // tu sa vytvori nova simulacia

            std::cout << "Enter simulation title: " << std::endl;
            std::string title;
            std::cin >> title;
            simulationCsvRecord.setTitle(title);
            simulationCsvRecord.setSeed(std::chrono::system_clock::now().time_since_epoch().count());
            std::cout << "Enter map width and height: " << std::endl;
            int width, height;
            std::cin >> width >> height;
            simulationCsvRecord.setWidth(width);
            simulationCsvRecord.setHeight(height);
            break;
    }

    Simulation simulation(simulationCsvRecord);
    simulation.run();

    // ask for save
    Options options;
    options.addOption(1, "yes");
    options.addOption(2, "no");
    int saveResult = options.getOptionCLI("Do you want to save simulation?");

    if (saveResult == 1) {
        loader.addSimulationRecord(simulation.toCsvRecord());
        loader.save();
    }


    return 0;
}
