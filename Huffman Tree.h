//
// Created by Jayde on 3/19/2026.
//

#ifndef BRICKEDGROUPPROJECT2B_HUFFMAN_TREE_H
#define BRICKEDGROUPPROJECT2B_HUFFMAN_TREE_H
#include <bits/exception_ptr.h>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

#endif //BRICKEDGROUPPROJECT2B_HUFFMAN_TREE_H

struct Node {
    int data;
    Node *left, *right;

    Node(int x) {
        data = x;
        left = nullptr;
        right = nullptr;
    }
};

class HuffmanTree {
    public:
    HuffmanTree(string const& s) {

    };


    ~HuffmanTree();

    void heapify(vector<Node*>& arr, int const i, int const n) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left]->data < arr[smallest]->data) {
            smallest = left;
        }
        if (right < n && arr[right]->data < arr[smallest]->data) {
            smallest = right;
        }

        if (smallest != i) {
            swap(arr[i], arr[smallest]);
            heapify(arr, smallest, n);
        }
    }
    void preOrder(Node* root, vector<string>& words, string s) {
        if (root == nullptr) {
            return;
        }
        if (root -> left == nullptr && root ->right == nullptr) {
            words.push_back(s);
            return;
        }
        preOrder(root -> left, words, s + '0');
        preOrder(root ->right, words, s + '1');
    }
};