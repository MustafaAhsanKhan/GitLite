#include "String.h"

//////////////////////////////////
// CUSTOM STRING DATATYPE CLASS //
//////////////////////////////////


String::String()
{
	data = nullptr;
	len = 0;
}

// Constructor from C-string
String::String(const char* c_str)
{
	len = strlen(c_str);
	data = new char[len + 1];
	strcpy(data, c_str);
}

// Copy constructor
String::String(const String& other)
{
	len = other.len;
	data = new char[len + 1];
	strcpy(data, other.data);
}

// Assignment operator
String& String::operator=(const String& other)
{

	delete[] data;  // Clear the current array

	// Making a copy of the "other" string object
	len = other.len;
	data = new char[len + 1];
	strcpy(data, other.data);

	return *this;
}

// length of the string
int String::getlen() const
{
	return len;
}

// The char array
char* String::getData() const
{
	return data;
}

// Access character at index
char& String::operator[](int index)
{
	if (index >= len || index < 0)  // Bounds check
	{
		exit(EXIT_FAILURE);
	}
	return data[index];
}

// Access character at index (const version)
const char& String::operator[](int index) const
{
	if (index >= len || index < 0)  // Bounds check
	{
		exit(EXIT_FAILURE);
	}
	return data[index];
}

// Overloaded + operator to concatenate two String objects
String String::operator+(const String& other) const
{
	int new_len = len + other.len;  // New len
	char* new_Data = new char[new_len + 1];  // Allocate memory

	if (data)
	{
		for (int i = 0; i < len; ++i)  // Left string
		{
			new_Data[i] = data[i];
		}
	}

	for (int i = 0; i < other.len; ++i)
	{
		new_Data[len + i] = other.data[i];  // Right string
	}

	new_Data[new_len] = '\0';  // Null terminate

	String result;  // New string object
	result.data = new_Data;
	result.len = new_len;

	return result;
}

// Overloaded + operator to concatenate a String object and a char array
String String::operator+(const char* c_str) const
{
	String new_s(c_str);  // String object from c-string  // Makes it easier to concatenate, as length and data will be allocated auto

	return *this + new_s;  // Concatenating a string object with another string object
}

// Overloaded + operator to concatenate a String object and a single character
String String::operator+(char x) const
{
	String new_s;  // Same logic  // Making the c-string into a String object
	new_s.len = 1;
	new_s.data = new char[2];  // One for the character and one for null terminator
	new_s.data[0] = x;
	new_s.data[1] = '\0';

	return *this + new_s;  // Concatenating a string object with another string object
}


// Remove all extra spaces  // Reduce spaces between words to 1  // Also shorten the array
void String::reduceSpaces()
{
	if (data)
	{
		int l = 0;  // Left pointer
		while (l < len && data[l] == ' ')  // Spaces before
		{
			++l;
		}

		int r = len;  // Right pointer
		while (r > l && data[r - 1] == ' ')  // Spaces after
		{
			--r;
		}

		if (l > 0 || r < len)  // If trailing spaced found
		{
			char* new_data = new char[r - l + 1];  // Just enough space for the words

			int i = l;

			while (i < r)
			{
				new_data[i - l] = data[i];
				++i;
			}
			new_data[r - l] = '\0';  // Null termination
			delete[] data;
			// Updating the char array and length
			data = new_data;
			len = r - l;
		}

		int short_len = 0;
		bool multipleSpace = false;

		for (int i = 0; i < len; ++i)  // Calculating the new shorter length  // With reduced spaces
		{
			if (data[i] != ' ')
			{
				multipleSpace = false;
				++short_len;
			}
			else if (multipleSpace == false)
			{
				multipleSpace = true;
				++short_len;
			}
		}

		// Allocate new data with the correct len
		char* new_data = new char[short_len + 1];

		multipleSpace = false;

		for (int i = 0, j = 0; i < len; ++i)  // Copying characters
		{
			if (data[i] != ' ')
			{
				multipleSpace = false;
				new_data[j] = data[i];
				++j;
			}
			else if (multipleSpace == false)
			{
				multipleSpace = true;
				new_data[j] = ' ';
				++j;
			}
		}
		new_data[short_len] = '\0';  // Null termination

		// Replace old data with new data
		delete[] data;
		data = new_data;
		len = short_len;
	}
}


// Compare with another string
bool String::compare(const String& other) const
{
	if (len != other.len)
	{
		return false;
	}
	return strcmp(data, other.data);
}

// Destructor
String::~String()
{
	delete[] data;
}

// Helper functions for string operations
int String::strlen(const char* c_str) const
{
	int len = 0;
	while (c_str[len] != '\0')  // End of char array isnt reached
	{
		++len;
	}
	return len;
}

void String::strcpy(char* dest, const char* src) const
{
	char* ptr = dest;
	while (*src)  // Loop until the end of src is reached
	{
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';
}


bool String::strcmp(const char* c_str_1, const char* c_str_2) const
{
	const char* temp1 = c_str_1;
	const char* temp2 = c_str_2;

	while (*temp1 && (*temp1 == *temp2))
	{
		++temp1;
		++temp2;
	}
	if (*temp1 == '\0' && *temp2 == '\0')  // Reached the end of both strings
	{
		return 1;
	}
	else
	{
		return 0;
	}
}