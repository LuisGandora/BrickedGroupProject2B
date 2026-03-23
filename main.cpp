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
        ofstream output = ofstream("public/uploads/encoded.txt" , ios_base::trunc);
        ifstream input = ifstream(path);
        //loop through path and compress all of the data (store row by row sep by '>')

        //input in huffman tree for creation

        //get preorder for huffman tree creation

        //get encoded string

        /*
        push huffman tree to the txt files
            -Need a length var in bin to designate the size of the tree to decode
            -(sort by pair values (char:bin string) where a parent node that doesnt have a char
         is given unique char ('>') (Make sure to notify users that this char is not allowed))
            -NEEDS a function for bin conversion

        push the encoded string message into the txt files
            -DOES NOT NEED function for bin conversion, just uses built in huffman tree

        */
    }

    else if (endsWith(path, ".txt")) {
        cout << "TXT detected" << endl;
        //Get the two paths for writing and reading

        /*
            Read the key for header based on structure and decode into a string seperated by spaces:
                -Length of how many 'nodes' are in tree
                -Node data (convert to string)
                    -char is 1 byte so read one bin char
                        -normal chars: fine to pair as "char: binstring"
                        -nodes with no chars (parent nodes): pair as "'>'": binstring
        */

        //recreate huffman tree based on this pattern
        
        //Then push the second encoded csv into the huffman tree and obtain the decoded message

        //Then reconstruct a csv file (like txt with new lines) and then end this portion
    }

    else {
        cout << "Invalid file type" << endl;
    }

    return 0;
}