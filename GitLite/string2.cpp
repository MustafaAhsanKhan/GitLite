#include "string2.h"
#include <fstream>
ostream& operator<<(ostream& os, const String& text) {
    os << text.getData();
    return os;
}

istream& operator>>(istream& in, String& text) {
    text = ""; // Clear the current content of text
    char ch;
    while (in.get(ch) && ch != '\n'&&ch!=' ') {
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