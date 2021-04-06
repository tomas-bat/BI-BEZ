//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <string>
#include <fstream>
#include <iostream>
#include <memory>

#include "AESDecryptor.hpp"
#include "TGAReader.hpp"

using namespace std;

const static int BUFFER_SIZE = 1024;

bool AESDecryptor::load_encrypted_tga_file(const std::string& filename) {
    int skip_count = TGAReader::get_size_of_tga_header(filename);
    if (skip_count < 0)
        return false;
    m_skip_count = skip_count;
    m_filename = filename;
    m_file_loaded = true;
    return true;
}

bool AESDecryptor::decrypt(const std::string& op_mode, const unsigned char* key, const unsigned char* iv) {
    cout << "=== Decrypting file " << m_filename << "... ===" << endl;

    if (!m_file_loaded)
        return false;

    size_t last_dot = m_filename.find_last_of('.');
    string raw_name = m_filename.substr(0, last_dot);

    string new_filename;
    if (op_mode == "ECB")
        new_filename = raw_name + "_dec.tga";
    else
        new_filename = raw_name + "_dec.tga";

    ofstream out_file(new_filename);
    ifstream in_file(m_filename);
    if (!out_file || !in_file)
        return false;

    shared_ptr<char> header_buff(new char[m_skip_count]);
    in_file.read(header_buff.get(), m_skip_count);
    if (in_file.fail())
        return false;
    out_file.write(header_buff.get(), m_skip_count);
    if (out_file.fail())
        return false;


    shared_ptr<char> file_buff(new char[2*BUFFER_SIZE]);
    shared_ptr<char> decrypted_buff(new char[2*BUFFER_SIZE]);
    EVP_CIPHER_CTX* ctx;
    int decrypted_len;
    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return false;
    }

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     */
    if (op_mode == "ECB") {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, iv) != 1) {
            return false;
        }
    } else {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1) {
            return false;
        }
    }

    while (!in_file.eof()) {
        in_file.read(file_buff.get(), BUFFER_SIZE);
        if (in_file.fail() && !in_file.eof())
            return false;
        int bytes_read = in_file.gcount();
        if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(decrypted_buff.get()), &decrypted_len,
                              reinterpret_cast<const unsigned char*>(file_buff.get()), bytes_read) != 1)
            return false;
        out_file.write(decrypted_buff.get(), decrypted_len);
        if (out_file.fail())
            return false;
    }

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(decrypted_buff.get()), &decrypted_len) != 1) {
        return false;
    }

    out_file.write(decrypted_buff.get(), decrypted_len);
    if (out_file.fail())
        return false;


    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    cout << "=== Decryption successful! (" << new_filename << ") ===" << endl;

    return true;
}
