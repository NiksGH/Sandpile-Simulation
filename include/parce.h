#pragma once

#include <cstdint>

struct Arguments {
    const char* input_file;
    const char* output_dir;

    uint64_t max_iter;
    uint64_t freq;

    bool has_max_iter;
};

bool equal(const char* s1, const char* s2);

bool parse_arguments(
    int argc,
    char* argv[],
    Arguments* args
);