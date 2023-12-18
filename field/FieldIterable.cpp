#include "FieldIterable.h"

template<class It>
FieldIterable<It>::FieldIterable(Field &field) : field(field){
}

template<class It>
It FieldIterable<It>::begin() {
    return It(field, 0);
}

template<class It>
It FieldIterable<It>::end() {
    return It(field, field.n * field.m);
}

FirstGardenerIterator::FirstGardenerIterator(Field &field, size_t i) : field(field), i(i){
}

FirstGardenerIterator &FirstGardenerIterator::operator++() {
    ++i;
    return *this;
}

FirstGardenerIterator &FirstGardenerIterator::operator--() {
    --i;
    return *this;
}

// Содержательная часть итерации здесь
std::tuple<size_t, size_t, Sector&> FirstGardenerIterator::operator*() const {
    size_t x = i / field.m;
    size_t y = x % 2 == 0 ? i % field.m : field.m - 1 - i % field.m;
    return {x, y, field[x][y]};
}

bool FirstGardenerIterator::operator!=(const FirstGardenerIterator &other) {
    return (i != other.i) || (&field != &other.field);
}

SecondGardenerIterator::SecondGardenerIterator(Field &field, size_t i) : field(field), i(i){
}

SecondGardenerIterator &SecondGardenerIterator::operator++() {
    ++i;
    return *this;
}

SecondGardenerIterator &SecondGardenerIterator::operator--() {
    --i;
    return *this;
}

// Содержательная часть итерации здесь
std::tuple<size_t, size_t, Sector&> SecondGardenerIterator::operator*() const {
    size_t y = field.m - 1 - i / field.n;
    size_t x = (field.m - 1 - y) % 2 == 0 ? field.n - 1 - i % field.n : i % field.n;
    return {x, y, field[x][y]};
}

bool SecondGardenerIterator::operator!=(const SecondGardenerIterator &other) {
    return (&field != &other.field) || (i != other.i);
}

template class FieldIterable<FirstGardenerIterator>;
template class FieldIterable<SecondGardenerIterator>;
