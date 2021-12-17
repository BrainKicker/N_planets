#include "system_of_planets.h"

system_of_planets::system_of_planets(int n, geo::dvector initial, std::vector<double> masses, sf::RenderWindow& window)
: _n(n), _initial(initial), _masses(masses), _window(window), _trajectories(_n) {
    _rk = rk::runge_kutta(_initial, _f, _h, _method);
    for (int j = 0; j < _trajectories.size(); ++j) {
        for (int k = 0; k < dimension; ++k) {
            _trajectories[j].push_back(_rk.get_x()[j * dimension + k]);
        }
    }
    _conditions.push_back(_rk.get_x());
//    evaluate_trajectories();
}

geo::dvector system_of_planets::g_acceleration(const geo::dvector& x, int index) {
    geo::dvector a(dimension);
    geo::dvector ri(dimension);
    for (int i = dimension * index; i < dimension * (index+1); ++i)
        ri[i%dimension] = x[i];
    for (int j = 0; j < _n; ++j) {
        if (index == j)
            continue;
        geo::dvector rj(dimension);
        for (int i = dimension * j; i < dimension * (j+1); ++i)
            rj[i%dimension] = x[i];
        geo::dvector diff = rj - ri;
        double len = diff.length();
        geo::dvector da = G * _masses[j] * diff / len / len / len;
        a += da;
    }
    return a;
}

void system_of_planets::next_tragectory() {
    _rk.run_until(_trajectories[0].size() / dimension);
    for (int j = 0; j < _trajectories.size(); ++j) {
        for (int k = 0; k < dimension; ++k) {
            _trajectories[j].push_back(_rk.get_x()[j * dimension + k]);
        }
    }
    _conditions.push_back(_rk.get_x());
}

void system_of_planets::remove_last_trajectory() {
    if (_trajectories[0].size() > dimension) {
        for (int j = 0; j < _trajectories.size(); ++j) {
            for (int k = 0; k < dimension; ++k) {
                _trajectories[j].pop_back();
            }
        }
        _rk.set_time(_trajectories[0].size() / dimension - 1);
        _rk.set_x(*_conditions.rbegin());
        _conditions.pop_back();
    }
}

void system_of_planets::draw_trajectory(const system_of_planets::trajectory& traj) {
    const int width = _window.getSize().x, height = _window.getSize().y;
    if (dimension == 2) {
        for (int i = 1; i < traj.size() / dimension; ++i) {
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(zoom*traj[(i-1)*2] + width / 2 + offset_x, zoom*traj[(i-1)*2+1] + height / 2 + offset_y)),
                    sf::Vertex(sf::Vector2f(zoom*traj[i*2] + width / 2 + offset_x, zoom*traj[i*2+1] + height / 2 + offset_y))
            };
            _window.draw(line, 2, sf::Lines);
        }
    } else
        throw 1;
}

void system_of_planets::handle_keys() {

}

void system_of_planets::start() {
    sf::Event event;
    while (_window.isOpen() && _window.waitEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                _window.close();
                break;
            case sf::Event::Resized:
                ((sf::View*)&_window.getView())->reset(sf::FloatRect(0, 0, _window.getSize().x, _window.getSize().y));
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::W:
                    case sf::Keyboard::Up:
                        keys[KEY_UP] = true;
                        offset_y += step_offset_y;
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Down:
                        keys[KEY_DOWN] = true;
                        offset_y -= step_offset_y;
                        break;
                    case sf::Keyboard::A:
                    case sf::Keyboard::Left:
                        keys[KEY_LEFT] = true;
                        offset_x += step_offset_x;
                        break;
                    case sf::Keyboard::D:
                    case sf::Keyboard::Right:
                        keys[KEY_RIGHT] = true;
                        offset_x -= step_offset_x;
                        break;
                    case sf::Keyboard::Add:
                        keys[KEY_ADD_ZOOM] = true;
                        zoom += step_zoom;
                        break;
                    case sf::Keyboard::Subtract:
                        keys[KEY_SUBTRACT_ZOOM] = true;
                        zoom -= step_zoom;
                        break;
                    case sf::Keyboard::Space:
                        keys[KEY_EVALUATE] = true;
                        break;
                    case sf::Keyboard::Enter:
                        keys[KEY_CONTINUE_EVALUATION] = true;
                        next_tragectory();
                        break;
                    case sf::Keyboard::Backspace:
                        keys[KEY_REMOVE] = true;
                        remove_last_trajectory();
                        break;
                    case sf::Keyboard::Delete:
                        keys[KEY_CONTINUE_REMOVING] = true;
                        remove_last_trajectory();
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        handle_keys();
        draw();
    }
}

void system_of_planets::draw() {
    _window.clear(sf::Color::Black);
    for (const trajectory& traj : _trajectories)
        draw_trajectory(traj);
    _window.display();
}

//void system_of_planets::evaluate_trajectories() {
//    // i - seconds
//    for (int i = 0;;) {
//        for (int j = 0; j < _trajectories.size(); ++j) {
//            for (int k = 0; k < dimension; ++k) {
//                _trajectories[j].push_back(_rk.get_x()[j * dimension + k]);
//            }
//        }
//        if (++i > 1000)
//            break;
//        _rk.run_until(i);
//    }
//}