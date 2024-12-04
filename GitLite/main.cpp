#include "../GitLite/Console.h"
#include "../GitLite/BTreeNode.h"
int main()
{
	BTree<int, 4> tree;
	//tree.insert(1);
	//cout << "-----------------";
	//tree.insert(2);
	//cout << "-----------------";
	//tree.insert(3);
	//cout << "-----------------";
	//tree.insert(4);
	//cout << "-----------------";
	//tree.insert(5);
	//cout << "-----------------";
	//tree.insert(6);
	//cout << "-----------------";
	//tree.insert(7);
	//cout << "-----------------";

	for (int i = 0; i < 50; i++) {
		tree.insert(i);
		cout << "-----------------";

	}

	//Console C1;
	//C1.run();  // Program Loop
	return 0;
}