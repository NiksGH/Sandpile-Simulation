#include "output.h"

void WriteU16(std::ofstream& file, uint16_t value) {
    file.put(static_cast<char>(value & 0xFF));
    file.put(static_cast<char>((value >> 8) & 0xFF));
}

void WriteU32(std::ofstream& file, uint32_t value) {
    file.put(static_cast<char>(value & 0xFF));
    file.put(static_cast<char>((value >> 8) & 0xFF));
    file.put(static_cast<char>((value >> 16) & 0xFF));
    file.put(static_cast<char>((value >> 24) & 0xFF));
}

void WriteFileHeader(std::ofstream& file, uint32_t file_size) {
    WriteU16(file, 0x4D42);
    WriteU32(file, file_size);
    WriteU16(file, 0);
    WriteU16(file, 0);
    WriteU32(file, 118);
}

void WriteInfoHeader(std::ofstream& file, uint32_t width, uint32_t height, uint32_t image_size) {
    WriteU32(file, 40);
    WriteU32(file, width);
    WriteU32(file, height);

    WriteU16(file, 1);
    WriteU16(file, 4);

    WriteU32(file, 0);
    WriteU32(file, image_size);

    WriteU32(file, 2835);
    WriteU32(file, 2835);

    WriteU32(file, 16);
    WriteU32(file, 0);
}


void WritePalette(std::ofstream& file) {
    //Black
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);

    // Green
    file.put(0);
    file.put(255);
    file.put(0);
    file.put(0);

    // Purple
    file.put(255);
    file.put(0);
    file.put(255);
    file.put(0);

    // Yellow
    file.put(0);
    file.put(255);
    file.put(255);
    file.put(0);

    // White (indices 4-15)
    for (int i = 4; i < 16; i++) {
        file.put(255);
        file.put(255);
        file.put(255);
        file.put(0);
    }
}

uint8_t GetColorNumber(uint64_t sand_count) {
    if (sand_count >= 4) {
        return 4;
    }

    return static_cast<uint8_t>(sand_count);
}

uint64_t CalculateRowSize(uint64_t width) {
    uint64_t bytes_per_row = (width + 1) / 2;

    while (bytes_per_row % 4 != 0) {
        bytes_per_row++;
    }

    return bytes_per_row;
}

bool WritePixelData(std::ofstream& file, const Grid* grid) {
    if (grid == nullptr || grid->cells == nullptr) {
        return false;
    }

    uint64_t row_size = CalculateRowSize(grid->width);

    for (int64_t y = static_cast<int64_t>(grid->height) - 1; y >= 0; y--) {
        for (uint64_t x = 0; x < grid->width; x += 2) {
            uint8_t first_pixel = GetColorNumber(grid->cells[y * grid->width + x]);

            uint8_t second_pixel = 0;

            if (x + 1 < grid->width) {
                second_pixel = GetColorNumber(grid->cells[y * grid->width + x + 1]);
            }

            uint8_t pixel_byte = (first_pixel << 4) | second_pixel;

            file.put(static_cast<char>(pixel_byte));
        }

        uint64_t pixel_bytes = (grid->width + 1) / 2;
        uint64_t padding = row_size - pixel_bytes;

        for (uint64_t i = 0; i < padding; i++) {
            file.put(0);
        }
    }

    return file.good();
}

bool SaveBmp(const char* filename, const Grid* grid) {
    //Подсчет параметров файла
    if (grid == nullptr || grid->cells == nullptr || filename == nullptr) {
        return false;
    }
    if (grid->height == 0 || grid->width == 0){
        return false;
    }
    if (grid->width > UINT32_MAX || grid->height > UINT32_MAX) {
        return false;
    }
    uint64_t row_size = CalculateRowSize(grid->width);
    if (grid->height != 0 && row_size > UINT64_MAX / grid->height) {
        return false;
    }
    uint64_t image_size = row_size * grid->height;
    if (image_size > UINT32_MAX - 118) {
        return false;
    }
    uint64_t file_size = 14 + 40 + 64 + image_size;

    //Открытие файла
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    //Запись файла
    WriteFileHeader(file, static_cast<uint32_t>(file_size));
    WriteInfoHeader(file, static_cast<uint32_t>(grid->width), static_cast<uint32_t>(grid->height), static_cast<uint32_t>(image_size));
    WritePalette(file);
    if (!file.good()) {
        return false;
    }
    if (!WritePixelData(file, grid)) {
        return false;
    }
    file.close();
    if (file.fail()) {
        return false;
    }
    return true;
}

bool MakeFilename(uint64_t itteration, char* buffer, uint64_t buffer_size) {
    if (buffer == nullptr || buffer_size < 5) {
        return false;
    }

    char digits[20];
    uint64_t count = 0;

    if (itteration == 0) {
        digits[count] = '0';
        count++;
    }

    while (itteration > 0) {
        digits[count] = static_cast<char>('0' + itteration % 10);
        count++;
        itteration/= 10;
    }

    uint64_t filename_size = count + 5;

    if (buffer_size < filename_size) {
        return false;
    }

    for (uint64_t i = 0; i < count; i++) {
        buffer[i] = digits[count - 1 - i];
    }

    buffer[count] = '.';
    buffer[count + 1] = 'b';
    buffer[count + 2] = 'm';
    buffer[count + 3] = 'p';
    buffer[count + 4] = '\0';

    return true;
}