#pragma once
#include "../GitLite/string2.h"

#include <filesystem>
using namespace filesystem;
class Console
{
private:
	String command;  // First Part
	String second;  // Second Part
	String third;
	path repoFolder = "";
	path currentBranchFolder = "";
	int treeType ;

public:
	Console();
	void run();
};