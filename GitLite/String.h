//#pragma once
//#include <iostream>
////////////////////////////////////
//// CUSTOM STRING DATATYPE CLASS //
////////////////////////////////////
//
//class String
//{
//private:
//	char* data;  // Character Array
//	int len;  // length of the string
//
//public:
//	// Default constructor
//	String();
//
//	// Constructor from char array
//	String(const char* c_str);
//
//	// Copy constructor
//	String(const String& other);
//
//	// Assignment operator
//	String& operator=(const String& other);
//
//	// Return len of the string
//	int getlen() const;
//
//	// Return the char array
//	char* getData() const;
//
//	// Access character at index
//	char& operator[](int index);
//	const char& operator[](int index) const;
//
//	// Overloaded + operator to concatenate two String objects
//	String operator+(const String& other) const;
//
//	// Overloaded + operator to concatenate a String object and a char array
//	String operator+(const char* c_str) const;
//
//	// Overloaded + operator to concatenate a String object and a single character
//	String operator+(char x) const;
//
//	// Remove all extra spaces  // Reduce spaces between words to 1  // Also shorten the array
//	void reduceSpaces();
//
//	// Compare with another string object
//	bool compare(const String& other) const;
//
//	// Compare with another char array
//	bool compare(const char* c_str_2) const;
//
//	// Function to check whether a char is a letter or not
//	bool isLetter(char x);
//
//	// Convert all alphabets to lowercase
//	void to_lower();
//
//	// Destructor
//	~String();
//
//	// Clear the string
//	void clear()
//	{
//		delete[] data;
//		data = nullptr;
//		len = 0;
//	}
//	String& getLine(std::istream& is, char delimiter);
//	friend std::istream& operator>>(std::istream& is, String& str);
//	friend std::ostream& operator<<(std::ostream& os, const String& str);
//
//private:
//	// Helper functions
//	int strlen(const char* c_str) const;
//	void strcpy(char* dest, const char* src) const;
//	bool strcmp(const char* c_str_1, const char* c_str_2) const;
//};
//std::istream& operator>>(std::istream& is, String& str);
//std::ostream& operator<<(std::ostream& os, const String& str);