//compilation command 
//cd public
//g++ -o out.exe main.cpp 'Huffman Tree'.h
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <string_view>
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

    if (argc != 4) {
        cout << "Not enough args" << endl;
        return 1;
    }

    string mode = argv[1];
    string path = argv[2];
    int currFile = stoi(argv[3]);

    if (endsWith(path, ".csv")) {
        string filePath = "public/uploads/encoded" + to_string(currFile)+ ".txt";
        ifstream input = ifstream(path);
        if(!input.is_open())
        {
            cout <<"Path not able to be open" << endl;
            return 1;
        }
        //Phase 1: Build frequency map without storing the whole file
        unordered_map<char, int> binMap;
        string totMesg;
        totMesg.reserve(5000000); //to store that much data (prevent dynamic errors)
        string line;
        while(getline(input, line)){
            for(char c: line)
            {
                binMap[c]++;
                
                
                
            }
            //directly push to prevent errors
            totMesg.append(line);
            totMesg.push_back('\n');
            binMap['\n']++;
        }
        //now encode the tot message effectively using string stream


        //Phase 2 Build the tree
        HuffmanTree t_squared;
        //avoid as many passes to binmap directory
        priority_queue<Node*, vector<Node*>, Compare> pq = t_squared.setUpQueue(binMap);
        t_squared.root = pq.top();
        unordered_map<char, string> encoder;
        t_squared.preOrder(t_squared.root, encoder, "");
        
        //Phase 3 Write header and encoder directly to file
        ofstream output(filePath, ios_base::trunc);
        
        stringstream ss(totMesg);

        string charKey = "";
        string stringVal ="";
        for(auto const& [charact, frequen]: binMap)
        {
            // 1. Convert char to int, THEN to string, then ADD a space
            charKey += to_string((int)charact) + " "; 
            
            // 2. Use to_string for frequency to avoid pointer arithmetic bugs
            stringVal += to_string(frequen) + " ";
        }
        // cout << " KEY: " << charKey << endl;
            
        output << "Key-First: " + charKey + '\n';
        output << "Key-Second: " + stringVal + '\n';
        
        //Now write encoded mesg
        while(getline(ss, line))
        {
            output << t_squared.encodeString(line +'\n', encoder);
            output << '\n';
        }

        output.close();
        input.close();
        //Output file path for server to read
        cout << filePath << endl;



        
    }
    else if (endsWith(path, ".txt")) {
        //Get the two paths for writing and reading
        string filePath = "public/uploads/decoded" + to_string(currFile)+ ".csv";
        ofstream output = ofstream(filePath, ios_base::trunc);
        ifstream input = ifstream(path);
        if(!input.is_open())
        {
            cout <<"Path not able to be open" << endl;
            return 1;
        }
        /*
            Read the key for header based on structure and decode into a string seperated by spaces:
                -Length of how many 'nodes' are in tree
                -Node data (convert to string)
                    -char is 1 byte so read one bin char
                        -normal chars: fine to pair as "char: binstring"
                        -nodes with no chars (parent nodes): pair as "'>'": binstring
        */
        //obtain the map code and values to be inputted into freqmap
        unordered_map<char, int> freqMap; //req map
        string charKey;
        string stringVal;
        getline(input, charKey);
        getline(input, stringVal);
        //Now updated at pos
        //recreate huffman tree based on this pattern
        vector<char> charVec;
        vector<int> binVal;
        size_t charStart = charKey.find(": ") + 2;
        size_t valStart = stringVal.find(": ") + 2;

        string temp = "";
        for(size_t i = charStart; i < charKey.size(); i++) {
            if(charKey[i] == ' ') {
                if(!temp.empty()) charVec.push_back((char)stoi(temp));
                temp = "";
            } else {
                temp += charKey[i];
            }
        }

        temp = "";
        for(size_t j = valStart; j < stringVal.size(); j++) {
            if(stringVal[j] == ' ') {
                if(!temp.empty()) binVal.push_back(stoi(temp));
                temp = "";
            } else {
                temp += stringVal[j];
            }
        }
        //cout <<endl;
        //Process to recreate tree
        HuffmanTree treeThatHuffs;
        priority_queue<Node*, vector<Node*>, Compare> temp_Queue = treeThatHuffs.setUpQueueDecrypt(charVec, binVal);
        if(temp_Queue.empty())
        {
            cout <<"Fatal Error: Priority Queue is empty!!!" << endl;
            return 1;
        }
        treeThatHuffs.root = temp_Queue.top();
        unordered_map<char, string> codes;
        treeThatHuffs.preOrder(treeThatHuffs.root, codes, "");
        
        //Then reconstruct a csv file (like txt with new lines) and then end this portion
        string line;
        while(getline(input, line))
        {
            output << treeThatHuffs.decode(line);
        }
        //Completed
        output.close();
        input.close();
        //output the final filepath for server to read
        cout << filePath<<endl;



    }

    else {
        cout << "Invalid file type" << endl;
    }

    return 0;
}