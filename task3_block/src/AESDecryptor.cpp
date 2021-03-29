//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <string>
#include <fstream>
#include <openssl/err.h>

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
    if (!m_file_loaded)
        return false;

    string new_filename;
    if (op_mode == "ECB")
        new_filename = m_filename + "_dec.tga";
    else
        new_filename = m_filename + "_dec.tga";

    ofstream out_file(new_filename);
    ifstream in_file(m_filename);
    if (!out_file || !in_file)
        return false;

    char* header_buff = new char[m_skip_count];
    in_file.read(header_buff, m_skip_count);
    out_file.write(header_buff, m_skip_count);



    char* file_buff = new char[BUFFER_SIZE];
    char* decrypted_buff = new char[BUFFER_SIZE];
    EVP_CIPHER_CTX* ctx;
    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        return false;

    while (!in_file.eof()) {
        in_file.read(file_buff, BUFFER_SIZE);
        int bytes_read = in_file.gcount();
        if (!decrypt_buffer(op_mode, ctx, reinterpret_cast<const unsigned char*>(file_buff),
                            reinterpret_cast<unsigned char*>(decrypted_buff), key, bytes_read, iv))
            return false;
        out_file.write(decrypted_buff, bytes_read);
    }

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return true;
}

bool AESDecryptor::decrypt_buffer(const std::string& op_mode, EVP_CIPHER_CTX* ctx, const unsigned char* buff,
                                  unsigned char* decrypted_buff, const unsigned char* key, int buff_len,
                                  const unsigned char* iv) {
    int len, plaintext_len;

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

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(EVP_DecryptUpdate(ctx, decrypted_buff, &len, buff, buff_len) != 1) {
        return false;
    }
    plaintext_len = len;

//    /* Finalise the decryption. Further plaintext bytes may be written at
//     * this stage.
//     */
//    if(EVP_DecryptFinal_ex(ctx, decrypted_buff + len, &len) != 1) {
//        ERR_print_errors_fp(stderr);
//        return false;
//    }
//    plaintext_len += len;
//    printf("===== all good\n");

    return true;
}
