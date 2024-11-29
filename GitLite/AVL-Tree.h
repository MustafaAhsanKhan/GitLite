#pragma once

#include <iostream>
using namespace std;
struct Node
{
	int data;
	Node* left;
	Node* right;
	int height;

};

class Tree
{
public:
	Node* root;
	Tree()
	{
		root = nullptr;
	}
	int getHeight(Node* node) {
		if (node == NULL) {
			return -1;
		}
		return node->height;
	}
	Node* rotateLeft(Node* node) {
		Node* top = node->right;
		Node* temp = top->left;
		top->left = node;
		node->right = temp;
		node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		top->height = 1 + max(getHeight(top->left), getHeight(top->right));
		return top;
	}
	Node* rotateRight(Node* node) {
		Node* top = node->left;
		Node* temp = top->right;
		top->right = node;
		node->left = temp;
		node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		top->height = 1 + max(getHeight(top->left), getHeight(top->right));
		return top;
	}
	Node* rotateRightLeft(Node* node) {
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}
	Node* rotateLeftRight(Node* node) {
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	Node* ins(Node* node, int data) {
		if (node == NULL) {
			node = new Node();
			node->data = data;
		}
		else if (data < node->data) {
			node->left = ins(node->left, data);
			if (abs(getHeight(node->left) - getHeight(node->right)) == 2) {
				if (data < node->left->data) {
					node = rotateRight(node);
				}
				else {
					node = rotateLeftRight(node);
				}
			}
		}
		else {
			node->right = ins(node->right, data);
			if (abs(getHeight(node->left) - getHeight(node->right)) == 2) {
				if (data > node->right->data) {
					node = rotateLeft(node);
				}
				else {
					node = rotateRightLeft(node);
				}
			}
		}
		node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		return node;
	}

	void insert(int data) {
		root = ins(root, data);
	}
	int getBalance(Node* N) {
		if (N == nullptr)
			return 0;
		return getHeight(N->left) - getHeight(N->right);
	}
	Node* minValueNode(Node* node) {
		Node* current = node;
		while (current->left)
		{
			current = current->left;
		}
		return current;

	}

	Node* deleteNode(Node* node, int data) {
		if (node == nullptr)
			return node;
		if (data < node->data)
			node->left = deleteNode(node->left, data);
		else if (data > node->data)
			node->right = deleteNode(node->right, data);

		else {
			if ((node->left == nullptr) || (node->right == nullptr)) {
				Node* temp = node->left ? node->left : node->right;

				if (temp == nullptr) {
					temp = node;
					node = nullptr;
				}
				else *node = *temp;
				delete temp;
			}
			else {
				Node* temp = minValueNode(node->right);
				node->data = temp->data;
				node->right = deleteNode(node->right, temp->data);
			}
		}
		if (node == nullptr)
			return node;
		node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		int balance = getBalance(node);
		if (balance > 1 && getBalance(node->left) >= 0)
			return rotateRight(node);
		if (balance > 1 && getBalance(node->left) < 0) {
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		if (balance < -1 && getBalance(node->right) <= 0)
			return rotateLeft(node);
		if (balance < -1 && getBalance(node->right) > 0) {
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}

		return node;
	}


	void del(int data) {
		root = deleteNode(root, data);
	}

	bool isAVL(Node* node) {
		if (!node) return true;
		if (abs(getHeight(node->left) - getHeight(node->right)) > 1) return false;
		return isAVL(node->left) && isAVL(node->right);
	}

	void inOrd(Node* node) {
		if (!node) return;
		inOrd(node->left);
		cout << node->data << " ";
		inOrd(node->right);
	}

	void preOrd(Node* node) {
		if (!node) return;
		cout << node->data << " ";
		preOrd(node->left);
		preOrd(node->right);
	}

	void postOrd(Node* node) {
		if (!node) return;
		postOrd(node->left);
		postOrd(node->right);
		cout << node->data << " ";
	}

	void inOrder() {
		inOrd(root);
		cout << endl;
	}

	void preOrder() {
		preOrd(root);
		cout << endl;
	}

	void postOrder() {
		postOrd(root);
		cout << endl;
	}

	int smallest() {
		Node* current = root;
		while (current->left != NULL) {
			current = current->left;
		}
		return current->data;
	}

	int largest() {
		Node* current = root;
		while (current->right != NULL) {
			current = current->right;
		}
		return current->data;
	}

	Node** split(Node* node, int K) {
		Node** nodes = new Node * [2];
		nodes[0] = nullptr;
		nodes[1] = nullptr;
		if (node == nullptr) return nodes;

		if (node->data <= K) {
			nodes = split(node->right, K);
			node->right = nodes[0];
			nodes[0] = node;

			node->height = 1 + max(getHeight(node->left), getHeight(node->right));
			int balance = getBalance(node);

			if (balance > 1) {
				if (getBalance(node->left) >= 0) nodes[0] = rotateRight(node);
				else nodes[0] = rotateLeftRight(node);
			}
			else if (balance < -1) {
				if (getBalance(node->right) <= 0) nodes[0] = rotateLeft(node);
				else nodes[0] = rotateRightLeft(node);
			}
		}
		else {
			nodes = split(node->left, K);
			node->left = nodes[1];
			nodes[1] = node;

			node->height = 1 + max(getHeight(node->left), getHeight(node->right));
			int balance = getBalance(node);

			if (balance > 1) {
				if (getBalance(node->left) >= 0) nodes[1] = rotateRight(node);
				else nodes[1] = rotateLeftRight(node);
			}
			else if (balance < -1) {
				if (getBalance(node->right) <= 0) nodes[1] = rotateLeft(node);
				else nodes[1] = rotateRightLeft(node);
			}
		}
		return nodes;
	}
};