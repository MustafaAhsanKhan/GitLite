#pragma once
#include<iostream>
#include<fstream>
using namespace std;
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
	//void operator=(string text) {
	//	len = 0;
	//	int i = 0;
	//	while (text[i++]) {
	//		push_back(text[i - 1]);
	//	}
	//	push_back('\0');
	//}
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

	//String operator+(string val) {
	//	len--;
	//	int i = 0;
	//	while (val[i++]) {
	//		push_back(val[i - 1]);
	//	}
	//	push_back('\0');
	//	return *this;
	//}
	String substr(size_t start, size_t n = std::string::npos) const  {
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
	char* operator+(String& other) {
		len--;
		int i = 0;
		while (other[i++]) {
			push_back(other[i - 1]);
		}
		push_back('\0');
		return getData();
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