#pragma once

#include <cstdint>

struct Grid {
    uint64_t* cells;

    uint64_t width;
    uint64_t height;

    int64_t min_x;
    int64_t min_y;
};

bool InitializeGrid(
    Grid* grid,
    uint64_t width,
    uint64_t height,
    int64_t min_x,
    int64_t min_y
);

void FreeGrid(Grid* grid);

bool GetCell(
    const Grid* grid,
    int64_t x,
    int64_t y,
    uint64_t* value);

bool SetCell(
    Grid* grid,
    int64_t x,
    int64_t y,
    uint64_t value);

bool ExpandGrid(
    Grid* grid,
    int64_t new_min_x,
    int64_t new_min_y,
    uint64_t new_width,
    uint64_t new_height);