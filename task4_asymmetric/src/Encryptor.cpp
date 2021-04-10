//
// Created by Tomáš Batěk on 10.04.2021.
//

#include <exception>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/pem.h>

#include "Encryptor.hpp"

#define BUFFER_SIZE 1024

using namespace std;

void Encryptor::encrypt() {
    EVP_PKEY* pub_key = get_pub_key();
    auto* encrypted_key = new unsigned char[EVP_PKEY_size(pub_key)];
    int encrypted_key_len;
    unsigned char iv[EVP_MAX_IV_LENGTH];

    EVP_CIPHER_CTX* ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        delete[] encrypted_key;
        throw runtime_error("Failed creating new context.");
    }

    const EVP_CIPHER* type = EVP_aes_128_cbc();

    if (EVP_SealInit(ctx, type, &encrypted_key, &encrypted_key_len, iv, &pub_key, 1) != 1) {
        delete[] encrypted_key;
        throw runtime_error("Failed initialising the context.");
    }

    write_header(type, encrypted_key, encrypted_key_len, iv);
    delete[] encrypted_key;

    ifstream in_file(m_input_file, ios::binary);
    if (!in_file)
        throw runtime_error("Failed opening input file.");

    ofstream out_file(m_output_file, ios::app);
    if (!out_file)
        throw runtime_error("Failed opening output file with append mode.");

    int len;
    unsigned char buffer[BUFFER_SIZE];
    unsigned char encrypted_buffer[BUFFER_SIZE];
    while (!in_file.eof()) {
        in_file.read(reinterpret_cast<char*>(buffer), BUFFER_SIZE);
        int bytes_read = in_file.gcount();
        if (in_file.fail() && !in_file.eof())
            throw runtime_error("Failed when reading input file");

        if (EVP_SealUpdate(ctx, encrypted_buffer, &len, buffer, bytes_read) != 1)
            throw runtime_error("Encrypting buffer failed.");

        out_file.write(reinterpret_cast<const char*>(encrypted_buffer), len);
        if (out_file.fail())
            throw runtime_error("Failed writing to output file.");
    }

    if (EVP_SealFinal(ctx, encrypted_buffer, &len) != 1)
        throw runtime_error("Encryption finalisation failed.");

    out_file.write(reinterpret_cast<const char*>(encrypted_buffer), len);
    if (out_file.fail())
        throw runtime_error("Failed writing to output file.");

    EVP_CIPHER_CTX_free(ctx);
}

EVP_PKEY* Encryptor::get_pub_key() {
    FILE* fp = fopen(m_key_file.c_str(), "r");

    if (!fp)
        throw runtime_error("Error opening file with public key.");

    return PEM_read_PUBKEY(fp, nullptr, nullptr, nullptr);
}

void Encryptor::write_header(const EVP_CIPHER* cipher_type, unsigned char* encrypted_key, int encrypted_key_len,
                             unsigned char* iv) {

    // Position: 0, length: 1 -- CIPHER TYPE
    //     AES_128_CBC = 1
    // Position: 1, length: 4 -- ENCRYPTED KEY LENGTH
    //     little endian
    // Position: 5, length: <encrypted key length> -- ENCRYPTED KEY
    // Position: 5 + <encrypted key length>, length: EVP_MAX_IV_LENGTH -- IV
    // Position: <first data entry> -- DATA

    ofstream out_file(m_output_file, ios::binary);

    if (!out_file)
        throw runtime_error("Failed opening output file.");

    char cipher;
    if (cipher_type == EVP_aes_128_cbc())
        cipher = 1;
    else
        throw runtime_error("Unknown cipher type.");
    out_file.write(&cipher, 1);

    if (out_file.fail())
        throw runtime_error("Error when writing header to output file.");

    char key_len[4];
    memcpy(key_len, &encrypted_key_len, 4);
    out_file.write(key_len, 4);
    if (out_file.fail())
        throw runtime_error("Error when writing header to output file.");

    out_file.write(reinterpret_cast<const char*>(encrypted_key), encrypted_key_len);
    if (out_file.fail())
        throw runtime_error("Error when writing header to output file.");

    out_file.write(reinterpret_cast<const char*>(iv), EVP_MAX_IV_LENGTH);
    if (out_file.fail())
        throw runtime_error("Error when writing header to output file.");
}
