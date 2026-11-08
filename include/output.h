#pragma once

#include <cstdint>
#include <fstream>
#include "grid.h"

void WriteU16(std::ofstream& file, uint16_t value);
void WriteU32(std::ofstream& file, uint32_t value);
void WriteFileHeader(std::ofstream& file, uint32_t file_size);
void WriteInfoHeader(std::ofstream& file, uint32_t width, uint32_t height, uint32_t image_size);
void WritePalette(std::ofstream& file);
uint8_t GetColorNumber(uint64_t sand_count);
uint64_t CalculateRowSize(uint64_t width);
bool WritePixelData(std::ofstream& file, const Grid* grid);
bool SaveBmp(const char* filename, const Grid* grid);
bool MakeFilename(uint64_t itteration, char* buffer, uint64_t buffer_size);