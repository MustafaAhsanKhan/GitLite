#pragma once
#include "String.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;


inline String custom_to_string(int value);

// An AVL tree node
struct Node
{
    String key;
    int isLeaf;
    int height;
    int balanceFactor;
    String leftFile;
    String rightFile;
    String dataRow; // Store the data row in a single String

    Node(const String& k, int leaf, int hgt, int balance, const String& left, const String& right, const String& data)
        : key(k), isLeaf(leaf), height(hgt), balanceFactor(balance), leftFile(left), rightFile(right), dataRow(data) {
    }
    ~Node()
    {
        // Explicitly clear the String members to ensure their destructors are called
        key.clear();
        leftFile.clear();
        rightFile.clear();
        dataRow.clear();
    }
};


class AVL {
private:
    fs::path rootFileName;
    int nodeCount;
    fs::path directoryPath;

    void createDirectory(const String& dirName) {
        directoryPath = fs::path(dirName.getData());
        if (!fs::exists(directoryPath))
        {
            std::cout << "Created branch: " << directoryPath << std::endl;
            fs::create_directory(directoryPath);
        }
    }

    fs::path generateFileName(String key)
    {
        String fileName = to_string(nodeCount++) + ".txt";
        return (directoryPath / fileName.getData());
    }

    // Modify insert and delete functions to work with filenames
public:
    AVL() : rootFileName(""), nodeCount(0) {}

    void initialize(fs::path dirPath) {
        directoryPath = dirPath;
    void queryRange(int lowerBound, int upperBound) {
        cout << "\033[33mAVL Tree: Records within range [" << lowerBound << ", " << upperBound << "]:\033[0m" << endl;
        queryRangeHelper(rootFileName, lowerBound, upperBound);
    }

    void queryRangeHelper(const fs::path& nodeFileName, int lowerBound, int upperBound) {
        if (nodeFileName.empty()) return;

        // Read the node from file
        Node node = readNodeFromFile(nodeFileName);

        // Convert the node's key to an integer for comparison
        int key = stoi(node.key.getData());

        // Perform in-order traversal
        if (key > lowerBound) {
            queryRangeHelper(node.leftFile.getData(), lowerBound, upperBound);
        }

        // Print the node if it is within the range
        if (key >= lowerBound && key <= upperBound) {
            cout << "Key: " << node.key << ", Data: " << node.dataRow << endl;
        }

        if (key < upperBound) {
            queryRangeHelper(node.rightFile.getData(), lowerBound, upperBound);
        }
    }



    void initialize(const String& dirName) {
        createDirectory(dirName);
    }

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

    Node readNodeFromFile(const fs::path& filePath) {
        std::ifstream nodeFile(filePath);
        if (!nodeFile.is_open()) {
            // Return a node indicating that it doesn't exist
            return Node("", 0, -1, 0, "", "", "");
        }

        String key, leftFile, rightFile, dataRow;
        int isLeaf, height, balanceFactor;

        customGetline(nodeFile, key);
        nodeFile >> isLeaf;
        nodeFile >> height;
        nodeFile >> balanceFactor;
        nodeFile.ignore(); // Ignore the newline after balanceFactor
        customGetline(nodeFile, leftFile);
        customGetline(nodeFile, rightFile);

        customGetline(nodeFile, dataRow); // Read the data row as a single string
        dataRow += "\n";

        nodeFile.close();

        return Node(key, isLeaf, height, balanceFactor, leftFile, rightFile, dataRow);
    }


    void writeNodeToFile(const Node& node, const fs::path& filePath) {
        std::ofstream nodeFile(filePath);
        nodeFile << node.key << '\n';
        nodeFile << node.isLeaf << '\n';
        nodeFile << node.height << '\n';
        nodeFile << node.balanceFactor << '\n';
        nodeFile << node.leftFile << '\n';
        nodeFile << node.rightFile << '\n';
        nodeFile << node.dataRow; // No extra newline here
        nodeFile.close();
    }


    void insert(const String& key, const String& dataRow) {
        rootFileName = insertHelper(rootFileName, key, dataRow);
    }

    fs::path insertHelper(const fs::path& nodeFileName, const String& key, const String& dataRow) {
        if (nodeFileName.empty()) {
            // Create new node with height 0
            fs::path newNodeFile = generateFileName(key);
            Node newNode(key, 1, 0, 0, "", "", dataRow);
            writeNodeToFile(newNode, newNodeFile);
            return newNodeFile;
        }

        Node node = readNodeFromFile(nodeFileName);

        if (key.isGreaterThan(node.key) == 2) {  // Less
            fs::path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow);
            node.leftFile = updatedLeftChild.string();
        }
        else {
            // Insert duplicates to the right subtree (greater or equal cases)
            fs::path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow);
            node.rightFile = updatedRightChild.string();
        }

        node.isLeaf = (node.leftFile.empty() && node.rightFile.empty()) ? 1 : 0;

        // **Immediately write the updated node back to file**
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
            return rightRotate(nodeFileName);
        }

        // Right Left Case
        if (balance < -1 && key.isGreaterThan(readNodeFromFile(node.rightFile.getData()).key) == 2) {
            node.rightFile = rightRotate(node.rightFile.getData()).string();
            writeNodeToFile(node, nodeFileName); // Update node after rotation
            return leftRotate(nodeFileName);
        }

        writeNodeToFile(node, nodeFileName);
        return nodeFileName;
    }

    fs::path deleteHelper(const fs::path& nodeFileName, const String& key) {
        if (nodeFileName.empty()) {
            // Key not found
            return nodeFileName;
        }

        Node node = readNodeFromFile(nodeFileName);

        // Perform standard BST deletion
        if (key.isGreaterThan(node.key) == 2) { // Key is less
            node.leftFile = deleteHelper(node.leftFile.getData(), key).string();
        }
        else if (key.isGreaterThan(node.key) == 1) { // Key is greater
            node.rightFile = deleteHelper(node.rightFile.getData(), key).string();
        }
        else {
            // Node with only one child or no child
            if (node.leftFile.empty() || node.rightFile.empty()) {
                fs::path tempFileName = node.leftFile.empty() ? node.rightFile.getData() : node.leftFile.getData();

                // Delete current node file
                fs::remove(nodeFileName);

                // If no child
                if (tempFileName.empty()) {
                    // No further action needed
                    return "";
                }
                else {
                    return tempFileName; // Return the non-empty child
                }
            }
            else {
                // Node with two children: Get the inorder successor
                fs::path tempFileName = minValueNode(node.rightFile.getData());
                Node tempNode = readNodeFromFile(tempFileName);

                // Copy the inorder successor's data to this node
                node.key = tempNode.key;
                node.dataRow = tempNode.dataRow;

                // Delete the inorder successor
                node.rightFile = deleteHelper(node.rightFile.getData(), tempNode.key).string();
            }
        }

        // Update `isLeaf` flag
        node.isLeaf = (node.leftFile.empty() && node.rightFile.empty()) ? 1 : 0;

        // Write the updated node back to the file
        writeNodeToFile(node, nodeFileName);

        // Update balance factors and heights
        updateBalanceFactor(nodeFileName);
        node = readNodeFromFile(nodeFileName);
        int balance = node.balanceFactor;

        // Balance the tree
        // Left Left Case
        if (balance > 1 && getBalanceFactor(node.leftFile.getData()) >= 0) {
            return rightRotate(nodeFileName);
        }

        // Left Right Case
        if (balance > 1 && getBalanceFactor(node.leftFile.getData()) < 0) {
            node.leftFile = leftRotate(node.leftFile.getData()).string();
            writeNodeToFile(node, nodeFileName);
            return rightRotate(nodeFileName);
        }

        // Right Right Case
        if (balance < -1 && getBalanceFactor(node.rightFile.getData()) <= 0) {
            return leftRotate(nodeFileName);
        }

        // Right Left Case
        if (balance < -1 && getBalanceFactor(node.rightFile.getData()) > 0) {
            node.rightFile = rightRotate(node.rightFile.getData()).string();
            writeNodeToFile(node, nodeFileName);
            return leftRotate(nodeFileName);
        }

        return nodeFileName;
    }

    void deleteKey(const String& key) {
        rootFileName = deleteHelper(rootFileName, key);
    }

    /*void updateHeight(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;

        Node node = readNodeFromFile(nodeFileName);

        int leftHeight = getHeight(node.leftFile.getData());
        int rightHeight = getHeight(node.rightFile.getData());
        node.height = 1 + max(leftHeight, rightHeight);

        writeNodeToFile(node, nodeFileName);
    }*/

    fs::path minValueNode(const fs::path& nodeFileName) {
        fs::path currentFileName = nodeFileName;
        Node currentNode = readNodeFromFile(currentFileName);

        // Loop down to find the leftmost leaf
        while (!currentNode.leftFile.empty()) {
            currentFileName = currentNode.leftFile.getData();
            currentNode = readNodeFromFile(currentFileName);
        }
        return currentFileName;
    }


    void updateBalanceFactor(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;
        Node node = readNodeFromFile(nodeFileName);

        // Ensure child heights are up-to-date
        int leftHeight = getHeight(node.leftFile.getData());
        int rightHeight = getHeight(node.rightFile.getData());

        // Compute balance factor
        node.balanceFactor = leftHeight - rightHeight;

        // **Write the updated node back to the file**
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

        // Update `isLeaf` flags
        y.isLeaf = (y.leftFile.empty() && y.rightFile.empty()) ? 1 : 0;
        x.isLeaf = (x.leftFile.empty() && x.rightFile.empty()) ? 1 : 0;

        // Write updates back to files
        writeNodeToFile(y, yFileName);
        writeNodeToFile(x, xFileName);

        // Update heights and balance factors recursively
        getHeight(yFileName);
        updateBalanceFactor(yFileName);
        getHeight(xFileName);
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

        // Update `isLeaf` flags
        x.isLeaf = (x.leftFile.empty() && x.rightFile.empty()) ? 1 : 0;
        y.isLeaf = (y.leftFile.empty() && y.rightFile.empty()) ? 1 : 0;

        // Write updated nodes back to files
        writeNodeToFile(x, xFileName);
        writeNodeToFile(y, yFileName);

        // Update heights and balance factors recursively
        getHeight(xFileName);
        updateBalanceFactor(xFileName);
        getHeight(yFileName);
        updateBalanceFactor(yFileName);

        return yFileName; // New root after rotation
    }




    int getHeight(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) {
            return -1; // Height of a non-existent node is -1
        }

        Node node = readNodeFromFile(nodeFileName);

        // Recursively get heights of left and right children
        int leftHeight = getHeight(node.leftFile.getData());
        int rightHeight = getHeight(node.rightFile.getData());

        // Compute current node's height
        node.height = 1 + std::max(leftHeight, rightHeight);

        // Write the updated node back to the file
        writeNodeToFile(node, nodeFileName);

        return node.height;
    }



    int getBalanceFactor(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return 0;
        Node node = readNodeFromFile(nodeFileName);
        return getHeight(node.leftFile.getData()) - getHeight(node.rightFile.getData());
    }

    void inOrderTraversal(const fs::path& nodeFileName) {
        if (nodeFileName.empty()) return;

        Node node = readNodeFromFile(nodeFileName);
        inOrderTraversal(node.leftFile.getData());
        std::cout << "Key: " << node.key << ", Height: " << node.height << ", Balance Factor: " << node.balanceFactor << std::endl;
        inOrderTraversal(node.rightFile.getData());
    }

    void inOrder() {
        inOrderTraversal(rootFileName);
    }


};

