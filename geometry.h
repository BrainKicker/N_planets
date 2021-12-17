#ifndef MY_CALMATH_GEOMETRY_H
#define MY_CALMATH_GEOMETRY_H

#include <cmath>
#include <vector>

namespace geo {

    template <typename T>
    class vector : public std::vector<T>{
    public:
        vector();
        vector(int n);
        vector(const std::initializer_list<T>& list);
        vector(const std::vector<T>& vect);
        vector(const vector<T>& vect);

        T length();

        vector<T>& operator+();
        vector<T>& operator-();

        vector<T> operator+(const vector<T>& other) const;
        vector<T>& operator+=(const vector<T>& other);
        vector<T> operator-(const vector<T>& other) const;
        vector<T>& operator-=(const vector<T>& other);

        vector<T> operator*(const T& n) const;
        vector<T>& operator*=(const T& n);
        vector<T> operator/(const T& n) const;
        vector<T>& operator/=(const T& n);

        template <typename R>
        friend vector<R> operator*(const R& n, const vector<R>& vect);
    };

    template <typename T>
    vector<T>::vector() = default;

    template <typename T>
    vector<T>::vector(int n) : std::vector<T>(n) {}

    template <typename T>
    vector<T>::vector(const std::initializer_list<T>& list) : std::vector<T>(list) {}

    template <typename T>
    vector<T>::vector(const std::vector<T>& vect) : std::vector<T>(vect) {}

    template <typename T>
    vector<T>::vector(const vector<T>& vect) : std::vector<T>(vect) {}

    template <typename T>
    T vector<T>::length() {
        T sum = 0;
        for (int i = 0; i < std::vector<T>::size(); ++i)
            sum += std::vector<T>::operator[](i) * std::vector<T>::operator[](i);
        return std::sqrt(sum);
    }

    template <typename T>
    vector<T>& vector<T>::operator+() {
        return *this;
    }

    template <typename T>
    vector<T>& vector<T>::operator-() {
        for (int i = 0; i < std::vector<T>::size(); ++i)
            std::vector<T>::operator[](i) = -std::vector<T>::operator[](i);
        return *this;
    }

    template <typename T>
    vector<T> vector<T>::operator+(const vector<T>& other) const {
        return vector<T>{*this} += other;
    }

    template <typename T>
    vector<T>& vector<T>::operator+=(const vector<T>& other) {
        int min_len = std::min(std::vector<T>::size(), other.size());
        for (int i = 0; i < min_len; ++i)
            std::vector<T>::operator[](i) += other[i];
        return *this;
    }

    template <typename T>
    vector<T> vector<T>::operator-(const vector<T>& other) const {
        return vector<T>{*this} -= other;
    }

    template <typename T>
    vector<T>& vector<T>::operator-=(const vector<T>& other) {
        int min_len = std::min(std::vector<T>::size(), other.size());
        for (int i = 0; i < min_len; ++i)
            std::vector<T>::operator[](i) -= other[i];
        return *this;
    }

    template <typename T>
    vector<T> vector<T>::operator*(const T& n) const {
        return vector<T>{*this} *= n;
    }

    template <typename T>
    vector<T>& vector<T>::operator*=(const T& n) {
        for (int i = 0; i < std::vector<T>::size(); ++i)
            std::vector<T>::operator[](i) *= n;
        return *this;
    }

    template <typename T>
    vector<T> vector<T>::operator/(const T& n) const {
        return vector<T>{*this} /= n;
    }

    template <typename T>
    vector<T>& vector<T>::operator/=(const T& n) {
        for (int i = 0; i < std::vector<T>::size(); ++i)
            std::vector<T>::operator[](i) /= n;
        return *this;
    }

    template <typename T>
    vector<T> operator*(const T& n, const vector<T>& vect) {
        return vect * n;
    }

    using dvector = vector<double>;
}

#endif //MY_CALMATH_GEOMETRY_H