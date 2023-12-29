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
    std::cout << "Listening on port " << port << ", cakam kym sa niekto pripoji" << std::endl;
    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    // potvrdenie spojenia s klientom
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }
    std::cout << "Pripojil sa klient a potvrdil som jeho pripojenie" << std::endl;

    bzero(buffer,256);

    // citanie zo socketu
    std::cout << "Cakam na spravu od klienta" << std::endl;
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return 4;
    }
    printf("Here is the message: %s\n", buffer);

    // get maptitle from buffer (with \n at the end)
    std::string mapTitle = buffer;
    mapTitle = mapTitle.substr(0, mapTitle.size()-1);
    SimulationCsvLoader loader("/home/kostor/sp/simulations.csv");
    SimulationCsvRecord simulationCsvRecord = loader.getByTitle(mapTitle);

    // zapis do socketu
    std::cout << "Posielam spravu klientovi" << std::endl;

    std::string msgStr = simulationCsvRecord.toCsv();
    const char* msg = msgStr.c_str();
    n = write(newsockfd, msg, strlen(msg)+1)    ;
    if (n < 0)
    {
        perror("Error writing to socket");
        return 5;
    }


    // zatvorenie socketov
    std::cout << "Zatvaram sockety" << std::endl;
    close(newsockfd);
    close(sockfd);

    return 0;
}

#endif //POS_SP_MYSOCKETSERVER_H
