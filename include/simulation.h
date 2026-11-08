#pragma once

#include "grid.h"

bool AddOffset(int64_t base, uint64_t offset, int64_t* result);
bool AddSand(Grid* grid, int64_t x, int64_t y);
bool SimulateStep(Grid* grid);
bool Simulate(Grid* grid, const uint64_t* max_iter);
bool IsStable(const Grid* grid);