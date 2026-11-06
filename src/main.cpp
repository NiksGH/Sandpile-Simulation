#include <iostream>
#include <filesystem>
#include <cstdint>
#include "grid.h"
#include "parse.h"
#include "input.h"
#include "simulation.h"
#include "output.h"

bool save_iteration(const char* output_dir, const Grid* grid, uint64_t iteration) {
    char filename[25];
    char full_path[4096];

    if (!make_filename(iteration, filename, sizeof(filename))) {
        return false;
    }
    uint64_t dir_lenght = 0;

    while (output_dir[dir_lenght] != '\0') {
        dir_lenght++;
    }

    bool has_separator = dir_lenght > 0 && (output_dir[dir_lenght - 1] == '/' || output_dir[dir_lenght - 1] == '\\');

    uint64_t filename_lenght = 0;

    while (filename[filename_lenght] != '\0') {
        filename_lenght++;
    }

    uint64_t separator_length = has_separator ? 0: 1;

    if (dir_lenght + separator_length + filename_lenght + 1 > sizeof(full_path)) {
        return false;
    }

    for (uint64_t i = 0; i < dir_lenght; i++) {
        full_path[i] = output_dir[i];
    }

    uint64_t position = dir_lenght;

    if (!has_separator) {
        full_path[position] = '/';
        position++;
    }

    for (uint64_t i = 0; i < filename_lenght; i++) {
        full_path[position + i] = filename[i];
    }

    full_path[position + filename_lenght] = '\0';

    return save_bmp(full_path, grid);
}

int main(int argc, char* argv[]) {
    //Парсинг аргументов
    Arguments args{};
    if (!parse_arguments(argc, argv, &args)) {
        std::cerr << "Error: invalid command line arguments\n";
        return 1;
    }

    try {
        std::filesystem::create_directories(args.output_dir);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: cannot create output directory: " << e.what() << '\n';
        return 1;
    }

    //Чтение файла
    Grid grid{};
    if (!read_input(args.input_file, &grid)) {
        std::cerr << "Error: failed to read inut file: " << args.input_file << '\n';
        free_grid(&grid);
        return 1;
    }

    uint64_t iteration = 0;
    uint64_t last_saved = 0;
    bool has_saved = false;

    while (!is_stable(&grid) && (!args.has_max_iter || iteration < args.max_iter)) {
        if (!simulate_step(&grid)) {
            std::cerr << "Error: simulation failed\n";
            return 1;
        }

        iteration++;

        if (args.freq != 0 && iteration % args.freq == 0) {
            if (!save_iteration(args.output_dir, &grid, iteration)) {
                std::cerr << "Error: failed to save BMP file\n";
                free_grid(&grid);
                return 1;
            }

            last_saved = iteration;
            has_saved = true;
        }
    }

    if (!has_saved || last_saved != iteration) {
        if (!save_iteration(args.output_dir, &grid, iteration)) {
            std::cerr << "Error: failed to save final BMP file\n";
            free_grid(&grid);
            return 1;
        }
    }

    free_grid(&grid);
    
    return 0;
}