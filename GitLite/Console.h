#pragma once
#include <iostream>
#include "String.h"
using namespace std;
class Console
{
private:
	String userInput;
	String command;
	String second;

public:
	Console();
	void run();
};