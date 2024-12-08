#pragma once
#include "String.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

// Custom to_string function for String class
inline String custom_to_string(int value);

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
        char digit = (value % 10) + '0'; // Get the last digit as a character
        result = result + digit;//result += digit;                // Append the digit to the result
        value = value / 10; ; // value =/ 10                // Remove the last digit
    } while (value > 0);

    // Add negative sign if necessary
    if (isNegative) {
        result = result + '-';// result += '-';
    }

    // Reverse the string since digits were added in reverse order
    int len = result.size();
    for (int i = 0; i < len / 2; ++i) {
        char temp = result[i];
        result[i] = result[len - i - 1];
        result[len - i - 1] = temp;
    }

    std::cout << "custom_to_string result: " << result.getData() << std::endl;
    return result;
}

// Red-Black Tree node stored in a file
struct RBNode {
    String key;
    String color;      // "RED" or "BLACK"
    String leftFile;
    String rightFile;
    String parentFile;
    String dataRow;

    RBNode(const String& k = "", const String& c = "RED",
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
        std::cout << "Created branch: " << directoryPath << std::endl;
        if (!fs::exists(directoryPath)) {
            fs::create_directory(directoryPath);
        }
    }
  

    fs::path generateFileName() {
        String fileName = std::to_string(nodeCount++) + ".txt";
        std::cout << "Generated file name: " << fileName << std::endl; // Debugging line
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
        line.push_back('\0');
    }

    // Read RBNode from file
    RBNode readNodeFromFile(const fs::path& filePath) {
        //std::cout << "Reading node from file: " << filePath << std::endl; // Debugging line
        std::ifstream nodeFile(filePath);
        if (!nodeFile.is_open()) {
            std::cerr << "Failed to open file for reading: " << filePath << std::endl; // Debugging line
            // Return a default node indicating it doesn't exist
            return RBNode();
        }
     
        String key, color, leftFile, rightFile, parentFile, dataRow;

        customGetline(nodeFile, key);
        customGetline(nodeFile, color);
        customGetline(nodeFile, leftFile);
        customGetline(nodeFile, rightFile);
        customGetline(nodeFile, parentFile);
        customGetline(nodeFile, dataRow);
        dataRow += "\n";

        nodeFile.close();

        return RBNode(key, color, leftFile, rightFile, parentFile, dataRow);
    }

    // Write RBNode to file
    void writeNodeToFile(const RBNode& node, const fs::path& filePath) {
       // std::cout << "Writing node to file: " << filePath << std::endl; // Debugging line
        std::ofstream nodeFile(filePath);
        if (!nodeFile.is_open()) {
            std::cerr << "Failed to open file for writing: " << filePath << std::endl; // Debugging line
            return;
        }
        nodeFile << node.key << '\n';
        nodeFile << node.color << '\n';
        nodeFile << node.leftFile << '\n';
        nodeFile << node.rightFile << '\n';
        nodeFile << node.parentFile << '\n';
        nodeFile << node.dataRow;
        nodeFile.close();
    }

    // Update parent link in child RBNode
    void updateParentInChild(const String& childFileName, const String& parentFileName) {
        if (childFileName.empty()) return;
        RBNode childNode = readNodeFromFile(childFileName.getData());
        childNode.parentFile = parentFileName;
        writeNodeToFile(childNode, childFileName.getData());
    }
   


    // Left rotation
    fs::path leftRotate(const fs::path& nodeFileName) {
        RBNode node = readNodeFromFile(nodeFileName);  // Get the node to be rotated
        fs::path rightFileName = node.rightFile.getData();  // Get the right child of the node
        if (rightFileName.empty()) {
            return nodeFileName;  // Cannot rotate without a right child
        }
        RBNode rightNode = readNodeFromFile(rightFileName);  // Get the right node

        // Perform the rotation:
        node.rightFile = rightNode.leftFile;  // Set the current node's right child to the left child of the right node
        rightNode.leftFile = nodeFileName.string();  // Set the right node's left child to the current node

        // Update the parent pointers:
        rightNode.parentFile = node.parentFile;  // Right node's parent becomes the current node's parent
        node.parentFile = rightFileName.string();  // Current node's parent becomes the right node's file

        // If the current node's right child exists, update its parent:
        if (!node.rightFile.empty()) {
            updateParentInChild(node.rightFile, nodeFileName.string());
        }

        // Update the parent node's child pointer (if it exists):
        if (!rightNode.parentFile.empty()) {
            RBNode parentNode = readNodeFromFile(rightNode.parentFile.getData());
            if (parentNode.leftFile == nodeFileName.string()) {
                parentNode.leftFile = rightFileName.string();
            }
            else if (parentNode.rightFile == nodeFileName.string()) {
                parentNode.rightFile = rightFileName.string();
            }
            writeNodeToFile(parentNode, rightNode.parentFile.getData());
        }
        else {
            // If the node was the root, update rootFileName
            rootFileName = rightFileName;
        }

        // Write the updated nodes back to the files:
        writeNodeToFile(node, nodeFileName);  // Write the updated current node
        writeNodeToFile(rightNode, rightFileName);  // Write the updated right node

        return rightFileName;  // Return the new root of the subtree
    }



    // Right rotation
    fs::path rightRotate(const fs::path& nodeFileName) {
        RBNode node = readNodeFromFile(nodeFileName);  // Get the node to be rotated
        fs::path leftFileName = node.leftFile.getData();  // Get the left child of the node
        if (leftFileName.empty()) {
            return nodeFileName;  // Cannot rotate without a left child
        }
        RBNode leftNode = readNodeFromFile(leftFileName);  // Get the left node

        // Perform the rotation:
        node.leftFile = leftNode.rightFile;  // Set the current node's left child to the right child of the left node
        leftNode.rightFile = nodeFileName.string();  // Set the left node's right child to the current node

        // Update the parent pointers:
        leftNode.parentFile = node.parentFile;  // Left node's parent becomes the current node's parent
        node.parentFile = leftFileName.string();  // Current node's parent becomes the left node's file

        // If the current node's left child exists, update its parent:
        if (!node.leftFile.empty()) {
            updateParentInChild(node.leftFile, nodeFileName.string());
        }

        // Update the parent node's child pointer (if it exists):
        if (!leftNode.parentFile.empty()) {
            RBNode parentNode = readNodeFromFile(leftNode.parentFile.getData());
            if (parentNode.leftFile == nodeFileName.string()) {
                parentNode.leftFile = leftFileName.string();
            }
            else if (parentNode.rightFile == nodeFileName.string()) {
                parentNode.rightFile = leftFileName.string();
            }
            writeNodeToFile(parentNode, leftNode.parentFile.getData());
        }
        else {
            // If the node was the root, update rootFileName
            rootFileName = leftFileName;
        }

        // Write the updated nodes back to the files:
        writeNodeToFile(node, nodeFileName);  // Write the updated current node
        writeNodeToFile(leftNode, leftFileName);  // Write the updated left node

        return leftFileName;  // Return the new root of the subtree
    }


    // Fix Red-Black Tree properties after insertion
    void fixInsert(fs::path& nodeFileName) {
        RBNode node = readNodeFromFile(nodeFileName);

        while (node.parentFile.getData() != "") {
            RBNode parentNode = readNodeFromFile(node.parentFile.getData());

            // If the parent is black, we’re done (no violation)
            if (parentNode.color == "BLACK") {
                break;
            }

            // Get the grandparent node
            RBNode grandparentNode = readNodeFromFile(parentNode.parentFile.getData());
            String uncleFileName;
            RBNode uncleNode;

            // Determine the uncle's file name
            if (parentNode.parentFile == grandparentNode.leftFile) {
                uncleFileName = grandparentNode.rightFile;
            }
            else {
                uncleFileName = grandparentNode.leftFile;
            }

            // If the uncle exists, get the uncle node
            if (!uncleFileName.empty()) {
                uncleNode = readNodeFromFile(uncleFileName.getData());
            }
            else {
                uncleNode.color = "BLACK"; // Treat null nodes as black
            }

            // Case 1: Uncle is red, perform recoloring
            if (uncleNode.color == "RED") {
                parentNode.color = "BLACK";
                uncleNode.color = "BLACK";
                grandparentNode.color = "RED";

                // Write the updated nodes
                writeNodeToFile(parentNode, node.parentFile.getData());
                writeNodeToFile(uncleNode, uncleFileName.getData());
                writeNodeToFile(grandparentNode, parentNode.parentFile.getData());

                nodeFileName = parentNode.parentFile.getData();
                node = readNodeFromFile(nodeFileName);  // Move the pointer up to the grandparent node
            }
            else {
                // Case 2: Uncle is black or null, perform rotation
                if (node.parentFile == grandparentNode.leftFile) {
                    if (parentNode.rightFile == nodeFileName.string()) {
                        // Perform a left rotation on parent
                        leftRotate(node.parentFile.getData());
                        nodeFileName = node.parentFile.getData();
                        node = readNodeFromFile(nodeFileName);  // Move node to the parent
                        parentNode = readNodeFromFile(node.parentFile.getData());
                    }
                    parentNode.color = "BLACK";
                    grandparentNode.color = "RED";
                    writeNodeToFile(parentNode, node.parentFile.getData());
                    writeNodeToFile(grandparentNode, parentNode.parentFile.getData());
                    rightRotate(parentNode.parentFile.getData());
                }
                else {
                    if (parentNode.leftFile == nodeFileName.string()) {
                        // Perform a right rotation on parent
                        rightRotate(node.parentFile.getData());
                        nodeFileName = node.parentFile.getData();
                        node = readNodeFromFile(nodeFileName);  // Move node to the parent
                        parentNode = readNodeFromFile(node.parentFile.getData());
                    }
                    parentNode.color = "BLACK";
                    grandparentNode.color = "RED";
                    writeNodeToFile(parentNode, node.parentFile.getData());
                    writeNodeToFile(grandparentNode, parentNode.parentFile.getData());
                    leftRotate(parentNode.parentFile.getData());
                }
                break;  // Exit after fixing
            }
        }

        // Ensure the root node is always black
        RBNode rootNode = readNodeFromFile(rootFileName);
        rootNode.color = "BLACK";
        writeNodeToFile(rootNode, rootFileName);
    }


  


    // Insert helper function
    //fs::path insertHelper(const fs::path& nodeFileName, const String& key, const String& dataRow, const String& parentFileName) {
    //    cout << nodeFileName.empty() << endl;
    //    if (nodeFileName.empty()) {
    //        fs::path newNodeFile = generateFileName();
    //        RBNode newNode(key, "RED", "", "", parentFileName, dataRow);
    //        writeNodeToFile(newNode, newNodeFile);
    //        return newNodeFile;
    //    }


    //    RBNode node = readNodeFromFile(nodeFileName);

    //    if (key.isGreaterThan(node.key) == 2) {  // Key is less
    //        fs::path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow, nodeFileName.string());
    //        node.leftFile = updatedLeftChild.string();
    //        writeNodeToFile(node, nodeFileName);

    //        // Update parent link in left child
    //        updateParentInChild(node.leftFile, nodeFileName.string());
    //    }
    //    else {
    //        fs::path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow, nodeFileName.string());
    //        node.rightFile = updatedRightChild.string();
    //        writeNodeToFile(node, nodeFileName);

    //        // Update parent link in right child
    //        updateParentInChild(node.rightFile, nodeFileName.string());
    //    }

    //    return nodeFileName;
    //}
    fs::path insertHelper(const fs::path& nodeFileName, const String& key, const String& dataRow, const String& parentFileName) {
        if (nodeFileName.empty()) {
            fs::path newNodeFile = generateFileName();
            RBNode newNode(key, "RED", "", "", parentFileName, dataRow);
            writeNodeToFile(newNode, newNodeFile);
            return newNodeFile;
        }

        RBNode node = readNodeFromFile(nodeFileName);
        std::cout << "Inserting into node: " << nodeFileName << ", Key: " << key << std::endl;

        if (key.isGreaterThan(node.key) == 2) {  // Key is less
            fs::path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow, nodeFileName.string());
            node.leftFile = updatedLeftChild.string();
            writeNodeToFile(node, nodeFileName);
        }
        else {
            fs::path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow, nodeFileName.string());
            node.rightFile = updatedRightChild.string();
            writeNodeToFile(node, nodeFileName);
        }

        return nodeFileName;
    }



public:
    RBTree() : rootFileName(""), nodeCount(0) {}

    void initialize(const String& dirName) {
        createDirectory(dirName);
    }

    void insert(const String& key, const String& dataRow) {
        std::cout << "Inserting key: " << key << ", dataRow: " << dataRow << std::endl; // Debugging line
        rootFileName = insertHelper(rootFileName, key, dataRow, "");
        fixInsert(rootFileName);
    }

    void inOrderTraversal(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;

        RBNode node = readNodeFromFile(nodeFileName);
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

