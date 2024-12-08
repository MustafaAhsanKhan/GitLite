#pragma once
#include "AVL-Tree.h"
#include "RB-Tree.h"
class Console
{
private:
	String command;  // First Part
	String second;  // Second Part
	String third;  // Third Part
	int selectedTree;  // 1 for AVL, 2 for B-Tree, 3 for Red-Black Tree

public:
	AVL avl;
	RBTree rb;
	void parseCsv(int selectedTree, String second);
	Console();
	void run();
};