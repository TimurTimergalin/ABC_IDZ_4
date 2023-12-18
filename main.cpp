#include "task_v1/solution.h"
#include "task_v2/solution.h"
#include "io/input.h"


int main(int argn, char *argv[]) {
    Input inp = input(argn, argv);  // Ввод данных

    // Запускаем нужную версию
    if (inp.solution == 1) {
        version1::solution(inp.n, inp.m, inp.time_unit_ms, inp.v1, inp.v2, inp.output_file);
    } else if (inp.solution == 2) {
        version2::solution(inp.n, inp.m, inp.time_unit_ms, inp.v1, inp.v2, inp.output_file);
    } else {
        throw std::runtime_error("Unsupported version");
    }
}
