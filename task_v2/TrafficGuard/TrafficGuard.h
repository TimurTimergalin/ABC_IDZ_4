#include "GardenerQueue.h"

#ifndef IDZ4_TRAFFICGUARD_H
#define IDZ4_TRAFFICGUARD_H


// Класс, содержащий все данные для работы потока-регулировщика
class TrafficGuard {
    GardenerQueue& queue;  // Очередь садовников
    Output &out;

public:
    TrafficGuard(GardenerQueue &queue, Output &out);
    void Routine();  // Непосредственно код потока
    static void *Handle(void * arg);  // Метод, вызывающий Routine(), совместимый с pthreads
};


#endif //IDZ4_TRAFFICGUARD_H
