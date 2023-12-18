#include "Task.h"

#include <unistd.h>

#include <sstream>

namespace version2 {
    template<class It>
    Task<It>::Task(GardenerQueue &queue, int id, FieldIterable<It> field, int cultivating_time,
                   int time_unit_ms, Output &out) : queue(queue), id(id), field(field),
                                                    cultivating_time(cultivating_time), time_unit_ms(time_unit_ms),
                                                    out(out) {

    }

    template<class It>
    void Task<It>::Routine() {
        auto end = field.end();

        for (auto sec = field.begin(); sec != end; ++sec) {
            auto [x, y, s] = *sec;  // x, y - координаты следующего поля, которое необходимо обработать,
                                    // ссылка на состояние этого поля
            queue.CheckIn(id, x, y, out);  // Двигаемся на следующее поле

            if (s == Sector::UNCULTIVATED) {  // Если поле не обработано
                std::stringstream ss{};
                ss << "Gardener " << (id + 1) << ": " << "cultivating " << x << ' ' << y << "\n";
                out.Write(ss.str());
                usleep(cultivating_time * time_unit_ms * 1000);
                s = Sector::CULTIVATED;  // Обрабатываем
                // Иначе - пропускаем
            } else if (s == Sector::CULTIVATED) {
                std::stringstream ss{};
                ss << "Gardener " << (id + 1) << ": " << x << ' ' << y << " already cultivated\n";
                out.Write(ss.str());
            } else {
                std::stringstream ss{};
                ss << "Gardener " << (id + 1) << ": " << x << ' ' << y << " is impossible to cultivate\n";
                out.Write(ss.str());
            }

            std::stringstream ss{};
            ss << "Gardener " << (id + 1) << ": " << "moving on" << "\n";
            out.Write(ss.str());
            usleep(time_unit_ms * 1000);
        }
    }

    template<class It>
    void *Task<It>::Handle(void *arg) {
        auto *actual = reinterpret_cast<Task *>(arg);
        actual->Routine();
        return nullptr;
    }

    template
    class Task<FirstGardenerIterator>;

    template
    class Task<SecondGardenerIterator>;
}