#include "Field.h"

#include <algorithm>

Field::Field(size_t n, size_t m) : field(n, std::vector<Sector>(m, Sector::UNCULTIVATED)), n(n), m(m),
                                   rm(std::random_device{}()) {  // такая инициализация гарантирует большую энтропию
    Fill();
}

// Заполняет поле необрабатываемыми полями
void Field::Fill() {
    std::uniform_real_distribution<long double> prob(0.1, 0.3);  // доля необрабатываемых полей - случайная величина,
    // которая распределяется по закону U[0.1;0.3]
    auto inaccessible_count = static_cast<size_t>(
            m * n * prob(rm)  // итоговое число необрабатываемых полей
    );
    std::vector<bool> ponds(m * n, false);

    for (size_t i = 0; i < inaccessible_count; ++i) {  // Создаем bitarray размером mn, меняем первые inaccessible_count
                                                       // элементов на 1...
        ponds[i] = true;
    }

    std::shuffle(ponds.begin(), ponds.end(), rm);  // ...и случайным образом перемешиваем

    // Делаем поля, соответствующие 1 в bitarray, на необрабатываемые
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (ponds[i * m + j]) {
                field[i][j] = Sector::INACCESSIBLE;
            }
        }
    }

}

std::vector<Sector> &Field::operator[](size_t x) {
    return field[x];
}




