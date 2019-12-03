#ifndef __TREE_H__
#define __TREE_H__

#include "TreeNode.hpp"

class Tree {
  public:
    Tree();
    Tree(int data);
    ~Tree();

    void insert(int data);
    void display();
    bool search(int data);
    bool remove(int data);

  private:
    void display(TreeNode *curNode, int depth);
    bool search(TreeNode *curNode, int data);
    bool remove(TreeNode *preNode, TreeNode *curNode, int data);
    TreeNode *find_max_node(TreeNode *preNode, TreeNode *curNode);
    TreeNode *rootnode;
};
#endif
