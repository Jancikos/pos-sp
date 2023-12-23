//
// Created by Ján Kostor on 23.12.2023.
//

#ifndef POS_SP_WIND_H
#define POS_SP_WIND_H

#include <string>

class Wind {
private:
    std::string title;
    int xOffSet = 0;
    int yOffSet = 0;

public:
    Wind() = default;
    Wind(std::string title, int xOffSet, int yOffSet) : title(title), xOffSet(xOffSet), yOffSet(yOffSet) {};

    std::string getTitle() const;
    int getXOffSet() const;
    int getYOffSet() const;
};

std::string Wind::getTitle() const {
    return this->title;
}

int Wind::getXOffSet() const {
    return this->xOffSet;
}

int Wind::getYOffSet() const {
    return this->yOffSet;
}

#endif //POS_SP_WIND_H
