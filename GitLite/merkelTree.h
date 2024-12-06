#pragma once
#include "string2.h"
#include <fstream>
#include "picosha2.h"
int instructorsHash(ifstream& ifs) {
	char ch;
	int hash = 1;
	while (ifs.get(ch)) {
		if (ch != '\n' && ch != ' ') {
			hash *= ch % 29;
		}
	}
	return hash%29;
}
int instructorsHash(String& str) {
	int hash = 1;
	for (int i = 0; i < str.size(); i++) {
		hash *= str[i] % 29;
	}
	return hash % 29;
}
int instructorsHash(int num) {
	int hash = 1, number = num, digit = 10;
	while (number) {
		hash *= (number % 10) % 29;
		number /= 10;
	}
	return hash % 29;
}