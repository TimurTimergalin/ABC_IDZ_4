#include "Output.h"

Output::Output(std::initializer_list<std::ostream *> streams) : streams(streams), mtx(){
    pthread_mutex_init(&mtx, nullptr);
}

void Output::Write(const std::string& str) {
    pthread_mutex_lock(&mtx);
    for (auto *x : streams) {
        *x << str << std::flush;
    }
    pthread_mutex_unlock(&mtx);
}

Output::~Output() {
    pthread_mutex_destroy(&mtx);
}

