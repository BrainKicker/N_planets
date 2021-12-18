#include <iostream>
#include <iomanip>

#include <SFML/Graphics.hpp>

#include "dymatrix.h"
#include "geometry.h"
#include "runge_kutta/runge_kutta.h"
#include "system_of_planets/system_of_planets.h"

int main() {

    sf::Event event;
    sf::Clock clock;

    system_of_planets system(3, { 0, -5, -2.5, 2.5, 5, 0, -0.05, 0, 0.025, 0.025, 0, +0.05 }, { 1, 2, 3 });

    system.start();

//    while (window.isOpen()) {
//        while (window.waitEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//            system.draw();
//        }
//    }

//    double x0 = 10;
//    double h = 0.01;
//    rk::runge_kutta::function _f = [](const geo::dvector& vect) -> geo::dvector { return { -0.1 * vect[0] }; };
//    rk::runge_kutta rungeKutta({x0}, _f, h, rk::runge_kutta::euler);
//    rk::runge_kutta rungeKutta1({x0}, _f, h, rk::runge_kutta::midpoint);
//    rk::runge_kutta rungeKutta2({x0}, _f, h, rk::runge_kutta::rk4);
//    rk::runge_kutta rungeKutta3({x0}, _f, h, rk::runge_kutta::method38);
//    rk::runge_kutta rungeKutta4({x0}, _f, h, rk::runge_kutta::dormand_prince);
//    for (int i = 0;;) {
//        std::cout << "step №" << std::setw(4) << i << ' ';
//        std::cout << "time = " << i << " seconds ";
//        std::cout << std::setw(15) << rungeKutta.get_x()[0] << ' ';
//        std::cout << std::setw(15) << rungeKutta1.get_x()[0] << ' ';
//        std::cout << std::setw(15) << rungeKutta2.get_x()[0] << ' ';
//        std::cout << std::setw(15) << rungeKutta3.get_x()[0] << ' ';
//        std::cout << std::setw(15) << rungeKutta4.get_x()[0] << '\n';
//        if (++i > 1000)
//            break;
//        rungeKutta.run_until(i);
//        rungeKutta1.run_until(i);
//        rungeKutta2.run_until(i);
//        rungeKutta3.run_until(i);
//        rungeKutta4.run_until(i);
//    }
}