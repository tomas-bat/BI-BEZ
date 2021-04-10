//
// Created by Tomáš Batěk on 10.04.2021.
//

#ifndef TASK4_ASYMMETRIC_DECRYPTOR_HPP
#define TASK4_ASYMMETRIC_DECRYPTOR_HPP

#include <string>
#include <utility>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/pem.h>


class Decryptor {
public:
    Decryptor(std::string encrypted_file, std::string key_file, std::string decrypted_file)
        : m_encrypted_file(std::move(encrypted_file)), m_key_file(std::move(key_file)),
          m_decrypted_file(std::move(decrypted_file)) {}

    void decrypt();

private:
    std::string m_encrypted_file;
    std::string m_key_file;
    std::string m_decrypted_file;

    EVP_PKEY* get_priv_key();

    int read_header(const EVP_CIPHER** cipher_type, unsigned char** encrypted_key, int& encrypted_key_len,
                     unsigned char* iv);
};


#endif //TASK4_ASYMMETRIC_DECRYPTOR_HPP
