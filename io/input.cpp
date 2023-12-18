#include "input.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

struct InputBuilder {  // Объект для удобной многоэтапной инициализации Input.
                       // Так как входные данные разбиты по нескольким источникам, полностью проинициализировать input
                       // разом невозможно. Чтобы решить эту проблему, для каждого элемента входных данных создается
                       // слот.
                       // Изначально все слоты пустые (т.е. в них лежит std::nullopt).
                       // По мере считывания входных данных, слоты будут заполняться.
                       // В итоге, все слоты будут заполнены, после чего можно будет вызвать init, и получить объект
                       // Input.
    std::optional<size_t> n;
    std::optional<size_t> m;
    std::optional<int> time_unit_ms;
    std::optional<int> v1;
    std::optional<int> v2;
    std::optional<std::string> output_file;
    std::optional<int> solution;

    bool is_init() {
        return n.has_value() && m.has_value() && time_unit_ms.has_value() && v1.has_value() && v2.has_value() &&
               output_file.has_value() && solution.has_value();
    }

    Input init() {
        return Input{n.value(), m.value(), time_unit_ms.value(), v1.value(), v2.value(), output_file.value(),
                     solution.value()};
    }
};

template<class T>
void check_is_not_initialized(const std::optional<T> &slot, const std::string &key_name) {
    if (slot.has_value()) {
        throw std::runtime_error("Repeated initialization of the key: " + key_name);
    }
}

// Считывает аргумент для данного ключа и делает необходимые проверки
void process_key(const std::string &key, std::istream &is, InputBuilder &input) {
    if (key == "n") {
        check_is_not_initialized(input.n, key);

        int n;
        is >> n;

        if (n <= 0) {
            throw std::runtime_error("n must be greater then 0");
        }
        input.n = n;
    } else if (key == "m") {
        check_is_not_initialized(input.m, key);

        int m;
        is >> m;

        if (m <= 0) {
            throw std::runtime_error("m must be greater than 0");
        }

        input.m = m;
    } else if (key == "time-unit-ms") {
        check_is_not_initialized(input.time_unit_ms, key);

        int time_unit_ms;
        is >> time_unit_ms;

        if (time_unit_ms <= 0) {
            throw std::runtime_error(key + " must be greater than 0");
        }
        input.time_unit_ms = time_unit_ms;
    } else if (key == "v1") {
        check_is_not_initialized(input.v1, key);

        int v1;
        is >> v1;

        if (v1 < 0) {
            throw std::runtime_error(key + " must be greater than 0");
        }
        input.v1 = v1;
    } else if (key == "v2") {
        check_is_not_initialized(input.v2, key);

        int v2;
        is >> v2;

        if (v2 < 0) {
            throw std::runtime_error(key + " must be greater than 0");
        }
        input.v2 = v2;
    } else if (key == "output-file") {
        check_is_not_initialized(input.output_file, key);

        std::string output_file;
        is >> output_file;
        input.output_file = output_file;
    } else if (key == "solution") {
        check_is_not_initialized(input.solution, key);

        int solution;
        is >> solution;
        input.solution = solution;
    } else {
        throw std::runtime_error("Unknown key: " + key);
    }
}

// Извлекает данные напрямую из аргументов командной строки
void parse_command_line(int argn, char *argv[], InputBuilder &input) {
    for (int i = 1; i < argn; i += 2) {
        if (*argv[i] != '-') {
            throw std::runtime_error("Unexpected value: " + std::string(argv[i]));
        }
        std::string arg(argv[i] + 1);
        if (i + 1 >= argn) {
            throw std::runtime_error("Value is absent for the key: " + arg);
        }
        std::stringstream ss{};  // Для единообразия - process_key принимает istream
        ss << argv[i + 1];
        process_key(arg, ss, input);
    }
}

// Извлекает входные данные из файла
// Формат данных: чередование ключей и значений, разделенных произвольным ненулевым количеством
// пробельных (и подобных) символов
void read_from_file(std::istream &is, InputBuilder &input) {
    while (!is.eof()) {
        std::string arg;
        is >> arg;
        if (is.eof()) {
            throw std::runtime_error("Value is absent for the arg: " + arg);
        }
        process_key(arg, is, input);
    }
}

// Считывает значение из консоли в "слот" InputBuilder-а
template<class T>
void get_from_console(std::optional<T> &slot, const std::string &message) {
    std::cout << message;
    T inp;
    std::cin >> inp;
    slot = inp;
}

// Проверяет, какие поля ещё не были проинициализированы, и считывает для них данные из консоли
void input_remaining(InputBuilder &input) {
    if (input.is_init()) {
        return;
    }
    std::cout << "The following keys were not present in the configuration. Please, input them manually:\n";
    if (!input.n.has_value()) {
        get_from_console(input.n, "N:");
    }
    if (!input.m.has_value()) {
        get_from_console(input.m, "M:");
    }
    if (!input.time_unit_ms.has_value()) {
        get_from_console(input.time_unit_ms, "Value of 1 time unit in milliseconds:");
    }
    if (!input.v1.has_value()) {
        get_from_console(input.v1, "V1:");
    }
    if (!input.v2.has_value()) {
        get_from_console(input.v2, "V2:");
    }
    if (!input.output_file.has_value()) {
        get_from_console(input.output_file, "Name of file for output logs:");
    }
    if (!input.solution.has_value()) {
        get_from_console(input.solution, "Solution version number:");
    }
}

Input input(int argn, char *argv[]) {
    InputBuilder input{};

    if (argn == 3 && strcmp(argv[1], "-input-file") == 0) {  // Если ввод из файла
        std::ifstream input_file(argv[2]);
        read_from_file(input_file, input);
        input_file.close();
    } else {  // ввод из командной строки
        parse_command_line(argn, argv, input);
    }
    input_remaining(input);  // Вводим оставшиеся из консоли
    return input.init();
}
