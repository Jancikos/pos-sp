#include "utils/MySocketServer.h"


int main() {
    MySocketServer server(2956, "/home/kostor/sp/simulations.csv");
    return server.run();
}
