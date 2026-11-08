#include <fstream>
#include <cstdint>
#include "grid.h"


bool ReadInput(const char* filename, Grid* grid) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    int16_t x;
    int16_t y;
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
    if (!InitializeGrid(grid, width, height, min_x, min_y)) {
        return false;
    }

    file.close();
    file.open(filename);

    if (!file.is_open()) {
        FreeGrid(grid);
        return false;
    }

    //Заполняем сетку
    has_data = false;
    while (file >> x >> y >> sand_count) {
        if (!SetCell(grid, x, y, sand_count)) {
            FreeGrid(grid);
            return false;
        }
    }
    
    return true;
}