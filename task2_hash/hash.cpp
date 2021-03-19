/**
 * Solution for BI-BEZ task 2.
 * @author batekto2@fit.cvut.cz
 *
 * I do not consider this code to be somehow efficient or well structured... But it seems to work.
 *
 * ./task2 1  -> 0.01s
 * ./task2 4  -> 0.01s
 * ./task2 16 -> 0.50s
 * ./task2 24 -> 5.38s
 */

#include <cstdio>
#include <cstring>
#include <string>
#include <random>
#include <ctime>
#include <openssl/evp.h>
using namespace std;

const static int TEXT_MAX_SIZE = 20;

string generate_text(int max_size) {
    string text;
    for (int i = 0; i < max_size; i++) {
        unsigned char c = rand() % 256;
        //printf("pushing back %u\n", c);
        text.push_back(c);
    }
    return text;
}

bool check_bits(const unsigned char* hash, const unsigned int hash_len, const int bits) {

    vector<bool> hash_bits;

    for (unsigned int i = 0; i < hash_len; i++) {
        unsigned char byte = hash[i];
        for (int j = 7; j >= 0; j--)
            hash_bits.push_back((byte >> j) & 1);
    }

    for (int i = 0; i < bits; i++) {
        if (hash_bits.at(i))
            return false;
    }

    /*
    // Check bit output:
    for (const auto bit: hash_bits)
        printf("%d", (bit ? 1 : 0));
    printf("\n");
     */

    return true;
}

int main (int argc, char * argv[]) {
    char hashFunction[] = "sha384";  // zvolena hashovaci funkce ("sha1", "md5", ...)

    EVP_MD_CTX * ctx;  // struktura kontextu
    const EVP_MD * type; // typ pouzite hashovaci funkce
    unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
    unsigned int length;  // vysledna delka hashe
    int bits;
    srand(time(nullptr));

    /* Inicializace OpenSSL hash funkci */
    OpenSSL_add_all_digests();
    /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
    type = EVP_get_digestbyname(hashFunction);

    if (argc != 2 || sscanf(argv[1], "%d", &bits) != 1) {
        printf("usage: %s number_of_bits\n", argv[0]);
        return 1;
    } else if (bits > 384 || bits < 0) {
        printf("Wrong number of bits.\n");
        return 1;
    }

    while (true) {

        string text = generate_text(TEXT_MAX_SIZE);

        /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
        if (!type) {
            printf("Hash %s neexistuje.\n", hashFunction);
            return 1;
        }

        ctx = EVP_MD_CTX_new(); // create context for hashing
        if (ctx == nullptr)
            return 2;

        /* Hash the text */
        if (!EVP_DigestInit_ex(ctx, type, nullptr)) // context setup for our hash type
            return 3;

        if (!EVP_DigestUpdate(ctx, text.c_str(), strlen(text.c_str()))) // feed the message in
            return 4;

        if (!EVP_DigestFinal_ex(ctx, hash, &length)) // get the hash
            return 5;

        EVP_MD_CTX_free(ctx); // destroy the context

        if (check_bits(hash, length, bits)) {
            for (const unsigned char c: text)
                printf("%02x", c);
            printf("\n");
            for (unsigned int i = 0; i < length; i++)
                printf("%02x", hash[i]);
            printf("\n");
            return 0;
        }
    }
}
