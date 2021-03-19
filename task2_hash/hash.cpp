/**
 * Solution for BI-BEZ task 2.
 * @author batekto2@fit.cvut.cz
 */

#include <cstdio>
#include <random>
#include <openssl/evp.h>
using namespace std;

const static int TEXT_MAX_SIZE = 20;
const static int LUCKY_NUMBER = 8; // some extra space in char arrays, must be at least 1 for '\0'

bool check_bits(const unsigned char* hash, const unsigned int hash_len, int bits) {
    if (bits > hash_len*8)
        throw invalid_argument("check_bits argument error: trying to check more bits than available");

    int byte_pos = 0;

    // Iterate over bytes, if there are still bytes to check:
    while (bits > 0) {
        // If more than 7 bits to check, check the whole byte
        if (bits >= 8) {
            if (hash[byte_pos] != 0)
                return false;
            bits -= 8;
        }
        // Check the given number of bits:
        else {
            bitset<8> byte(hash[byte_pos]);
            for (int i = 0; i < bits; i++) {
                if (byte.test(7-i))
                    return false;
            }
            break;
        }
        ++byte_pos;
    }
    return true;
}

int main (int argc, char* argv[]) {

    int bits;
    if (argc != 2 || sscanf(argv[1], "%d", &bits) != 1) {
        printf("usage: %s number_of_bits\n", argv[0]);
        return 1;
    } else if (bits > 384 || bits < 0) {
        printf("Wrong number of bits.\n");
        return 1;
    }

    char hashFunction[] = "sha384";  // zvolena hashovaci funkce ("sha1", "md5", ...)

    EVP_MD_CTX * ctx;  // struktura kontextu
    const EVP_MD * type; // typ pouzite hashovaci funkce
    unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
    unsigned int length;  // vysledna delka hashe

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 256);


    /* Inicializace OpenSSL hash funkci */
    OpenSSL_add_all_digests();
    /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
    type = EVP_get_digestbyname(hashFunction);

    /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
    if (!type) {
        printf("Hash %s neexistuje.\n", hashFunction);
        return 1;
    }

    ctx = EVP_MD_CTX_new(); // create context for hashing
    if (ctx == nullptr)
        return 2;

    unsigned char text[TEXT_MAX_SIZE + LUCKY_NUMBER];

    while (true) {

        for (int i = 0; i < TEXT_MAX_SIZE; i++)
            text[i] = dist(mt);
        text[TEXT_MAX_SIZE] = '\0';

        /* Hash the text */
        if (!EVP_DigestInit_ex(ctx, type, nullptr)) // context setup for our hash type
            return 3;

        if (!EVP_DigestUpdate(ctx, text, TEXT_MAX_SIZE)) // feed the message in
            return 4;

        if (!EVP_DigestFinal_ex(ctx, hash, &length)) // get the hash
            return 5;

        if (check_bits(hash, length, bits)) {
            for (int i = 0; i < TEXT_MAX_SIZE; i++)
                printf("%02x", text[i]);
            printf("\n");
            for (unsigned int i = 0; i < length; i++)
                printf("%02x", hash[i]);
            printf("\n");
            EVP_MD_CTX_free(ctx); // destroy the context
            return 0;
        }
    }
}
