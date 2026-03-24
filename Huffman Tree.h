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

/*
 This is our Hu ffman Tree as implemented by me, Reggie Borelus. I used https://www.geeksforgeeks.org/dsa/huffman-coding-greedy-algo-3/ and https://www.w3schools.com/dsa/dsa_ref_huffman_coding.php as references
 when implementing this tree and used my experience coding the AVL Tree and experience from Prog 2 to implement this tree to the best of my ability.

 A general workflow for how this works:
 1. We input a string into the buildTree function.
 2. buildTree created a frequency map for how many times a character appears. Then each character is inserted into a priority queue, which retains the min-heap properties, as a node.
 3. The priority queue is then used to build a Huffman Tree.
 4. We then use our map and the same string to encode our string into a binary representation of itself.
 5. Since all chars are stored in the leaves of the tree, the decode function traverse (0 meaning go left and 1 meaning go right) the tree until it hits the leaves and prints out the characters there.

 EXAMPLE:

int main() {
    HuffmanTree ht;
    string s = "Hello World";
    unordered_map<char, string> map = ht.buildTree(s);
    string encoded = ht.encodeString(s, map);
    string decoded = ht.decode(encoded);

    cout << encoded << "\n" << decoded << endl;
}
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
    private:
    Node *root;
    public:

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

    /*This function builds the tree and returns a map of their binary code */
    unordered_map<char, string> buildTree(string s) {
        int n = s.length();
        priority_queue<Node*, vector<Node*>, Compare> prio_queue;
        unordered_map<char, int> freqMap;

        for (int i = 0; i < n; i++) {
            freqMap[s[i]]++;
        }

        for (auto it = freqMap.begin(); it != freqMap.end(); it++  ) {
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
        this->root = prio_queue.top();
        unordered_map<char, string> codes;
        preOrder(root, codes, "");
        return codes;
    }

    /*Uses the tree from above to encode the string into the binary representation*/
    string encodeString(string s, unordered_map<char, string>& codes) {
        string result = "";
        for (int i = 0; i < s.size(); i++) {
            result += codes[s[i]];
        }
        return result;
    }


    string decode(string encoded) {
        string result = "";
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

    string quickEncode(string s) {
        unordered_map<char, string> codes = buildTree(s);
        string result = encodeString(s, codes);
        return result;
    }

};

#endif