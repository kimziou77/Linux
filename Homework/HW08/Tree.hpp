#pragma once
#ifndef __TREE_H__
#define __TREE_H__
#include "TreeNode.hpp"
class Tree {
public:
	Tree();
	Tree(int data);
	void insert(int data);
	void remove(int data);
	void display();
	bool search(int data);
	TreeNode* find_Max(TreeNode* root_node);
private:
	bool Nsearch(int data);
	TreeNode* dnode(int data);
	void display(TreeNode* curNode);
	TreeNode* rootnode;
};
#endif
