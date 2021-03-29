//
// Created by Tomáš Batěk on 28.03.2021.
//

#include "AESDecryptor.hpp"

bool AESDecryptor::load_encrypted_tga_file(const std::string& filename) {
    return false;
}

bool AESDecryptor::decrypt(const std::string& op_mode, const unsigned char* key, const unsigned char* iv) {
    return false;
}

bool AESDecryptor::decrypt_buffer(const std::string& op_mode, EVP_CIPHER_CTX* ctx, const unsigned char* buff,
                                  unsigned char* encrypted_buff, const unsigned char* key, int buff_len,
                                  const unsigned char* iv) {
    return false;
}
