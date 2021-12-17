#include "runge_kutta.h"

namespace rk {

    runge_kutta::runge_kutta(geo::dvector init_x, function f, double h, Method method)
    : _x0(init_x), _x(init_x), _f(f), _h(h), _method(method) {}

    geo::dvector runge_kutta::get_x() {
        return _x;
    }

    void runge_kutta::set_x(geo::dvector x) {
        _x = x;
    }

    double runge_kutta::get_step() {
        return _h;
    }

    void runge_kutta::set_step(double h) {
        _h = h;
    }

    double runge_kutta::get_time() {
        return _t;
    }

    void runge_kutta::set_time(double t) {
        _t = t;
    }

    runge_kutta::function runge_kutta::get_f() {
        return _f;
    }

    void runge_kutta::set_f(function f) {
        _f = f;
    }

    runge_kutta::Method runge_kutta::get_method() {
        return _method;
    }

    void runge_kutta::set_method(Method method) {
        _method = method;
    }

    void runge_kutta::step() {
        _t += _h;
        if (_method._type == Method::Type::EXPLICIT) {
            std::vector<geo::dvector> ks;
            ks.push_back(_f(_x));
            for (int i = 0; i < _method._dm.size() - 1; ++i) {
                geo::dvector cur_x = _x;
                for (int j = 0; j <= i; ++j)
                    cur_x += _h * _method._dm[i][j] * ks[j];
                ks.push_back(_f(cur_x));
            }
            geo::dvector next_x = _x;
            const row<double>& last_row = _method._dm[_method._dm.size()-1];
            for (int i = 0; i < last_row.size(); ++i)
                next_x += _h * last_row[i] * ks[i];
            _x = next_x;
        } else {
            throw 1;
        }
    }

    void runge_kutta::run_until(double time) {
        while (_t < time)
            step();
    }

    void runge_kutta::restart() {
        _x = _x0;
        _t = 0;
    }
}