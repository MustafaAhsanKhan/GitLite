#include "Console.h"

#include <iostream>
using namespace std;
#include "../GitLite/AVL-Tree.h"
int main() {
    AVL<String> tree;

    // Insert nodes
    Vector<String> dataRow1 = { "Data for node 1" };
    tree.insert("Key1", dataRow1);

    Vector<String> dataRow2 = { "Data for node 2" };
    tree.insert("Key2", dataRow2);

    // Display in-order traversal
    tree.inOrder();

    return 0;
}
