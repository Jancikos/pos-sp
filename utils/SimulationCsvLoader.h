//
// Created by Ján Kostor on 28.12.2023.
//

#ifndef POS_SP_SIMULATIONCSVLOADER_H
#define POS_SP_SIMULATIONCSVLOADER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "../models/SimulationCsvRecord.h"

class SimulationCsvLoader {
public:
    static const char DELIMETER = ';';
private:
    std::string filePath;
    std::unordered_map<std::string, SimulationCsvRecord> simulationRecords;

public:
    SimulationCsvLoader(std::string filePath) : filePath(filePath) {
        load();
    };
    void load();
    void save();
    void addSimulationRecord(SimulationCsvRecord simulationCsvRecord);

    std::unordered_map<std::string, SimulationCsvRecord>& getSimulationRecords();
    SimulationCsvRecord getByTitle(std::string basicString);
};

void SimulationCsvLoader::load() {
    std::ifstream inputFile(this->filePath);

    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            SimulationCsvRecord simulationCsvRecord;

            // rozparsuj tento riadok a nastav atributy uzemnej jednotky
            std::stringstream ss(line);
            std::string item;
            int i = 0;
            while (getline(ss, item, ';'))
            {
                switch (i)
                {
                    case 0:
                        simulationCsvRecord.setTitle(item);
                        break;
                    case 1:
                        simulationCsvRecord.setSeed(std::stoul(item));
                        break;
                    case 2:
                        simulationCsvRecord.setWidth(std::stoi(item));
                        break;
                    case 3:
                        simulationCsvRecord.setHeight(std::stoi(item));
                        break;
                    case 4:
                        simulationCsvRecord.setTime(std::stoul(item));
                        break;
                    default:
                        break;
                }
                i++;
            }

            this->simulationRecords[simulationCsvRecord.getTitle()] = simulationCsvRecord;
        }

        inputFile.close();
    }
}

void SimulationCsvLoader::save() {
    std::ofstream outputFile(this->filePath);

    if (outputFile.is_open())
    {
        for (auto const& [key, simulationCsvRecord] : this->simulationRecords)
        {
            outputFile << simulationCsvRecord.toCsv() << std::endl;
        }

        outputFile.close();
    }
}

void SimulationCsvLoader::addSimulationRecord(SimulationCsvRecord simulationCsvRecord) {
    this->simulationRecords[simulationCsvRecord.getTitle()] = simulationCsvRecord;
    this->save();
}

SimulationCsvRecord SimulationCsvLoader::getByTitle(std::string basicString) {
    return this->simulationRecords[basicString];
}

std::unordered_map<std::string, SimulationCsvRecord> &SimulationCsvLoader::getSimulationRecords() {
    return this->simulationRecords;
}

#endif //POS_SP_SIMULATIONCSVLOADER_H
