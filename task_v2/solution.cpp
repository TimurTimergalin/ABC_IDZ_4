#include "../field/FieldIterable.h"
#include "../io/Output.h"
#include "Task.h"
#include "TrafficGuard/TrafficGuard.h"

#include <fstream>
#include <iostream>

#include "pthread.h"
#include "solution.h"

namespace version2 {
    void solution(size_t n, size_t m, int time_unit_ms, int v1, int v2, const std::string &output_file) {
        Field f(n, m);  // Создаём поле,

        GardenerQueue queue{};  // очередь для садовников,
        std::ofstream log(output_file);
        Output out{&std::cout, &log};  // объект для вывода

        // Группируем все в объекты типа Task...
        Task<FirstGardenerIterator> ftask(queue, 0, FieldIterable<FirstGardenerIterator>(f), v1,
                                          time_unit_ms, out);
        Task<SecondGardenerIterator> stask(queue, 1, FieldIterable<SecondGardenerIterator>(f), v2,
                                           time_unit_ms, out);

        // ... и TrafficGuard
        TrafficGuard guard(queue, out);

        pthread_t first_gardener_thread;
        pthread_t second_gardener_thread;
        pthread_t guard_thread;

        // Создаём потоки
        pthread_create(&first_gardener_thread, nullptr, Task<FirstGardenerIterator>::Handle,
                       reinterpret_cast<void *>(&ftask));
        pthread_create(&second_gardener_thread, nullptr, Task<SecondGardenerIterator>::Handle,
                       reinterpret_cast<void *>(&stask));
        pthread_create(&guard_thread, nullptr, TrafficGuard::Handle, reinterpret_cast<void *>(&guard));
        pthread_detach(guard_thread);  // Помечаем поток регулировщика detached - ведь мы не будем join-ить его

        // Ждем завершение работы садовников
        pthread_join(first_gardener_thread, nullptr);
        pthread_join(second_gardener_thread, nullptr);
        pthread_cancel(guard_thread);  // Останавливаем регулировщика
        // Освобождаем ресурсы
        log.close();
    }
}
