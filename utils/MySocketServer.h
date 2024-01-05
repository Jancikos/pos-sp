//
// Created by Ján Kostor on 29.12.2023.
//

#ifndef POS_SP_MYSOCKETSERVER_H
#define POS_SP_MYSOCKETSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "SimulationCsvLoader.h"
#include <atomic>

enum ServerCommands : int {
    END = 0,
    LIST = 1,
    LOAD = 2,
    SAVE = 3,
    TURN_OFF = 100
};

class MySocketServer {
private:
    int port;
    int sockfd;
    std::atomic<bool> stopSignal = false;
    std::vector<std::thread> threads;
    SimulationCsvLoader simulationLoader;

    std::mutex mtx;

public:
    MySocketServer(int port, std::string pathToCsv) : port(port), simulationLoader(pathToCsv) {
    };
    ~MySocketServer() = default;
    int run();

private:
    bool sendDataToClient(int sockfd, std::string data);

    bool sendSimulationsList(int sockfd);
    bool sendSimulation(int sckfd, std::string simTitle);
    bool saveSimulation(int sckfd, std::string csvRecordStr);

    void manageSocket(int newsockfd);

    bool turnOffServer(int newsockfd);
};

int MySocketServer::run() {socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n, newsockfd;

    // initialize socket structure and bind socket to port
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->port);

    // create socket v domene AF_INET, typu SOCK_STREAM (TCP)
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    // priprava adresy servera a bindovanie socketu na port
    if (bind(this->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    while (true) {
        if (this->stopSignal) {
            std::cout << "Server turned off by client" << std::endl;
            break;
        }

        std::cout << "Waiting for client to connect" << std::endl;

        // listen na sockete
        std::cout << "Listening on port " << port << std::endl;
        listen(this->sockfd, 5);
        cli_len = sizeof(cli_addr);
        
        // potvrdenie spojenia s klientom
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            return 3;
        }

        std::cout << "Client " << newsockfd << " connected" << std::endl;

        // vytvorenie noveho threadu
        this->threads.push_back(std::thread(&MySocketServer::manageSocket, this, newsockfd));
    }

    // zatvorenie threadov
    for (auto& thread : this->threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    close(sockfd);

    return 0;
}

bool MySocketServer::sendSimulationsList(int sockfd) {
    // posli zoznam simulacii
    std::string msg;

    for (std::pair<const std::basic_string<char>, SimulationCsvRecord> item : this->simulationLoader.getSimulationRecords()) {
        msg += item.second.getTitle() + "\n";
    }

    return this->sendDataToClient(sockfd, msg);
}

bool MySocketServer::sendSimulation(int sckfd, std::string simTitle) {
    // nacitaj simulaciu zo suboru
    SimulationCsvRecord simulationCsvRecord = this->simulationLoader.getByTitle(simTitle);

    // zapis do socketu
    std::cout << "Sending simulation info to client" << std::endl;
    std::cout << simulationCsvRecord.toCsv() << std::endl;

    return this->sendDataToClient(sckfd, simulationCsvRecord.toCsv());
}

bool MySocketServer::sendDataToClient(int sockfd, std::string data) {
    // zapisu sa data do socketu
    const char* msg = data.c_str();
    int n = write(sockfd, msg, strlen(msg) + 1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return false;
    }

    std::cout << "Message sent to client {" << data << "}" << std::endl;

    return true;
}

bool MySocketServer::saveSimulation(int sckfd, std::string csvRecordStr) {
    SimulationCsvRecord simulationCsvRecord(csvRecordStr);

    std::cout << "Client " << sckfd << " wants to save simulation " << simulationCsvRecord.getTitle() << std::endl;
    {
        // tu sa vytvori lock na mutex, aby sa zabezpecilo, ze ziadny iny thread nepristupuje k tomuto kodu
        // v tomto pripade je to zapis do suboru
        // sleep je v tomto pripade iba pre demonstraciu
        std::unique_lock<std::mutex> lck(this->mtx);
        std::cout << "Client " << sckfd << " is saving simulation " << simulationCsvRecord.getTitle() << std::endl;
        sleep(5);
        this->simulationLoader.addSimulationRecord(simulationCsvRecord);
        std::cout << "Client " << sckfd << " saved simulation " << simulationCsvRecord.getTitle() << std::endl;
        sleep(1);
    }

    auto msg = "Simulation " + simulationCsvRecord.getTitle() + " saved";
    std::cout << msg << std::endl;

    return this->sendDataToClient(sckfd, msg);
}

void MySocketServer::manageSocket(int newsockfd) {
    int n;
    char buffer[256];

    ServerCommands command;
    std::string data;
    // citanie zo socketu
    do {
        if (this->stopSignal) {
            std::cout << "Server is turning off (client " << newsockfd << ")" << std::endl;
            break;
        }

        std::cout << "Waiting for client to send message" << std::endl;
        bzero(buffer,256);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
        {
            throw std::runtime_error("Error reading from socket");
        }
        printf("Here is the message: %s\n", buffer);

        // pripravim si command a data
        std::string bufferStr(buffer);
        int pos = bufferStr.find(';');
        command = static_cast<ServerCommands>(std::stoi(bufferStr.substr(0, pos)));
        data = bufferStr.substr(pos + 1);

        // spracovanie commandu
        bool ok = true;
        switch (command) {
            case ServerCommands::LIST:
                // posli zoznam simulacii
                std::cout << "Client " << newsockfd << " wants to list simulations" << std::endl;
                ok = this->sendSimulationsList(newsockfd);
                break;
            case ServerCommands::LOAD:
                // posli simulaciu
                std::cout << "Client " << newsockfd << " wants to load simulation" << std::endl;
                ok = this->sendSimulation(newsockfd, data);
                break;
            case ServerCommands::SAVE:
                // uloz simulaciu
                std::cout << "Client " << newsockfd << " wants to save simulation" << std::endl;
                ok = this->saveSimulation(newsockfd, data);
                break;
            case ServerCommands::TURN_OFF:
                // vypni server
                std::cout << "Client " << newsockfd << " wants to turn off server" << std::endl;
                ok = this->turnOffServer(newsockfd);
            default:
                // ukonci spojenie
                command = ServerCommands::END;
                std::cout << "Client " << newsockfd << " wants to end connection" << std::endl;
                break;
        }

        // pokial sa nepodarilo odoslat data, tak posli chybovu spravu
        if (!ok) {
            std::cout << "Error sending data to client " << newsockfd << std::endl;
            throw std::runtime_error("Error sending data to client");
        }
    } while (command != ServerCommands::END);

    // zatvorenie socketov
    std::cout << "Closing socket" << std::endl;
    close(newsockfd);
}

bool MySocketServer::turnOffServer(int newsockfd) {
    // nastav signal na vypnutie servera
    {
        std::unique_lock<std::mutex> lck(this->mtx);
        std::cout << "Client " << newsockfd << " is turning off server" << std::endl;
        // sleep(5);
        this->stopSignal = true;
    }

    return true;
}

#endif //POS_SP_MYSOCKETSERVER_H
