#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

template <typename T, int o>
struct BTreeNode {
    int order = o, numOfKeys = 0;
    T keys[o];
    fs::path children[o];
    bool isLeaf = true;

    BTreeNode() {
        for (int i = 0; i < o; i++) {
            children[i] = "";
        }
    }

    void writeKeys(const fs::path& path) {
        for (int i = 0; i < numOfKeys; i++) {
            ofstream keyFile(path.string() + "_key_" + to_string(i) + ".txt");
            keyFile << keys[i];
            keyFile.close();
        }
    }

    void writeChildren(const fs::path& path) {
        for (int i = 0; i < o; i++) {
            ofstream childFile(path.string() + "_child_" + to_string(i) + ".txt");
            childFile << children[i].string();
            childFile.close();
        }
    }

    void readKeys(const fs::path& path) {
        for (int i = 0; i < numOfKeys; i++) {
            ifstream keyFile(path.string() + "_key_" + to_string(i) + ".txt");
            keyFile >> keys[i];
            keyFile.close();
        }
    }

    void readChildren(const fs::path& path) {
        for (int i = 0; i < o; i++) {
            ifstream childFile(path.string() + "_child_" + to_string(i) + ".txt");
            string childPath;
            childFile >> childPath;
            children[i] = childPath;
            childFile.close();
        }
    }

    string toString() {
        stringstream ss;
        ss << numOfKeys << " " << isLeaf << " ";
        return ss.str();
    }

    void fromString(const string& str) {
        stringstream ss(str);
        ss >> numOfKeys >> isLeaf;
    }
};

template <typename T, int o>
class BTree {
    int nodeCount = 0;
    fs::path rootPath;

    void writeNode(const fs::path& path, BTreeNode<T, o>* node) {
        ofstream file("tree/" + path.string());
        file << node->toString();
        file.close();
        node->writeKeys("tree/" + path.string());
        node->writeChildren("tree/" + path.string());
    }

    BTreeNode<T, o>* readNode(const fs::path& path) {
        ifstream file("tree/" + path.string());
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        BTreeNode<T, o>* node = new BTreeNode<T, o>();
        node->fromString(buffer.str());
        node->readKeys("tree/" + path.string());
        node->readChildren("tree/" + path.string());
        return node;
    }

    void sort(T* arr, int numKeys) {
        for (int i = numKeys - 1; i > 0; i--) {
            if (arr[i - 1] > arr[i]) {
                T temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
            }
        }
    }

    BTreeNode<T, o>* insert(BTreeNode<T, o>* node, T data, BTreeNode<T, o>*& newChild, T& newKey, const fs::path& path) {
        if (!node) {
            node = new BTreeNode<T, o>();
            node->keys[node->numOfKeys++] = data;
            newChild = nullptr;
            writeNode(path, node);
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
                    writeNode(path, node);
                    return node;
                }
                else {
                    newChild = nullptr;
                    writeNode(path, node);
                    return node;
                }
            }
            else {
                int i = 0;
                while (i < node->numOfKeys && data > node->keys[i]) {
                    i++;
                }
                BTreeNode<T, o>* child = readNode(node->children[i]);
                BTreeNode<T, o>* newChildLocal = nullptr;
                T newKeyLocal;
                child = insert(child, data, newChildLocal, newKeyLocal, node->children[i]);
                if (newChildLocal) {
                    for (int j = node->numOfKeys; j > i; j--) {
                        node->keys[j] = node->keys[j - 1];
                        node->children[j + 1] = node->children[j];
                    }
                    node->keys[i] = newKeyLocal;
                    fs::path newChildPath = "node_" + to_string(nodeCount) + ".txt";
                    nodeCount++;
                    node->children[i + 1] = newChildPath;
                    writeNode(newChildPath, newChildLocal);
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
                        writeNode(path, node);
                        return node;
                    }
                    else {
                        newChild = nullptr;
                        writeNode(path, node);
                        return node;
                    }
                }
                else {
                    writeNode(path, node);
                    return node;
                }
            }
        }
    }

    void print(BTreeNode<T, o>* node, int indent) {
        if (!node) return;
        for (int i = 0; i < node->numOfKeys; i++) {
            if (!node->isLeaf) {
                BTreeNode<T, o>* child = readNode(node->children[i]);
                print(child, indent + 4);
                delete child;
            }
            cout << setw(indent) << " " << node->keys[i] << endl;
        }
        if (!node->isLeaf) {
            BTreeNode<T, o>* child = readNode(node->children[node->numOfKeys]);
            print(child, indent + 4);
            delete child;
        }
    }

public:
    BTree() {
        rootPath = "root.txt";
        ofstream file(rootPath);
        file.close();
    }

    void insert(T data) {
        BTreeNode<T, o>* newChild = nullptr;
        T newKey;
        BTreeNode<T, o>* root = readNode(rootPath);
        root = insert(root, data, newChild, newKey, rootPath);
        if (newChild) {
            BTreeNode<T, o>* newRoot = new BTreeNode<T, o>();
            newRoot->keys[0] = newKey;
            fs::path newChildPath = "node_" + to_string(nodeCount) + ".txt";
            newRoot->children[0] = rootPath;
            newRoot->children[1] = newChildPath;
            newRoot->numOfKeys = 1;
            newRoot->isLeaf = false;
            writeNode(newChildPath, newChild);
            rootPath = "root_" + to_string(nodeCount) + ".txt";
            writeNode(rootPath, newRoot);
        }
        else {
            writeNode(rootPath, root);
        }
        cout << endl;
        print(root, 0);
    }
};