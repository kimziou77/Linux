#include "Tree.hpp"
#include "TreeNode.hpp"
#include <iostream>
#include <stddef.h>
Tree::Tree() { this->rootnode = NULL; }
Tree::Tree(int data) {
TreeNode *node = new TreeNode(data);
this->rootnode = node;
}
void Tree::insert(int data) {
    TreeNode *node = new TreeNode(data);
    TreeNode *cursor = this->rootnode;
    if (cursor == NULL) {
        this->rootnode = node;
        return;
    }
    while (1) {
        int value = cursor->getData();
        if (value < data) {
            if (cursor->getRight() == NULL) {
                cursor->setRight(node);
                break;
            }
            else {
                cursor = cursor->getRight();
            }
        }
        else{
            if (cursor->getLeft() == NULL) {
                cursor->setLeft(node);
                break;
            }
            else {
                cursor = cursor->getLeft();
            }
        }
    }
    return;
}
void Tree::display() {
    display(this->rootnode);
    return;
}
void Tree::display(TreeNode *curNode) {
    if (curNode == NULL)
        return;
    display(curNode->getLeft());
    std::cout << " ";
    std::cout << curNode->getData();
    display(curNode->getRight());

    return;
}
