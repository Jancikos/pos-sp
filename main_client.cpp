//
// Created by brisa on 29. 12. 2023.
//

#include <chrono>
#include "utils/SimulationCsvLoader.h"
#include "utils/Options.h"
#include "utils/Simulation.h"
#include "utils/MySocketClient.h"

int main(int argc, char *argv[]) {
    MySocketClient client;
    return client.run("localhost", 2956);
}

