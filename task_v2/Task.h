#include "../field/FieldIterable.h"
#include "../io/Output.h"
#include "TrafficGuard/GardenerQueue.h"

#include <string>

#ifndef IDZ4_TASK_H
#define IDZ4_TASK_H

namespace version2 {
    template<class It>
    class Task {
        GardenerQueue &queue;
        const int id;
        FieldIterable<It> field;
        int cultivating_time;
        int time_unit_ms;
        Output &out;
    public:
        Task(GardenerQueue &queue, int id, FieldIterable<It> field, int cultivating_time,
             int time_unit_ms, Output &out);

        void Routine();

        static void *Handle(void *arg);
    };
}


#endif //IDZ4_TASK_H
