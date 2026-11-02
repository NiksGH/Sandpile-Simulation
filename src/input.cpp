#include <fstream>
#include <cstdint>
#include "grid.h"


bool read_input(const char* filename, Grid* grid) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    int64_t x;
    int64_t y;
    uint64_t sand_count;
    bool has_data = false;

    //Находим границы сетки
    int64_t min_x = INT64_MAX;
    int64_t min_y = INT64_MAX;
    int64_t max_x = INT64_MIN;
    int64_t max_y = INT64_MIN;

    while (file >> x >> y >> sand_count) {
        if (!has_data) {
            has_data = true;
        }
        if (x < min_x) {
            min_x = x;
        }
        if(x > max_x) {
            max_x = x;
        }
        if (y < min_y) {
            min_y = y;
        }
        if (y > max_y) {
            max_y = y;
        }
    }

    //Проверяем наличие данных в файле и считаем длину и ширину сетки
    if (!has_data) {
        return false;
    }

    uint64_t width = static_cast<uint64_t>(max_x) - static_cast<uint64_t>(min_x) + 1;
    uint64_t height = static_cast<uint64_t>(max_y) - static_cast<uint64_t>(min_y) + 1;

    //Инициализируем сетку
    if (!initialize_grid(grid, width, height, min_x, min_y)) {
        return false;
    }

    file.close();
    file.open(filename);

    if (!file.is_open()) {
        free_grid(grid);
        return false;
    }

    //Заполняем сетку
    has_data = false;
    while (file >> x >> y >> sand_count) {
        if (!has_data) {
            has_data = true;
        }
        if (!set_cell(grid, x, y, sand_count)) {
            free_grid(grid);
            return false;
        }
    }
    
    return true;
}