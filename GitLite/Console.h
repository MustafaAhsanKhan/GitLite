#pragma once
#include "String.h"
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