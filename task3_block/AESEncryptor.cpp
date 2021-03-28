//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <fstream>
#include <iostream>
#include <cstdio>
#include "AESEncryptor.hpp"
using namespace std;

bool AESEncryptor::load_tga_file(const std::string& filename) {
    printf("ENCRYPTOR: Loading file %s...\n", filename.c_str());
    ifstream input(filename);
    if (!input)
        return false;

    // OFFSET: 0, LENGTH: 1
    // Number of Characters in Identification Field. (1B)
    char c;
    input.read(&c, 1);
    auto n_characters_in_identification_field = (unsigned char)c;

    printf("DEBUG: Number of characters in identification field: %d\n", n_characters_in_identification_field);

    // skip to position 3
    input.ignore(2);

    // OFFSET: 3, LENGHT: 5
    // Color Map Specification.
    // 3-4: Integer ( lo-hi ) index of first color map entry. (2B, little endian)
    // 5-6: Integer ( lo-hi ) count of color map entries. (2B, little endian)
    // 7:   Number of bits in each color map entry. (1B)
    char color_map_type[5];
    input.read(color_map_type, 5);
    int idx_first_color_map_entry = (unsigned char)color_map_type[1] << 8 | (unsigned char)color_map_type[0];
    int color_map_length = (unsigned char)color_map_type[3] << 8 | (unsigned char)color_map_type[2];
    int n_bits_in_each_color_map_entry = (unsigned char)color_map_type[4];

    printf("DEBUG: Idx of first color map entry: %d\n", idx_first_color_map_entry);
    printf("DEBUG: Color map length: %d\n", color_map_length);
    printf("DEBUG: Idx of first color map entry: %d\n", n_bits_in_each_color_map_entry);

    // skip to position 12
    input.ignore(4);

    // OFFSET: 8, LENGHT: 10
    // Image Specification.
    // 12-13: Width (2B, little endian)
    // 14-15: Height (2B, little endian)
    char size_buff[2];
    input.read(size_buff, 2);
    int size = (unsigned char)size_buff[1] << 8 | (unsigned char)size_buff[0];
    printf("- Width: %d\n", size);

    input.read(size_buff, 2);
    size = (unsigned char)size_buff[1] << 8 | (unsigned char)size_buff[0];
    printf("- Height: %d\n", size);


    // skip to position 18
    input.ignore(3);

    // skip identification field
    input.ignore(n_characters_in_identification_field);



    return false;
}

bool AESEncryptor::encrypt(const std::string& op_mode) {
    return false;
}
