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

class MySocketClient {
public:
    static int run(int argc, char *argv[]);
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

    // tu sa pripravi loader a ak je to potrebne, tak sa nacita simulacia zo suboru
    SimulationCsvLoader loader("/home/kostor/sp/simulations.csv");
    // SimulationCsvLoader loader("./data/simulations.csv");
    SimulationCsvRecord simulationCsvRecord;

    Options nacitanie;
    std::cout << "Do you want to load map from server?" << std::endl;
    nacitanie.addOption(1, "yes");
    nacitanie.addOption(2, "no");
    int result = nacitanie.getOptionCLI("Enter number of your choice: ");

    switch (result) {
        case 1: {
            // ulozene simulacie sa vypisu
            for (auto const& [key, simulationCsvRecord] : loader.getSimulationRecords())
            {
                std::cout << key << std::endl;
            }

            // tu sa nacita simulacia zo suboru
            // posli nazov simulacie na server
            std::string simTitle = Helper::readLineFromConsole("Enter simulation title: ");
            // poslem spravu na server
            strcpy(buffer, simTitle.c_str());
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                return 5;
            }

            // precitam spravu od servera a parsnem ju do simulationCsvRecord
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            if (n < 0) {
                perror("Error reading from socket");
                return 6;
            }
            std::cout << buffer << std::endl;
            std::string item;
            int i = 0;
            std::stringstream ss(buffer);
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
            break;
        }
        case 2:
            // tu sa vytvori nova simulacia
            // posli spravu na server, ze chcem vytvorit novu simulaciu
            std::string simTitle = "new";
            strcpy(buffer, simTitle.c_str());
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                return 5;
            }

            // vypytam si potrebne udaje od uzivatela a vytvorim novu simulaciu
            std::cout << "Enter simulation title: " << std::endl;
            std::string title;
            std::cin >> title;
            simulationCsvRecord.setTitle(title);
            simulationCsvRecord.setSeed(std::chrono::system_clock::now().time_since_epoch().count());
            std::cout << "Enter map width and height: " << std::endl;
            int width, height;
            std::cin >> width >> height;
            simulationCsvRecord.setWidth(width);
            simulationCsvRecord.setHeight(height);
            break;
    }

    // tu zbehne simulacia
    Simulation simulation(simulationCsvRecord);
    simulation.run();

    // spyta sa ci chce ulozit simulaciu
    Options options;
    options.addOption(1, "yes");
    options.addOption(2, "no");
    int saveResult = options.getOptionCLI("Do you want to save simulation?");

    if (saveResult == 1) {
        // posli spravu na server, ze chcem ulozit simulaciu
        std::string resultSave = "save";
        strcpy(buffer, resultSave.c_str());
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }

        // precitam spravu od servera, ze je ready na prijatie simulacie
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            perror("Error reading from socket");
            return 6;
        }

        // simulacia sa posle na server
        simulationCsvRecord = simulation.toCsvRecord();
        std::string simulationString = simulationCsvRecord.toCsv();
        strcpy(buffer, simulationString.c_str());
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }
    }

    // zatvorim socket
    close(sockfd);

    return 0;
}


#endif //POS_SP_MYSOCKETCLIENT_H
