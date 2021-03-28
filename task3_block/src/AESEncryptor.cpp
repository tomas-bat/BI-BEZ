//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <fstream>
#include <iostream>
#include <cstdio>

#include "AESEncryptor.hpp"
#include "TGAReader.hpp"

using namespace std;

bool AESEncryptor::load_tga_file(const std::string& filename) {
    int skip_count = TGAReader::get_size_of_tga_header(filename);
    if (skip_count < 0)
        return false;
    m_skip_count = skip_count;
    m_filename = filename;
    m_file_loaded = true;
    return true;
}

bool AESEncryptor::encrypt(const std::string& op_mode) {
    return false;
}
