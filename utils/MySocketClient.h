//
// Created by brisa on 1. 1. 2024.
//

#ifndef POS_SP_MYSOCKETCLIENT_H
#define POS_SP_MYSOCKETCLIENT_H

#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "MySocketServer.h"

class MySocketClient {
public:
    int run(int argc, char *argv[]);

private:
    std::string getFromSocket(int sockfd, ServerCommands command, std::string data = "0");
};

int MySocketClient::run(int argc, char **argv) {
    // create socket
    int sockfd, n;
    struct sockaddr_in serv_addr{};
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " hostname port\n";
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == nullptr) {
        std::cerr << "Error, no such host\n";
        return 2;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 3;
    }

    if (::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to socket");
        return 4;
    }

    // klient je pripojeny na socket, moze zacat


    // nacita sa simulacia zo suboru alebo sa vytvori nova
    SimulationCsvRecord simulationCsvRecord;

    // todo refaktor - spravit si specialneho pootomka options pre yes/no odpoved (aj s enumom...)
    Options nacitanie;
    std::cout << "Do you want to load map from server?" << std::endl;
    nacitanie.addOption(1, "yes");
    nacitanie.addOption(2, "no");
    int result = nacitanie.getOptionCLI("Enter number of your choice: ");

    switch (result) {
        // nacitaj ulozenu simulaciu zo socketu
        case 1: {
            // ulozene simulacie sa vypisu
            std::string result = this->getFromSocket(sockfd, ServerCommands::LIST);
            std::cout << "List of saved simulations: " << std::endl << result << std::endl;

            // posli nazov simulacie na server
            std::string simTitle = Helper::readLineFromConsole("Enter selected simulation title: ");
            // poslem spravu na server
            simulationCsvRecord = SimulationCsvRecord(
                    this->getFromSocket(sockfd, ServerCommands::LOAD, simTitle)
            );
            break;
        }
        // vytvor nova simulacia
        case 2:

            // vypytam si potrebne udaje od uzivatela
            std::string title = Helper::readLineFromConsole("Enter simulation title: ");
            int width = Helper::readIntFromConsole("Enter simulation width: ", 5, 200);
            int height = Helper::readIntFromConsole("Enter simulation height: ", 5, 200);

            // vytvorim zaznam simulacie
            simulationCsvRecord.setTitle(title);
            simulationCsvRecord.setSeed(std::chrono::system_clock::now().time_since_epoch().count());
            simulationCsvRecord.setWidth(width);
            simulationCsvRecord.setHeight(height);
            break;
    }

    // tu zbehne simulacia
    Simulation simulation(simulationCsvRecord);
    simulation.run();

    // spyta sa ci chce ulozit simulaciu
    // todo refaktor - spravit si specialneho pootomka options pre yes/no odpoved (aj s enumom...)
    Options options;
    options.addOption(1, "yes");
    options.addOption(2, "no");
    int saveResult = options.getOptionCLI("Do you want to save simulation?");

    if (saveResult == 1) {
        // posli spravu na server, ze chcem ulozit simulaciu
        std::string result = this->getFromSocket(sockfd, ServerCommands::SAVE, simulationCsvRecord.toCsv());
        std::cout << "Simulation saved" << std::endl;
    }

    // zatvorim socket
    close(sockfd);

    return 0;
}

std::string MySocketClient::getFromSocket(int sockfd, ServerCommands command, std::string data) {
    int n;
    char buffer[256];

    // posli spravu na server, aby poslal naspat nieco
    int commandValue = static_cast<int>(command);
    std::string commandStr = std::to_string(commandValue) + ";" + data;
    strcpy(buffer, commandStr.c_str());
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        throw std::runtime_error("Error writing to socket (code 5)");
    }

    // precitam spravu od servera
    bzero(buffer, strlen(buffer));
    n = read(sockfd, buffer, strlen(buffer) - 1);
    if (n < 0) {
        throw std::runtime_error("Error reading from socket (code 6)");
    }

    return std::string(buffer);
}


#endif //POS_SP_MYSOCKETCLIENT_H
