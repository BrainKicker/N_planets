#include "system_of_planets.h"

system_of_planets::system_of_planets(int n, geo::dvector initial, std::vector<double> masses)
: _n(n), _initial(initial), _masses(masses), _trajectories(_n) {
    _window.create(sf::VideoMode(initial_width, initial_height), window_name);
    _rk = rk::runge_kutta(_initial, _f, _h, _method);
    for (int j = 0; j < _trajectories.size(); ++j) {
        for (int k = 0; k < dimension; ++k) {
            _trajectories[j].push_back(_rk.get_x()[j * dimension + k]);
        }
    }
    _conditions.push_back(_rk.get_x());
}

geo::dvector system_of_planets::g_acceleration(const geo::dvector& x, int index) {
    geo::dvector a(dimension);
    geo::dvector ri(dimension);
    for (int k = 0; k < dimension; ++k)
        ri[k] = _rk.get_x()[dimension*index + k];
    for (int j = 0; j < _n; ++j) {
        if (index == j)
            continue;
        geo::dvector rj(dimension);
        for (int k = 0; k < dimension; ++k)
            rj[k] = _rk.get_x()[dimension*j + k];
        geo::dvector diff = rj - ri;
        double len = diff.length();
        geo::dvector da = G * _masses[j] * diff / len / len / len;
        a += da;
    }
    return a;
}

//void system_of_planets::print_invariants() {
//    double E = 0, p = 0;
//    geo::dvector L (dimension);
//    for (int i = 0; i < _n; ++i) {
//        geo::dvector v(dimension);
//        for (int k = 0; k < dimension; ++k)
//            v[k] = _rk.get_x()[dimension*_n + dimension*i + k];
//        double v_len = v.length();
//        double E_K = _masses[i] * v_len * v_len / 2;
//        p += _masses[i] * v_len;
//        E += E_K;
//        geo::dvector ri(dimension);
//        L += ri * p;
//        for (int k = 0; k < dimension; ++k)
//            ri[k] = _rk.get_x()[dimension*i + k];
//        for (int j = i + 1; j < _n; ++j) {
//            geo::dvector rj(dimension);
//            for (int k = 0; k < dimension; ++k)
//                rj[k] = _rk.get_x()[dimension*j + k];
//            double E_P = G * _masses[i] * _masses[j] / (rj - ri).length();
//            E += E_P;
//        }
//    }
//    std::cout << "enegry of system: " << E << '\n';
//    std::cout << "impulce of system: " << p << '\n';
//    std::cout << "impulce momuntum of system: " << L << '\n';
//}

void system_of_planets::next_tragectory() {
    check_h();
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

void system_of_planets::check_h() {
    double min_distance = MAXFLOAT;
    for (int i = 0; i < _n; ++i) {
        for (int j = i + 1; j < _n; ++j) {
            geo::dvector dist_vector (dimension);
            for (int k = 0; k < dimension; ++k)
                dist_vector[k] = _rk.get_x()[dimension*i+k] - _rk.get_x()[dimension*j+k];
            min_distance = std::min(min_distance, dist_vector.length());
        }
    }
    min_distance = std::min(min_distance / 1000, 1.0);
//    min_distance = std::min(min_distance / 3, 1.0);
//    min_distance *= min_distance * min_distance * min_distance;
    _h = _initial_h * min_distance;
    _rk.set_step(_h);
}

void system_of_planets::draw_trajectory(const system_of_planets::trajectory& traj, const sf::Color& color) {
    const int width = _window.getSize().x, height = _window.getSize().y;
    if (dimension == 2) {
        for (int i = std::max(1, ((int) traj.size() / dimension) - max_amount_of_steps_drawen); i < traj.size() / dimension; ++i) {
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(zoom*traj[(i-1)*2] + width/2 + offset_x, zoom*traj[(i-1)*2+1] + height/2 + offset_y), color),
                    sf::Vertex(sf::Vector2f(zoom*traj[i*2] + width/2 + offset_x, zoom*traj[i*2+1] + height/2 + offset_y), color)
            };
            _window.draw(line, 2, sf::Lines);
        }
    } else
        throw 1;
}

void system_of_planets::move_camera_to_next_body() {
    static int cur = _n - 1;
    cur = (cur + 1) % _n;
    if (dimension == 2) {
        offset_x = - zoom * _trajectories[cur][_trajectories[cur].size()-2];
        offset_y = - zoom * _trajectories[cur][_trajectories[cur].size()-1];
    } else
        throw 1;
}

void system_of_planets::handle_keys() {
    if (keys[KEY_CONTINUE_EVALUATION] || keys[KEY_EVALUATE])
        next_tragectory();
    if (keys[KEY_CONTINUE_REMOVING] || keys[KEY_REMOVE])
        remove_last_trajectory();
    if (keys[KEY_UP])
        offset_y += _window.getSize().y * step_offset_y;
    if (keys[KEY_DOWN])
        offset_y -= _window.getSize().y * step_offset_y;
    if (keys[KEY_LEFT])
        offset_x += _window.getSize().x * step_offset_x;
    if (keys[KEY_RIGHT])
        offset_x -= _window.getSize().x * step_offset_x;
    if (keys[KEY_ADD_ZOOM]) {
        zoom *= zoom_speed;
        offset_x *= zoom_speed;
        offset_y *= zoom_speed;
    }
    if (keys[KEY_SUBTRACT_ZOOM]) {
        zoom /= zoom_speed;
        offset_x /= zoom_speed;
        offset_y /= zoom_speed;
    }
    if (keys[KEY_TAB])
        move_camera_to_next_body();
}

void system_of_planets::start() {
    sf::Event event;
    while (_window.isOpen()) {
        while (_window.pollEvent(event)) {
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
                            break;
                        case sf::Keyboard::S:
                        case sf::Keyboard::Down:
                            keys[KEY_DOWN] = true;
                            break;
                        case sf::Keyboard::A:
                        case sf::Keyboard::Left:
                            keys[KEY_LEFT] = true;
                            break;
                        case sf::Keyboard::D:
                        case sf::Keyboard::Right:
                            keys[KEY_RIGHT] = true;
                            break;
                        case sf::Keyboard::Add:
                            keys[KEY_ADD_ZOOM] = true;
                            break;
                        case sf::Keyboard::Subtract:
                            keys[KEY_SUBTRACT_ZOOM] = true;
                            break;
                        case sf::Keyboard::Space:
                            keys[KEY_EVALUATE] = true;
                            break;
                        case sf::Keyboard::Enter:
                            keys[KEY_CONTINUE_EVALUATION] = !keys[KEY_CONTINUE_EVALUATION];
                            break;
                        case sf::Keyboard::Backspace:
                            keys[KEY_REMOVE] = true;
                            break;
                        case sf::Keyboard::Delete:
                            keys[KEY_CONTINUE_REMOVING] = !keys[KEY_CONTINUE_REMOVING];
                            break;
                        case sf::Keyboard::Tab:
                            keys[KEY_TAB] = true;
                            break;
                        case sf::Keyboard::Escape:
                            _window.close();
                            break;
                        case sf::Keyboard::F11:
                            if (_fullscreen) {
                                _window.create(sf::VideoMode(_prev_size.first, _prev_size.second), window_name);
                            } else {
                                _prev_size = { _window.getSize().x, _window.getSize().y };
                                _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), window_name, sf::Style::Fullscreen);
                            }
                            _fullscreen = !_fullscreen;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                        case sf::Keyboard::Up:
                            keys[KEY_UP] = false;
                            break;
                        case sf::Keyboard::S:
                        case sf::Keyboard::Down:
                            keys[KEY_DOWN] = false;
                            break;
                        case sf::Keyboard::A:
                        case sf::Keyboard::Left:
                            keys[KEY_LEFT] = false;
                            break;
                        case sf::Keyboard::D:
                        case sf::Keyboard::Right:
                            keys[KEY_RIGHT] = false;
                            break;
                        case sf::Keyboard::Add:
                            keys[KEY_ADD_ZOOM] = false;
                            break;
                        case sf::Keyboard::Subtract:
                            keys[KEY_SUBTRACT_ZOOM] = false;
                            break;
                        case sf::Keyboard::Space:
                            keys[KEY_EVALUATE] = false;
                            break;
                        case sf::Keyboard::Backspace:
                            keys[KEY_REMOVE] = false;
                            break;
                        case sf::Keyboard::Tab:
                            keys[KEY_TAB] = false;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::MouseWheelMoved:
                    if (event.mouseWheel.delta > 0) {
                        keys[KEY_ADD_ZOOM] = true;
                        keys[KEY_SUBTRACT_ZOOM] = false;
                    } else {
                        keys[KEY_ADD_ZOOM] = false;
                        keys[KEY_SUBTRACT_ZOOM] = true;
                    }
                    break;
                default:
                    break;
            }
        }
        handle_keys();
//        print_invariants();
        draw();

        keys[KEY_TAB] = false;
        keys[KEY_ADD_ZOOM] = false;
        keys[KEY_SUBTRACT_ZOOM] = false;
    }
}

void system_of_planets::draw() {
    _window.clear(sf::Color::Black);
    for (int i = 0; i < _n; ++i)
        draw_trajectory(_trajectories[i], i < colors.size() ? colors[i] : sf::Color::White);
    _window.display();
}