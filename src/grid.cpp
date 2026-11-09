#include <iostream>
#include <grid.h>
#include <new>

bool initialize_grid(Grid* grid, uint64_t width, uint64_t height, int64_t min_x, int64_t min_y) {
    if (grid == nullptr) {
        return false;
    }

    if ((height != 0 && width > UINT64_MAX/height) || (height == 0) || (width == 0)) {
        return false;
    }

    grid->width = width;
    grid->height = height;
    grid->min_x = min_x;
    grid->min_y = min_y;

    uint64_t grid_size = width * height;
    try {
        grid->cells = new uint64_t[grid_size]{};
    }
    catch (const std::bad_alloc& e) {
        return false;
    }
    return true;
}

void free_grid(Grid* grid) {
    if (grid == nullptr) {
        return;
    }

    grid->width = 0;
    grid->height = 0;
    grid->min_x = 0;
    grid->min_y = 0;

    delete[] grid->cells;
    grid->cells = nullptr;

}