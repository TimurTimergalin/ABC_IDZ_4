#include "TrafficGuard.h"

TrafficGuard::TrafficGuard(GardenerQueue &queue, Output &out) : queue(queue), out(out) {}

void TrafficGuard::Routine() {
    // Решаем противостояния до скончания времен!..
    while (true) {
        queue.FixConfrontation(out);
    }
    // ...или пока поток не отменят...
}

void *TrafficGuard::Handle(void *arg) {
    auto *actual = reinterpret_cast<TrafficGuard *>(arg);
    actual->Routine();
    return nullptr;
}
