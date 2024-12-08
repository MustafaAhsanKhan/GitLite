#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include "picosha2.h"
using namespace std;
using namespace filesystem;

#pragma once
#include<iostream>
using namespace std;
template<typename T>
struct QNode {
	T data;
	QNode<T>* next;
};

template<typename T>
class Queue {
private:
	QNode<T>* front;
	QNode<T>* rear;
	int len = 0;
public:
	Queue() {
		front = nullptr;
		rear = nullptr;
	}
	void push(T item) {
		QNode<T>* newNode = new QNode<T>;
		newNode->data = item;
		newNode->next = nullptr;
		if (rear == nullptr) {
			front = newNode;
			rear = newNode;
			len++;
			return;
		}
		rear->next = newNode;
		rear = newNode;
		len++;
	}
	T pop() {
		if (front == nullptr) {
			return T(-1);
		}
		QNode<T>* temp = front;
		front = front->next;
		if (front == nullptr) {
			rear = nullptr;
		}
		T data = temp->data;
		//delete temp;
		len--;
		return data;
	}
	int size() {
		return len;
	}
	bool empty() {
		return len == 0;
	}
	T p() {
		if (front == nullptr) {
			return T(-1);
		}
		return front->data;
	}
	//void print() {
	//	QNode<T>* temp = front;
	//	while (temp != nullptr) {
	//		std::cout << temp->data << " ";
	//		temp = temp->next;
	//	}
	//	std::cout << std::endl;
	//}
	//bool is_empty() { return front == nullptr; }
	//bool isPalindrome(char* str) {
	//	Queue<char> q;
	//	/*int i = 0;
	//	while (str[i]) {
	//		q.enqueue(str[i]);
	//		i++;
	//	}*/
	//	return checkpal(q, str, 0);
	//}
	//bool checkpal(Queue<char>& q, char* str, int i) {
	//	if (str[i] == 0) return true;
	//	q.enqueue(str[i]);
	//	if (!checkpal(q, str, i + 1)) return false;
	//	if (q.dequeue() != str[i]) return false;
	//	return true;
	//}
};

template <typename T>
class Vector
{
protected:
	T* data;
	int max;
	int len;
public:
	Vector() {
		len = 0;
		max = 2;
		data = new T[max]();
	}
	Vector(int len) {
		this->len = len;
		max = 250;
		data = new T[max]();
	}
	virtual ~Vector() {
		delete[] data;
	}
	Vector(const Vector<T>& other) {
		len = other.len;
		max = other.max;
		data = new T[max];
		for (int i = 0; i < len; ++i) {
			data[i] = other.data[i];
		}
	}
	Vector<T>& operator=(const Vector<T>& other) {
		if (this != &other) {
			delete[] data; // Free existing data

			len = other.len;
			max = other.max;
			data = new T[max];
			for (int i = 0; i < len; ++i) {
				data[i] = other.data[i];
			}
		}
		return *this;
	}
	T* getData() const {
		return data;
	}
	T& operator[](int index) {
		return data[index]; // returns data at index
	}
	int size() {
		return len; // returns length of the array
	}
	void push_back(T val) {
		data[len] = val; // add val to last index
		len++;
		if (len == max) { // if array is full, increase length by 100
			max *= 2;
			T* temp = data;
			data = new T[max];
			for (int i = 0; i < max / 2; i++) data[i] = temp[i];
		}
	}
	int find(T val) { // get the index of val
		for (int i = 0; i < len; i++) {
			if (data[i] == val) return i;
		}
		return -1; // if not found return -1
	}
	void clear() { // clear the array
		len = 0;
		data = new T[max];
		push_back('\0');
	}
	T* getDup() { // get a duplicate of the array
		T* dup = new T[len];
		for (int i = 0; i < len; i++) dup[i] = data[i];
		return dup;
	}
};

class String : public Vector<char> {
public:
	String() {
		push_back('\0');
	}
	String(char* text) {
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]); // add text to the array
		}
		push_back('\0'); // add null character at the end
	}
	String(const char* text) {
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]); // add text to the array
		}
		push_back('\0'); // add null character at the end
	}
	String(String& text) {
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]); // add text to the array
		}
		push_back('\0'); // add null character at the end
	}
	String(const std::string& text) {
		int i = 0;
		while (i < text.length()) {
			push_back(text[i++]); // Add characters from std::string
		}
		push_back('\0'); // Add null character at the end
	}
	String(const String& text)
	{
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]); // add text to the array
		}
		push_back('\0'); // add null character at the end
	}
	char& operator [](int index) const {
		return data[index]; // returns data at index
	}
	void operator+=(char val) {
		len--;
		push_back(val); // add val to the array
		push_back('\0'); // add null character at the end
	}
	void operator+=(const char* text) {
		len--;
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]);
		}
		push_back('\0');
	}
	void operator+=(String text) {
		len--;
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]);
		}
		push_back('\0');
	}
	void operator+=(string text) {
		len--;
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]);
		}
		push_back('\0');
	}
	void operator=(const char* text) {
		len = 0;
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]);
		}
		push_back('\0');
	}
	void readTill(String& other, char delimeter) {
		len = 0;
		int i = 0;
		while (other[i++] && other[i - 1] != delimeter) {
			push_back(other[i - 1]);
		}
		push_back('\0');
	}
	void operator=(String text) {
		len = 0;
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]);
		}
		push_back('\0');
	}
	void operator=(string text) {
		len = 0;
		int i = 0;
		while (text[i++]) {
			push_back(text[i - 1]);
		}
		push_back('\0');
	}
	String operator+(char val) {
		len--;
		push_back(val);
		push_back('\0');
		return *this;
	}

	String operator+(String val) {
		len--;
		int i = 0;
		while (val[i++]) {
			push_back(val[i - 1]);
		}
		push_back('\0');
		return *this;
	}

	String operator+(string val) {
		len--;
		int i = 0;
		while (val[i++]) {
			push_back(val[i - 1]);
		}
		push_back('\0');
		return *this;
	}
	String substr(size_t start, size_t n = std::string::npos) const {
		String result;

		// Ensure the start index is within bounds
		if (start >= len - 1) { // `len - 1` because `len` includes the null terminator
			return result; // Return an empty String
		}

		// Calculate the actual length to copy
		size_t copyLength = (n == std::string::npos) ? (len - 1 - start) : std::min(n, len - 1 - start);

		// Copy the characters to the result
		for (size_t i = 0; i < copyLength; ++i) {
			result.push_back(data[start + i]);
		}

		return result;
	}
	String trim() const {
		size_t start = 0;
		size_t end = len - 2; // `len - 2` because `len` includes the null terminator

		// Find the first non-whitespace character
		while (start <= end && (data[start] == ' ' || data[start] == '\t' || data[start] == '\n')) {
			start++;
		}

		// Find the last non-whitespace character
		while (end >= start && (data[end] == ' ' || data[end] == '\t' || data[end] == '\n')) {
			end--;
		}

		// Create the trimmed string
		String result;
		for (size_t i = start; i <= end; ++i) {
			result.push_back(data[i]);
		}

		return result;
	}


	void toLower() {
		for (int i = 0; i < len; i++) {
			if (data[i] >= 65 && data[i] <= 90) {
				data[i] += 32;
			}
		}
	}
	int isGreaterThan(String other) const  // 1 means greater // 2 means less // 3 means equal
	{
		int i = 0;
		while (data[i] && other.data[i])
		{
			if (data[i] > other.data[i]) return 1;
			if (data[i] < other.data[i]) return 2;
			i++;
		}
		// If one string is a prefix of the other, the longer string is greater
		if (data[i] && !other.data[i]) return 1;
		if (!data[i] && other.data[i]) return 2;
		return 3; // Strings are equal
	}
	int search(String text) { // returns the index of the subStringing
		int i = 0;
		while (data[i]) {
			if (data[i] == text[0] && (!i || data[i - 1] == ' ')) { // subStringing must be a whole word
				int j = 0;
				while (text[j] && data[i + j] == text[j]) {
					j++;
				}
				if (!text[j] && (data[i + j] == ' ' || data[i + j] == '\0')) return i;
			}
			i++;
		}
		return -1;
	}
	bool operator==(const char* text) { // check character by character
		int i = 0;
		while (data[i] && text[i]) {
			if (data[i] != text[i]) return false;
			i++;
		}
		if (data[i] || text[i]) return false;
		return true;
	}
	bool operator==(char* text) { // check character by character
		int i = 0;
		while (data[i] && text[i]) {
			if (data[i] != text[i]) return false;
			i++;
		}
		if (data[i] || text[i]) return false;
		return true;
	}
	bool operator==(String text) {
		int i = 0;
		while (data[i] && text[i]) {
			if (data[i] != text[i]) return false;
			i++;
		}
		if (data[i] || text[i]) return false;
		return true;
	}
	bool operator!=(String text) {
		return !(*this == text);
	}

	void getLine(istream& is, char delimiter) {
		char ch;
		while (is.get(ch) && ch != delimiter) {
			push_back(ch);
		}
		push_back('\0');
	}
	void getLine(ifstream& is, char delimiter) {
		char ch;
		while (is.get(ch) && ch != delimiter) {
			push_back(ch);
		}
		push_back('\0');
	}
	void readComplete(ifstream& ifs) {
		len = 0;
		char ch;
		while (ifs.get(ch)) {
			if (ch != '\n')	push_back(ch);
			else push_back(' ');
		}
		push_back('\0');
	}

	bool empty()
	{
		return len == 1;
	}

	bool empty() const
	{
		return len == 1;
	}

	friend ostream& operator<<(ostream& os, const  String& text);
	friend istream& operator>>(istream& in, String& text);
	friend ofstream& operator<<(ofstream& os, String& text);
	friend ifstream& operator>>(ifstream& in, String& text);
};

ostream& operator<<(ostream& os, const String& text) {
	os << text.getData();
	return os;
}

istream& operator>>(istream& in, String& text) {
	text = ""; // Clear the current content of text
	char ch;
	while (in.get(ch) && ch != '\n' && ch != ' ') {
		text += ch;
	}
	return in;
}


ofstream& operator<<(ofstream& os, String& text) {
	os << text.getData();
	return os;
}

ifstream& operator>>(ifstream& in, String& text) {
	text = ""; // Clear the current content of text
	char ch;
	while (in.get(ch) && ch != '\n' && ch != ' ') {
		text += ch;
	}
	return in;
}


bool customGetline(std::ifstream& file, String& line, char delimiter = '\n') {
	line.clear();
	char ch;
	while (file.get(ch)) {
		if (ch == delimiter) {
			return true; // Successfully read a line
		}
		line += ch;
	}
	return !line.empty(); // Return true if there was any content read before EOF
}

// custom to string function to convert an integer to its respective string
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
		result = result + digit;//result += digit;                // to add the digit to the result
		value = value / 10; ; // value =/ 10                // Remove the last digit
	} while (value > 0);

	// if negative add the negative sign
	if (isNegative) {
		result = result + '-';
	}

	// because digits were added in reverse order 
	int len = result.size();
	for (int i = 0; i < len / 2; ++i) {
		char temp = result[i];
		result[i] = result[len - i - 1];
		result[len - i - 1] = temp;
	}

	std::cout << "custom_to_string result: " << result.getData() << std::endl;
	return result;
}

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
	int isLeaf = 0;
	MerkleNode* left;
	MerkleNode* right;
	path dataNodePath;
	path nodePath;
	int hashPref; // 0 for instructor 1 for sha256
	MerkleNode() {}
	MerkleNode(String hash)
	{
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
		file << isLeaf << " ";
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
		path fol = output / "merkle";
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


template <typename T, int o>
struct BTreeNode {
	path currentNodePath;
	path parentPath;
	int numOfKeys = 0;
	bool isLeaf = true;
	T keys[o];
	path keyPath[o];
	path children[o + 1];

	BTreeNode(path nodePath, path currentPath) {
		currentNodePath = currentPath;
		parentPath = nodePath;

		for (int i = 0; i < o + 1; i++) {
			children[i] = "";
		}
	}

	BTreeNode(path p) {
		currentNodePath = p;
		ifstream file(p);
		file >> numOfKeys;
		file >> isLeaf;
		file >> parentPath;

		for (int i = 0; i < numOfKeys; i++) {
			file >> keys[i];
		}
		for (int i = 0; i < numOfKeys; i++) {
			file >> keyPath[i];
		}
		for (int i = 0; i < o + 1; i++) {
			file >> children[i];
		}
		file.close();
	}

	void write() {
		ofstream file(currentNodePath);
		file << numOfKeys << endl;
		file << isLeaf << endl;
		file << parentPath << endl;
		for (int i = 0; i < numOfKeys; i++) {
			file << keys[i] << endl;
		}
		for (int i = 0; i < numOfKeys; i++) {
			file << keyPath[i] << endl;
		}
		for (int i = 0; i < o + 1; i++) {
			file << children[i] << endl;
		}
		file.close();
	}
	void writeKey(int index, String& key) {
		path keyFilePath = currentNodePath.parent_path() / "keys" / (currentNodePath.filename().string() + to_string(index) + ".txt");
		ofstream keyFile(keyFilePath);
		keyFile << key;
		keyFile.close();
		keyPath[index] = keyFilePath;
	}
	T readKey(int index) {
		path keyFilePath = keyPath[index];
		ifstream keyFile(keyFilePath);
		T key;
		keyFile >> key;
		keyFile.close();
		return key;
	}
};

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
	path rootFileName;
	int nodeCount;
	path directoryPath;

	void createDirectory(const String& dirName) {
		directoryPath = path(dirName.getData());
		if (!exists(directoryPath))
		{
			std::cout << "Created branch: " << directoryPath << std::endl;
			create_directory(directoryPath);
		}
	}

	path generateFileName(String key)
	{
		String fileName = to_string(nodeCount++) + ".txt";
		return (directoryPath / fileName.getData());
	}

	// Modify insert and delete functions to work with filenames
public:
	AVL() : rootFileName(""), nodeCount(0) {}

	void initialize(path dirPath)
	{
		directoryPath = dirPath;
	}
	void queryRange(int lowerBound, int upperBound)
	{
		cout << "\033[33mAVL Tree: Records within range [" << lowerBound << ", " << upperBound << "]:\033[0m" << endl;
		queryRangeHelper(rootFileName, lowerBound, upperBound);
	}

	void queryRangeHelper(const path& nodeFileName, int lowerBound, int upperBound)
	{
		if (nodeFileName.empty())
			return;

		// Read the node from file
		Node node = readNodeFromFile(nodeFileName);

		// Convert the node's key to an integer for comparison
		int key = stoi(node.key.getData());

		// Perform in-order traversal
		// Left subtree
		if (key > lowerBound)
		{
			queryRangeHelper(node.leftFile.getData(), lowerBound, upperBound);
		}

		// Current node
		if (key >= lowerBound && key <= upperBound)
		{
			cout << "Key: " << node.key << ", Data: " << node.dataRow << endl;
		}

		// Right subtree
		if (key < upperBound)
		{
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

	Node readNodeFromFile(const path& filePath) {
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


	void writeNodeToFile(const Node& node, const path& filePath) {
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

	path insertHelper(const path& nodeFileName, const String& key, const String& dataRow) {
		if (nodeFileName.empty()) {
			// Create new node with height 0
			path newNodeFile = generateFileName(key);
			Node newNode(key, 1, 0, 0, "", "", dataRow);
			writeNodeToFile(newNode, newNodeFile);
			return newNodeFile;
		}

		Node node = readNodeFromFile(nodeFileName);

		if (key.isGreaterThan(node.key) == 2) {  // Less
			path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow);
			node.leftFile = updatedLeftChild.string();
		}
		else {
			// Insert duplicates to the right subtree (greater or equal cases)
			path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow);
			node.rightFile = updatedRightChild.string();
		}

		node.isLeaf = (node.leftFile.empty() && node.rightFile.empty()) ? 1 : 0;

		// *Immediately write the updated node back to file*
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

	path deleteHelper(const path& nodeFileName, const String& key) {
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
				path tempFileName = node.leftFile.empty() ? node.rightFile.getData() : node.leftFile.getData();

				// Delete current node file
				remove(nodeFileName);

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
				path tempFileName = minValueNode(node.rightFile.getData());
				Node tempNode = readNodeFromFile(tempFileName);

				// Copy the inorder successor's data to this node
				node.key = tempNode.key;
				node.dataRow = tempNode.dataRow;

				// Delete the inorder successor
				node.rightFile = deleteHelper(node.rightFile.getData(), tempNode.key).string();
			}
		}

		// Update isLeaf flag
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

	/*void updateHeight(const path& nodeFileName) {
		if (nodeFileName.empty()) return;

		Node node = readNodeFromFile(nodeFileName);

		int leftHeight = getHeight(node.leftFile.getData());
		int rightHeight = getHeight(node.rightFile.getData());
		node.height = 1 + max(leftHeight, rightHeight);

		writeNodeToFile(node, nodeFileName);
	}*/

	path minValueNode(const path& nodeFileName) {
		path currentFileName = nodeFileName;
		Node currentNode = readNodeFromFile(currentFileName);

		// Loop down to find the leftmost leaf
		while (!currentNode.leftFile.empty()) {
			currentFileName = currentNode.leftFile.getData();
			currentNode = readNodeFromFile(currentFileName);
		}
		return currentFileName;
	}


	void updateBalanceFactor(const path& nodeFileName) {
		if (nodeFileName.empty()) return;
		Node node = readNodeFromFile(nodeFileName);

		// Ensure child heights are up-to-date
		int leftHeight = getHeight(node.leftFile.getData());
		int rightHeight = getHeight(node.rightFile.getData());

		// Compute balance factor
		node.balanceFactor = leftHeight - rightHeight;

		// *Write the updated node back to the file*
		writeNodeToFile(node, nodeFileName);
	}




	path rightRotate(const path& yFileName) {
		Node y = readNodeFromFile(yFileName);
		path xFileName = y.leftFile.getData();
		Node x = readNodeFromFile(xFileName);
		path T2FileName = x.rightFile.getData();

		// Perform rotation
		x.rightFile = yFileName.string();
		y.leftFile = T2FileName.string();

		// Update isLeaf flags
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




	path leftRotate(const path& xFileName) {
		Node x = readNodeFromFile(xFileName);
		path yFileName = x.rightFile.getData();
		Node y = readNodeFromFile(yFileName);
		path T2FileName = y.leftFile.getData();

		// Perform rotation
		y.leftFile = xFileName.string();
		x.rightFile = T2FileName.string();

		// Update isLeaf flags
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




	int getHeight(const path& nodeFileName) {
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



	int getBalanceFactor(const path& nodeFileName) {
		if (nodeFileName.empty()) return 0;
		Node node = readNodeFromFile(nodeFileName);
		return getHeight(node.leftFile.getData()) - getHeight(node.rightFile.getData());
	}

	void inOrderTraversal(const path& nodeFileName) {
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


namespace fs = std::filesystem;

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
	path rootFileName;
	int nodeCount;
	path directoryPath;



	// create a file to store the tree in 
	void createDirectory(const String& dirName) {
		directoryPath = path(dirName.getData());
		std::cout << "Created branch: " << directoryPath << std::endl;
		if (!exists(directoryPath)) {
			create_directory(directoryPath);
		}
	}

	// generating uniquw file names which depend on node count hence unique
	path generateFileName() {
		String fileName = std::to_string(nodeCount++) + ".txt";
		std::cout << "Generated file name: " << fileName << std::endl; // Debugging line
		return (directoryPath / fileName.getData());
	}


	// Custom getline function it reads lines from the file
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
	RBNode readNodeFromFile(const path& filePath) {
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

	// to write RB Nodes to files
	void writeNodeToFile(const RBNode& node, const path& filePath) {
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
	path leftRotate(const path& nodeFileName) {
		RBNode node = readNodeFromFile(nodeFileName);  // Get the node to be rotated
		path rightFileName = node.rightFile.getData();  // Get the right child of the node
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
	path rightRotate(const path& nodeFileName) {
		RBNode node = readNodeFromFile(nodeFileName);  // Get the node to be rotated
		path leftFileName = node.leftFile.getData();  // Get the left child of the node
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
	void fixInsert(path& nodeFileName) {
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

	path insertHelper(const path& nodeFileName, const String& key, const String& dataRow, const String& parentFileName) {
		if (nodeFileName.empty()) {
			path newNodeFile = generateFileName();
			RBNode newNode(key, "RED", "", "", parentFileName, dataRow);
			writeNodeToFile(newNode, newNodeFile);
			return newNodeFile;
		}

		RBNode node = readNodeFromFile(nodeFileName);
		std::cout << "Inserting into node: " << nodeFileName << ", Key: " << key << std::endl;

		if (key.isGreaterThan(node.key) == 2) {  // Key is less
			path updatedLeftChild = insertHelper(node.leftFile.getData(), key, dataRow, nodeFileName.string());
			node.leftFile = updatedLeftChild.string();
			writeNodeToFile(node, nodeFileName);
		}
		else {
			path updatedRightChild = insertHelper(node.rightFile.getData(), key, dataRow, nodeFileName.string());
			node.rightFile = updatedRightChild.string();
			writeNodeToFile(node, nodeFileName);
		}

		return nodeFileName;
	}



public:
	RBTree() : rootFileName(""), nodeCount(0) {}

	void initialize(path dirName)
	{
		//createDirectory(dirName);
		directoryPath = dirName;
	}

	void insert(const String& key, const String& dataRow) {
		std::cout << "Inserting key: " << key << ", dataRow: " << dataRow << std::endl; // Debugging line
		rootFileName = insertHelper(rootFileName, key, dataRow, "");
		fixInsert(rootFileName);
	}

	void inOrderTraversal(const path& nodeFileName) {
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




template <typename T, int o>
class BTree {
	path rootPath;
	BTreeNode<T, o>* root;
	int nodeCount = 0; // Node count to keep track of the number of nodes

	void sort(T* arr, int numKeys) {
		for (int i = numKeys - 1; i > 0; i--) {
			if (arr[i - 1] > arr[i]) {
				T temp = arr[i - 1];
				arr[i - 1] = arr[i];
				arr[i] = temp;
			}
		}
	}

	BTreeNode<T, o>* createNode(path parentPath) {
		nodeCount++;
		path nodePath = rootPath / (to_string(nodeCount) + ".txt");
		return new BTreeNode<T, o>(parentPath, nodePath);
	}

	BTreeNode<T, o>* insert(BTreeNode<T, o>* node, T data, BTreeNode<T, o>*& newChild, T& newKey, String& nodeData) {
		if (!node) {
			node = createNode(rootPath);
			node->writeKey(node->numOfKeys, nodeData);
			node->keys[node->numOfKeys++] = data;
			newChild = nullptr;
			node->write();
			return node;
		}
		else {
			if (node->isLeaf) {
				node->writeKey(node->numOfKeys, nodeData);
				node->keys[node->numOfKeys++] = data;
				sort(node->keys, node->numOfKeys);
				if (node->numOfKeys == o) {
					newChild = createNode(node->currentNodePath);
					newKey = node->keys[o / 2];
					for (int i = o / 2 + 1; i < o; i++) {
						String datdfa;
						ifstream file(node->keyPath[i]);
						datdfa.readComplete(file);
						file.close();
						newChild->writeKey(newChild->numOfKeys, datdfa);
						newChild->keys[newChild->numOfKeys++] = node->keys[i];
					}
					node->numOfKeys = o / 2;
					node->write();
					newChild->write();
					return node;
				}
				else {
					newChild = nullptr;
					node->write();
					return node;
				}
			}
			else {
				int i = 0;
				while (i < node->numOfKeys && data > node->keys[i]) {
					i++;
				}
				BTreeNode<T, o>* child = new BTreeNode<T, o>(node->children[i]);
				child = insert(child, data, newChild, newKey, nodeData);
				if (newChild) {

					for (int j = node->numOfKeys; j > i; j--) {
						String datdfa;
						ifstream file(node->keyPath[j - 1]);
						datdfa.readComplete(file);
						file.close();
						node->writeKey(j, datdfa);
						node->keys[j] = node->keys[j - 1];
						node->children[j + 1] = node->children[j];
					}
					String datdfa;
					ifstream file(node->keyPath[i]);
					datdfa.readComplete(file);
					file.close();
					node->writeKey(i, datdfa);
					node->keys[i] = newKey;
					node->children[i + 1] = newChild->currentNodePath;
					node->numOfKeys++;
					if (node->numOfKeys == o) {
						newChild = createNode(node->currentNodePath);
						newChild->isLeaf = false;
						newKey = node->keys[o / 2];
						for (int j = o / 2 + 1; j < o; j++) {
							String datdfa;
							ifstream file(node->keyPath[i]);
							datdfa.readComplete(file);
							file.close();
							newChild->writeKey(newChild->numOfKeys, datdfa);
							newChild->keys[newChild->numOfKeys++] = node->keys[j];
							newChild->children[newChild->numOfKeys] = node->children[j + 1];
						}
						newChild->children[0] = node->children[o / 2 + 1];
						node->numOfKeys = o / 2;
						node->write();
						newChild->write();
						return node;
					}
					else {
						newChild = nullptr;
						node->write();
						return node;
					}
				}
				else {
					node->write();
					return node;
				}
			}
		}
	}

public:
	BTree(path rootPath) {
		this->rootPath = rootPath;
		root = nullptr;
	}

	void insert(T data, String rowData) {
		BTreeNode<T, o>* newChild = nullptr;
		T newKey;
		root = insert(root, data, newChild, newKey, rowData);
		if (newChild) {
			BTreeNode<T, o>* newRoot = createNode(rootPath);
			String datdfa;
			ifstream file(newRoot->keyPath[0]);
			datdfa.readComplete(file);
			file.close();
			newRoot->writeKey(0, datdfa);
			newRoot->keys[0] = newKey;
			newRoot->children[0] = root->currentNodePath;
			newRoot->children[1] = newChild->currentNodePath;
			newRoot->numOfKeys = 1;
			newRoot->isLeaf = false;
			root = newRoot;
			root->write();
		}
		//cout << endl;
		//print(root, 0);
	}
	void print() {
		print(root, 0);
	}
	void remove(T k) {
		if (!root) {
			cout << "The tree is empty\n";
			return;
		}
		remove(root, k);
		if (root->numOfKeys == 0) {
			BTreeNode<T, o>* tmp = root;
			if (root->isLeaf) {
				root = nullptr;
			}
			else {
				root = new BTreeNode<T, o>(root->children[0]);
			}
			delete tmp;
		}
		cout << endl;
		print(root, 0);
	}
};



class Console
{
private:
	String command;  // First Part
	String second;  // Second Part
	String third;  // Third Part
	int treeType;  // 1 for AVL, 2 for B-Tree, 3 for Red-Black Tree
	path repoFolder = "";
	path currentBranchFolder = "";

public:
	AVL avl;
	RBTree rb;
	//BTree<int, 3> bt;
	void parseCsv(int selectedTree, String second, path treeDir);
	Console();
	void run();
};

Console::Console()
{
	command = "";
	second = "";
	third = "";
	treeType = 0;
}

void Console::parseCsv(int selectedTree, String second, path treeDir)
{
	String csvFileName = second;  // csv file

	String directoryName = "tree";

	// Create the tree using a switch based on the selected tree
	switch (selectedTree)
	{
	case(1):
	{
		avl.initialize(treeDir);
		break;
	}
	case(2):
	{
		//Btree.initialize(directoryName);
		break;
	}
	case(3):
	{
		rb.initialize(treeDir);
		break;
	}
	}


	ifstream csvFile(csvFileName.getData());
	if (!csvFile.is_open()) {
		cout << "Unable to open the CSV file." << endl;
		return;
	}

	// Read the header line to get column names
	String headerLine;
	customGetline(csvFile, headerLine);
	string headerLineStr;
	vector<String> headers;
	String header;
	int pos = 0;
	for (int i = 0; headerLine[i] != '\0'; i++) {
		if (headerLine[i] == ',') {
			headers.push_back(header);
			header.clear();
		}
		else {
			header += headerLine[i];
		}
		pos = i;
	}
	// Add the last header after the loop
	header += headerLine[pos + 1];
	headers.push_back(header);

	// Display available columns
	cout << "Available columns:" << endl;
	for (size_t i = 0; i < headers.size(); ++i) {
		cout << i + 1 << ": " << headers[i].getData() << endl;
	}

	int columnChoice;
	cout << "Select a column number to use as the key: ";
	cin.ignore(); // Ignore the newline character left by previous input
	cin >> columnChoice;
	cin.ignore();
	--columnChoice; // Adjust for zero-based index

	if (columnChoice < 0 || columnChoice >= headers.size()) {
		cout << "Invalid column choice." << endl;
		return;
	}

	// Read and process each row
	String line;
	while (customGetline(csvFile, line)) {
		vector<String> rowData;
		String cell;
		for (int i = 0; line[i] != '\0'; ++i) {
			if (line[i] == ',') {
				rowData.push_back(cell);
				cell.clear();
			}
			else {
				cell += line[i];
			}
		}
		// Add the last cell after the loop
		rowData.push_back(cell);

		if (columnChoice < rowData.size()) {
			String key = rowData[columnChoice];
			// Reconstruct the data row as a single string
			String dataRow;
			for (size_t i = 0; i < rowData.size(); ++i) {
				dataRow += rowData[i];
				if (i != rowData.size() - 1) {
					dataRow += ',';
				}
			}
			switch (selectedTree)
			{
			case(1):
			{
				avl.insert(key, dataRow);
				break;
			}
			case(2):
			{
				//bt.insert(key, dataRow);
				break;
			}
			case(3):
			{
				rb.insert(key, dataRow);
				break;
			}
			}
		}
		else
		{
			cout << "Row has fewer columns than expected." << endl;
		}
	}

	csvFile.close();

}

void Console::run()  // Program Loop
{
	cin >> std::noskipws;  // To read spaces as well
	while (true)
	{
		// The user can either input a single word command (init)
		// Or a command that has an additional part seperated by a space (commit "message"
		// The additional part can be enclosed in quotes or angle brackets
		// The command and second strings allow us to split these into two
		// Further allowing us to use them where needed
		cout << '>';  // Ready for user input
		command.clear();
		second.clear();
		third.clear();
		cin >> command;
		command.toLower();  // Convert to lowercase to make everything consistent

		if (command == "exit")
		{
			cout << "\033[33mExiting GitLite...\033[0m";
			break;
		}
		// For all commands that only have one part
		if (!(command == "log" || command == "branches" || command == "current-branch" || command == "save"))
		{
			cin >> second;
			String temp;
			for (int i = 0; i < second.size(); ++i)
			{
				char c = second[i];
				if (c == '<' || c == '>' || c == '"')  // Ignoring enclosing characters
				{
					continue;
				}
				temp += c;
			}
			second = temp;  // Removed enclosing characters
		}
		if (command == "merge" || command == "select")  // This command needs two further inputs  // So taking the third input from the user
		{
			cin >> third;
			String temp;
			for (int i = 0; i < third.size(); ++i)
			{
				char c = third[i];
				if (c == '<' || c == '>' || c == '"')  // Ignoring enclosing characters
				{
					continue;
				}
				temp += c;
			}
			third = temp;  // Removed enclosing characters
		}
		if (command == "select")
		{
			// Read the lower and upper bounds

			// Ensure that both bounds are provided
			if (second.empty() || third.empty())
			{
				cout << "\033[91mUsage: select <lower_bound> <upper_bound>\033[0m" << endl;
				continue;
			}

			// Convert bounds to integers
			int lowerBound = stoi(second.getData());
			int upperBound = stoi(third.getData());

			// Check if repository is initialized
			if (treeType == 0)
			{
				cout << "\033[91mNo tree initialized. Please initialize a repository first.\033[0m" << endl;
				continue;
			}

			cout << "\033[33mFetching records where keys are between "
				<< lowerBound << " and " << upperBound << ".\033[0m" << endl;
			cout << endl;

			// Query the selected tree for records within the range
			switch (treeType)
			{
			case 1: // AVL Tree
				avl.queryRange(lowerBound, upperBound);
				break;
			case 3: // Red-Black Tree
				// rb.queryRange(lowerBound, upperBound);
				break;
			default:
				cout << "\033[91mTree type not supported for querying.\033[0m" << endl;
			}
		}
		// All inputs
		if (command == "init")
		{
			String folderName;
			folderName.readTill(second, '.');


			repoFolder = current_path() / folderName.getData();
			if (exists(repoFolder))
			{
				cout << "\033[91mRepository already exists. Please load the repository.\033[0m";
				cout << endl;
				continue;
			}
			create_directory(repoFolder);
			// Create the repo using the filename provided
			cout << "\033[33mWhich tree would you like to use?\033[0m" << endl;
			cout << "1: AVL" << endl;
			cout << "2: B-Tree" << endl;
			cout << "3: Red-Black Tree" << endl;
			ofstream file(repoFolder / "repo_data.txt");
			int x = 0;
			while (1)
			{
				cin >> x;
				if (x == 1)
				{
					treeType = 1;
					cout << "\033[33mSelected AVL tree.\033[0m";
					file << x << endl;
					break;
				}
				else if (x == 2)
				{
					treeType = 2;
					cout << "\033[33mSelected B-Tree.\033[0m";
					file << x << endl;
					break;
				}
				else if (x == 3)
				{
					treeType = 3;
					cout << "\033[33mSelected Red-Black Tree.\033[0m";
					file << x << endl;
					break;
				}
			}
			cout << endl;
			file << "master";
			create_directory(repoFolder / "master");
			currentBranchFolder = repoFolder / "master";
			create_directory(currentBranchFolder / "tree");
			parseCsv(treeType, second, currentBranchFolder / "tree");
			if (treeType == 1 || treeType == 3) {
				MerkleTree mt(1);
				mt.generateFromFolder(currentBranchFolder / "tree", currentBranchFolder);
			}
			file.close();
		}
		else if (command == "branch")
		{

			create_directory(repoFolder / second.getData());
			copy(currentBranchFolder, repoFolder / second.getData(), copy_options::overwrite_existing | copy_options::recursive);
			currentBranchFolder = repoFolder / second.getData();
			ofstream file(repoFolder / "repo_data.txt");
			file << treeType << endl << second;
			// second stores branch name
			cout << "\033[33mBranch " << '\'' << second << '\'' << " created successfully.\033[0m";
			file.close();
		}
		else if (command == "checkout")
		{
			if (exists(repoFolder / second.getData())) {
				currentBranchFolder = repoFolder / second.getData();
				ofstream file(repoFolder / "repo_data.txt");
				file << treeType << endl << second;
				file.close();
				cout << "\033[33mSwitched to branch " << '\'' << second << '\'' << ".\033[0m";
			}
			else {
				cout << "\033[91mBranch " << '\'' << second << '\'' << " does not exist.\033[0m";
			}
			// second stores branch name
		}
		else if (command == "commit")
		{
			// second stores commit message
			cout << "\033[33mChanges committed with message: " << '"' << second << '"' << ".\033[0m";
		}
		else if (command == "branches")
		{
			for (const auto& folder : directory_iterator(repoFolder)) {
				if (is_directory(folder)) {
					cout << "\033[33m" << folder.path().filename() << "\033[0m" << endl;
				}
			}
		}
		else if (command == "delete-branch")
		{
			if (exists(repoFolder / second.getData())) {
				remove_all(repoFolder / second.getData());
				cout << "\033[33mBranch " << '\'' << second << '\'' << " deleted successfully.\033[0m";
			}
			else {
				cout << "\033[91mBranch " << '\'' << second << '\'' << " does not exist.\033[0m";
			}
		}
		else if (command == "merge")
		{

			// second stores the source branch
			// third stores the target branch
			cout << "\033[33mMerged " << '\'' << second << '\'' << " into " << '\'' << third << '\'' << " successfully.\033[0m";
		}
		else if (command == "visualize-tree")
		{
			// BONUS //
			// DO AT THE END //
			// second stores the branch the visualize
			// Call the respective display functions

			switch (treeType)
			{
			case(1):
			{
				avl.inOrder();
				break;
			}
			case(2):
			{
				//Btree.initialize(directoryName);
				break;
			}
			case(3):
			{
				rb.inOrder();
				break;
			}
			}
		}
		else if (command == "log")
		{
			cout << "\033[33mCommit History for " << '\'' << "master" << '\'' << ":\033[0m" << endl;  // Replace with current branch
			cout << "\033[33mCommit #3: \"deleted 46 key\".\033[0m" << endl;  // Replace with the real message
			cout << "\033[33mCommit #2: \"Added 11 key\".\033[0m" << endl;
			cout << "\033[33mCommit #1: \"Initialized branch\".\033[0m";
		}
		else if (command == "current-branch")
		{
			cout << "\033[33m" << currentBranchFolder.filename() << "\033[0m";
			//cout << "\033[33mYou are on branch: 'main'.\033[0m";  // Replace with current branch
		}
		else if (command == "save")
		{
			cout << "\033[33mRepository saved successfully to 'repo_data.txt'.\033[0m";  // Replace with the real filename
		}
		else if (command == "load")
		{
			if (repoFolder == "") {
				if (exists(current_path() / second.getData())) {
					repoFolder = current_path() / second.getData();
					ifstream file(repoFolder / "repo_data.txt");
					file >> treeType;
					String Branch;
					file >> Branch;
					currentBranchFolder = repoFolder / Branch.getData();
					file.close();

					cout << "\033[33mRepository loaded successfully from '" << second << "'.\033[0m";
				}

				else {
					cout << "\033[91mNo repository found. Please initialize a repository first.\033[0m";
				}
			}
			else {

				cout << "\033[91mAlready in a repository. Close this repo first.\033[0m";
			}

		}
		else
		{
			cout << "\033[91mInvalid command. Please try again.\033[0m";
		}

		cout << endl;
		//cin.ignore();
	}
}
















int main()
{
	Console C1;
	C1.run();

	return 0;
}
