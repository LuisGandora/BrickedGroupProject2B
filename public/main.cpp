//compilation command 
//cd public
//g++ -o out.exe main.cpp 'Huffman Tree'.h
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
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
        ofstream output = ofstream(filePath , ios_base::trunc);
        ifstream input = ifstream(path);
        if(!input.is_open())
        {
            cout <<"Path not able to be open" << endl;
            return 1;
        }
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
            totalMesg+='\n';
        }
        //get huffman tree creation
        HuffmanTree t_squared;
        unordered_map<char, string> encoder = t_squared.buildTree(totalMesg);
        unordered_map<char, int> binMap = t_squared.makeFreqMap(totalMesg, totalMesg.size());
        
        //get  encoded string
        string encodedMesg = t_squared.encodeString(totalMesg, encoder);
        /*
        push huffman tree to the txt files
            -push a inorder tree of chars that are encoded

        push the encoded string message into the txt files
            -DOES NOT NEED function for bin conversion, just uses built in huffman tree
        */
        //get all keys and vals for output
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
            stringVal += to_string(it->second) + " ";
        }
        // cout << " KEY: " << charKey << endl;
            
        output << "Key-First: " + charKey + '\n';
        output << "Key-Second: " + stringVal + '\n';
        output << "EncodedCSV: " + encodedMesg + '\n';
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
        string encodedMesg;
        getline(input, charKey);
        getline(input, stringVal);
        getline(input, encodedMesg);
        //recreate huffman tree based on this pattern
        string temp = "";
        vector<char> charVec;
        vector<int> binVal;
        for(int i = 11; i < charKey.size(); i++) //start at 11th index
        {
            if(charKey[i] == ' ')
            {   
                if(temp == "[NL]")  charVec.push_back('\n');
                else if(temp =="[CR]") charVec.push_back('\r'); //to stop carrige register error
                else if(temp == "[TAB]") charVec.push_back('\t');
                else if(temp== "[SPC]") charVec.push_back(' ');
                else charVec.push_back(temp[0]);
                //cout << charVec[charVec.size()-1] << "check" << " ";//Extra space??
                temp = "";
                
                continue;
            }
            temp += charKey[i];
        }
        //cout <<endl;
        for(int j = 12; j < stringVal.size(); j++)
        {
            if(stringVal[j] == ' ')
            {
                binVal.push_back(stoi(temp));
                //cout<<temp <<" ";
                temp = "";
                continue;
            }
            temp += stringVal[j];
        }
        //cout <<endl;
        //Process to recreate tree
        HuffmanTree treeThatHuffs;
        priority_queue<Node*, vector<Node*>, Compare> temp_Queue = treeThatHuffs.setUpQueueDecrypt(charVec, binVal);
        treeThatHuffs.root = temp_Queue.top();
        unordered_map<char, string> codes;
        treeThatHuffs.preOrder(treeThatHuffs.root, codes, "");
        
        string decodedMesg = treeThatHuffs.decode(encodedMesg.substr(12, encodedMesg.size()));
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
        //output the final filepath for server to read
        cout << filePath<<endl;
    }

    else {
        cout << "Invalid file type" << endl;
    }

    return 0;
}