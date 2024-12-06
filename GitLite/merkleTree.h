#pragma once
#include "string2.h"
#include <fstream>
#include "picosha2.h"
#include "Vector"
#include<filesystem>
using namespace filesystem;
void instructorsHash(ifstream& ifs, path out) {
	char ch;
	int hash = 1;
	while (ifs.get(ch)) {
		if (ch != '\n' && ch != ' ') {
			hash *= ch % 29;
		}
	}
	ofstream file(out);
	file << hash;
	file.close();
	//return hash%29;
}
int instructorsHash(String& str) {
	int hash = 1;
	for (int i = 0; i < str.size(); i++) {
		hash *= str[i] % 29;
	}
	return hash % 29;
}
int instructorsHash(int num) {
	int hash = 1;
	while (num) {
		hash *= ((num % 10)) % 29;
		num /= 10;
	}
	return hash % 29;
}

struct MerkleNode {
	String hash;
	MerkleNode* left;
	MerkleNode* right;
	path nodePath;
	int hashPref; // 1 for instructor 2 for sha256
	MerkleNode(String hash) {
		this->hash = hash;
		left = right = nullptr;
	}
	MerkleNode(MerkleNode* left, MerkleNode* right) {
		this->hash = picosha2::hash256_hex_string(left->hash + right->hash);

		this->left = left;
		this->right = right;
	}
	MerkleNode(path path) {
		nodePath = path;
		readNode();
	}
	void writeNode() {
		ofstream file(nodePath);
		file << hash;
		if (left) cout << left->nodePath << endl;
		else cout << endl;
		if (right) cout << right->nodePath;
		else cout << endl;
	}
	void readNode() {
		ifstream file(nodePath);
		file >> hash;
		path leftPath;
		file >> leftPath;
		if (leftPath != "") {
			left = new MerkleNode(leftPath);
			left->nodePath = leftPath;
			left->readNode();
		}
		path rightPath;
		file >> rightPath;
		if (rightPath != "") {
			right = new MerkleNode(rightPath);
			right->nodePath = rightPath;
			right->readNode();
		}
	}
};



class MerkleTree {
	MerkleNode root;
public:
	
	
};