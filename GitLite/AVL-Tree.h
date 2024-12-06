#pragma once
#include "string2.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

String to_string(int value) {
    String result;
    bool isNegative = false;

    // Handle negative numbers
    if (value < 0) {
        isNegative = true;
        value = -value;
    }

    // Convert integer to string
    do {
        result += (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    // Add negative sign if necessary
    if (isNegative) {
        result += '-';
    }

    // Reverse the string since the digits are added in reverse order
    int len = result.size();
    for (int i = 0; i < len / 2; ++i) {
        char temp = result[i];
        result[i] = result[len - i - 1];
        result[len - i - 1] = temp;
    }

    return result;
}

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
    fs::path rootFileName;
    int nodeCount;

    String generateFileName(String key)
    {
        String fileName = key + ' ' + to_string(nodeCount++) + ".txt";
        return fileName;
    }

    // Modify insert and delete functions to work with filenames
public:
    AVL() : rootFileName(""), nodeCount(0) {}

    void customGetline(std::ifstream& file, String& line, char delimiter = '\n') {
        char ch;
        while (file.get(ch)) {
            if (ch == delimiter) {
                break;
            }
            line += ch;
        }
    }

    Node readNodeFromFile(const fs::path& filePath) {
        std::ifstream nodeFile(filePath);
        if (!nodeFile.is_open()) {
            // Return a node indicating that it doesn't exist
            return Node("", 0, 0, "", "", "");
        }

        String key, leftFile, rightFile, dataRow, line;
        int isLeaf, balanceFactor;

        customGetline(nodeFile, key);
        nodeFile >> isLeaf;
        nodeFile >> balanceFactor;
        nodeFile.ignore(); // Ignore the newline after balanceFactor
        customGetline(nodeFile, leftFile);
        customGetline(nodeFile, rightFile);

        customGetline(nodeFile, dataRow); // Read the data row as a single string
        dataRow += "\n";

        nodeFile.close();

        return Node(key, isLeaf, balanceFactor, leftFile, rightFile, dataRow);
    }

    void writeNodeToFile(const Node& node, const fs::path& filePath) {
        std::ofstream nodeFile(filePath);
        nodeFile << node.key << std::endl;
        nodeFile << node.isLeaf << std::endl;
        nodeFile << node.balanceFactor << std::endl;
        nodeFile << node.leftFile << std::endl;
        nodeFile << node.rightFile << std::endl;
        nodeFile << node.dataRow; // Write the data row as a single string
        nodeFile.close();
    }

    void insert(const String& key, const String& dataRow) {
        rootFileName = insertHelper(rootFileName, key, dataRow);
    }

    fs::path insertHelper(const fs::path& nodeFileName, const String& key, const String& dataRow) {
        if (nodeFileName.empty()) {
            // Create new node
            fs::path newNodeFile = generateFileName(key).getData();
            Node newNode(key, 1, 0, "", "", dataRow);
            writeNodeToFile(newNode, newNodeFile);
            return newNodeFile;
        }

        Node node = readNodeFromFile(nodeFileName);

        // In your insertHelper function, before assigning child files:
        if (node.leftFile.getData() == nodeFileName) { node.leftFile = ""; }
        if (node.rightFile.getData() == nodeFileName) { node.rightFile = ""; }


        if (key.isGreaterThan(node.key) == 2)
        {
            node.leftFile = insertHelper(node.leftFile.getData(), key, dataRow).string();
        }
        else if (key.isGreaterThan(node.key) == 1) {
            node.rightFile = insertHelper(node.rightFile.getData(), key, dataRow).string();
        }
        else {
            // Duplicate keys are not allowed
            return nodeFileName;
        }

        // Update heights and balance factors
        updateHeight(nodeFileName);
        updateBalanceFactor(nodeFileName);

        // Perform rotations if necessary
        int balance = getBalanceFactor(nodeFileName);

        // Left Left Case
        if (balance > 1 && key.isGreaterThan(node.leftFile) == 2)
            return rightRotate(nodeFileName);

        // Right Right Case
        if (balance < -1 && key.isGreaterThan(node.rightFile) == 1)
            return leftRotate(nodeFileName);

        // Left Right Case
        if (balance > 1 && key.isGreaterThan(node.leftFile) == 1) {
            node.leftFile = leftRotate(node.leftFile.getData()).string();
            return rightRotate(nodeFileName);
        }

        // Right Left Case
        if (balance < -1 && key.isGreaterThan(node.rightFile) == 2) {
            node.rightFile = rightRotate(node.rightFile.getData()).string();
            return leftRotate(nodeFileName);
        }

        writeNodeToFile(node, nodeFileName);
        return nodeFileName;
    }

    void updateHeight(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;
        Node node = readNodeFromFile(nodeFileName);
        node.balanceFactor = 1 + max(getHeight(node.leftFile.getData()), getHeight(node.rightFile.getData()));
        writeNodeToFile(node, nodeFileName);
    }

    void updateBalanceFactor(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;
        Node node = readNodeFromFile(nodeFileName);
        node.balanceFactor = getHeight(node.leftFile.getData()) - getHeight(node.rightFile.getData());
        writeNodeToFile(node, nodeFileName);
    }


    fs::path rightRotate(const fs::path& yFileName) {
        Node y = readNodeFromFile(yFileName);
        Node x = readNodeFromFile(y.leftFile.getData());
        String T2File = x.rightFile;

        // Perform rotation
        x.rightFile = yFileName.string();
        y.leftFile = T2File;

        // Update balance factors
        updateHeight(yFileName);
        updateHeight(x.leftFile.getData());

        // Write updated nodes back to files
        writeNodeToFile(y, yFileName);
        writeNodeToFile(x, x.leftFile.getData());

        return x.leftFile.getData(); // New root after rotation
    }

    fs::path leftRotate(const fs::path& xFileName) {
        Node x = readNodeFromFile(xFileName);
        Node y = readNodeFromFile(x.rightFile.getData());
        String T2File = y.leftFile;

        // Perform rotation
        y.leftFile = xFileName.string();
        x.rightFile = T2File;

        // Update balance factors
        updateHeight(xFileName);
        updateHeight(y.leftFile.getData());

        // Write updated nodes back to files
        writeNodeToFile(x, xFileName);
        writeNodeToFile(y, y.leftFile.getData());

        return y.leftFile.getData(); // New root after rotation
    }

    int getHeight(const fs::path& nodeFileName)
    {
        if (nodeFileName.empty()) return 0;
        Node node = readNodeFromFile(nodeFileName);
       
        if (node.key.empty()) return 0;
        // Base case: if node doesn't exist
       
        return 1 + max(getHeight(node.leftFile.getData()), getHeight(node.rightFile.getData()));
    }
   

    int getBalanceFactor(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return 0;
        Node node = readNodeFromFile(nodeFileName);
        return getHeight(node.leftFile.getData()) - getHeight(node.rightFile.getData());
    }

    //void inOrderTraversal(const String& nodeFileName) {
    //    if (nodeFileName.empty()) return;
    //    Node node = readNodeFromFile(nodeFileName.getData());
    //    inOrderTraversal(node.leftFile);
    //    cout << node.key << " ";
    //    inOrderTraversal(node.rightFile);
    //}

    //void inOrder() {
    //    inOrderTraversal(rootFileName.string());
    //}
};

