cmake_minimum_required(VERSION 3.18)
project(pos_sp)

set(CMAKE_CXX_STANDARD 17)

add_executable(pos_sp main.cpp models/Biotope.h utils/BiotopeManager.h models/Bunk.h utils/Map.h
        utils/Map.h)
