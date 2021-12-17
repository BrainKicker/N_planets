cmake_minimum_required(VERSION 3.20)
project(my_calmath)

set(CMAKE_CXX_STANDARD 20)

add_executable(my_calmath main.cpp dymatrix.h geometry.h runge_kutta/runge_kutta.h runge_kutta/runge_kutta.cpp system_of_planets/system_of_planets.cpp system_of_planets/system_of_planets.h)

set(CMAKE_CXX_FLAGS "-lsfml-system -lsfml-window -lsfml-graphics")