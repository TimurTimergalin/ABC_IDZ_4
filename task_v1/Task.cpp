#include "Task.h"

#include <unistd.h>

#include <sstream>


template<class T>
Task<T>::Task(const std::string &name,
              Gardener &me,
              Gardener &other,
              FieldIterable<T> field,
              int cultivating_time,
              int time_unit_ms,
              pthread_mutex_t *mtx,
              Output &out)
        : name(name),
          me(me),
          other(other),
          field(field),
          cultivating_time(cultivating_time),
          time_unit_ms(time_unit_ms),
          gardener_mutex(mtx),
          out(out) {
}

template<class T>
void Task<T>::Routine() {
    auto end = field.end();
    for (auto sec = field.begin(); sec != end; ++sec) {
        auto [x, y, s] = *sec;  // x, y - координаты следующего поля, которое необходимо обработать,
                                // ссылка на состояние этого поля
        pthread_mutex_lock(gardener_mutex);  // Захватываем мьютекс - для проверки координат
        if (x == other.x && y == other.y) {  // Если другой садовник на следующей клетке...
            std::stringstream ss{};
            ss << name << ": " << "another gardener encountered " << x << ' ' << y << "\n";
            out.Write(ss.str());
            --sec;  // ...откатываемся назад,...
            pthread_mutex_unlock(gardener_mutex);
            usleep(time_unit_ms * 1000); //...ждем (минимально возможное время)...
            continue;  // ...и пробуем еще раз
        }
        // клетка свободна

        // двигаемся на следующую клетку
        me.x = x;
        me.y = y;
        pthread_mutex_unlock(gardener_mutex);  // Отпускаем мьютекс - работа с положением садовников закончена

        if (s == Sector::UNCULTIVATED) {  // Если сектор не обработан
            std::stringstream ss{};
            ss << name << ": " << "cultivating " << x << ' ' << y << "\n";
            out.Write(ss.str());
            usleep(cultivating_time * time_unit_ms * 1000);
            s = Sector::CULTIVATED;  // обрабатываем
        } else if (s == Sector::CULTIVATED) {  // Если уже обработан
            // пропускаем
            std::stringstream ss{};
            ss << name << ": " << x << ' ' << y << " already cultivated\n";
            out.Write(ss.str());
        } else {  // если невозможно обработать
            // тоже пропускаем
            std::stringstream ss{};
            ss << name << ": " << x << ' ' << y << " is impossible to cultivate\n";
            out.Write(ss.str());
        }

        pthread_mutex_lock(gardener_mutex);  // захватываем мьютекс, чтобы изменить свое положение на...
        me.Rest();  // ...недостежимое (см. Gardener.Rest)
        pthread_mutex_unlock(gardener_mutex);  // отпускаем мьютекс

        std::stringstream ss{};
        ss << name << ": " << "moving on" << "\n";
        out.Write(ss.str());
        usleep(time_unit_ms * 1000);  // Двигаемся дальше
    }
}

template<class T>
void *Task<T>::Handle(void *this_) {
    auto *actual = reinterpret_cast<Task<T> *>(this_);
    actual->Routine();
    return nullptr;
}

template
class Task<FirstGardenerIterator>;

template
class Task<SecondGardenerIterator>;
