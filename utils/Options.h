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
//    TODO: pridat getter pre id a title
public:
    const int id;
    const std::string title;

    Option(const int id, const std::string title) : id(id), title(title) {}
};

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
        return options.size() > 0;
    }

    int getOptionCLI(std::string question, bool checkForMinMax = true)
    {
        std::cout << std::endl << question << std::endl;

        auto it = this->options.begin();
        const auto end = this->options.end();
        while (it != end)
        {
            auto option = *(*it);
            std::cout << "\t" << option.id << " - " << option.title << std::endl;
            ++it;
        }

        int min = this->options.at(0)->id;
        int max = this->options.at(this->options.size() - 1)->id;
        if (checkForMinMax)
        {
            for (auto option : this->options)
            {
                if (option->id < min)
                {
                    min = option->id;
                }
                if (option->id > max)
                {
                    max = option->id;
                }
            }
        }

        return Helper::readIntFromConsole(min, max);
    }
};


#endif //POS_SP_OPTIONS_H
