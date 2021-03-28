//
// Created by Tomáš Batěk on 28.03.2021.
//

#include <openssl/evp.h>
#include <random>

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

    unsigned char key[16];
    unsigned char iv[16];
    random_128_key(key);
    random_128_key(iv);

    char plain_text[] = "Hello World!";
    unsigned char cipher_text[20];

    EVP_CIPHER_CTX* ctx;
    int len, cipher_text_len, plain_text_len = strlen(plain_text);

    /* Create and initialise the context */
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

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, cipher_text, &len, reinterpret_cast<const unsigned char*>(plain_text), plain_text_len))
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

    printf("DEBUG: Ciphered text: %s\n", cipher_text);

    // =============================================



    int plaintext_len;
    unsigned char plaintext[20];

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        return false;

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
    */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
        return false;

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, cipher_text, cipher_text_len))
        return false;
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        return false;
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    printf("DEBUG: Plain text: %s\n", plain_text);

    return true;
}

void AESEncryptor::random_128_key(unsigned char key[16]) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 256);

    for (int i = 0; i < 16; i++)
        key[i] = dist(mt);
}
