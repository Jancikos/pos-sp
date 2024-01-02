//
// Created by Ján Kostor on 28.12.2023.
//

#ifndef POS_SP_SIMULATIONCSVRECORD_H
#define POS_SP_SIMULATIONCSVRECORD_H

#include <string>
#include <sstream>

class SimulationCsvRecord {
public:
    static const char DELIMETER = ';';
private:
    std::string title;
    unsigned long seed = 0;
    int width = 0;
    int height = 0;
    unsigned long time = 0;

public:
    SimulationCsvRecord() = default;
    SimulationCsvRecord(std::string csvRecordStr);
    SimulationCsvRecord(std::string title, unsigned long seed, int width, int height, unsigned long time) : title(title), seed(seed), width(width), height(height), time(time) {};
    std::string getTitle() const;
    unsigned long getSeed() const;
    int getWidth() const;
    int getHeight() const;
    unsigned long getTime() const;

    void setTitle(std::string title);
    void setSeed(unsigned long seed);
    void setWidth(int width);
    void setHeight(int height);
    void setTime(unsigned long time);

    std::string toCsv() const;
};

std::string SimulationCsvRecord::getTitle() const {
    return this->title;
}

unsigned long SimulationCsvRecord::getSeed() const {
    return this->seed;
}

int SimulationCsvRecord::getWidth() const {
    return this->width;
}

int SimulationCsvRecord::getHeight() const {
    return this->height;
}

unsigned long SimulationCsvRecord::getTime() const {
    return this->time;
}

void SimulationCsvRecord::setTitle(std::string title) {
    this->title = title;
}

void SimulationCsvRecord::setSeed(unsigned long seed) {
    this->seed = seed;
}

void SimulationCsvRecord::setWidth(int width) {
    this->width = width;
}

void SimulationCsvRecord::setHeight(int height) {
    this->height = height;
}

void SimulationCsvRecord::setTime(unsigned long time) {
    this->time = time;
}

std::string SimulationCsvRecord::toCsv() const {
    return this->title + DELIMETER
    + std::to_string(this->seed) + DELIMETER
    + std::to_string(this->width) + DELIMETER
    + std::to_string(this->height) + DELIMETER
    + std::to_string(this->time);
}

SimulationCsvRecord::SimulationCsvRecord(std::string csvRecordStr) {
    std::string item;
    int i = 0;
    std::stringstream ss(csvRecordStr);
    while (getline(ss, item, ';'))
    {
        bool ok = true;
        switch (i)
        {
            case 0:
                this->setTitle(item);
                break;
            case 1:
                this->setSeed(std::stoul(item));
                break;
            case 2:
                this->setWidth(std::stoi(item));
                break;
            case 3:
                this->setHeight(std::stoi(item));
                break;
            case 4:
                this->setTime(std::stoul(item));
                break;
            default:
                break;
        }
        i++;
    }
}


#endif //POS_SP_SIMULATIONCSVRECORD_H
