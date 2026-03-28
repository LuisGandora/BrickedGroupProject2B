//
// Created by Jayde on 3/19/2026.
//

#ifndef BRICKEDGROUPPROJECT2B_HUFFMAN_TREE_H
#define BRICKEDGROUPPROJECT2B_HUFFMAN_TREE_H
#include <bits/exception_ptr.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <queue>
#include <functional>

using namespace std;

/*\
 This is our Huffman Tree as implemented by me, Reggie Borelus. I used https://www.geeksforgeeks.org/dsa/huffman-coding-greedy-algo-3/ and https://www.w3schools.com/dsa/dsa_ref_huffman_coding.php as references
 when implementing this tree and used  my expiernece coding the AVL Tree and expiernce from Prog 2 to implement this tree to the best of my ability.
 */

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char ch, int freq) {
        this->ch = ch;
        this->freq = freq;
        left = nullptr;
        right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) { //min-heap comparison for priority queue
        return a->freq > b->freq;
    }
};

class HuffmanTree {
    public:

    Node *root;
    HuffmanTree() {
        root = nullptr;
    }

    ~HuffmanTree() {
        function<void(Node*)> deleteNode = [&](Node* node) {  //Wrapped a lambda expression in a function to recursively delete our nodes. Learned this one last semester when working on Minesweeper :D
            if (node == nullptr) {
                return;
            }
            if (node != nullptr) {
                deleteNode(node->left);
                deleteNode(node->right);
                delete node;
            }
        };
        deleteNode(root);
        root = nullptr;
    }

    /*A preorder traversal to convert each character into a binary Huffman code*/

    void preOrder(Node* root, unordered_map<char, string>& codes, string s) {
        if (root == nullptr) {
            return;
        }
        if (root -> left == nullptr && root ->right == nullptr) {
            if (s.empty()) {
                codes[root->ch] = "0";
            }
            else {
                codes[root->ch] = s;
            }
            return;
        }
        preOrder(root -> left, codes, s + '0');
        preOrder(root ->right, codes, s + '1');
    }

    //accepts a string and int for length of string 
    unordered_map<char, int> makeFreqMap(string s, int n)
    {
        unordered_map<char, int> freqMap;
        for (int i = 0; i < n; i++) {
            freqMap[s[i]]++;
        }
        // freqMap
        return freqMap;
    }

    //Function meant to abstract and set up root with 2 vectors representing the freqmap due to errors with main
    priority_queue<Node*, vector<Node*>, Compare> setUpQueue(unordered_map<char, int> freqMap)
    {
        priority_queue<Node*, vector<Node*>, Compare> prio_queue;
        for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
            prio_queue.push(new Node(it->first, it->second));
        }

        while (prio_queue.size() >= 2) {
            Node* left = prio_queue.top();
            prio_queue.pop();
            Node* right = prio_queue.top();
            prio_queue.pop();

            Node* newNode = new Node('\0', left->freq+ right->freq);
            newNode->left = left;
            newNode->right = right;

            prio_queue.push(newNode);
        }

        if (prio_queue.empty()) {
            return {};
        }
        return prio_queue;
    }

    //Function meant to abstract and set up root with 2 vectors representing the freqmap due to errors with main
    priority_queue<Node*, vector<Node*>, Compare> setUpQueueDecrypt(vector<char> first, vector<int> second)
    {
        priority_queue<Node*, vector<Node*>, Compare> prio_queue;
        for (int i = 0; i < first.size();i++ ) {
            prio_queue.push(new Node(first[i], second[i]));
        }

        while (prio_queue.size() >= 2) {
            Node* left = prio_queue.top();
            prio_queue.pop();
            Node* right = prio_queue.top();
            prio_queue.pop();

            Node* newNode = new Node('\0', left->freq+ right->freq);
            newNode->left = left;
            newNode->right = right;

            prio_queue.push(newNode);
        }

        if (prio_queue.empty()) {
            return {};
        }
        return prio_queue;
    }

    /*This function builds the tree and returns a map of their binary code */
    unordered_map<char, string> buildTree(string s) {
        int n = s.length();
        
        unordered_map<char, int> freqMap = makeFreqMap(s, n);


        priority_queue<Node*, vector<Node*>, Compare> prio_queue = setUpQueue(freqMap);
        this->root = prio_queue.top();
        unordered_map<char, string> codes;
        preOrder(root, codes, "");
        return codes;
    }

    /*Uses the tree from above to encode the string into the binary representation*/
    string encodeString(string s, unordered_map<char, string>& codes) {
        string result = "";
        result.reserve(s.size()*8); //So RAM doesnt break
        for (int i = 0; i < s.size(); i++) {
            result += codes[s[i]];
        }
        return result;
    }


    string decode(string encoded) {
        string result = "";
        result.reserve(encoded.size()*8);
        Node* temp = root;

        if (temp == nullptr) {
            return result;
        }

        for (int i = 0; i < encoded.size(); i++) {
            char ch = encoded[i];

            if (ch == '0') {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }

            if (!temp->left && !temp->right) {
                result += temp->ch;
                temp = root;
            }
        }
        return result;
    }

};

#endif