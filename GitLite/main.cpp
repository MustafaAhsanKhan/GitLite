#include "../GitLite/Console.h"
//#include "../GitLite/BTreeNode.h"

int main()
{
	Console C1;
	C1.run();  // Program Loop
	return 0;
}

//#include <iostream>
//using namespace std;
//#include "../GitLite/AVL-Tree.h"
//int main()
//{
//	//BTree<int, 4> tree;
//	//tree.insert(1);
//	//cout << "-----------------";
//	//tree.insert(2);
//	//cout << "-----------------";
//	//tree.insert(3);
//	//cout << "-----------------";
//	//tree.insert(4);
//	//cout << "-----------------";
//	//tree.insert(5);
//	//cout << "-----------------";
//	//tree.insert(6);
//	//cout << "-----------------";
//	//tree.insert(7);
//	//cout << "-----------------";
//
//	//for (int i = 0; i < 50; i++) {
//	//	tree.insert(i);
//	//	cout << "-----------------";
//	//}
//
//	//Console C1;
//	//C1.run();  // Program Loop
//	AVL tree;
//
//	// Constructing tree given in the 
//	// above figure
//	tree.insert(10);
//	tree.insert(20);
//	tree.insert(30);
//	tree.insert(40);
//	tree.insert(50);
//	tree.insert(25);
//	tree.insert(35);
//	tree.insert(45);
//
//	// The constructed AVL Tree would be
//	//			 30
//	//		    /  \
//    //		  20    40
//	//		 /  \   / \
//    //		10   25 35 50
//	//				   /
//	//				 45
//
//	cout << "Preorder traversal of the constructed AVL tree is \n";
//	tree.LevelOrderTraversal();
//
//	// Output: 30 20 10 25 40 50
//
//	// deletion
//	tree.deleteNode(20);
//	tree.deleteNode(30);
//
//	// The AVL Tree after deletion of 20 and 30
//	//			40
//	//		   /  \
//    //		  10   50		
//
//	cout << "\nPreorder traversal after deletion of 20 and 30 \n";
//	tree.LevelOrderTraversal();
//
//	return 0;
//}