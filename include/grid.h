#pragma once

#include <cstdint>

struct Grid {
    uint64_t* cells;

    uint64_t width;
    uint64_t height;

    int64_t min_x;
    int64_t min_y;
};

bool initialize_grid(
    Grid* grid,
    uint64_t width,
    uint64_t height,
    int64_t min_x,
    int64_t min_y
);

void free_grid(Grid* grid);

bool get_cell(
    const Grid* grid,
    int64_t x,
    int64_t y,
    uint64_t* value);

bool set_cell(
    Grid* grid,
    int64_t x,
    int64_t y,
    uint64_t value);

bool expand_grid(
    Grid* grid,
    int64_t new_min_x,
    int64_t new_min_y,
    uint64_t new_width,
    uint64_t new_height);