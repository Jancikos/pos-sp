cmake_minimum_required(VERSION 3.18)
project(pos_sp)

set(CMAKE_CXX_STANDARD 17)

# Common source files for both client and server
set(COMMON_SOURCES
        models/Biotope.h
        utils/BiotopeManager.h
        models/Cell.h
        utils/Map.h
        utils/Simulation.h
        utils/Options.h
        utils/Helper.h
        models/Wind.h
        utils/WindManager.h
        utils/SimulationCsvLoader.h
        models/SimulationCsvRecord.h
        utils/MySocketServer.h
        utils/MySocketClient.h
)

# Client executable
add_executable(pos_sp_client
        ${COMMON_SOURCES}
        main_client.cpp
)

# Server executable
add_executable(pos_sp_server
        ${COMMON_SOURCES}
        main_server.cpp
        )

target_link_libraries(pos_sp_server pthread)
target_link_libraries(pos_sp_client pthread)
