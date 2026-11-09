#include <iostream>
#include <grid.h>

int main() {
    Grid grid;
    bool succes = initialize_grid(&grid, 5, 4, 0, 0);
    if (!succes) {
        std::cout << "Error\n";
    }
    std::cout << "Width: " << grid.width << '\n';
    std::cout << "Height: " << grid.height << '\n';
    free_grid(&grid);
    if (grid.cells == nullptr && grid.height == 0 && grid.width == 0 && grid.min_x == 0 && grid.min_y == 0) {
        std::cout << "Delete succes\n";
    }
}