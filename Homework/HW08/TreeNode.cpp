#include "TreeNode.hpp"
#include <stddef.h>
TreeNode::TreeNode() {
	data = 0;
	left = NULL;
	right = NULL;
}
TreeNode::TreeNode(int data) {
	this->data = data;
	this->left = NULL;
	this->right = NULL;
}
void TreeNode::setData(int data) { this->data = data; }
void TreeNode::setLeft(TreeNode* left) { this->left = left; }
void TreeNode::setRight(TreeNode* right) { this->right = right; }

int TreeNode::getData(void) { return this->data; }
TreeNode* TreeNode::getLeft(void) { return this->left; }
TreeNode* TreeNode::getRight(void) { return this->right; }
