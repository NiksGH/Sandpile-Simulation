#pragma once

#include "grid.h"

bool add_offset(int64_t base, uint64_t offset, int64_t* result);
bool add_sand(Grid* grid, int64_t x, int64_t y);
bool simulate_step(Grid* grid);
bool simulate(Grid* grid, const uint64_t* max_iter);
bool is_stable(const Grid* grid);