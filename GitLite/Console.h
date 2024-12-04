#pragma once
#include "../GitLite/string2.h"
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