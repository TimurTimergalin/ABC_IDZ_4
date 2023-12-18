#include <cstdint>
#include <string>

#ifndef IDZ4_INPUT_H
#define IDZ4_INPUT_H

// Объект для хранения значений всех элементов входных данных
struct Input {
    size_t n;
    size_t m;
    int time_unit_ms;
    int v1;
    int v2;
    std::string output_file;
    int solution;
};

Input input(int argn, char *argv[]);

#endif //IDZ4_INPUT_H
