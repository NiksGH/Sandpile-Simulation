#include <cstdint>
#include <limits>
#include "simulation.h"

//Безопасное прибавление беззнакового смещения к знаковой координате
bool AddOffset(int64_t base, uint64_t offset, int64_t* result) {
    if (result == nullptr) {
        return false;
    }

    if (base >= 0) {
        uint64_t available = static_cast<uint64_t>(INT64_MAX - base);

        if (offset > available) {
            return false;
        }

        *result = base + static_cast<int64_t>(offset);
        return true;
    }

    uint64_t magnitude = static_cast<uint64_t>(-(base + 1)) + 1;

    if (offset < magnitude) {
        uint64_t remaining = magnitude - offset;

        if (remaining == (uint64_t{1} << 63)) {
            *result = INT64_MIN;
        } else {
            *result = -static_cast<int64_t>(remaining);
        }

        return true;
    }

    uint64_t positive = offset - magnitude;

    if (positive > static_cast<uint64_t>(INT64_MAX)) {
        return false;
    }

    *result = static_cast<int64_t>(positive);
    return true;
}


//Добавляю одну песчинку в указанную клетку
bool AddSand(Grid* grid, int64_t x, int64_t y) {
    uint64_t value;

    if (grid == nullptr || grid->cells == nullptr) {
        return false;
    }

    //Проверяю, существует ли клетка
    if (!GetCell(grid, x, y, &value)) {

        //Расширение для левого соседа
        if (x < grid->min_x) {
            if (grid->min_x == INT64_MIN || grid->width == UINT64_MAX) {
                return false;
            }

            if (!ExpandGrid(grid, grid->min_x - 1, grid->min_y,
                             grid->width + 1, grid->height)) {
                return false;
            }
        }

        //Расширение для верхнего соседа
        else if (y < grid->min_y) {
            if (grid->min_y == INT64_MIN || grid->height == UINT64_MAX) {
                return false;
            }

            if (!ExpandGrid(grid, grid->min_x, grid->min_y - 1,
                             grid->width, grid->height + 1)) {
                return false;
            }
        }

        //Расширение для правого соседа
        else if (static_cast<uint64_t>(x) -
                 static_cast<uint64_t>(grid->min_x) >= grid->width) {
            if (grid->width == UINT64_MAX) {
                return false;
            }

            if (!ExpandGrid(grid, grid->min_x, grid->min_y,
                             grid->width + 1, grid->height)) {
                return false;
            }
        }

        //Расширение для нижнего соседа
        else if (static_cast<uint64_t>(y) -
                 static_cast<uint64_t>(grid->min_y) >= grid->height) {
            if (grid->height == UINT64_MAX) {
                return false;
            }

            if (!ExpandGrid(grid, grid->min_x, grid->min_y,
                             grid->width, grid->height + 1)) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    //Получаю значение клетки
    if (!GetCell(grid, x, y, &value)) {
        return false;
    }

    //Проверка переполнения
    if (value == UINT64_MAX) {
        return false;
    }

    //Добавляю песчинку
    return SetCell(grid, x, y, value + 1);
}


//Выполняю одну итерацию симуляции
bool SimulateStep(Grid* grid) {
    //Создаю новую сетку
    if (grid == nullptr) {
        return false;
    }

    //Проверка переполнения grid_size
    if (grid->width != 0 && grid->height > UINT64_MAX / grid->width) {
        return false;
    }

    uint64_t grid_size = grid->height * grid->width;

    if (grid_size != 0 && grid->cells == nullptr) {
        return false;
    }

    Grid new_grid;

    if (!InitializeGrid(&new_grid, grid->width, grid->height,
                         grid->min_x, grid->min_y)) {
        return false;
    }

    //Копирую в новую сетку старую
    for (uint64_t i = 0; i < grid_size; i++) {
        new_grid.cells[i] = grid->cells[i];
    }

    for (uint64_t i = 0; i < grid_size; i++) {
        if (grid->cells[i] < 4) {
            continue;
        }

        //Вычисляю координаты клетки
        uint64_t offset_x = i % grid->width;
        uint64_t offset_y = i / grid->width;

        int64_t x;
        int64_t y;

        if (!AddOffset(grid->min_x, offset_x, &x) ||
            !AddOffset(grid->min_y, offset_y, &y)) {
            FreeGrid(&new_grid);
            return false;
        }

        //Проверка переполнения координат соседей
        if (x == INT64_MIN || x == INT64_MAX ||
            y == INT64_MIN || y == INT64_MAX) {
            FreeGrid(&new_grid);
            return false;
        }

        //Обработка верхнего соседа
        if (!AddSand(&new_grid, x, y - 1)) {
            FreeGrid(&new_grid);
            return false;
        }

        //Обработка нижнего соседа
        if (!AddSand(&new_grid, x, y + 1)) {
            FreeGrid(&new_grid);
            return false;
        }

        //Обработка левого соседа
        if (!AddSand(&new_grid, x - 1, y)) {
            FreeGrid(&new_grid);
            return false;
        }

        //Обработка правого соседа
        if (!AddSand(&new_grid, x + 1, y)) {
            FreeGrid(&new_grid);
            return false;
        }

        //Обвал центральной клетки
        uint64_t temp;

        if (!GetCell(&new_grid, x, y, &temp) ||
            temp < 4 ||
            !SetCell(&new_grid, x, y, temp - 4)) {
            FreeGrid(&new_grid);
            return false;
        }
    }

    //Свап сеток
    FreeGrid(grid);
    *grid = new_grid;

    return true;
}

bool IsStable(const Grid* grid) {
    if (grid == nullptr || grid->cells == nullptr) {
        return false;
    }

    //Проверка переполнения
    if (grid->width == 0 || grid->height == 0 || grid->width > UINT64_MAX / grid->height) {
        return false;
    }

    uint64_t grid_size = grid->height * grid->width;

    for (uint64_t i = 0; i < grid_size; i++) {
        if (grid->cells[i] >= 4) {
            return false;
        }
    }
    return true;
}

// bool Simulate(Grid* grid, const uint64_t* max_iter) {
//     if (grid == nullptr || grid->cells == nullptr) {
//         return false;
//     }

//     if (IsStable(grid)) {
//         return true;
//     }

//     if (max_iter != nullptr) {
//         for (uint64_t i = 0; i < *max_iter; i++) {

//             if (IsStable(grid)) {
//                 break;
//             }

//             if (!SimulateStep(grid)) {
//                 return false;
//             }
//         }
//     }

//     if (max_iter == nullptr) {
//         while (!IsStable(grid)) {
//             if (!SimulateStep(grid)) {
//                 return false;
//             }
//         }
//     }
    
//     return true;
// }
