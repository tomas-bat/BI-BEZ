//
// Created by Tomáš Batěk on 28.03.2021.
//

#ifndef TASK3_BLOCK_AESDECRYPTOR_HPP
#define TASK3_BLOCK_AESDECRYPTOR_HPP

#include <string>
#include <openssl/evp.h>


class AESDecryptor {
public:
    bool load_encrypted_tga_file(const std::string& filename);

    bool decrypt(const std::string& op_mode, const unsigned char* key, const unsigned char* iv);

private:
    bool        m_file_loaded = false;
    std::string m_filename;
    int         m_skip_count;

    bool decrypt_buffer(const std::string& op_mode, EVP_CIPHER_CTX* ctx, const unsigned char* buff,
                        unsigned char* decrypted_buff, const unsigned char* key, int buff_len, int& decrypted_len,
                        const unsigned char* iv);
};


#endif //TASK3_BLOCK_AESDECRYPTOR_HPP
