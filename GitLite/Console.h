#pragma once
#include "AVL-Tree.h"
#include "RB-Tree.h"
#include "../GitLite/string2.h"

#include <filesystem>
using namespace filesystem;
class Console
{
private:
	String command;  // First Part
	String second;  // Second Part
	String third;  // Third Part
	int selectedTree;  // 1 for AVL, 2 for B-Tree, 3 for Red-Black Tree
	String third;
	path repoFolder = "";
	path currentBranchFolder = "";
	int treeType ;

public:
	AVL avl;
	RBTree rb;
	void parseCsv(int selectedTree, String second);
	Console();
	void run();
};