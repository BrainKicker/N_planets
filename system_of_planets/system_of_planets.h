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

    inline static const char *const window_name = "N planets";

    inline static const int initial_width = 1080, initial_height = 720;

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
        KEY_TAB,

        KEYS_COUNT
    };

    using trajectory = std::vector<double>;

    inline static constexpr int dimension = 2;
    inline static constexpr double G = 6.67430151515151515151515151515e-3;
//    inline static constexpr double G = 6.67430151515151515151515151515e-11;

    inline static const std::vector<sf::Color> colors = {
            sf::Color::Red,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Magenta,
            sf::Color::White,
            sf::Color::Cyan,
            sf::Color::Yellow
    };

    double zoom = 20;
    double zoom_speed = 1.2;

    double offset_x = 0, offset_y = 0;
    double step_offset_x = 0.0003, step_offset_y = 0.0003;

    int max_amount_of_steps_drawen = 100000;

    bool keys[KEYS_COUNT] {};

    sf::RenderWindow _window {};

    bool _fullscreen = false;
    std::pair<int,int> _prev_size {};

    int _n;
    std::vector<double> _masses;
    // first dimension * n elements - rs, second dimension * n elements - vs
    geo::dvector _initial;
    const double _initial_h = 0.01;
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

    void print_invariants();

    void check_h();

    void next_tragectory();
    void remove_last_trajectory();

    void draw_trajectory(const trajectory& traj, const sf::Color& color);

    void move_camera_to_next_body();

    void handle_keys();

public:
    system_of_planets(int n, geo::dvector initial, std::vector<double> masses);

    void start();

    void draw();
};


#endif //MY_CALMATH_SYSTEM_OF_PLANETS_H