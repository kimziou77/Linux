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
    cout << "--- Tree display ---" << endl;
    tree.display();
    cout << endl << endl;

    cout << "--- Tree search ---" << endl;
    tree.search(15);
    tree.search(30);
    cout << endl;

    cout << "--- Tree remove ---" << endl;
    tree.remove(15);
    tree.display();
    cout << endl << endl;
    tree.remove(25);
    tree.display();
    cout << endl << endl;
    tree.remove(75);
    tree.display();
    cout << endl << endl;
    return 0;
}
