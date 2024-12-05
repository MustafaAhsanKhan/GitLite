#pragma once
#include "string2.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
using namespace filesystem;

// An AVL tree node
struct Node
{
    String key;
    int isLeaf;
    int balanceFactor;
    String leftFile;
    String rightFile;
    String dataRow; // Store the data row in a single String

    Node(const String& k, int leaf, int balance, const String& left, const String& right, const String& data)
        : key(k), isLeaf(leaf), balanceFactor(balance), leftFile(left), rightFile(right), dataRow(data) {
    }
};

class AVL {
private:
    filesystem::path& filePath
    int nodeCount;

    // Helper functions
    String generateFileName(String key)
    {
        return key + "_" + char(nodeCount++) + ".txt";
    }

    // Modify insert and delete functions to work with filenames
public:
    AVL() : rootFileName(""), nodeCount(0) {}

    void customGetline(ifstream& file, String& line, char delimiter = '\n')
    {
        char ch;
        while (file.get(ch)) {
            if (ch == delimiter) {
                break;
            }
            line += ch;
        }
    }

    Node readNodeFromFile(const filesystem::path& filePath)
    {
        ifstream nodeFile(filePath);
        if (!nodeFile.is_open())
        {
            String empty_str("");
            // Handle file not found
            return Node(empty_str, 0, 0, empty_str, empty_str, empty_str);
        }

        String key, leftFile, rightFile, dataRow, line;
        int isLeaf, balanceFactor;

        customGetline(nodeFile, key);
        nodeFile >> isLeaf;
        nodeFile >> balanceFactor;
        nodeFile.ignore(); // Ignore the newline after balanceFactor
        customGetline(nodeFile, leftFile);
        customGetline(nodeFile, rightFile);

        while (customGetline(nodeFile, line)) {
            dataRow += line + "\n";
        }
        nodeFile.close();

        return Node(key, isLeaf, balanceFactor, leftFile, rightFile, dataRow);
    }

    void writeNodeToFile(const Node& node, const String& fileName)
    {
        ofstream nodeFile(fileName.getData());
        nodeFile << node.key << endl;
        nodeFile << node.isLeaf << endl;
        nodeFile << node.balanceFactor << endl;
        nodeFile << node.leftFile << endl;
        nodeFile << node.rightFile << endl;
        nodeFile << node.dataRow; // Write the data row as a single string
        nodeFile.close();
    }

    void insert(const String& key, const String& dataRow) {
        rootFileName = insertHelper(rootFileName, key, dataRow);
    }

    String insertHelper(const filesystem::path& nodeFileName, const String& key, const String& dataRow) {
        if (nodeFileName.empty()) {
            // Create new node
            String newNodeFile = generateFileName(key);
            Node newNode(key, 1, 0, "", "", dataRow);
            writeNodeToFile(newNode, newNodeFile);
            return newNodeFile;
        }

        Node node = readNodeFromFile(nodeFileName);

        if (key < node.key) {
            node.leftFile = insertHelper(node.leftFile, key, dataRow);
        }
        else if (key > node.key) {
            node.rightFile = insertHelper(node.rightFile, key, dataRow);
        }
        else {
            // Duplicate keys are not allowed
            return nodeFileName;
        }

        // Update heights and balance factors
        // Implement getHeight and updateBalanceFactor functions
        // Perform rotations if necessary

        writeNodeToFile(node, nodeFileName);
        return nodeFileName;
    }

    String rightRotate(const String& yFileName) {
        Node y = readNodeFromFile(yFileName);
        Node x = readNodeFromFile(y.leftFile);
        String T2File = x.rightFile;

        // Perform rotation
        x.rightFile = yFileName;
        y.leftFile = T2File;

        // Update balance factors (implement updateBalanceFactor)
        // write updated nodes back to files
        writeNodeToFile(y, yFileName);
        writeNodeToFile(x, x.leftFile);

        return x.leftFile; // New root after rotation
    }

    int getHeight(const String& nodeFileName) {
        if (nodeFileName.empty()) return 0;
        Node node = readNodeFromFile(nodeFileName);
        return 1 + max(getHeight(node.leftFile), getHeight(node.rightFile));
    }

    int getBalanceFactor(const String& nodeFileName) {
        if (nodeFileName.empty()) return 0;
        Node node = readNodeFromFile(nodeFileName);
        return getHeight(node.leftFile) - getHeight(node.rightFile);
    }

    void inOrderTraversal(const String& nodeFileName) {
        if (nodeFileName.empty()) return;
        Node node = readNodeFromFile(nodeFileName);
        inOrderTraversal(node.leftFile);
        cout << node.key << " ";
        inOrderTraversal(node.rightFile);
    }

    void inOrder() {
        inOrderTraversal(rootFileName);
    }

    // Similarly implement leftRotate

    // Implement delete and rotation methods similarly
};
