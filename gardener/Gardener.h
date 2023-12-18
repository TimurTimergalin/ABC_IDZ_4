#include <cstdint>
#include <limits>

#ifndef IDZ4_GARDENER_H
#define IDZ4_GARDENER_H


// Контейнер для хранения информации о местоположении одного садовника
// Встроенных механизмов синхронизации нет
struct Gardener {
    size_t x;
    size_t y;

    Gardener() : x(std::numeric_limits<size_t>::max()), y(std::numeric_limits<size_t>::max()) {}
    Gardener(size_t x, size_t y) : x(x), y(y) {}

    // Решение проблемы противостояния методом "повышенной толерантности".
    // Когда садовник не работает, его местоположение помечается как
    // (std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()).
    // Такое число никогда не будет достигнуто - поле такого размера даже не уместится в оперативную память,
    // таким образом, такой садовник гарантированно не помешает другому
    void Rest() {
        x = std::numeric_limits<size_t>::max();
        y = std::numeric_limits<size_t>::max();
    }
};


#endif //IDZ4_GARDENER_H
