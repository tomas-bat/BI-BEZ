//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <fstream>
#include <iostream>
#include <cstdio>
#include <limits>

#include "TGAReader.hpp"

using namespace std;

int TGAReader::get_size_of_tga_header(const std::string& filename) {
    printf("TGA Reader: Loading file %s...\n", filename.c_str());
    ifstream input(filename);
    if (!input)
        return -1;

    // Get file size:
    input.ignore(numeric_limits<streamsize>::max());
    streamsize len = input.gcount();
    input.clear();
    input.seekg(0, ios_base::beg);
    //printf("DEBUG: File size: %td\n", len);

    if (len < 18)
        return -1;

    int skip_count = 0;

    // OFFSET: 0, LENGTH: 1
    // Number of Characters in Identification Field. (1B)
    char c;
    input.read(&c, 1);
    skip_count += 1;
    auto n_characters_in_identification_field = (unsigned char)c;

    //printf("DEBUG: Number of characters in identification field: %d\n", n_characters_in_identification_field);

    // skip to position 3
    input.ignore(2);
    skip_count += 2;

    // OFFSET: 3, LENGHT: 5
    // Color Map Specification.
    // 3-4: Integer ( lo-hi ) index of first color map entry. (2B, little endian)
    // 5-6: Integer ( lo-hi ) count of color map entries. (2B, little endian)
    // 7:   Number of bits in each color map entry. (1B)
    char color_map_type[5];
    input.read(color_map_type, 5);
    skip_count += 5;

    int color_map_entry = (unsigned char)color_map_type[1] << 8 | (unsigned char)color_map_type[0];
    int n_color_map_entries = (unsigned char)color_map_type[3] << 8 | (unsigned char)color_map_type[2];
    auto n_bits_in_color_map_entry = (unsigned char)color_map_type[4];

    if (color_map_entry > len)
        return -1;

    skip_count += n_color_map_entries * (n_bits_in_color_map_entry / 8);

    //printf("DEBUG: index of first color map entry: %d\n", color_map_entry);
    //printf("DEBUG: number of color map entries: %d\n", n_color_map_entries);
    //printf("DEBUG: number of bits in each color map entry: %d\n", n_bits_in_color_map_entry);

    // skip to position 12
    input.ignore(4);
    skip_count += 4;

    // OFFSET: 8, LENGHT: 10
    // Image Specification.
    // 12-13: Width (2B, little endian)
    // 14-15: Height (2B, little endian)
    char size_buff[2];
    input.read(size_buff, 2);
    skip_count += 2;
    int size = (unsigned char)size_buff[1] << 8 | (unsigned char)size_buff[0];
    printf("- Width: %d\n", size);

    if (size <= 0)
        return -1;

    input.read(size_buff, 2);
    skip_count += 2;
    size = (unsigned char)size_buff[1] << 8 | (unsigned char)size_buff[0];
    printf("- Height: %d\n", size);

    if (size <= 0)
        return -1;


    // skip to position 18
    input.ignore(2);
    skip_count += 2;

    // skip identification field
    input.ignore(n_characters_in_identification_field);
    skip_count += n_characters_in_identification_field;

    if (input.fail())
        return false;
    input.close();

    if (skip_count >= len)
        return -1;

    //printf("DEBUG: Skip count: %d\n", skip_count);
    return skip_count;
}
