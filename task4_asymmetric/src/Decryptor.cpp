//
// Created by Tomáš Batěk on 10.04.2021.
//

#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <openssl/err.h>
#include <cstdlib>

#include "Decryptor.hpp"

using namespace std;

#define BUFFER_SIZE 1024

void Decryptor::decrypt() {
    EVP_PKEY* priv_key = get_priv_key();

    EVP_CIPHER_CTX* ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        throw runtime_error("Failed creating new context.");

    unsigned char* encrypted_key = nullptr;
    int encrypted_key_len;
    unsigned char iv[EVP_MAX_IV_LENGTH];
    const EVP_CIPHER* type = nullptr;

    int skip_cnt = read_header(&type, &encrypted_key, encrypted_key_len, iv);

    if (!EVP_OpenInit(ctx, type, encrypted_key, encrypted_key_len, iv, priv_key)) {
        delete[] encrypted_key;
        throw runtime_error("Faled initialising the context.");
    }
    delete[] encrypted_key;

    ifstream in_file(m_encrypted_file, ios::binary);
    if (!in_file)
        throw runtime_error("Failed opening input file.");
    in_file.ignore(skip_cnt);

    ofstream out_file(m_decrypted_file, ios::binary);
    if (!out_file)
        throw runtime_error("Failed opening output file.");

    int len;
    unsigned char encrypted_buffer[2*BUFFER_SIZE];
    unsigned char decrypted_buffer[2*BUFFER_SIZE];
    while(!in_file.eof()) {
        in_file.read(reinterpret_cast<char*>(encrypted_buffer), BUFFER_SIZE);
        int bytes_read = in_file.gcount();
        if (in_file.fail() && !in_file.eof())
            throw runtime_error("Failed when reading input file");

        if (EVP_OpenUpdate(ctx, decrypted_buffer, &len, encrypted_buffer, bytes_read) != 1)
            throw runtime_error("Failed while decrypting buffer.");

        out_file.write(reinterpret_cast<const char*>(decrypted_buffer), len);
        if (out_file.fail())
            throw runtime_error("Failed writing to output file.");
    }

    if (EVP_OpenFinal(ctx, decrypted_buffer, &len) != 1) {
        ERR_print_errors_fp(stderr);
        throw runtime_error("Decryption finalisation failed.");
    }

    out_file.write(reinterpret_cast<const char*>(decrypted_buffer), len);
    if (out_file.fail())
        throw runtime_error("Failed writing to output file.");

    EVP_CIPHER_CTX_free(ctx);
}

EVP_PKEY* Decryptor::get_priv_key() {
    FILE* fp = fopen(m_key_file.c_str(), "r");

    if (!fp)
        throw runtime_error("Error opening file with private key.");
    EVP_PKEY* priv_key = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return priv_key;
}

int Decryptor::read_header(const EVP_CIPHER** cipher_type, unsigned char** encrypted_key, int& encrypted_key_len,
                            unsigned char* iv) {
    // Position: 0, length: 1 -- CIPHER TYPE
    //     AES_128_CBC = 1
    // Position: 1, length: 4 -- ENCRYPTED KEY LENGTH
    //     int, little endian
    // Position: 5, length: <encrypted key length> -- ENCRYPTED KEY
    // Position: 5 + <encrypted key length>, length: EVP_MAX_IV_LENGTH -- IV
    // Position: 5 + <encrypted key length> + <EVP_MAX_IV_LENGTH> -- DATA

    ifstream in_file(m_encrypted_file, ios::binary);
    if (!in_file)
        throw runtime_error("Failed opening input file.");

    char cipher;
    in_file.read(&cipher, 1);
    if (in_file.fail())
        throw runtime_error("Failed while reading header.");
    if (cipher == 1)
        *cipher_type = EVP_aes_128_cbc();
    else
        throw runtime_error("Found invalid AES mode when reading header.");

    char key_len[4];
    in_file.read(key_len, 4);
    if (in_file.fail())
        throw runtime_error("Failed while reading header.");
    memcpy(&encrypted_key_len, key_len, 4);

    *encrypted_key = new unsigned char[encrypted_key_len];
    in_file.read(reinterpret_cast<char*>(*encrypted_key), encrypted_key_len);
    if (in_file.fail())
        throw runtime_error("Failed while reading header.");

    in_file.read(reinterpret_cast<char*>(iv), EVP_MAX_IV_LENGTH);
    if (in_file.fail())
        throw runtime_error("Failed while reading header.");

    return 5 + encrypted_key_len + EVP_MAX_IV_LENGTH;
}
