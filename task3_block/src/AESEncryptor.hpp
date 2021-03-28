//
// Created by Tomáš Batěk on 28.03.2021.
//

#ifndef TASK3_BLOCK_AESENCRYPTOR_HPP
#define TASK3_BLOCK_AESENCRYPTOR_HPP

#include <string>


class AESEncryptor {
public:
    bool load_tga_file(const std::string& filename);

    static void random_128_key(unsigned char* key);

    bool encrypt(const std::string& op_mode);
private:
    __unused bool        m_file_loaded = false;
    __unused std::string m_filename;
    __unused int         m_skip_count;
};


#endif //TASK3_BLOCK_AESENCRYPTOR_HPP
