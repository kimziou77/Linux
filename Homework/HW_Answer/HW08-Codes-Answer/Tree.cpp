#include "Tree.hpp"
#include "TreeNode.hpp"
#include <iostream>
#include <stddef.h>

Tree::Tree() { this->rootnode = NULL; }
Tree::Tree(int data) {
    TreeNode *node = new TreeNode(data);
    this->rootnode = node;
}
Tree::~Tree() {
    delete rootnode->getLeft();
    delete rootnode->getRight();
    delete rootnode;
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
            } else {
                cursor = cursor->getRight();
            }
        } else {
            if (cursor->getLeft() == NULL) {
                cursor->setLeft(node);
                break;
            } else {
                cursor = cursor->getLeft();
            }
        }
    }
    return;
}
void Tree::display() {
    display(this->rootnode, 0);
    return;
}
void Tree::display(TreeNode *curNode, int depth) {
    int i;
    if (curNode == NULL)
        return;
    display(curNode->getRight(), depth + 1);
    for (i = 0; i < depth; i++) {
        std::cout << "   ";
    }
    std::cout << curNode->getData() << std::endl;
    display(curNode->getLeft(), depth + 1);
    return;
}
bool Tree::search(int data) {
    bool result;
    result = search(this->rootnode, data);
    return result;
}
bool Tree::search(TreeNode *curNode, int data) {
    if (curNode == NULL) {
        std::cout << std::endl;
        std::cout << data << " search failed" << std::endl;
        return false;
    }
    if (curNode->getData() == data) {
        std::cout << " " << curNode->getData() << std::endl;
        std::cout << data << " search success!" << std::endl;
        return true;
    }
    if (curNode->getData() > data) {
        std::cout << " " << curNode->getData();
        return search(curNode->getLeft(), data);
    } else {
        std::cout << " " << curNode->getData();
        return search(curNode->getRight(), data);
    }
}
bool Tree::remove(
    int data) { // public의 remove가 호출되면 private의 remove를 호출
    bool result;
    result = remove(NULL, this->rootnode, data);
    return result;
}
bool Tree::remove(TreeNode *preNode, TreeNode *curNode, int data) {
    if (curNode == NULL) { // 삭제할 노드를 찾지 못한 경우
        std::cout << std::endl;
        std::cout << data << " remove failed" << std::endl;
        return false;
    }
    TreeNode *left = curNode->getLeft();
    TreeNode *right = curNode->getRight();
    if (curNode->getData() == data) { // 삭제할 노드를 찾은 경우
        if (preNode == NULL) {        // 루트 노드인 경우
            if (left == NULL &&
                right == NULL) { // 좌측과 우측 노드가 모두 없는 경우
                this->rootnode = NULL;
            } else if (left == NULL && right != NULL) { // 우측 노드만 있는 경우
                this->rootnode = right;
            } else if (left != NULL && right == NULL) { // 좌측 노드만 있는 경우
                this->rootnode = left;
            } else { // 좌측 노드와 우측 노드가 모두 있는 경우
                TreeNode *maxNode = find_max_node(
                    curNode, left); // 좌측 노드에서 가장 큰 노드를 찾는 함수
                maxNode->setLeft(curNode->getLeft());
                maxNode->setRight(curNode->getRight());
                this->rootnode = maxNode;
            }
        } else { // 루트 노드가 아닌 경우
            if (left == NULL &&
                right == NULL) { // 좌측과 우측 노드가 모두 없는 경우
                if (preNode->getLeft() == curNode) {
                    preNode->setLeft(NULL);
                } else {
                    preNode->setRight(NULL);
                }
            } else if (left == NULL && right != NULL) { // 우측 노드만 있는 경우
                if (preNode->getLeft() == curNode) {
                    preNode->setLeft(right);
                } else {
                    preNode->setRight(right);
                }

            } else if (left != NULL && right == NULL) { // 좌측 노드만 있는 경우
                if (preNode->getLeft() == curNode) {
                    preNode->setLeft(left);
                } else {
                    preNode->setRight(left);
                }
            } else { // 좌측 노드와 우측 노드가 모두 있는 경우
                TreeNode *maxNode = find_max_node(
                    curNode, left); // 좌측 노드에서 가장 큰 노드를 찾는 함수
                maxNode->setLeft(curNode->getLeft());
                maxNode->setRight(curNode->getRight());
                if (preNode->getLeft() == curNode) {
                    preNode->setLeft(maxNode);
                } else {
                    preNode->setRight(maxNode);
                }
            }
        }
        delete curNode;
        std::cout << data << " remove success!" << std::endl;
        return true;
    } else { // 탐색하는 노드가 삭제하려는 노드가 아닌 경우
        if (curNode->getData() > data) {
            return remove(curNode, left, data);
        } else {
            return remove(curNode, right, data);
        }
    }
}
TreeNode *Tree::find_max_node(TreeNode *preNode, TreeNode *curNode) {
    TreeNode *pre = preNode;
    TreeNode *cur = curNode;
    TreeNode *left;
    TreeNode *right;
    while (1) {
        left = cur->getLeft();
        right = cur->getRight();
        if (right ==
            NULL) { // 탐색하는 노드에 우측 노드가 더이상 없는 경우 가장 큰 노드
            if (left == NULL) { // 가장 큰 노드의 좌측 노드가 없는 경우
                if (pre->getLeft() == cur) {
                    pre->setLeft(NULL);
                } else {
                    pre->setRight(NULL);
                }
            } else { // 가장 큰 노드의 좌측 노드가 있는 경우
                if (pre->getLeft() == cur) {
                    pre->setLeft(cur->getLeft());
                } else {
                    pre->setRight(cur->getLeft());
                }
            }
            return cur;
        }
        if (right == NULL) {
            pre = cur;
            cur = left;
        } else {
            pre = cur;
            cur = right;
        }
    }
}
