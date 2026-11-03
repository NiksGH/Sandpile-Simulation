#include "grid.h"
#include <new>
#include <limits>

bool initialize_grid(Grid* grid, uint64_t width, uint64_t height, int64_t min_x, int64_t min_y) {
    if (grid == nullptr) {
        return false;
    }

    if ((height != 0 && width > UINT64_MAX/height) || (height == 0) || (width == 0)) {
        return false;
    }

    uint64_t grid_size = width * height;
    try {
        grid->cells = new uint64_t[grid_size]{};
    }
    catch (const std::bad_alloc& e) {
        return false;
    }

    grid->width = width;
    grid->height = height;
    grid->min_x = min_x;
    grid->min_y = min_y;

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

bool get_cell(const Grid* grid, int64_t x, int64_t y, uint64_t* value) {
    if (grid == nullptr || value == nullptr || grid->cells == nullptr) {
        return false;
    }
    if(x < grid->min_x || y < grid->min_y) {
        return false;
    }
    uint64_t distance_x = static_cast<uint64_t>(x) - static_cast<uint64_t>(grid->min_x);
    uint64_t distance_y = static_cast<uint64_t>(y) - static_cast<uint64_t>(grid->min_y);
    if (distance_x >= grid->width || distance_y >= grid->height) {
        return false;
    }

    uint64_t grid_index = (distance_y)*grid->width + (distance_x);
    *value = grid->cells[grid_index];
    return true;
}

bool set_cell(Grid* grid, int64_t x, int64_t y, uint64_t value) {
    if (grid == nullptr || grid->cells == nullptr) {
        return false;
    }
    if(x < grid->min_x || y < grid->min_y) {
        return false;
    }
    uint64_t distance_x = static_cast<uint64_t>(x) - static_cast<uint64_t>(grid->min_x);
    uint64_t distance_y = static_cast<uint64_t>(y) - static_cast<uint64_t>(grid->min_y);
    if (distance_x >= grid->width || distance_y >= grid->height) {
        return false;
    }

    uint64_t grid_index = (distance_y)*grid->width + (distance_x);
    grid->cells[grid_index] = value;
    return true;
}

bool expand_grid(Grid* grid, int64_t new_min_x, int64_t new_min_y, uint64_t new_width, uint64_t new_height) {

    if (grid == nullptr || grid->cells == nullptr) {
        return false;
    }

    // Проверяем размеры
    if (new_width == 0 || new_height == 0 ||
        grid->width == 0 || grid->height == 0) {
        return false;
    }

    // Проверяем переполнение старой и новой сетки
    if (grid->width > UINT64_MAX / grid->height ||
        new_width > UINT64_MAX / new_height) {
        return false;
    }

    // Новая сетка должна начинаться не правее и не ниже старой
    if (new_min_x > grid->min_x || new_min_y > grid->min_y) {
        return false;
    }

    // Вычисляем смещения старой сетки
    uint64_t offset_x =
        static_cast<uint64_t>(grid->min_x) -
        static_cast<uint64_t>(new_min_x);

    uint64_t offset_y =
        static_cast<uint64_t>(grid->min_y) -
        static_cast<uint64_t>(new_min_y);

    // Проверяем, помещается ли старая сетка в новую
    if (offset_x > new_width ||
        grid->width > new_width - offset_x ||
        offset_y > new_height ||
        grid->height > new_height - offset_y) {
        return false;
    }

    // Проверяем, что все координаты новой сетки представимы в int64_t
    const uint64_t max_x_distance =
        static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) -
        static_cast<uint64_t>(new_min_x);

    const uint64_t max_y_distance =
        static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) -
        static_cast<uint64_t>(new_min_y);

    if (new_width - 1 > max_x_distance ||
        new_height - 1 > max_y_distance) {
        return false;
    }

    // Вычисляем размер нового массива
    uint64_t new_grid_size = new_width * new_height;

    // Выделяем новую память
    uint64_t* new_grid = nullptr;

    try {
        new_grid = new uint64_t[new_grid_size]{};
    }
    catch (const std::bad_alloc& e) {
        return false;
    }

    // Переносим данные из старой сетки в новую
    for (uint64_t row = 0; row < grid->height; row++) {
        for (uint64_t col = 0; col < grid->width; col++) {

            uint64_t old_index = row * grid->width + col;

            uint64_t new_index =
                (row + offset_y) * new_width + (col + offset_x);

            new_grid[new_index] = grid->cells[old_index];
        }
    }

    // Освобождаем старую память
    delete[] grid->cells;

    // Обновляем структуру
    grid->cells = new_grid;
    grid->width = new_width;
    grid->height = new_height;
    grid->min_x = new_min_x;
    grid->min_y = new_min_y;

    return true;
}