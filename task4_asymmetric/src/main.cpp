/**
 * @author Tomáš Batěk, batekto2@fit.cvut.cz
 */


#include <iostream>
#include <string>
#include <exception>

#include "Encryptor.hpp"
#include "Decryptor.hpp"

using namespace std;

bool check_input(const int argc, char** argv, string& mode, string& key_file, string& in_file, string& out_file) {
    if (argc != 5)
        return false;

    mode = argv[1];
    key_file = argv[2];
    in_file = argv[3];
    out_file = argv[4];

    return mode == "-e" || mode == "-d";
}

int main(int argc, char* argv[]) {
    string mode, key_file, in_file, out_file;
    if (!check_input(argc, argv, mode, key_file, in_file, out_file)) {
        cout << "Usage:" << endl;
        cout << "  Encryption:" << endl;
        cout << "    " << argv[0] << " -e <public key> <filename> <output filename>" << endl;
        cout << "  Decryption:" << endl;
        cout << "    " << argv[0] << " -d <private key> <encrypted filename> <output filename>" << endl;
        return 1;
    }

    if (mode == "-e") {
        Encryptor encryptor(in_file, key_file, out_file);
        try {
            encryptor.encrypt();
        } catch (const exception& ex) {
            cerr << "Encryption failed: " << ex.what() << endl;
            return 2;
        }
    } else {
        Decryptor decryptor(in_file, key_file, out_file);
        try {
            decryptor.decrypt();
        } catch (const exception& ex) {
            cerr << "Decryption failed: " << ex.what() << endl;
            return 3;
        }
    }



	cout << "Hello World!" << endl;

	return 0;
}