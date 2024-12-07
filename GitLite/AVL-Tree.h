#pragma once
#include "string2.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

// Custom to_string function for String class
String custom_to_string(int value) {
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
    // cout << "CUSTOM TO STRING " << result << endl; // Debugging line

    return result;
}

// An AVL tree node with parentFile added
struct Node {
    String key;
    int isLeaf;
    int height;
    int balanceFactor;
    String leftFile;
    String rightFile;
    String parentFile; // New member for parent file path
    String dataRow;    // Store the data row in a single String

    Node(const String& k = "", int leaf = 1, int hgt = 0, int balance = 0,
        const String& left = "", const String& right = "", const String& parent = "",
        const String& data = "")
        : key(k), isLeaf(leaf), height(hgt), balanceFactor(balance),
        leftFile(left), rightFile(right), parentFile(parent), dataRow(data) {
    }
};

class AVL {
private:
    fs::path rootFileName;
    int nodeCount;
    fs::path directoryPath;

    void createDirectory(const String& dirName) {
        directoryPath = fs::path(dirName.getData());
        if (!fs::exists(directoryPath)) {
            fs::create_directory(directoryPath);
        }
    }

    fs::path generateFileName() {
        String fileName = custom_to_string(nodeCount++) + ".txt";
        return (directoryPath / fileName.getData());
    }

public:
    AVL() : rootFileName(""), nodeCount(0) {}

    void initialize(const String& dirName) {
        createDirectory(dirName);
    }

    // Custom getline function
    void customGetline(std::ifstream& file, String& line, char delimiter = '\n') {
        line.clear();
        char ch;
        while (file.get(ch)) {
            if (ch == delimiter) {
                break;
            }
            line += ch;
        }
        // No need to add null terminator; String class handles it
    }

    // Read a node from file including parentFile
    Node readNodeFromFile(const fs::path& filePath) {
        std::ifstream nodeFile(filePath);
        if (!nodeFile.is_open()) {
            // Return a node indicating that it doesn't exist
            return Node();
        }

        String key, leftFile, rightFile, parentFile, dataRow;
        int isLeaf, height, balanceFactor;

        customGetline(nodeFile, key);
        nodeFile >> isLeaf;
        nodeFile >> height;
        nodeFile >> balanceFactor;
        nodeFile.ignore(); // Ignore the newline after balanceFactor
        customGetline(nodeFile, leftFile);
        customGetline(nodeFile, rightFile);
        customGetline(nodeFile, parentFile);
        customGetline(nodeFile, dataRow); // Read the data row as a single string

        nodeFile.close();

        return Node(key, isLeaf, height, balanceFactor, leftFile, rightFile, parentFile, dataRow);
    }

    // Write a node to file including parentFile
    void writeNodeToFile(const Node& node, const fs::path& filePath) {
        std::ofstream nodeFile(filePath);
        nodeFile << node.key << '\n';
        nodeFile << node.isLeaf << '\n';
        nodeFile << node.height << '\n';
        nodeFile << node.balanceFactor << '\n';
        nodeFile << node.leftFile << '\n';
        nodeFile << node.rightFile << '\n';
        nodeFile << node.parentFile << '\n';
        nodeFile << node.dataRow;
        nodeFile.close();
    }

    // Update parent links in child nodes
    void updateParentInChild(const String& childFileName, const String& parentFileName) {
        if (childFileName.empty()) return;
        Node childNode = readNodeFromFile(childFileName.getData());
        childNode.parentFile = parentFileName;
        writeNodeToFile(childNode, childFileName.getData());
    }

    void insert(const String& key, const String& dataRow) {
        rootFileName = insertHelper(rootFileName, key, dataRow, "");
    }

    fs::path insertHelper(const fs::path& nodeFileName, const String& key, const String& dataRow, const String& parentFileName) {
        if (nodeFileName.empty()) {
            // Create new node with height 0
            fs::path newNodeFile = generateFileName();
            Node newNode(key, 1, 0, 0, "", "", parentFileName, dataRow);
            writeNodeToFile(newNode, newNodeFile);
            return newNodeFile;
        }

        Node node = readNodeFromFile(nodeFileName);

        if (key.isGreaterThan(node.key) == 2) {  // Less
            fs::path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow, nodeFileName.string());
            node.leftFile = updatedLeftChild.string();

            // Update parent link in left child
            updateParentInChild(node.leftFile, nodeFileName.string());
        }
        else {
            // Insert duplicates to the right subtree (greater or equal cases)
            fs::path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow, nodeFileName.string());
            node.rightFile = updatedRightChild.string();

            // Update parent link in right child
            updateParentInChild(node.rightFile, nodeFileName.string());
        }

        node.isLeaf = (node.leftFile.empty() && node.rightFile.empty()) ? 1 : 0;

        // Write the updated node back to file
        writeNodeToFile(node, nodeFileName);

        // Update balance factor
        updateBalanceFactor(nodeFileName);

        // Read the node again to get updated balance factor
        node = readNodeFromFile(nodeFileName);
        int balance = node.balanceFactor;

        // Perform rotations if necessary
        // Left Left Case
        if (balance > 1 && key.isGreaterThan(readNodeFromFile(node.leftFile.getData()).key) == 2) {
            return rightRotate(nodeFileName);
        }

        // Right Right Case
        if (balance < -1 && key.isGreaterThan(readNodeFromFile(node.rightFile.getData()).key) != 2) {
            return leftRotate(nodeFileName);
        }

        // Left Right Case
        if (balance > 1 && key.isGreaterThan(readNodeFromFile(node.leftFile.getData()).key) != 2) {
            node.leftFile = leftRotate(node.leftFile.getData()).string();
            writeNodeToFile(node, nodeFileName); // Update node after rotation

            // Update parent link in left child
            updateParentInChild(node.leftFile, nodeFileName.string());

            return rightRotate(nodeFileName);
        }

        // Right Left Case
        if (balance < -1 && key.isGreaterThan(readNodeFromFile(node.rightFile.getData()).key) == 2) {
            node.rightFile = rightRotate(node.rightFile.getData()).string();
            writeNodeToFile(node, nodeFileName); // Update node after rotation

            // Update parent link in right child
            updateParentInChild(node.rightFile, nodeFileName.string());

            return leftRotate(nodeFileName);
        }

        writeNodeToFile(node, nodeFileName);
        return nodeFileName;
    }

    void updateBalanceFactor(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;
        Node node = readNodeFromFile(nodeFileName);

        // Ensure child heights are up-to-date
        int leftHeight = getHeight(node.leftFile.getData());
        int rightHeight = getHeight(node.rightFile.getData());

        // Compute balance factor
        node.balanceFactor = leftHeight - rightHeight;

        // Update node's height
        node.height = 1 + max(leftHeight, rightHeight);

        // Write the updated node back to the file
        writeNodeToFile(node, nodeFileName);
    }

    fs::path rightRotate(const fs::path& yFileName) {
        Node y = readNodeFromFile(yFileName);
        fs::path xFileName = y.leftFile.getData();
        Node x = readNodeFromFile(xFileName);
        fs::path T2FileName = x.rightFile.getData();

        // Perform rotation
        x.rightFile = yFileName.string();
        y.leftFile = T2FileName.string();

        // Update parent pointers
        x.parentFile = y.parentFile;
        y.parentFile = xFileName.string();

        // Update parent link for T2, if it exists
        if (!T2FileName.empty()) {
            updateParentInChild(T2FileName.string(), yFileName.string());
        }

        // Write updated nodes back to files
        writeNodeToFile(y, yFileName);
        writeNodeToFile(x, xFileName);

        // Update heights and balance factors
        updateBalanceFactor(yFileName);
        updateBalanceFactor(xFileName);

        return xFileName; // New root after rotation
    }

    fs::path leftRotate(const fs::path& xFileName) {
        Node x = readNodeFromFile(xFileName);
        fs::path yFileName = x.rightFile.getData();
        Node y = readNodeFromFile(yFileName);
        fs::path T2FileName = y.leftFile.getData();

        // Perform rotation
        y.leftFile = xFileName.string();
        x.rightFile = T2FileName.string();

        // Update parent pointers
        y.parentFile = x.parentFile;
        x.parentFile = yFileName.string();

        // Update parent link for T2, if it exists
        if (!T2FileName.empty()) {
            updateParentInChild(T2FileName.string(), xFileName.string());
        }

        // Write updated nodes back to files
        writeNodeToFile(x, xFileName);
        writeNodeToFile(y, yFileName);

        // Update heights and balance factors
        updateBalanceFactor(xFileName);
        updateBalanceFactor(yFileName);

        return yFileName; // New root after rotation
    }

    int getHeight(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) {
            return -1; // Height of a non-existent node is -1
        }

        Node node = readNodeFromFile(nodeFileName);

        // Return already computed height
        return node.height;
    }

    void inOrderTraversal(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;

        Node node = readNodeFromFile(nodeFileName);
        inOrderTraversal(node.leftFile.getData());
        std::cout << "Key: " << node.key
            << ", Height: " << node.height
            << ", Balance Factor: " << node.balanceFactor
            << ", Parent File: " << node.parentFile << std::endl;
        inOrderTraversal(node.rightFile.getData());
    }

    void inOrder() {
        inOrderTraversal(rootFileName);
    }
};
