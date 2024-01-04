OUTPUTS = client server
CC = g++
COMMON_SOURCES = models/Biotope.h utils/BiotopeManager.h models/Cell.h utils/Map.h utils/Simulation.h utils/Options.h utils/Helper.h models/Wind.h utils/WindManager.h utils/SimulationCsvLoader.h models/SimulationCsvRecord.h utils/MySocketServer.h utils/MySocketClient.h

CXXFLAGS = -std=c++17 -pthread

all: $(OUTPUTS)

clean:
	rm -f $(OUTPUTS)

client: main_client.cpp
	$(CC) -o client main_client.cpp $(CXXFLAGS)

server: main_server.cpp
	$(CC) -o server main_server.cpp $(COMMON_SOURCES) $(CXXFLAGS)
