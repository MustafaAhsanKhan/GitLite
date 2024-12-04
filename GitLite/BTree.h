#pragma once

#include<iostream>
#include<iomanip>
using namespace std;

template <typename T, int o>
struct BTreeNode {
    int order = o, numOfKeys = 0;
    T keys[o];
    BTreeNode<T, o>* children[o];
    bool isLeaf = true;
    BTreeNode() {
        for (int i = 0; i < o; i++) {
            children[i] = nullptr;
        }
    }
};

template <typename T, int o>
class BTree {
    BTreeNode<T, o>* root;

    void sort(T* arr, int numKeys) {
        for (int i = numKeys - 1; i > 0; i--) {
            if (arr[i - 1] > arr[i]) {
                T temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
            }
        }
    }

    BTreeNode<T, o>* insert(BTreeNode<T, o>* node, T data, BTreeNode<T, o>*& newChild, T& newKey) {
        if (!node) {
            node = new BTreeNode<T, o>();
            node->keys[node->numOfKeys++] = data;
            newChild = nullptr;
            return node;
        }
        else {
            if (node->isLeaf) {
                node->keys[node->numOfKeys++] = data;
                sort(node->keys, node->numOfKeys);
                if (node->numOfKeys == o) {
                    newChild = new BTreeNode<T, o>();
                    newKey = node->keys[o / 2];
                    for (int i = o / 2 + 1; i < o; i++) {
                        newChild->keys[newChild->numOfKeys++] = node->keys[i];
                    }
                    node->numOfKeys = o / 2;
                    return node;
                }
                else {
                    newChild = nullptr;
                    return node;
                }
            }
            else {
                int i = 0;
                while (i < node->numOfKeys && data > node->keys[i]) {
                    i++;
                }
                BTreeNode<T, o>* child = insert(node->children[i], data, newChild, newKey);
                if (newChild) {
                    for (int j = node->numOfKeys; j > i; j--) {
                        node->keys[j] = node->keys[j - 1];
                        node->children[j + 1] = node->children[j];
                    }
                    node->keys[i] = newKey;
                    node->children[i + 1] = newChild;
                    node->numOfKeys++;
                    if (node->numOfKeys == o) {
                        newChild = new BTreeNode<T, o>();
                        newChild->isLeaf = false;
                        newKey = node->keys[o / 2];
                        for (int j = o / 2 + 1; j < o; j++) {
                            newChild->keys[newChild->numOfKeys++] = node->keys[j];
                            newChild->children[newChild->numOfKeys] = node->children[j + 1];
                        }
                        newChild->children[0] = node->children[o / 2 + 1];
                        node->numOfKeys = o / 2;
                        return node;
                    }
                    else {
                        newChild = nullptr;
                        return node;
                    }
                }
                else {
                    return node;
                }
            }
        }
    }

    void print(BTreeNode<T, o>* node, int indent) {
        if (!node) return;
        for (int i = 0; i < node->numOfKeys; i++) {
            print(node->children[i], indent + 4);
            cout << setw(indent) << " " << node->keys[i] << endl;
        }
        print(node->children[node->numOfKeys], indent + 4);
    }

public:
    BTree() {
        root = nullptr;
    }

    void insert(T data) {
        BTreeNode<T, o>* newChild = nullptr;
        T newKey;
        root = insert(root, data, newChild, newKey);
        if (newChild) {
            BTreeNode<T, o>* newRoot = new BTreeNode<T, o>();
            newRoot->keys[0] = newKey;
            newRoot->children[0] = root;
            newRoot->children[1] = newChild;
            newRoot->numOfKeys = 1;
            newRoot->isLeaf = false;
            root = newRoot;
        }
        cout << endl;
        print(root, 0);
    }
};
