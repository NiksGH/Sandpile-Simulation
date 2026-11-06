#pragma once

#include <cstdint>
#include <fstream>
#include "grid.h"

void write_u16(std::ofstream& file, uint16_t value);
void write_u32(std::ofstream& file, uint32_t value);
void write_file_header(std::ofstream& file, uint32_t file_size);
void write_info_header(std::ofstream& file, uint32_t width, uint32_t height, uint32_t image_size);
void write_palette(std::ofstream& file);
uint8_t get_color_number(uint64_t sand_count);
uint64_t calculate_row_size(uint64_t width);
bool write_pixel_data(std::ofstream& file, const Grid* grid);
bool save_bmp(const char* filename, const Grid* grid);
bool make_filename(uint64_t itteration, char* buffer, uint64_t buffer_size);