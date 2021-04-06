/**
 * @author Tomáš Batěk, batekto2@fit.cvut.cz
 */

#include <iostream>
#include <string>
#include <algorithm>

#include "AESDecryptor.hpp"
#include "AESEncryptor.hpp"

using namespace std;

bool check_input(const int argc, char** argv, string& mode, string& op_mode, string& file_name) {
    if (argc != 4)
        return false;

    mode = string(argv[1]);
    op_mode = string(argv[2]);
    file_name = string(argv[3]);
    transform(op_mode.begin(), op_mode.end(), op_mode.begin(), ::toupper);

    if (mode != "-e" && mode != "-d")
        return false;
    if (op_mode != "ECB" && op_mode != "CBC")
        return false;
    return true;
}

int main(int argc, char** argv) {
    string mode, op_mode, file_name;
    if (!check_input(argc, argv, mode, op_mode, file_name)) {
        cout << "usage:   " << argv[0] << " [ -e | -d ] [ ECB | CBC ] <filename>" << endl;
        cout << "example: " << argv[0] << " -e ecb picture.tga" << endl;
        return 1;
    }

    unsigned char key[16];
    unsigned char iv[16];
    AESEncryptor::static_128_key(key);
    AESEncryptor::static_128_key(iv);

    if (mode == "-e") {
        AESEncryptor encryptor;
        if (!encryptor.load_tga_file(file_name)) {
            cerr << "ERROR: Failed loading file: " << file_name << endl;
            return 2;
        }
        if (!encryptor.encrypt(op_mode, key, iv)) {
            cerr << "ERROR: Failed encrypting file." << endl;
            return 3;
        }
    } else {
        AESDecryptor decryptor;
        if (!decryptor.load_encrypted_tga_file(file_name)) {
            cerr << "ERROR: Failed loading file: " << file_name << endl;
            return 2;
        }
        if (!decryptor.decrypt(op_mode, key, iv)) {
            cerr << "ERROR: Failed decrypting file." << endl;
            return 3;
        }
    }

    return 0;
}