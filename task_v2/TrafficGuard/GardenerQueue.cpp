#include "GardenerQueue.h"

#include <algorithm>
#include <sstream>

GardenerQueue::GardenerQueue() : awaiting(0), mtx(), cond_for_gardeners(), cond_for_guard(),
                                 gardeners() {
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cond_for_gardeners, nullptr);
    pthread_cond_init(&cond_for_guard, nullptr);
}

GardenerQueue::~GardenerQueue() {
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond_for_gardeners);
    pthread_cond_destroy(&cond_for_guard);
}

// gardener_id - номер садовника, который хочет сдвинуться,
// next_x, next_y - куда он хочет сдвинуться
// out - объект для вывода
void GardenerQueue::CheckIn(int gardener_id, size_t next_x, size_t next_y, Output &out) {
    pthread_mutex_lock(&mtx);  // Захватываем мьютекс - идет доступ к координатам
    Gardener &this_gardener = gardeners[gardener_id];
    Gardener &other_gardener = gardeners[1 - gardener_id];
    while (next_x == other_gardener.x && next_y == other_gardener.y) {  // Если клетка занята

        ++awaiting;  // увеличиваем количество ждущих
        std::stringstream ss{};
        ss << "Gardener " << (gardener_id + 1) << ": another gardener encountered at " << next_x << ' ' << next_y
           << '\n';
        out.Write(ss.str());
        if (awaiting == 2) {  // Если ждут уже двое...
            pthread_cond_broadcast(&cond_for_guard);  // ...зовем регулировщика
        }
        pthread_cond_wait(&cond_for_gardeners, &mtx);  // ждем, когда клетка освободится

        if (awaiting > 0) {
            --awaiting;  // Когда прекратили ждать - уменьшаем на 1
        }

    }
    // Двигаемся на следующую клетку
    this_gardener.x = next_x;
    this_gardener.y = next_y;

    // Будим другого садовника, если тот ждет
    pthread_cond_broadcast(&cond_for_gardeners);
    pthread_mutex_unlock(&mtx);  // отдаем мьютекс
}

void GardenerQueue::FixConfrontation(Output &out) {
    pthread_mutex_lock(&mtx);  // Захватываем мьютекс
    while (awaiting < 2) {  // Пока нет противостояния...
        pthread_cond_wait(&cond_for_guard, &mtx);  // ...ждем противостояния - такая уж у нас работа!
    }
    std::stringstream ss{};
    ss << "Guard: confrontation spotted - Gardener 1 at " << gardeners[0].x << ' ' << gardeners[0].y
       << ", Gardener 2 at " << gardeners[1].x << ' ' << gardeners[1].y << ". Resolving\n";
    out.Write(ss.str());
    // Меняем местами садовников
    std::swap(gardeners[0].x, gardeners[1].x);
    std::swap(gardeners[0].y, gardeners[1].y);
    pthread_cond_broadcast(&cond_for_gardeners);
    // Сразу обнуляем очередь! Иначе мы можем начать бесконечно менять садовников местами!
    awaiting = 0;
    pthread_mutex_unlock(&mtx);  // Отпускам мьютекс
}
