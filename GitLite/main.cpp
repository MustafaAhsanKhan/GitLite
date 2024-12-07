#include "Console.h"
////#include "../GitLite/BTreeNode.h"
//#include <fstream>
//#include <iostream>
//int main()
//{
//	ifstream file("input.txt");
//	String filetext;
//	cin >> filetext;
//	cout << filetext;
//	return 0;
//}

#include <iostream>
using namespace std;
//#include "../GitLite/AVL-Tree.h"
#include "string2.h"
#include "BTreeFileSystem.h"
#include "merkleTree.h"
int main()
{
	//MerkleTree tree(1);
	//tree.generateFromFolder("tree");
	MerkleTree::getChangedFiles("merkle\\14.txt", "merkle2\\14.txt");

	//BTree<int, 4> tree("tree");
	//for (int i = 0; i < 10000; i++) {
	//	tree.insert(i);







	//}
	
	//cout << "pappo";
	/*
	for (int i = 0; i < 998; i++) {
		tree.remove(i);

	}*/
	//BTreeNode<string, 4> node("root.txt");

	//BTree<int, 4> tree("tree\\");
	//for (int i = 1; i <=10; i++) {
		//tree.insert(i);
		//tree.print();
		//cout << "-----------------";
	//}

	//for (int i = 1; i <= 124; i++) {
		//tree.remove(i);
		//cout << "-----------------";
	//}

	//Console C1;
	//C1.run();  // Program Loop
	//AVL tree;

	//// Constructing tree given in the 
	//// above figure
	//tree.insert(10);
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
	//tree.LevelOrderTraversal();

	//// Output: 30 20 10 25 40 50

	//// deletion
	//tree.deleteNode(20);
	//tree.deleteNode(30);

	//// The AVL Tree after deletion of 20 and 30
	////			40
	////		   /  \
 //   //		  10   50		

	//cout << "\nPreorder traversal after deletion of 20 and 30 \n";
	//tree.LevelOrderTraversal();

	return 0;
}