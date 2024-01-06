//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_HELPER_H
#define POS_SP_HELPER_H

#include <string>
#include <iostream>
#include <limits>

class Helper {
public:
    static int readIntFromConsole(std::string msg, int min = 0, int max = 10) {
        std::cout << msg;
        return Helper::readIntFromConsole(min, max);
    }
    static int readIntFromConsole(int min = 0, int max = 10) {
        int input;

        if (max < min) {
            std::swap(min, max);
        }

        while (true) {
            std::cout << "Enter number from " << min << " to " << max << ": ";
            std::cin >> input;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            const bool failed = std::cin.fail();
            if (failed) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (failed || input < min || input > max) {
                std::cout << "Invalid input. Try again." << std::endl;
            } else {
                return input;
            }
        }
    }

    /**
     * \brief zadaj min max take ako naozaj ocakavas. no uzivatel bude vkladat o jedno vacsie cisla
     * \param min
     * \param max
     * \return
     */
    static int readIntOptionFromConsole(int min = 0, int max = 10) {
        return Helper::readIntFromConsole(min + 1, max + 1) - 1;
    }

    static std::string readLineFromConsole(std::string message) {
        std::string line;

        std::cout << message;
        std::getline(std::cin, line);

        return line;
    }
};

#endif //POS_SP_HELPER_H
