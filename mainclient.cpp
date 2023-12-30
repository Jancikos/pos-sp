//
// Created by brisa on 29. 12. 2023.
//

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    std::cout << "Hello, World! CLIENT" << std::endl;
    return 0;

    int sockfd, n;
    struct sockaddr_in serv_addr{};
    struct hostent* server;

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

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 3;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to socket");
        return 4;
    }

    std::cout << "Please enter a message: ";
    bzero(buffer, 256);
    std::cin.getline(buffer, 255);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error writing to socket");
        return 5;
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        perror("Error reading from socket");
        return 6;
    }

    std::cout << buffer << std::endl;
    close(sockfd);

    return 0;
}

