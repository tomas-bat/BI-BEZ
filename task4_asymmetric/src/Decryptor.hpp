//
// Created by Tomáš Batěk on 10.04.2021.
//

#ifndef TASK4_ASYMMETRIC_DECRYPTOR_HPP
#define TASK4_ASYMMETRIC_DECRYPTOR_HPP

#include <string>
#include <utility>


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
};


#endif //TASK4_ASYMMETRIC_DECRYPTOR_HPP
