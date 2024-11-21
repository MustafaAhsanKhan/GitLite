#pragma once
#include <iostream>
#include "String.h"
using namespace std;
class Console
{
private:
	String command;  // First Part
	String second;  // Second Part
	String third;

public:
	Console();
	void run();
};