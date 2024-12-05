#pragma once
#include <iostream>
#include <iomanip>
#include <queue>
#include "string2.h"

using namespace std;

// An AVL tree node
template<class T>
struct Node
{
    T key;
    Node<T>* left;
    Node<T>* right;
    int height;

    Node(T k)
    {
        key = k;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};
template<class T>
class AVL
{

private:
    Node<T>* root;
public:

	AVL()
	{
		root = nullptr;
	}

	Node<T>* getRoot()
	{
		return root;
	}

    // A utility function to get the height 
    // of the tree
    int height(Node<T>* N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    // A utility function to right rotate 
    // subtree rooted with y
    Node<T>* rightRotate(Node<T>* y)
    {
        Node<T>* x = y->left;
        Node<T>* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + max(height(y->left),
            height(y->right));
        x->height = 1 + (height(x->left),
            height(x->right));

        // Return new root
        return x;
    }

    // A utility function to left rotate 
    // subtree rooted with x
    Node<T>* leftRotate(Node<T>* x)
    {
        Node<T>* y = x->right;
        Node<T>* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + max(height(x->left),
            height(x->right));
        y->height = 1 + max(height(y->left),
            height(y->right));

        // Return new root
        return y;
    }

    // Get Balance factor of node N
    int getBalance(Node<T>* N)
    {
        if (N == nullptr)
            return 0;
        return height(N->left) -
            height(N->right);
    }

    Node<T>* insertHelper(Node<T>* node, int key)
    {
        // 1. Perform the normal BST rotation
        if (node == nullptr)
            return new Node<T>(key);

        if (key < node->key)
            node->left = insertHelper(node->left, key);
        else if (key > node->key)
            node->right = insertHelper(node->right, key);
        else // Equal keys not allowed
            return node;

        // 2. Update height of this ancestor node
        node->height = 1 + max(height(node->left),
            height(node->right));

        // 3. Get the balance factor of this 
        // ancestor node to check whether this 
        // node became unbalanced
        int balance = getBalance(node);

        // If this node becomes unbalanced, then 
        // there are 4 cases

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // return the (unchanged) node pointer
        return node;
    }

    // Given a non-empty binary search tree, 
    // return the node with minimum key value 
    // found in that tree. Note that the entire 
    // tree does not need to be searched.
    Node<T>* minValueNode(Node<T>* node)
    {
        Node<T>* current = node;

        // loop down to find the leftmost leaf
        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    // Recursive function to delete a node with 
    // given key from subtree with given root. 
    // It returns root of the modified subtree.
    Node<T>* deleteNodeHelper(Node<T>* root, int key)
    {
        // STEP 1: PERFORM STANDARD BST DELETE
        if (root == nullptr)
            return root;

        // If the key to be deleted is smaller 
        // than the root's key, then it lies in 
        // left subtree
        if (key < root->key)
            root->left = deleteNodeHelper(root->left, key);

        // If the key to be deleted is greater 
        // than the root's key, then it lies in 
        // right subtree
        else if (key > root->key)
            root->right = deleteNodeHelper(root->right, key);

        // if key is same as root's key, then 
        // this is the node to be deleted
        else
        {
            // node with only one child or no child
            if ((root->left == nullptr) ||
                (root->right == nullptr))
            {
                Node<T>* temp = root->left ?
                    root->left : root->right;

                // No child case
                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else // One child case
                    *root = *temp; // Copy the contents of 
                // the non-empty child
                free(temp);
            }
            else
            {
                // node with two children: Get the 
                // inorder successor (smallest in 
                // the right subtree)
                Node<T>* temp = minValueNode(root->right);

                // Copy the inorder successor's 
                // data to this node
                root->key = temp->key;

                // Delete the inorder successor
                root->right = deleteNodeHelper(root->right, temp->key);
            }
        }

        // If the tree had only one node then return
        if (root == nullptr)
            return root;

        // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
        root->height = 1 + max(height(root->left),
            height(root->right));

        // STEP 3: GET THE BALANCE FACTOR OF THIS 
        // NODE (to check whether this node 
        // became unbalanced)
        int balance = getBalance(root);

        // If this node becomes unbalanced, then 
        // there are 4 cases

        // Left Left Case
        if (balance > 1 &&
            getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // A utility function to print preorder 
    // traversal of the tree. 
    void InOrder(Node<T>* root)
    {
        if (root != nullptr)
        {
            InOrder(root->left);
            cout << root->key << " ";
            InOrder(root->right);
        }
    }

    //void LevelOrderTraversal()
    //{
    //    if (!root) {
    //        cout << "Tree is empty." << endl;
    //        return;
    //    }

    //    int height = root->height;
    //    int maxWidth = pow(2, height) - 1; // Maximum width of the last level
    //    queue<Node<T>*> q; // Explicitly specify the type of the queue
    //    q.push(root);

    //    for (int level = 0; level < height; ++level) {
    //        int levelNodes = pow(2, level);
    //        int spaceBetween = maxWidth / levelNodes;

    //        // Print the level nodes
    //        for (int i = 0; i < levelNodes; ++i) {
    //            Node<T>* current = q.front();
    //            q.pop();

    //            if (i == 0) // Print leading spaces for the first node
    //                cout << string((spaceBetween - 1) / 2, ' ');

    //            if (current) {
    //                cout << setw(5) << current->key;
    //                q.push(current->left);
    //                q.push(current->right);
    //            }
    //            else {
    //                cout << "  ";
    //                q.push(nullptr); // Placeholder for left and right of null
    //                q.push(nullptr);
    //            }

    //            if (i != levelNodes - 1) // Print spaces between nodes
    //                cout << string(spaceBetween, ' ');
    //        }
    //        cout << endl;
    //        maxWidth /= 2; // Halve the max width for the next level
    //    }
    //}


    void insert(int key)
    {
        root = insertHelper(root, key); // Update root
    }

    void deleteNode(int key)
    {
        root = deleteNodeHelper(root, key); // Update root
    }

    void InOrderTraversal()
    {
        InOrder(root);
    }
};