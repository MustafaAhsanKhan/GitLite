#pragma once
#include<iostream>
#include<filesystem>
#include<iomanip>
#include<fstream>
#include "String.h"
#include <string>
#include <queue>
using namespace std;
using namespace filesystem;

template <typename T, int o>
struct BTreeNode {
    path currentNodePath;
    path parentPath;
    int numOfKeys = 0;
    bool isLeaf = true;
    T keys[o];
    path keyPath[o];
    path children[o + 1];

    BTreeNode(path nodePath, path currentPath) {
        currentNodePath = currentPath;
        parentPath = nodePath;

        for (int i = 0; i < o + 1; i++) {
            children[i] = "";
        }
    }

    BTreeNode(path p) {
        currentNodePath = p;
        ifstream file(p);
        file >> numOfKeys;
        file >> isLeaf;
        file >> parentPath;

        for (int i = 0; i < numOfKeys; i++) {
            file >> keys[i];
        }
        for (int i = 0; i < numOfKeys; i++) {
            file >> keyPath[i];
        }
        for (int i = 0; i < o + 1; i++) {
            file >> children[i];
        }
        file.close();
    }

    void write() {
        ofstream file(currentNodePath);
        file << numOfKeys << endl;
        file << isLeaf << endl;
        file << parentPath << endl;
        for (int i = 0; i < numOfKeys; i++) {
            file << keys[i] << endl;
        }
        for (int i = 0; i < numOfKeys; i++) {
            file << keyPath[i] << endl;
        }
        for (int i = 0; i < o + 1; i++) {
            file << children[i] << endl;
        }
        file.close();
    }
    void writeKey(int index, String& key) {
        path keyFilePath = currentNodePath.parent_path() / "keys" / (currentNodePath.filename().string() + to_string(index) + ".txt");
        ofstream keyFile(keyFilePath);
        keyFile << key;
        keyFile.close();
        keyPath[index] = keyFilePath;
    }
    T readKey(int index) {
        path keyFilePath = keyPath[index];
        ifstream keyFile(keyFilePath);
        T key;
        keyFile >> key;
        keyFile.close();
        return key;
    }
};

template <typename T, int o>
class BTree {
    path rootPath;
    BTreeNode<T, o>* root;
    int nodeCount = 0; // Node count to keep track of the number of nodes

    void sort(T* arr, int numKeys) {
        for (int i = numKeys - 1; i > 0; i--) {
            if (arr[i - 1] > arr[i]) {
                T temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
            }
        }
    }

    BTreeNode<T, o>* createNode(path parentPath) {
        nodeCount++;
        path nodePath = rootPath / (to_string(nodeCount) + ".txt");
        return new BTreeNode<T, o>(parentPath, nodePath);
    }

    BTreeNode<T, o>* insert(BTreeNode<T, o>* node, T data, BTreeNode<T, o>*& newChild, T& newKey, String& nodeData) {
        if (!node) {
            node = createNode(rootPath);
            node->writeKey(node->numOfKeys, nodeData);
            node->keys[node->numOfKeys++] = data;
            newChild = nullptr;
            node->write();
            return node;
        }
        else {
            if (node->isLeaf) {
				node->writeKey(node->numOfKeys, nodeData);
                node->keys[node->numOfKeys++] = data;
                sort(node->keys, node->numOfKeys);
                if (node->numOfKeys == o) {
                    newChild = createNode(node->currentNodePath);
                    newKey = node->keys[o / 2];
                    for (int i = o / 2 + 1; i < o; i++) {
                        String datdfa;
						ifstream file(node->keyPath[i]);
						datdfa.readComplete(file);
                        file.close();
						newChild->writeKey(newChild->numOfKeys, datdfa);
                        newChild->keys[newChild->numOfKeys++] = node->keys[i];
                    }
                    node->numOfKeys = o / 2;
                    node->write();
                    newChild->write();
                    return node;
                }
                else {
                    newChild = nullptr;
                    node->write();
                    return node;
                }
            }
            else {
                int i = 0;
                while (i < node->numOfKeys && data > node->keys[i]) {
                    i++;
                }
                BTreeNode<T, o>* child = new BTreeNode<T, o>(node->children[i]);
                child = insert(child, data, newChild, newKey, nodeData);
                if (newChild) {

                    for (int j = node->numOfKeys; j > i; j--) {
                        String datdfa;
                        ifstream file(node->keyPath[j - 1]);
                        datdfa.readComplete(file);
                        file.close();
                        node->writeKey(j, datdfa);
                        node->keys[j] = node->keys[j - 1];
                        node->children[j + 1] = node->children[j];
                    }
                    String datdfa;
                    ifstream file(node->keyPath[i]);
                    datdfa.readComplete(file);
                    file.close();
					node->writeKey(i, datdfa);
                    node->keys[i] = newKey;
                    node->children[i + 1] = newChild->currentNodePath;
                    node->numOfKeys++;
                    if (node->numOfKeys == o) {
                        newChild = createNode(node->currentNodePath);
                        newChild->isLeaf = false;
                        newKey = node->keys[o / 2];
                        for (int j = o / 2 + 1; j < o; j++) {
                            String datdfa;
                            ifstream file(node->keyPath[i]);
                            datdfa.readComplete(file);
                            file.close();
							newChild->writeKey(newChild->numOfKeys, datdfa);
                            newChild->keys[newChild->numOfKeys++] = node->keys[j];  
                            newChild->children[newChild->numOfKeys] = node->children[j + 1];
                        }
                        newChild->children[0] = node->children[o / 2 + 1];
                        node->numOfKeys = o / 2;
                        node->write();
                        newChild->write();
                        return node;
                    }
                    else {
                        newChild = nullptr;
                        node->write();
                        return node;
                    }
                }
                else {
                    node->write();
                    return node;
                }
            }
        }
    }

    
public:
    BTree(path rootPath) {
        this->rootPath = rootPath;
        root = nullptr;
    }

    void insert(T data, String rowData) {
        BTreeNode<T, o>* newChild = nullptr;
        T newKey;
        root = insert(root, data, newChild, newKey, rowData);
        if (newChild) {
            BTreeNode<T, o>* newRoot = createNode(rootPath);
            String datdfa;
            ifstream file(newRoot->keyPath[0]);
            datdfa.readComplete(file);
            file.close();
            newRoot->writeKey(0, datdfa);
            newRoot->keys[0] = newKey;
            newRoot->children[0] = root->currentNodePath;
            newRoot->children[1] = newChild->currentNodePath;
            newRoot->numOfKeys = 1;
            newRoot->isLeaf = false;
            root = newRoot;
            root->write();
        }
        //cout << endl;
        //print(root, 0);
    }
    void print() {
		print(root, 0);
    }
    void remove(T k) {
        if (!root) {
            cout << "The tree is empty\n";
            return;
        }
        remove(root, k);
        if (root->numOfKeys == 0) {
            BTreeNode<T, o>* tmp = root;
            if (root->isLeaf) {
                root = nullptr;
            }
            else {
                root = new BTreeNode<T, o>(root->children[0]);
            }
            delete tmp;
        }
        cout << endl;
        print(root, 0);
    }
};