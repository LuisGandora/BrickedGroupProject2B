#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool endsWith(const string& str, const string& suffix) {

    if (str.length() < suffix.length()) {
        return false;
    }

    bool validSuffix = str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;

    return validSuffix;
};

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Encrypt/decrypt file" << endl;
        return 1;
    }

    string mode = argv[1];
    string path = argv[2];

    if (endsWith(path, ".csv")) {
        cout << "CSV detected" << endl;
    }

    else if (endsWith(path, ".txt")) {
        cout << "TXT detected" << endl;
    }

    else {
        cout << "Invalid file type" << endl;
    }

    return 0;
}