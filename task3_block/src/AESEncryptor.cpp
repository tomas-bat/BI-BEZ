//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <openssl/evp.h>
#include <random>
#include <fstream>
#include <vector>
#include <iostream>

#include "AESEncryptor.hpp"
#include "TGAReader.hpp"

using namespace std;

const static int BUFFER_SIZE = 1024;

bool AESEncryptor::load_tga_file(const std::string& filename) {
    int skip_count = TGAReader::get_size_of_tga_header(filename);
    if (skip_count < 0)
        return false;
    m_skip_count = skip_count;
    m_filename = filename;
    m_file_loaded = true;
    return true;
}

bool AESEncryptor::encrypt(const std::string& op_mode, const unsigned char* key, const unsigned char* iv) {
    cout << "=== Encrypting file " << m_filename << "... ===" << endl;

    if (!m_file_loaded)
        return false;

    size_t last_dot = m_filename.find_last_of('.');
    string raw_name = m_filename.substr(0, last_dot);

    string new_filename;
    if (op_mode == "ECB")
        new_filename = raw_name + "_ecb.tga";
    else
        new_filename = raw_name + "_cbc.tga";

    ofstream out_file(new_filename);
    ifstream in_file(m_filename);
    if (!out_file || !in_file)
        return false;

    char* header_buff = new char[m_skip_count];
    in_file.read(header_buff, m_skip_count);
    out_file.write(header_buff, m_skip_count);
    delete [] header_buff;


    char* file_buff = new char[BUFFER_SIZE];
    char* encrypted_buff = new char[BUFFER_SIZE];
    EVP_CIPHER_CTX* ctx;
    int cipher_text_len;
    /* Create and init the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        return false;

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * In this example we are using 128 bit AES (i.e. a 128 bit key).
     */
    if (op_mode == "ECB") {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, iv) != 1)
            return false;
    } else {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1)
            return false;
    }

    while (!in_file.eof()) {
        in_file.read(file_buff, BUFFER_SIZE);
        int bytes_read = in_file.gcount();
        if (EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(encrypted_buff), &cipher_text_len,
                              reinterpret_cast<const unsigned char*>(file_buff), bytes_read)
            != 1)
            return false;
        out_file.write(encrypted_buff, cipher_text_len);
    }

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encrypted_buff), &cipher_text_len) != 1)
        return false;

    out_file.write(encrypted_buff, cipher_text_len);

    delete [] file_buff;
    delete [] encrypted_buff;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    cout << "=== Encryption successful! (" << new_filename << ") ===" << endl;
    return true;
}

void AESEncryptor::random_128_key(unsigned char key[16]) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 256);

    for (int i = 0; i < 16; i++)
        key[i] = dist(mt);
}

void AESEncryptor::static_128_key(unsigned char* key) {
    for (int i = 0; i < 16; i++)
        key[i] = i;
}
