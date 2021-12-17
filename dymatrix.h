#ifndef MY_CALMATH_DYMATRIX_H
#define MY_CALMATH_DYMATRIX_H

#include <vector>

template <typename T>
using row = std::vector<T>;

template <typename T>
class dymatrix {

    std::vector<row<T>> _rows;

public:

    dymatrix();
    dymatrix(const std::initializer_list<row<T>>& list);

    int size();

    row<T>& operator[](int index);

    dymatrix<T>& operator=(std::initializer_list<row<T>> list);
};

template <typename T>
dymatrix<T>::dymatrix() = default;

template <typename T>
dymatrix<T>::dymatrix(const std::initializer_list<row<T>>& list) {
    operator=(list);
}

template <typename T>
int dymatrix<T>::size() {
    return _rows.size();
}

template <typename T>
row<T>& dymatrix<T>::operator[](int index) {
    return _rows[index];
}

template <typename T>
dymatrix<T>& dymatrix<T>::operator=(std::initializer_list<row<T>> list) {
    _rows.resize(list.size());
    auto iter = list.begin();
    for (int i = 0; i < _rows.size(); ++i, ++iter)
        _rows[i] = std::move(*iter);
    return *this;
}

#endif //MY_CALMATH_DYMATRIX_H