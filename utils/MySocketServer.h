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
#include "SimulationCsvLoader.h"

enum ServerCommands : int {
    END = 0,
    LIST = 1,
    LOAD = 2,
    SAVE = 3
};

class MySocketServer {
private:
    int port;
    int sockfd;
    std::vector<int> newsockfds;
    SimulationCsvLoader simulationLoader;

public:
    MySocketServer(int port, std::string pathToCsv) : port(port), simulationLoader(pathToCsv) {
    };
    ~MySocketServer() {};
    int run();

private:
    bool sendDataToClient(int sockfd, std::string data);

    bool sendSimulationsList(int sockfd);
    bool sendSimulation(int sckfd, std::string simTitle);
    bool saveSimulation(int sckfd, std::string csvRecordStr);
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

    // listen na sockete
    std::cout << "Listening on port " << port << ", waiting for client to connect" << std::endl;
    listen(this->sockfd, 5);
    cli_len = sizeof(cli_addr);

    // potvrdenie spojenia s klientom
    // todo od tohto miesta vytvorit novy thread
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }
    std::cout << "Client connected" << std::endl;

    char buffer[256];
    bzero(buffer,256);

    ServerCommands command;
    std::string data;
    // citanie zo socketu
    do {
        std::cout << "Waiting for client to send message" << std::endl;
        n = read(newsockfd, buffer, 255);
        if (n < 0)
        {
            perror("Error reading from socket");
            return 4;
        }
        printf("Here is the message: %s\n", buffer);

        // split buffer into command and data (; as separator)
        command = ServerCommands(atoi(strtok(buffer, ";")));
        data = strtok(NULL, ";");

        bool ok = false;
        switch (command) {
            case ServerCommands::LIST:
                std::cout << "Client wants to list simulations" << std::endl;
                ok = this->sendSimulationsList(newsockfd);
                break;
            case ServerCommands::LOAD:
                std::cout << "Client wants to load simulation" << std::endl;
                ok = this->sendSimulation(newsockfd, data);
                break;
            case ServerCommands::SAVE:
                std::cout << "Client wants to save simulation" << std::endl;
                ok = this->saveSimulation(newsockfd, data);
                break;
            default:
                command = ServerCommands::END;
                std::cout << "Client wants to end connection" << std::endl;
                break;
        }

        if (!ok) {
            std::cout << "Error sending data to client" << std::endl;
            return 5;
        }
    } while (command != ServerCommands::END);


    // zatvorenie socketov
    // todo - zatvorit sockety az po prijati spravy na ukoncenie spojenia
    std::cout << "Closing sockets" << std::endl;
    close(newsockfd);
    close(sockfd);

    return 0;
}

bool MySocketServer::sendSimulationsList(int sockfd) {
    std::string msg;

    for (std::pair<const std::basic_string<char>, SimulationCsvRecord> item : this->simulationLoader.getSimulationRecords()) {
        msg += item.second.getTitle() + "\n";
    }

    return this->sendDataToClient(sockfd, msg);
}

bool MySocketServer::sendSimulation(int sckfd, std::string simTitle) {
    SimulationCsvRecord simulationCsvRecord = this->simulationLoader.getByTitle(simTitle);

    // zapis do socketu
    std::cout << "Sending simulation info to client" << std::endl;
    std::cout << simulationCsvRecord.toCsv() << std::endl;

    return this->sendDataToClient(sckfd, simulationCsvRecord.toCsv());
}

bool MySocketServer::sendDataToClient(int sockfd, std::string data) {
    const char* msg = data.c_str();
    int n = write(sockfd, msg, strlen(msg)+1)    ;
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
    this->simulationLoader.addSimulationRecord(simulationCsvRecord);

    auto msg = "Simulation " + simulationCsvRecord.getTitle() + " saved";
    std::cout << msg << std::endl;

    return this->sendDataToClient(sckfd, msg);
}

#endif //POS_SP_MYSOCKETSERVER_H
