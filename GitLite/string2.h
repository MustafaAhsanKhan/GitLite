#pragma once
#include<iostream>
using namespace std;
class Vector
{
protected:
	char* data;
	int max;
	int len;
public:
	Vector() {
		len = 0;
		max = 1;
		data = new char[max]();
	}
	Vector(int len) {
		this->len = len;
		max = 250;
		data = new char[max]();
	}
	char* getData() const {
		return data;
	}
	char& operator[](int index) {
		return data[index]; // returns data at index
	}
	int size() {
		return len; // returns length of the array
	}
	void push_back(char val) {
		data[len] = val; // add val to last index
		len++;
		if (len == max) { // if array is full, increase length by 100
			max *= 2;
			char* temp = data;
			data = new char[max];
			for (int i = 0; i < max / 2; i++) data[i] = temp[i];
		}
	}
	int find(char val) { // get the index of val
		for (int i = 0; i < len; i++) {
			if (data[i] == val) return i;
		}
		return -1; // if not found return -1
	}
	void clear() { // clear the array
		len = 0;
		data = new char[max];
		push_back('\0');
	}
	char* getDup() { // get a duplicate of the array
		char* dup = new char[len];
		for (int i = 0; i < len; i++) dup[i] = data[i];
		return dup;
	}
};

class String : public Vector {
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

	String operator+(const char* val) {
		len--;
		int i = 0;
		while (val[i++]) {
			push_back(val[i - 1]);
		}
		push_back('\0');
		return *this;
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

	void toLower() { // convert all characters to lower case
		int i = 0;
		while (data[i]) {
			if (data[i] >= 'A' && data[i] <= 'Z') data[i] += 32;
			i++;
		}
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
