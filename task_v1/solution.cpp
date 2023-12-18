#include "../field/Field.h"
#include "../gardener/Gardener.h"
#include "../io/Output.h"
#include "Task.h"

#include <fstream>
#include <iostream>

#include "solution.h"

namespace version1 {
    void solution(size_t n, size_t m, int time_unit_ms, int v1, int v2, const std::string &output_file) {
        Field f(n, m);  // Создаём поле,

        Gardener first{};  // координаты садовников,
        Gardener second{};

        pthread_mutex_t mtx;  // мьютекс для доступа к координатам
        pthread_mutex_init(&mtx, nullptr);

        std::ofstream log(output_file);
        Output out{&std::cout, &log};  // Объект для вывода

        // Группируем их в объекты типа Task
        Task<FirstGardenerIterator> ftask("Gardener 1", first, second, FieldIterable<FirstGardenerIterator>(f), v1,
                                          time_unit_ms, &mtx, out);
        Task<SecondGardenerIterator> stask("Gardener 2", second, first, FieldIterable<SecondGardenerIterator>(f), v2,
                                           time_unit_ms, &mtx, out);

        pthread_t ft;
        pthread_t st;

        // Создаём потоки
        pthread_create(&ft, nullptr, Task<FirstGardenerIterator>::Handle, reinterpret_cast<void *>(&ftask));
        pthread_create(&st, nullptr, Task<SecondGardenerIterator>::Handle, reinterpret_cast<void *>(&stask));

        // Ждём их завершения
        pthread_join(ft, nullptr);
        pthread_join(st, nullptr);
        // Освобождаем ресурсы
        pthread_mutex_destroy(&mtx);
        log.close();
    }
}

