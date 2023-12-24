//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_OPTIONS_H
#define POS_SP_OPTIONS_H

#include <string>
#include <vector>
#include <iostream>
#include "Helper.h"

class Option
{
private:
    const int id;
    const std::string title;
public:
    int getId() const;
    std::string getTitle();

    Option(const int id, const std::string title) : id(id), title(title) {}
};

int Option::getId() const {
    return this->id;
}

std::string Option::getTitle() {
    return this->title;
}

class Options
{
public:
    std::vector<Option*> options;

    ~Options()
    {
        for (auto* option : options)
        {
            delete option;
        }
    }

    void addOption(const int id, const std::string title)
    {
        auto* option = new Option(id, title);
        options.push_back(option);
    }

    bool hasOptions() const
    {
        return !options.empty();
    }

    int getOptionCLI(const std::string& question, bool checkForMinMax = true)
    {
        std::cout << std::endl << question << std::endl;

        auto it = this->options.begin();
        const auto end = this->options.end();
        while (it != end)
        {
            auto option = *(*it);
            std::cout << "\t" << option.getId() << " - " << option.getTitle() << std::endl;
            ++it;
        }

        int min = this->options.at(0)->getId();
        int max = this->options.at(this->options.size() - 1)->getId();
        if (checkForMinMax)
        {
            for (auto option : this->options)
            {
                if (option->getId() < min)
                {
                    min = option->getId();
                }
                if (option->getId() > max)
                {
                    max = option->getId();
                }
            }
        }

        return Helper::readIntFromConsole(min, max);
    }
};


#endif //POS_SP_OPTIONS_H
