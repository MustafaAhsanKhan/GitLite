#pragma once
#include "string2.h"
#include <fstream>
#include "picosha2.h"
#include "Vector"
#include<filesystem>
#include "queue.h"
using namespace filesystem;
void instructorsHash(path in, path out) {
	char ch;
	int hash = 1;
	ifstream ifs(in);
	while (ifs.get(ch)) {
		if (ch != '\n' && ch != ' ') {
			hash *= ch % 29;
		}
	}
	ifs.close();
	ofstream file(out);
	file << 0 << " " << hash << " " << out << " " << in << " " << " " << " ";
	file.close();
}
void sha256Hash(path in, path out) {
	String data;
	ifstream ifs(in);
	data.readComplete(ifs);
	ifs.close();
	String hash;
	hash = picosha2::hash256_hex_string(data.getData());
	ofstream file(out);
	file << 1 << " " << hash << " " << out << " " << in << " " << " " << " ";
	file.close();
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
	path dataNodePath;
	path nodePath;
	int hashPref; // 0 for instructor 1 for sha256
	
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
		file << hashPref << " ";
		file << hash<<" ";
		file << nodePath << " ";
		if (dataNodePath != "") file << dataNodePath << " ";
		else file << "  ";
		if (left) file << left->nodePath << " ";
		else file << "  ";
		if (right) file << right->nodePath;
		else file << "  ";
	}
	void readNode() {
		ifstream file(nodePath);
		file >> hashPref;
		char sp;
		file.get(sp);
		hash.getLine(file, ' ');
		file >> nodePath;
		file >> dataNodePath;
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
		file.close();
	}
	
};

MerkleNode*& generateMerkleNodeFromFile(path filepath, path out, int hpref) {
	if (hpref) sha256Hash(filepath, out);
	else instructorsHash(filepath, out);
	MerkleNode* node = new MerkleNode(out);
	return node;
}

class MerkleTree {
	MerkleNode* root;
	int hpref;
	queue<MerkleNode*> nodes;
	int currentNodeIndex = 0;
public:
	MerkleTree(int hpref) {
		this->hpref = hpref;
	}
	void generateFromFolder(path folderPath) {
		path fol = "merkle";
		if (!exists(folderPath)) return;
		for (const auto& file : directory_iterator(folderPath)) {
			if (!is_directory(file)) {
				path out = fol / (to_string(currentNodeIndex)+".txt");
				MerkleNode* node = generateMerkleNodeFromFile(file.path(), out, hpref);
				node->dataNodePath = file.path();
				nodes.push(node);
				currentNodeIndex++;
			}
		}
		while (nodes.size() > 1) {
			MerkleNode* left = nodes.front();
			nodes.pop();
			MerkleNode* right = nodes.front();
			nodes.pop();
			path out = fol / (to_string(currentNodeIndex)+".txt");
			MerkleNode* node = new MerkleNode(left, right);
			
			node->nodePath = out;
			nodes.push(node);
			node->writeNode();
			currentNodeIndex++;
		}
		root = nodes.front();
	}
	Vector<Vector<path>> getChangedFiles()
	
};