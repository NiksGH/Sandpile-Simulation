#include "parse.h"

bool equal(const char* s1,
            const char* s2) {

    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) { // проверка равенства двух строк
            return false;
        }
        i++;
    }
    return s1[i] == s2[i];
}

uint64_t parse_uint64(const char* str, bool* success) {
    if (success == nullptr) {
        return 0;
    }

    *success = false;

    if (str == nullptr || *str == '\0') {
        return 0;
    }

    uint64_t value = 0;

    for (uint64_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }

        uint64_t digit = str[i] - '0';

        if (value > (UINT64_MAX - digit) / 10) {
            return 0;
        }

        value = value * 10 + digit;
    }

    *success = true;
    return value;
}



bool parse_arguments(int argc, char* argv[], Arguments* args) {
    if (args == nullptr) {
        return false;
    }

    args->input_file = nullptr;
    args->output_dir = nullptr;
    args->max_iter = 0;
    args->freq = 0;
    args->has_max_iter = false;

    bool input_flag = false;
    bool output_flag = false;
    bool max_iter_flag = false;
    bool freq_flag = false;

    for (int i = 1; i < argc; i++) {
        if (equal(argv[i], "--input") || equal(argv[i], "-i")) {
            if (i + 1 >= argc || input_flag || argv[i + 1][0] == '-') {
                return false;
            }

            args->input_file = argv[++i];
            input_flag = true;

        } else if (equal(argv[i], "--output") || equal(argv[i], "-o")) {
            if (i + 1 >= argc || output_flag || argv[i + 1][0] == '-') {
                return false;
            }

            args->output_dir = argv[++i];
            output_flag = true;

        } else if (equal(argv[i], "--max-iter") || equal(argv[i], "-m")) {
            if (i + 1 >= argc || max_iter_flag || argv[i + 1][0] == '-') {
                return false;
            }

            bool success;
            uint64_t res = parse_uint64(argv[++i], &success);

            if (!success) {
                return false;
            }

            args->max_iter = res;
            args->has_max_iter = true;
            max_iter_flag = true;

        } else if (equal(argv[i], "--freq") || equal(argv[i], "-f")) {
            if (i + 1 >= argc || freq_flag || argv[i + 1][0] == '-') {
                return false;
            }

            bool success;
            uint64_t res = parse_uint64(argv[++i], &success);

            if (!success) {
                return false;
            }

            args->freq = res;
            freq_flag = true;

        } else {
            return false;
        }
    }

    if (!(input_flag && output_flag)) {
        return false;
    }

    return true;
}