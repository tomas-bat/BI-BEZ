//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <openssl/evp.h>

#include "AESEncryptor.hpp"
#include "TGAReader.hpp"

using namespace std;

bool AESEncryptor::load_tga_file(const std::string& filename) {
    int skip_count = TGAReader::get_size_of_tga_header(filename);
    if (skip_count < 0)
        return false;
    m_skip_count = skip_count;
    m_filename = filename;
    m_file_loaded = true;
    return true;
}

bool AESEncryptor::encrypt(const std::string& op_mode) {

    unsigned char* key;
    unsigned char* iv;

    unsigned char* cipher_text;
    unsigned char* plain_text;

    EVP_CIPHER_CTX* ctx;
    int len, cipher_text_len, plain_text_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        return false;

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * In this example we are using 128 bit AES (i.e. a 128 bit key).
     */
    if(EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, iv) != 1)
        return false;

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, cipher_text, &len, plain_text, plain_text_len))
        return false;
    cipher_text_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, cipher_text + len, &len))
        return false;
    cipher_text_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return false;
}
