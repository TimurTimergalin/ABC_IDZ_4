#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

#include <pthread.h>

#ifndef IDZ4_OUTPUT_H
#define IDZ4_OUTPUT_H


// Класс для синхронизированного вывода в несколько потоков
// При записи автоматически захватывается необходимый мьютекс
class Output {
private:
    std::vector<std::ostream *> streams;
    pthread_mutex_t mtx;
public:
    Output(std::initializer_list<std::ostream *> streams);

    void Write(const std::string& str);

    ~Output();
};


#endif //IDZ4_OUTPUT_H
