//#pragma once
//#include<iostream>
//#include<filesystem>
//#include<iomanip>
//#include<fstream>
//#include "string2.h"
//#include <string>
//using namespace std;
//using namespace filesystem;
//
//
//template <typename T, int o>
//struct BTreeNode {
//	path currentNodePath;
//	path parentPath;
//	int numOfKeys = 0;
//	bool isLeaf = true;
//	T keys[o];
//	path keyPath[o];
//	path children[o];
//	BTreeNode(path nodePath, path currentPath) {
//		currentNodePath = currentPath;
//		parentPath = nodePath;
//
//		for (int i = 0; i < o; i++) {
//			children[i] = "";
//		}
//	}
//	BTreeNode(path p) {
//		currentNodePath = p;
//		ifstream file(p);
//		file >> numOfKeys;
//		file >> isLeaf;
//		file >> parentPath;
//
//		for (int i = 0; i < numOfKeys; i++) {
//			file >> keys[i];
//		}
//		for (int i = 0; i < numOfKeys; i++) {
//			file >> keyPath[i];
//		}
//		for (int i = 0; i < o; i++) {
//			file >> children[i];
//		}
//	}
//	void write() {
//		ofstream file(currentNodePath);
//		file << numOfKeys << endl;
//		file << isLeaf << endl;
//		file << parentPath << endl;
//		for (int i = 0; i < numOfKeys; i++) {
//			file << keys[i] << endl;
//		}
//		for (int i = 0; i < numOfKeys; i++) {
//			file << keyPath[i] << endl;
//		}
//		for (int i = 0; i < o; i++) {
//			file << children[i] << endl;
//		}
//
//	}
//};
//
//template <typename T, int o>
//class BTree {
//	
//	path rootPath;
//	BTreeNode<T, o>* root;
//	void sort(T* arr, int numKeys) {
//		for (int i = numKeys - 1; i > 0; i--) {
//			if (arr[i - 1] > arr[i]) {
//				T temp = arr[i - 1];
//				arr[i - 1] = arr[i];
//				arr[i] = temp;
//			}
//		}
//	}
//
//
//
//public:
//
//};

#pragma once
#include<iostream>
#include<filesystem>
#include<iomanip>
#include<fstream>
#include "string2.h"
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
    void writeKey(int index, T key) {
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

    BTreeNode<T, o>* insert(BTreeNode<T, o>* node, T data, BTreeNode<T, o>*& newChild, T& newKey) {
        if (!node) {
            node = createNode(rootPath);
            node->writeKey(node->numOfKeys, data);
            node->keys[node->numOfKeys++] = data;
            newChild = nullptr;
            node->write();
            return node;
        }
        else {
            if (node->isLeaf) {
				node->writeKey(node->numOfKeys, data);
                node->keys[node->numOfKeys++] = data;
                sort(node->keys, node->numOfKeys);
                if (node->numOfKeys == o) {
                    newChild = createNode(node->currentNodePath);
                    newKey = node->keys[o / 2];
                    for (int i = o / 2 + 1; i < o; i++) {
						newChild->writeKey(newChild->numOfKeys, node->keys[i]);
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
                child = insert(child, data, newChild, newKey);
                if (newChild) {
                    for (int j = node->numOfKeys; j > i; j--) {
                        node->writeKey(j, node->keys[j - 1]);
                        node->keys[j] = node->keys[j - 1];
                        node->children[j + 1] = node->children[j];
                    }
					node->writeKey(i, newKey);
                    node->keys[i] = newKey;
                    node->children[i + 1] = newChild->currentNodePath;
                    node->numOfKeys++;
                    if (node->numOfKeys == o) {
                        newChild = createNode(node->currentNodePath);
                        newChild->isLeaf = false;
                        newKey = node->keys[o / 2];
                        for (int j = o / 2 + 1; j < o; j++) {
							newChild->writeKey(newChild->numOfKeys, node->keys[j]);
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

    //void print(BTreeNode<T, o>* node, int indent) {
    //    if (!node) return;
    //    for (int i = 0; i < node->numOfKeys; i++) {
    //        print(new BTreeNode<T, o>(node->children[i]), indent + 4);
    //        cout << setw(indent) << " " << node->keys[i] << endl;
    //    }
    //    print(new BTreeNode<T, o>(node->children[node->numOfKeys]), indent + 4);
    //}

    //void removeFromLeaf(BTreeNode<T, o>* node, int idx) {
    //    for (int i = idx + 1; i < node->numOfKeys; ++i) {
    //        node->keys[i - 1] = node->keys[i];
    //    }
    //    node->numOfKeys--;
    //    node->write();
    //}

    //void removeFromNonLeaf(BTreeNode<T, o>* node, int idx) {
    //    T k = node->keys[idx];
    //    if ((new BTreeNode<T, o>(node->children[idx]))->numOfKeys >= o / 2) {
    //        T pred = getPredecessor(node, idx);
    //        node->keys[idx] = pred;
    //        remove(new BTreeNode<T, o>(node->children[idx]), pred);
    //    }
    //    else if ((new BTreeNode<T, o>(node->children[idx + 1]))->numOfKeys >= o / 2) {
    //        T succ = getSuccessor(node, idx);
    //        node->keys[idx] = succ;
    //        remove(new BTreeNode<T, o>(node->children[idx + 1]), succ);
    //    }
    //    else {
    //        merge(node, idx);
    //        remove(new BTreeNode<T, o>(node->children[idx]), k);
    //    }
    //}

    //T getPredecessor(BTreeNode<T, o>* node, int idx) {
    //    BTreeNode<T, o>* cur = new BTreeNode<T, o>(node->children[idx]);
    //    while (!cur->isLeaf) {
    //        cur = new BTreeNode<T, o>(cur->children[cur->numOfKeys]);
    //    }
    //    return cur->keys[cur->numOfKeys - 1];
    //}

    //T getSuccessor(BTreeNode<T, o>* node, int idx) {
    //    BTreeNode<T, o>* cur = new BTreeNode<T, o>(node->children[idx + 1]);
    //    while (!cur->isLeaf) {
    //        cur = new BTreeNode<T, o>(cur->children[0]);
    //    }
    //    return cur->keys[0];
    //}

    //void merge(BTreeNode<T, o>* node, int idx) {
    //    BTreeNode<T, o>* child = new BTreeNode<T, o>(node->children[idx]);
    //    BTreeNode<T, o>* sibling = new BTreeNode<T, o>(node->children[idx + 1]);
    //    child->keys[o / 2 - 1] = node->keys[idx];
    //    for (int i = 0; i < sibling->numOfKeys; ++i) {
    //        child->keys[i + o / 2] = sibling->keys[i];
    //    }
    //    if (!child->isLeaf) {
    //        for (int i = 0; i <= sibling->numOfKeys; ++i) {
    //            child->children[i + o / 2] = sibling->children[i];
    //        }
    //    }
    //    for (int i = idx + 1; i < node->numOfKeys; ++i) {
    //        node->keys[i - 1] = node->keys[i];
    //    }
    //    for (int i = idx + 2; i <= node->numOfKeys; ++i) {
    //        node->children[i - 1] = node->children[i];
    //    }
    //    child->numOfKeys += sibling->numOfKeys + 1;
    //    node->numOfKeys--;
    //    child->write();
    //    node->write();
    //    delete sibling;
    //}

    //void fill(BTreeNode<T, o>* node, int idx) {
    //    if (idx != 0 && (new BTreeNode<T, o>(node->children[idx - 1]))->numOfKeys >= o / 2) {
    //        borrowFromPrev(node, idx);
    //    }
    //    else if (idx != node->numOfKeys && (new BTreeNode<T, o>(node->children[idx + 1]))->numOfKeys >= o / 2) {
    //        borrowFromNext(node, idx);
    //    }
    //    else {
    //        if (idx != node->numOfKeys) {
    //            merge(node, idx);
    //        }
    //        else {
    //            merge(node, idx - 1);
    //        }
    //    }
    //}

    //void borrowFromPrev(BTreeNode<T, o>* node, int idx) {
    //    BTreeNode<T, o>* child = new BTreeNode<T, o>(node->children[idx]);
    //    BTreeNode<T, o>* sibling = new BTreeNode<T, o>(node->children[idx - 1]);
    //    for (int i = child->numOfKeys - 1; i >= 0; --i) {
    //        child->keys[i + 1] = child->keys[i];
    //    }
    //    if (!child->isLeaf) {
    //        for (int i = child->numOfKeys; i >= 0; --i) {
    //            child->children[i + 1] = child->children[i];
    //        }
    //    }
    //    child->keys[0] = node->keys[idx - 1];
    //    if (!child->isLeaf) {
    //        child->children[0] = sibling->children[sibling->numOfKeys];
    //    }
    //    node->keys[idx - 1] = sibling->keys[sibling->numOfKeys - 1];
    //    child->numOfKeys += 1;
    //    sibling->numOfKeys -= 1;
    //    child->write();
    //    sibling->write();
    //    node->write();
    //}

    //void borrowFromNext(BTreeNode<T, o>* node, int idx) {
    //    BTreeNode<T, o>* child = new BTreeNode<T, o>(node->children[idx]);
    //    BTreeNode<T, o>* sibling = new BTreeNode<T, o>(node->children[idx + 1]);
    //    child->keys[child->numOfKeys] = node->keys[idx];
    //    if (!child->isLeaf) {
    //        child->children[child->numOfKeys + 1] = sibling->children[0];
    //    }
    //    node->keys[idx] = sibling->keys[0];
    //    for (int i = 1; i < sibling->numOfKeys; ++i) {
    //        sibling->keys[i - 1] = sibling->keys[i];
    //    }
    //    if (!sibling->isLeaf) {
    //        for (int i = 1; i <= sibling->numOfKeys; ++i) {
    //            sibling->children[i - 1] = sibling->children[i];
    //        }
    //    }
    //    child->numOfKeys += 1;
    //    sibling->numOfKeys -= 1;
    //    child->write();
    //    sibling->write();
    //    node->write();
    //}

    //void remove(BTreeNode<T, o>* node, T k) {
    //    int idx = 0;
    //    while (idx < node->numOfKeys && node->keys[idx] < k) {
    //        ++idx;
    //    }
    //    if (idx < node->numOfKeys && node->keys[idx] == k) {
    //        if (node->isLeaf) {
    //            removeFromLeaf(node, idx);
    //        }
    //        else {
    //            removeFromNonLeaf(node, idx);
    //        }
    //    }
    //    else {
    //        if (node->isLeaf) {
    //            cout << "The key " << k << " is not present in the tree\n";
    //            return;
    //        }
    //        bool flag = (idx == node->numOfKeys);
    //        if ((new BTreeNode<T, o>(node->children[idx]))->numOfKeys < o / 2) {
    //            fill(node, idx);
    //        }
    //        if (flag && idx > node->numOfKeys) {
    //            remove(new BTreeNode<T, o>(node->children[idx - 1]), k);
    //        }
    //        else {
    //            remove(new BTreeNode<T, o>(node->children[idx]), k);
    //        }
    //    }
    //}

public:
    BTree(path rootPath) {
        this->rootPath = rootPath;
        root = nullptr;
    }

    void insert(T data) {
        BTreeNode<T, o>* newChild = nullptr;
        T newKey;
        root = insert(root, data, newChild, newKey);
        if (newChild) {
            BTreeNode<T, o>* newRoot = createNode(rootPath);
            newRoot->writeKey(0, newKey);
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