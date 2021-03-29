//
// Created by Tomáš Batěk on 28.03.2021.
//

#ifndef TASK3_BLOCK_AESENCRYPTOR_HPP
#define TASK3_BLOCK_AESENCRYPTOR_HPP

#include <string>
#include <openssl/evp.h>


class AESEncryptor {
public:
    bool load_tga_file(const std::string& filename);

    static void random_128_key(unsigned char* key);

    static void static_128_key(unsigned char* key);

    bool encrypt(const std::string& op_mode, const unsigned char* key, const unsigned char* iv);
private:
    bool        m_file_loaded = false;
    std::string m_filename;
    int         m_skip_count;

    bool encrypt_buffer(const std::string& op_mode, EVP_CIPHER_CTX* ctx, const unsigned char* buff,
                        unsigned char* encrypted_buff, const unsigned char* key, int buff_len, const unsigned char* iv);
};


#endif //TASK3_BLOCK_AESENCRYPTOR_HPP
