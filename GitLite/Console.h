#pragma once
#include "AVL-Tree.h"
#include "RB-Tree.h"
#include "BTreeFileSystem.h"
#include "String.h"
#include <filesystem>
using namespace filesystem;
class Console
{
private:
	String command;  // First Part
	String second;  // Second Part
	String third;  // Third Part
	int treeType;  // 1 for AVL, 2 for B-Tree, 3 for Red-Black Tree
	path repoFolder = "";
	path currentBranchFolder = "";

public:
	AVL avl;
	RBTree rb;
	//BTree<int, 3> bt;
	void parseCsv(int selectedTree, String second, path treeDir);
	Console();
	void run();
};