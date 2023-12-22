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
    char code;
    bool isFlammable;

public:
    Biotope(std::string title, char code, bool isFlammable) : title(title), code(code), isFlammable(isFlammable) {};
    std::string getTitle() {
        return this->title;
    }
    char getCode() const;
    bool getIsFlammable() const;
    void print();
};

char Biotope::getCode() const {
    return this->code;
}

bool Biotope::getIsFlammable() const {
    return this->isFlammable;
}

void Biotope::print() {
    std::cout << "Biotope: " << this->title << " (" << this->code << ")" << std::endl;
//    std::cout << "Is flammable: " << (isFlammable ? "Yes" : "No") << std::endl;
}

#endif //POS_SP_BIOTOPE_H
