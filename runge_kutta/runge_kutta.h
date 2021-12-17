#ifndef MY_CALMATH_RUNGE_KUTTA_H
#define MY_CALMATH_RUNGE_KUTTA_H

#include <functional>

#include "../dymatrix.h"
#include "../geometry.h"

namespace rk {

    class runge_kutta {
    public:

        class Method {
        public:
            enum class Type {
                EXPLICIT,
                IMPLICIT
            };

            Type _type;
            dymatrix<double> _dm;
        };

        inline static const Method euler = { Method::Type::EXPLICIT, {{1}} };

        inline static const Method midpoint = { Method::Type::EXPLICIT, {{0.5},{0,1}} };

        inline static const Method rk4 =
                {
                    Method::Type::EXPLICIT,
                    {
                        {0.5},
                        {0, 0.5},
                        {0, 0, 1},
                        {1.0/6, 1.0/3, 1.0/3, 1.0/6}
                    }
                };

        inline static const Method method38 =
                {
                        Method::Type::EXPLICIT,
                        {
                                {1.0/3},
                                {-1.0/3, 1},
                                {1, -1, 1},
                                {1.0/8, 3.0/8, 3.0/8, 1.0/8}
                        }
                };

        inline static const Method dormand_prince =
                {
                        Method::Type::EXPLICIT,
                        {
                                {1.0/5},
                                {3.0/40, 9.0/40},
                                {44.0/45, -56.0/15, 32.0/9},
                                {19372.0/6561, -25360.0/2187, 64448.0/6561, -212.0/729},
                                {9017.0/3168, -355.0/33, 46732.0/5247, 49.0/176, -5103.0/18656},
                                {35.0/384, 0, 500.0/1113, 125.0/192, -2187.0/6784, 11.0/84}
                        }
                };

        using function = std::function<geo::dvector(const geo::dvector&)>;
//        using function = geo::dvector (*)(const geo::dvector&);

    private:

        geo::dvector _x0;
        geo::dvector _x;
        function _f;
        double _h;
        double _t = 0;
        Method _method;

    public:
        runge_kutta() {}
        runge_kutta(geo::dvector init_x, function f, double h, Method method);

        geo::dvector get_x();
        void set_x(geo::dvector x);

        double get_step();
        void set_step(double h);

        double get_time();
        void set_time(double t);

        function get_f();
        void set_f(function f);

        Method get_method();
        void set_method(Method method);

        void step();

        void run_until(double time);

        void restart();
    };
}

#endif //MY_CALMATH_RUNGE_KUTTA_H