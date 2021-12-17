#ifndef MY_CALMATH_SYSTEM_OF_PLANETS_H
#define MY_CALMATH_SYSTEM_OF_PLANETS_H

#include <iostream>
#include <iomanip>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "../geometry.h"
#include "../runge_kutta/runge_kutta.h"

class system_of_planets {

    enum Key {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_EVALUATE,
        KEY_REMOVE,
        KEY_ADD_ZOOM,
        KEY_SUBTRACT_ZOOM,
        KEY_CONTINUE_EVALUATION,
        KEY_CONTINUE_REMOVING,

        KEYS_COUNT
    };

    using trajectory = std::vector<double>;

    inline static constexpr int dimension = 2;
    inline static constexpr double G = 6.67430151515151515151515151515e-3;
//    inline static constexpr double G = 6.67430151515151515151515151515e-11;

    int zoom = 20;
    int step_zoom = 1;

    int offset_x = 0, offset_y = 0;
    int step_offset_x = 10, step_offset_y = 10;

    bool keys[KEYS_COUNT] {};

    sf::RenderWindow& _window;
    int _n;
    std::vector<double> _masses;
    // first dimension * n elements - rs, second dimension * n elements - vs
    geo::dvector _initial;
    double _h = 0.01;
    rk::runge_kutta::Method _method = rk::runge_kutta::method38;

    rk::runge_kutta _rk;

    std::vector<trajectory> _trajectories;
    std::vector<geo::dvector> _conditions;

    const rk::runge_kutta::function _f = [this](const geo::dvector& x) -> geo::dvector {
        geo::dvector result;
        for (int i = 0; i < dimension * _n; ++i) {
            result.push_back(x[dimension * _n + i]);
        }
        for (int i = 0; i < _n; ++i) {
            geo::dvector dv = g_acceleration(x, i);
            for (int j = 0; j < dimension; ++j) {
                result.push_back(dv[j]);
            }
        }
        return result;
    };

    geo::dvector g_acceleration(const geo::dvector& x, int index);

    void next_tragectory();
    void remove_last_trajectory();

    void draw_trajectory(const trajectory& traj);

    void handle_keys();

public:
    system_of_planets(int n, geo::dvector initial, std::vector<double> masses, sf::RenderWindow& window);

//    void test() {
//        for (int i = 0;;) {
//            std::cout << "step №" << std::setw(4) << i;
//            std::cout << ", time = " << i << " sec, ";
//            std::cout << '{' << _rk.get_x()[0] << ',' << _rk.get_x()[1] << "}, ";
//            std::cout << '{' << _rk.get_x()[2] << ',' << _rk.get_x()[3] << "}, ";
//            std::cout << '{' << _rk.get_x()[4] << ',' << _rk.get_x()[5] << "}\n";
//            if (++i > 10)
//                break;
//            _rk.run_until(i);
//        }
//    }

    void start();

    void draw();

//    void evaluate_trajectories();
};


#endif //MY_CALMATH_SYSTEM_OF_PLANETS_H