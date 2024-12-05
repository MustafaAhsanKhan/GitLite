#include "Console.h"

#include <iostream>
using namespace std;
#include "../GitLite/AVL-Tree.h"
int main()
{
	AVL<int> tree;

	// Constructing tree given in the 
	// above figure
	tree.insert(10);
	//tree.insert(20);
	//tree.insert(30);
	//tree.insert(40);
	//tree.insert(50);
	//tree.insert(25);
	//tree.insert(35);
	//tree.insert(45);

	//// The constructed AVL Tree would be
	////			 30
	////		    /  \
 //   //		  20    40
	////		 /  \   / \
 //   //		10   25 35 50
	////				   /
	////				 45

	//cout << "Preorder traversal of the constructed AVL tree is \n";
	////tree.LevelOrderTraversal();

	//// Output: 30 20 10 25 40 50

	//// deletion
	//tree.deleteNode(20);
	//tree.deleteNode(30);

	//// The AVL Tree after deletion of 20 and 30
	////			40
	////		   /  \
 //   //		  10   50		

	//cout << "\nPreorder traversal after deletion of 20 and 30 \n";
	////tree.LevelOrderTraversal();

	return 0;
}