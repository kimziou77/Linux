#include "Tree.hpp"
#include <iostream>

using namespace std;
int main() {
    Tree tree = Tree();
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(90);
    tree.insert(10);
    tree.insert(35);
    tree.insert(5);
    tree.insert(15);
    tree.insert(30);
    tree.insert(80);
    tree.insert(100);
    cout << "--- Tree display ---" << endl;
    tree.display();
    cout << endl << endl;

    cout << "--- Tree search ---" << endl;
    tree.search(30);
    tree.search(80);
    tree.search(40);
    cout << endl;

    cout << "--- Tree remove ---" << endl;
    tree.remove(2);
    tree.display();
    cout << endl << endl;
    tree.remove(100);
    tree.display();
    cout << endl << endl;
    tree.remove(75);
    tree.display();
    cout << endl << endl;
    tree.remove(25);
    tree.display();
    cout << endl << endl;
    tree.remove(50);
    tree.display();
    cout << endl << endl;
    return 0;
}
