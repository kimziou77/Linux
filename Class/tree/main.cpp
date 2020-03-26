#include "Tree.hpp"
#include <iostream>
using namespace std;
int main() {
    Tree tree = Tree();

    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(100);
    tree.insert(30);
    tree.insert(10);
    tree.display();
    cout << endl;

    return 0;
}
