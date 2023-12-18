#include "../gardener/Gardener.h"
#include "../field/FieldIterable.h"
#include "../io/Output.h"

#include <pthread.h>

#ifndef IDZ4_TASK_H
#define IDZ4_TASK_H

// Класс, содержащий все данные для работы одного потока-садовника
template<class T>
class Task {
private:
    const std::string &name;  // Имя садовника - для красивого вывода
    Gardener &me;  // Координаты этого садовника
    Gardener &other;  // Координаты его коллеги
    FieldIterable<T> field;  // Iterable-объект сада
    int cultivating_time;  // Время обработки одного поля (в единицах времени)
    int time_unit_ms;  // Длина одной единицы времени в миллисекундах
    pthread_mutex_t *gardener_mutex;  // Мьютекс для доступа к координатам садовников (обоих - один на двоих)
    Output &out;  // Объект вывода
public:
    Task(const std::string &name, Gardener &me, Gardener &other, FieldIterable<T> field, int cultivating_time,
         int time_unit_ms,
         pthread_mutex_t *mtx, Output &out);

    void Routine();  // Метод, содержащий непосредственную логику потока

    static void *Handle(void *);  // Метод, вызывающий Routine(), совместимый с pthreads
};


#endif //IDZ4_TASK_H
