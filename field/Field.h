#pragma once

#include <random>
#include <vector>

#ifndef IDZ4_FIELD_H
#define IDZ4_FIELD_H

// Представляет одно поле сада
enum class Sector {
    CULTIVATED,
    UNCULTIVATED,
    INACCESSIBLE
};


// Класс, представляющий сад.
// Поскольку садовники гарантированно находятся в разных клетках,
// состояния гонки не возникнет, поэтому никаких инструментов синхронизации
// не предусмотрено
class Field {
public:
    const size_t n;
    const size_t m;
private:
    std::vector<std::vector<Sector>> field;
    std::mt19937 rm;  // генератор случайных чисел

    void Fill();
public:
    Field(size_t n, size_t m);
    std::vector<Sector>& operator[](size_t x);
};


#endif //IDZ4_FIELD_H
