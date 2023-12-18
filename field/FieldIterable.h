#include "Field.h"

#include <tuple>

#ifndef IDZ4_FIELDITERABLE_H
#define IDZ4_FIELDITERABLE_H


// Iterable-объект для обхода сада циклом for
template<class It>
class FieldIterable {
private:
    Field& field;
public:
    explicit FieldIterable(Field& field);
    It begin();
    It end();
};

// Итератор обхода сада первым садовником (слева направа и обратно - сверху вних)
class FirstGardenerIterator {
private:
    Field& field;
    size_t i;
public:
    FirstGardenerIterator(Field &field, size_t i);
    FirstGardenerIterator& operator++();
    FirstGardenerIterator& operator--();
    std::tuple<size_t, size_t, Sector&> operator*() const;
    bool operator!=(const FirstGardenerIterator& other);
};

// Итератор обхода сада вторым садовником (снизу вверх и обратно - справа налево)
class SecondGardenerIterator {
private:
    Field& field;
    size_t i;
public:
    SecondGardenerIterator(Field &field, size_t i);
    SecondGardenerIterator& operator++();
    SecondGardenerIterator& operator--();
    std::tuple<size_t, size_t, Sector&> operator*() const;
    bool operator!=(const SecondGardenerIterator& other);
};

#endif //IDZ4_FIELDITERABLE_H
