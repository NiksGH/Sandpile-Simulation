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