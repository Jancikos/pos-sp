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
#include <signal.h>
#include <errno.h>
#include "MySocketServer.h"
#include "Helper.h"
#include "Options.h"
#include "Simulation.h"

class MySocketClient {
public:
    int run(std::string hostname, int port);

private:
    std::string getFromSocket(int sockfd, ServerCommands command, std::string data = "0");
    void sendToSocket(int sockfd, ServerCommands command, std::string data = "0");

    SimulationCsvRecord manualyCreateSimulation();
};

int MySocketClient::run(std::string hostname, int port) {
    // create socket
    int sockfd, n;
    struct sockaddr_in serv_addr{};
    struct hostent *server = gethostbyname(hostname.c_str());

    char buffer[256];
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(port);

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

    Options options;
    options.addYesNoOptions();

    int result = options.getOptionCLI("Do you want to load map from server? ");
    try {
        switch (result) {
            // nacitaj ulozenu simulaciu zo socketu
            case 1: {
                try {
                    // ulozene simulacie sa vypisu
                    std::string response = this->getFromSocket(sockfd, ServerCommands::LIST);
                    std::cout << "List of saved simulations: " << std::endl << response << std::endl;

                    // posli nazov simulacie na server
                    std::string simTitle = Helper::readLineFromConsole("Enter selected simulation title: ");
                    // poslem spravu na server
                    simulationCsvRecord = SimulationCsvRecord(
                            this->getFromSocket(sockfd, ServerCommands::LOAD, simTitle)
                    );
                    break;
                } catch (std::exception& e) {
                    if (strcmp(e.what(), "EMPPY_RESPONSE") == 0) {
                        std::cout << "Problem with server, create new simulation manually" << std::endl;
                        simulationCsvRecord = this->manualyCreateSimulation();
                        break;
                    }
                    std::cout << e.what() << std::endl;
                    return 0;
                }
            }
                // vytvor si novu simulaciu
            case 2:
                simulationCsvRecord = this->manualyCreateSimulation();
                break;
        }

        // tu zbehne simulacia
        Simulation simulation(simulationCsvRecord);
        simulation.run();

        // spyta sa ci chce ulozit simulaciu
        int saveResult = options.getOptionCLI("Do you want to save simulation?");

        if (saveResult == 1) {
            try {
                // posli spravu na server, ze chcem ulozit simulaciu
                SimulationCsvRecord newSimulationCsvRecord = simulation.toCsvRecord();
                this->getFromSocket(sockfd, ServerCommands::SAVE, newSimulationCsvRecord.toCsv());
                std::cout << "Simulation saved" << std::endl;
            } catch (std::exception& e) {
                std::cout << "Server error - simulation not saved" << std::endl;
            }
        }

        // spytaj sa, ci chce uzivatel celkovo ukoncit server
        int endResult = options.getOptionCLI("Do you want to turn off server?");
        try {
            if (endResult == 1) {
                this->sendToSocket(sockfd, ServerCommands::TURN_OFF);
            } else {
                this->sendToSocket(sockfd, ServerCommands::END);
            }
        } catch (std::exception& e) {
            std::cout << "Server error - server already dead" << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    // zatvorim socket
    close(sockfd);

    return 0;
}

std::string MySocketClient::getFromSocket(int sockfd, ServerCommands command, std::string data) {
    int n;
    char buffer[256];

    // posli spravu na server, aby poslal naspat nieco
    this->sendToSocket(sockfd, command, data);

    // precitam spravu od servera
    bzero(buffer, 256);

    // n = read(sockfd, buffer, 255);
    n = recv(sockfd, buffer, 255, MSG_NOSIGNAL);
    if (n < 0) {
        throw std::runtime_error("Error reading from socket (code 6)");
    }

    if (n == 0) {
        throw std::runtime_error("EMPPY_RESPONSE");
    }

    return {std::string(buffer)};
}

void MySocketClient::sendToSocket(int sockfd, ServerCommands command, std::string data) {
    int n;
    char buffer[256];

    // posli spravu na server, aby poslal naspat nieco
    int commandValue = static_cast<int>(command);
    std::string commandStr = std::to_string(commandValue) + ";" + data;
    strcpy(buffer, commandStr.c_str());

    // ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    // n = write(sockfd, buffer, strlen(buffer) + 1);
    n = send(sockfd, buffer, strlen(buffer) + 1, MSG_NOSIGNAL);

    if (n < 0) {
        throw std::runtime_error("Error writing to socket (code  5)");
    }
}

SimulationCsvRecord MySocketClient::manualyCreateSimulation() {
    // vypytam si potrebne udaje od uzivatela
    std::string title = Helper::readLineFromConsole("Enter simulation title: ");
    int width = Helper::readIntFromConsole("Enter simulation width: ", 5, 200);
    int height = Helper::readIntFromConsole("Enter simulation height: ", 5, 200);

    // vytvorim zaznam simulacie
    SimulationCsvRecord record;
    record.setTitle(title);
    record.setSeed(std::chrono::system_clock::now().time_since_epoch().count());
    record.setWidth(width);
    record.setHeight(height);

    return record;
}


#endif //POS_SP_MYSOCKETCLIENT_H
