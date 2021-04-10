//
// Created by Tomáš Batěk on 10.04.2021.
//

#ifndef TASK4_ASYMMETRIC_ENCRYPTOR_HPP
#define TASK4_ASYMMETRIC_ENCRYPTOR_HPP

#include <string>
#include <utility>
#include <memory>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/pem.h>


class Encryptor {
public:
    Encryptor(std::string in_file, std::string key_file, std::string out_file)
        : m_input_file(std::move(in_file)), m_key_file(std::move(key_file)), m_output_file(std::move(out_file)) {}

    void encrypt();

private:
    std::string m_input_file;
    std::string m_key_file;
    std::string m_output_file;

    EVP_PKEY* get_pub_key();

    void write_header(const EVP_CIPHER* cipher_type, unsigned char* encrypted_key, int encrypted_key_len,
                      unsigned char* iv);
};


#endif //TASK4_ASYMMETRIC_ENCRYPTOR_HPP
