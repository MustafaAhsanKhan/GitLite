#pragma once
#include "String.h"
#include <fstream>
#include "picosha2.h"
#include "Vector"
#include<filesystem>
#include "Queue.h"
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
	file << hash << " " << 0 << " " << 1 << " " << out << " " << in << " " << " " << " ";
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
	file << hash << " " << 1 << " " << 1 << " " << out << " " << in << " " << " " << " ";
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
	int isLeaf=0;
	MerkleNode* left;
	MerkleNode* right;
	path dataNodePath;
	path nodePath;
	int hashPref; // 0 for instructor 1 for sha256
	MerkleNode() {	}
	MerkleNode(String hash) {
		this->hash = hash;
		left = right = nullptr;
	}
	MerkleNode(MerkleNode* left, MerkleNode* right) {
		this->hash = picosha2::hash256_hex_string((left->hash + right->hash).getData());
		this->hashPref = left->hashPref;
		this->left = left;
		this->right = right;
	}
	MerkleNode(path path) {
		nodePath = path;
		readNode();
	}
	void writeNode() {
		ofstream file(nodePath);
		file << hash << " ";
		file << hashPref << " ";
		file << isLeaf<<" ";
		file << nodePath << " ";
		if (isLeaf) file << dataNodePath << " ";
		else {
			if (left) file << 1 << " " << left->nodePath << " ";
			else file << 0 << " ";
			if (right) file << 1 << " " << right->nodePath << " ";
			else file << 0 << " ";
		}
		file.close();
	}
	void readNode() {
		ifstream file(nodePath);
		file >> hash;
		file >> hashPref;
		file >> isLeaf;
		file >> nodePath;
		if (isLeaf) file >> dataNodePath;
		else {
			int leftExists;
			file >> leftExists;
			if (leftExists) {
				path leftPath;
				file >> leftPath;
				if (leftPath != "") {
					left = new MerkleNode();
					left->nodePath = leftPath;
					//left->readNode();
				}
			}
			int rightExists;
			file >> rightExists;
			if (rightExists) {
				path rightPath;
				file >> rightPath;
				if (rightPath != "") {
					right = new MerkleNode();
					right->nodePath = rightPath;
					//right->readNode();
				}
			}
		}
		file.close();
	}

};

MerkleNode*& generateMerkleNodeFromFile(path filepath, path out, int hpref) {
	if (hpref) sha256Hash(filepath, out);
	else instructorsHash(filepath, out);
	MerkleNode* node = new MerkleNode(out);
	node->hashPref = hpref;
	node->isLeaf = 1;
	return node;
}

class MerkleTree {
	MerkleNode* root;
	int hpref;
	Queue<MerkleNode*> nodes;
	int currentNodeIndex = 0;
public:
	MerkleTree(int hpref) {
		this->hpref = hpref;
	}
	void generateFromFolder(path folderPath, path output) {
		path fol = output/"merkle";
		create_directory(fol);
		if (!exists(folderPath)) return;
		for (const auto& file : directory_iterator(folderPath)) {
			if (!is_directory(file)) {
				path out = fol / (to_string(currentNodeIndex) + ".txt");
				MerkleNode* node = generateMerkleNodeFromFile(file.path(), out, hpref);
				node->dataNodePath = file.path();
				nodes.push(node);
				currentNodeIndex++;
			}
		}
		while (nodes.size() > 1) {
			MerkleNode* left = nodes.pop();
			//nodes.pop();
			MerkleNode* right = nodes.pop();
			//nodes.pop();
			path out = fol / (to_string(currentNodeIndex) + ".txt");
			MerkleNode* node = new MerkleNode(left, right);

			node->nodePath = out;
			nodes.push(node);
			node->writeNode();
			currentNodeIndex++;
		}

		root = nodes.p();
		ofstream file(output / "merkleRootAddr.txt");
		file << root->nodePath;
		file.close();

	}
	static Vector<Vector<path>> getChangedFiles(path branch1, path branch2) {
		Vector<Vector<path>> changedFiles;
		MerkleNode* root1 = new MerkleNode(branch1);
		MerkleNode* root2 = new MerkleNode(branch2);
		if (root1->hash == root2->hash) return changedFiles;
		Vector<MerkleNode*> nodes;
		nodes.push_back(root1);
		nodes.push_back(root2);
		cout << nodes[0]->hash << " " << nodes[1]->hash << endl;
		Queue <Vector< MerkleNode* >> q;
		q.push(nodes);
		//nodes.clear();
		while (!q.empty()) {
			Vector<MerkleNode*> currentNodes = q.pop();
			Vector<MerkleNode*> nextNodes;
			cout << currentNodes[0]->hash << " " << currentNodes[1]->hash << endl;
			currentNodes[0]->readNode();
			currentNodes[1]->readNode();
			cout << currentNodes[0]->hash << " " << currentNodes[1]->hash << endl;
			cout << endl;
			if (currentNodes[0]->hash != currentNodes[1]->hash) {
				if (currentNodes[0]->isLeaf && currentNodes[1]->isLeaf) {
					Vector<path> temp;
					temp.push_back(currentNodes[0]->dataNodePath);
					temp.push_back(currentNodes[1]->dataNodePath);
					changedFiles.push_back(temp);
				}
				else {
					if (currentNodes[0]->left && currentNodes[1]->left) {
						Vector<MerkleNode*> temp;
						temp.push_back(currentNodes[0]->left);
						temp.push_back(currentNodes[1]->left);
						q.push(temp);
					}
					if (currentNodes[0]->right && currentNodes[1]->right) {
						Vector<MerkleNode*> temp;
						temp.push_back(currentNodes[0]->right);
						temp.push_back(currentNodes[1]->right);
						q.push(temp);
					}
				}
			}
		}
		for (int i = 0; i < changedFiles.size(); i++) {
			cout << changedFiles[i][0] << " " << changedFiles[i][1] << endl;
		}
		return changedFiles;
	};

};