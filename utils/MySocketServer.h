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
#include "SimulationCsvLoader.h"

class MySocketServer {
public:
    static int run(int port);

};

int MySocketServer::run(int port) {
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];

    // initialize socket structure and bind socket to port
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // create socket v domene AF_INET, typu SOCK_STREAM (TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    // priprava adresy servera a bindovanie socketu na port
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    // listen na sockete
    std::cout << "Listening on port " << port << ", waiting for client to connect" << std::endl;
    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    // potvrdenie spojenia s klientom
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }
    std::cout << "Client connected" << std::endl;

    bzero(buffer,256);

    // citanie zo socketu
    std::cout << "Waiting for client to send map title" << std::endl;
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return 4;
    }
    printf("Here is the message: %s\n", buffer);
    if (strcmp(buffer, "new") != 0)
    {
        // get maptitle from buffer (with \n at the end)
        std::string mapTitle = buffer;
        SimulationCsvLoader loader("/home/kostor/sp/simulations.csv");
        SimulationCsvRecord simulationCsvRecord = loader.getByTitle(mapTitle);
        // zapis do socketu
        std::cout << "Sending simulation info to client" << std::endl;
        std::cout << simulationCsvRecord.toCsv() << std::endl;

        std::string msgStr = simulationCsvRecord.toCsv();
        const char* msg = msgStr.c_str();
        n = write(newsockfd, msg, strlen(msg)+1)    ;
        if (n < 0)
        {
            perror("Error writing to socket");
            return 5;
        }
    }

    // precitam zo soketu ci chce klient ulozit simulaciu
    bzero(buffer,256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return 6;
    }
    std::cout << "buffer: " << buffer << std::endl;
    // if it read "1", then it means that client wants to save simulation
    if (strcmp(buffer, "save") == 0)
    {
        // server sends info that it is ready to receive simulation
        std::cout << "Sending ready message to client" << std::endl;
        std::string msgStr = "ready";
        const char* msg = msgStr.c_str();
        n = write(newsockfd, msg, strlen(msg)+1);
        if (n < 0)
        {
            perror("Error writing to socket");
            return 5;
        }

        // server reads simulation from socket
        std::cout << "Client wants to save simulation" << std::endl;
        bzero(buffer,256);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
        {
            perror("Error reading from socket");
            return 6;
        }

        // parse the simulation from buffer and save it to csv
        SimulationCsvLoader loader("/home/kostor/sp/simulations.csv");
        SimulationCsvRecord simulationCsvRecord;

        std::string item;
        int i = 0;
        std::stringstream ss(buffer);
        std::cout << "buffer: " << buffer << std::endl;
        while (getline(ss, item, ';'))
        {
            switch (i)
            {
                case 0:
                    simulationCsvRecord.setTitle(item);
                    break;
                case 1:
                    simulationCsvRecord.setSeed(std::stoul(item));
                    break;
                case 2:
                    simulationCsvRecord.setWidth(std::stoi(item));
                    break;
                case 3:
                    simulationCsvRecord.setHeight(std::stoi(item));
                    break;
                case 4:
                    simulationCsvRecord.setTime(std::stoul(item));
                    break;
                default:
                    break;
            }
            i++;
        }
        loader.addSimulationRecord(simulationCsvRecord);
        loader.save();
        std::cout << "Simulation saved" << std::endl;
    }

    // zatvorenie socketov
    std::cout << "Closing sockets" << std::endl;
    close(newsockfd);
    close(sockfd);

    return 0;
}

#endif //POS_SP_MYSOCKETSERVER_H
