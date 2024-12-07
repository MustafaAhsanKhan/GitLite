#pragma once
#include "string2.h"
#include <filesystem>
#include <fstream>
#include <iostream>


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

    return result;
}

// Red-Black Tree node stored in a file
struct Node {
    String key;
    String color;      // "RED" or "BLACK"
    String leftFile;
    String rightFile;
    String parentFile;
    String dataRow;

    Node(const String& k = "", const String& c = "RED",
        const String& left = "", const String& right = "",
        const String& parent = "", const String& data = "")
        : key(k), color(c), leftFile(left), rightFile(right),
        parentFile(parent), dataRow(data) {
    }
};

class RBTree {
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
    }

    // Read node from file
    Node readNodeFromFile(const fs::path& filePath) {
        std::ifstream nodeFile(filePath);
        if (!nodeFile.is_open()) {
            // Return a default node indicating it doesn't exist
            return Node();
        }

        String key, color, leftFile, rightFile, parentFile, dataRow;

        customGetline(nodeFile, key);
        customGetline(nodeFile, color);
        customGetline(nodeFile, leftFile);
        customGetline(nodeFile, rightFile);
        customGetline(nodeFile, parentFile);
        customGetline(nodeFile, dataRow);

        nodeFile.close();

        return Node(key, color, leftFile, rightFile, parentFile, dataRow);
    }

    // Write node to file
    void writeNodeToFile(const Node& node, const fs::path& filePath) {
        std::ofstream nodeFile(filePath);
        nodeFile << node.key << '\n';
        nodeFile << node.color << '\n';
        nodeFile << node.leftFile << '\n';
        nodeFile << node.rightFile << '\n';
        nodeFile << node.parentFile << '\n';
        nodeFile << node.dataRow;
        nodeFile.close();
    }

    // Update parent link in child node
    void updateParentInChild(const String& childFileName, const String& parentFileName) {
        if (childFileName.empty()) return;
        Node childNode = readNodeFromFile(childFileName.getData());
        childNode.parentFile = parentFileName;
        writeNodeToFile(childNode, childFileName.getData());
    }

    // Left rotate subtree rooted at x
    void leftRotate(const fs::path& xFileName) {
        Node x = readNodeFromFile(xFileName);
        fs::path yFileName = x.rightFile.getData();
        if (yFileName.empty()) return; // Cannot rotate if right child does not exist

        Node y = readNodeFromFile(yFileName);

        // Perform rotation
        x.rightFile = y.leftFile;
        if (!y.leftFile.empty()) {
            Node yLeftChild = readNodeFromFile(y.leftFile.getData());
            yLeftChild.parentFile = xFileName.string();
            writeNodeToFile(yLeftChild, y.leftFile.getData());
        }

        y.parentFile = x.parentFile;
        if (x.parentFile.empty()) {
            rootFileName = yFileName;
        }
        else {
            Node xParent = readNodeFromFile(x.parentFile.getData());
            if (xParent.leftFile.isGreaterThan(xFileName.string()) == 3) { // xParent.leftFile == xFileName.string()
                xParent.leftFile = yFileName.string();
            }
            else {
                xParent.rightFile = yFileName.string();
            }
            writeNodeToFile(xParent, x.parentFile.getData());
        }

        y.leftFile = xFileName.string();
        x.parentFile = yFileName.string();

        writeNodeToFile(x, xFileName);
        writeNodeToFile(y, yFileName);
    }

    // Right rotate subtree rooted at y
    void rightRotate(const fs::path& yFileName) {
        Node y = readNodeFromFile(yFileName);
        fs::path xFileName = y.leftFile.getData();
        if (xFileName.empty()) return; // Cannot rotate if left child does not exist

        Node x = readNodeFromFile(xFileName);

        // Perform rotation
        y.leftFile = x.rightFile;
        if (!x.rightFile.empty()) {
            Node xRightChild = readNodeFromFile(x.rightFile.getData());
            xRightChild.parentFile = yFileName.string();
            writeNodeToFile(xRightChild, x.rightFile.getData());
        }

        x.parentFile = y.parentFile;
        if (y.parentFile.empty()) {
            rootFileName = xFileName;
        }
        else {
            Node yParent = readNodeFromFile(y.parentFile.getData());
            if (yParent.leftFile.isGreaterThan(yFileName.string()) == 3) { // yParent.leftFile == yFileName.string()
                yParent.leftFile = xFileName.string();
            }
            else {
                yParent.rightFile = xFileName.string();
            }
            writeNodeToFile(yParent, y.parentFile.getData());
        }

        x.rightFile = yFileName.string();
        y.parentFile = xFileName.string();

        writeNodeToFile(y, yFileName);
        writeNodeToFile(x, xFileName);
    }

    // Fix the Red-Black Tree after insertion
    void fixInsert(const fs::path& nodeFileName) {
        fs::path currentFileName = nodeFileName;

        while (true) {
            Node currentNode = readNodeFromFile(currentFileName);
            if (currentNode.parentFile.empty()) {
                currentNode.color = "BLACK";
                writeNodeToFile(currentNode, currentFileName);
                break;
            }

            Node parentNode = readNodeFromFile(currentNode.parentFile.getData());
            if (parentNode.color == "BLACK") {
                break;
            }

            Node grandparentNode = readNodeFromFile(parentNode.parentFile.getData());
            String uncleFileName;
            Node uncleNode;

            if (currentNode.parentFile == grandparentNode.leftFile) {
                uncleFileName = grandparentNode.rightFile;
            }
            else {
                uncleFileName = grandparentNode.leftFile;
            }

            if (!uncleFileName.empty()) {
                uncleNode = readNodeFromFile(uncleFileName.getData());
            }
            else {
                uncleNode.color = "BLACK"; // Treat null nodes as black
            }

            if (uncleNode.color == "RED") {
                parentNode.color = "BLACK";
                uncleNode.color = "BLACK";
                grandparentNode.color = "RED";

                writeNodeToFile(parentNode, currentNode.parentFile.getData());
                writeNodeToFile(uncleNode, uncleFileName.getData());
                writeNodeToFile(grandparentNode, parentNode.parentFile.getData());

                currentFileName = parentNode.parentFile.getData();
            }
            else {
                if (currentNode.parentFile == grandparentNode.leftFile) {
                    if (parentNode.rightFile.isGreaterThan(currentFileName.string()) == 3) { 
                        leftRotate(currentNode.parentFile.getData());
                        currentFileName = currentNode.parentFile.getData();
                        currentNode = readNodeFromFile(currentFileName);
                        parentNode = readNodeFromFile(currentNode.parentFile.getData());
                    }
                    parentNode.color = "BLACK";
                    grandparentNode.color = "RED";
                    writeNodeToFile(parentNode, currentNode.parentFile.getData());
                    writeNodeToFile(grandparentNode, parentNode.parentFile.getData());
                    rightRotate(parentNode.parentFile.getData());
                }
                else {
                    if (parentNode.leftFile.isGreaterThan(currentFileName.string()) == 3) {
                        rightRotate(currentNode.parentFile.getData());
                        currentFileName = currentNode.parentFile.getData();
                        currentNode = readNodeFromFile(currentFileName);
                        parentNode = readNodeFromFile(currentNode.parentFile.getData());
                    }
                    parentNode.color = "BLACK";
                    grandparentNode.color = "RED";
                    writeNodeToFile(parentNode, currentNode.parentFile.getData());
                    writeNodeToFile(grandparentNode, parentNode.parentFile.getData());
                    leftRotate(parentNode.parentFile.getData());
                }
                break;
            }
        }
    }

    // Insert helper function
    fs::path insertHelper(const fs::path& nodeFileName, const String& key, const String& dataRow, const String& parentFileName) {
        if (nodeFileName.empty()) {
            fs::path newNodeFile = generateFileName();
            Node newNode(key, "RED", "", "", parentFileName, dataRow);
            writeNodeToFile(newNode, newNodeFile);
            return newNodeFile;
        }

        Node node = readNodeFromFile(nodeFileName);

        if (key.isGreaterThan(node.key) == 2) {  // Less
            fs::path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow, nodeFileName.string());
            node.leftFile = updatedLeftChild.string();
            writeNodeToFile(node, nodeFileName);

            // Update parent link in left child
            updateParentInChild(node.leftFile, nodeFileName.string());
        }
        else {
            fs::path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow, nodeFileName.string());
            node.rightFile = updatedRightChild.string();
            writeNodeToFile(node, nodeFileName);

            // Update parent link in right child
            updateParentInChild(node.rightFile, nodeFileName.string());
        }

        return nodeFileName;
    }

public:
    RBTree() : rootFileName(""), nodeCount(0) {}

    void initialize(const String& dirName) {
        createDirectory(dirName);
    }

    void insert(const String& key, const String& dataRow) {
        fs::path insertedNodeFile = insertHelper(rootFileName, key, dataRow, "");
        if (rootFileName.empty()) {
            rootFileName = insertedNodeFile;
        }
        fixInsert(insertedNodeFile);
    }

    void inOrderTraversal(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;

        Node node = readNodeFromFile(nodeFileName);
        inOrderTraversal(node.leftFile.getData());
        std::cout << "Key: " << node.key
            << ", Color: " << node.color
            << ", Parent: " << node.parentFile << std::endl;
        inOrderTraversal(node.rightFile.getData());
    }

    void inOrder() {
        inOrderTraversal(rootFileName);
    }
};
