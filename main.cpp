#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Huffman Tree.h"

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
        string totalMesg = ""; //sep with new lines for returning /n
        string temp = "";
        
        //input in huffman tree for creation
        while(getline(input, temp))
        {
            if(temp != "" && temp.back() == '\r')
            {
                temp.pop_back();
            }
            totalMesg += temp;
        }
        //get huffman tree creation
        HuffmanTree t_squared;
        unordered_map<char, string> binMap = t_squared.buildTree(totalMesg);
        
        //get  encoded string
        string encodedMesg = t_squared.encodeString(totalMesg, binMap);
        string decodedMesg = t_squared.decode(encodedMesg);
        /*
        push huffman tree to the txt files
            -push a inorder tree of chars that are encoded

        push the encoded string message into the txt files
            -DOES NOT NEED function for bin conversion, just uses built in huffman tree
        */
        //get all keys and vals
        string charKey = "";
        string stringVal ="";
        for(auto it = binMap.begin(); it != binMap.end(); it++)
        {
            char c  = it->first;
            if(c == '\n') charKey += "[NL] ";
            else if(c=='\r') charKey += "[CR] "; //to stop carrige register error
            else if(c == '\t') charKey += "[TAB] ";
            else if(c== ' ') charKey += "[SPC] ";
            else{
                charKey += c;
                charKey += " ";
            }
            stringVal += it->second + " ";
        }
        cout << " KEY: " << charKey << endl;
        output << "Key-First: " + charKey + '\n';
        output << "Key-Second: " + stringVal + '\n';
        output << "EncodedCSV\n";
        output << encodedMesg; //PLACEHOLDER
        output.close();
        input.close();
        cout << "OG: " << totalMesg << endl;
        cout << "Encoded: " << encodedMesg << endl;
        cout << "Decoded: " << decodedMesg << endl;
    }
    else if (endsWith(path, ".txt")) {
        cout << "TXT detected" << endl;
        //Get the two paths for writing and reading
        ofstream output = ofstream("public/uploads/decoded.csv", ios_base::trunc);
        ifstream input = ifstream(path);
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
        string decodedMesg = "";
        vector<string> writeBack;
        //Then reconstruct a csv file (like txt with new lines) and then end this portion
        int i = 0;
        while(i < decodedMesg.size())
        {
            string currLine = "";
            int currWord = 0;
            while(decodedMesg[i] !='\n')
            {
                currLine+= decodedMesg[i];
                i+=1;
            }
            writeBack.push_back(currLine);
            i+=1;

        }

        for(int i = 0; i < writeBack.size();i++)
        {
            output << writeBack[i];
            output << '\n';
        }

        //Completed
        output.close();
        input.close();
    }

    else {
        cout << "Invalid file type" << endl;
    }

    return 0;
}