//
// Created by Ján Kostor on 22.12.2023.
//

#ifndef POS_SP_BIOTOPE_H
#define POS_SP_BIOTOPE_H


#include <string>
#include <iostream>

class Biotope {
private:
    std::string title;
    std::string code;
    std::string color;
    bool flammable;

public:
    Biotope(std::string title, std::string code, std::string color, bool isFlammable) : title(title), code(code), flammable(isFlammable), color(color) {};
    std::string getTitle();
    std::string getColor() const;
    std::string getCode() const;
    bool isFlammable() const;
    void print();
};

std::string Biotope::getColor() const {
    return this->color;
}

std::string Biotope::getCode() const {
    return this->code;
}

bool Biotope::isFlammable() const {
    return this->flammable;
}

void Biotope::print() {
    std::cout << "Biotope: " << this->title << " (" << this->code << ")" << std::endl;
//    std::cout << "Is flammable: " << (flammable ? "Yes" : "No") << std::endl;
}

std::string Biotope::getTitle() {
    return this->title;
}

#endif //POS_SP_BIOTOPE_H