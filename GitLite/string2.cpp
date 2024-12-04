#include "string2.h"

ostream& operator<<(ostream& os, String& text)
{
	os << text.getData();
	return os;
}
istream& operator>>(istream& in, String& text)
{
	char ch;
	while (in.get(ch)) text += ch; // read entire input stream
	text += '\0';
	return in;
}