#include "Tree.hpp"
#include "TreeNode.hpp"
#include <iostream>
#include <stddef.h>
Tree::Tree() { this->rootnode = NULL; }
Tree::Tree(int data) {
	TreeNode* node = new TreeNode(data);
	this->rootnode = node;
}
void Tree::insert(int data) {
	TreeNode* node = new TreeNode(data);
	TreeNode* cursor = this->rootnode;
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
		else {
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
void Tree::display(TreeNode* curNode) {
	if (curNode == NULL)
		return;
	if (curNode->getData() == NULL)
		return;
	display(curNode->getLeft());
	std::cout << " ";
	std::cout << curNode->getData();
	display(curNode->getRight());

	return;
}
bool Tree::Nsearch(int data) {
	TreeNode* tmp = this->rootnode;
	while (1) {
		if (tmp == NULL)
			return false;
		if (tmp->getData() == data)
			return true;
		else if (tmp->getData() > data) {
			tmp = tmp->getLeft();
			continue;
		}
		else if (tmp->getData() < data) {
			tmp = tmp->getRight();
			continue;
		}
	}
}
bool Tree::search(int data) {
	TreeNode* tmp = this->rootnode;
	while (1) {
		if (tmp == NULL) {
			std::cout << std::endl << data << " search failed\n";
			return false;
		}
		std::cout << tmp->getData() << " ";

		if (tmp->getData() == data) {
			std::cout << std::endl << data << " search success!\n";
			return true;
		}
		else if (tmp->getData() > data) {
			tmp = tmp->getLeft();
			continue;
		}
		else if (tmp->getData() < data) {
			tmp = tmp->getRight();
			continue;
		}
	}
}
TreeNode* Tree::dnode(int data) {
	TreeNode* tmp = this->rootnode;
	TreeNode* prev = tmp;
	while (1) {
		if (tmp == NULL) {
			return NULL;
		}
		if (tmp->getData() == data) {
			return tmp;
		}
		else if (tmp->getData() > data) {
			prev = tmp;
			tmp = tmp->getLeft();
			continue;
		}
		else if (tmp->getData() < data) {
			prev = tmp;
			tmp = tmp->getRight();
			continue;
		}
	}

}
void Tree::remove(int data) {

	if (Nsearch(data)) {
		TreeNode* data_node = dnode(data);
		if (data_node->getLeft()) {
			TreeNode* max_node = find_Max(data_node->getLeft());
			data_node->setData(max_node->getData());
			TreeNode* tmp = max_node;
			max_node = max_node->getLeft();
			data_node->setLeft(max_node);
			delete tmp;
			tmp = NULL;
		}
		else if(data_node->getRight()) {
			//left가없고 Right는 있을때
			TreeNode* origin = data_node;
			TreeNode * next = data_node->getRight();
			origin->setData(next->getData());
			origin->setLeft(next->getLeft());
			origin->setRight(next->getRight());
			delete next;
		}
		else {//left도없고 Right도 없을때
			data_node->setData(NULL);
			data_node->setLeft(NULL);
			data_node->setRight(NULL);

		}
		std::cout << data << " remove success!\n";
	}
	else {
		std::cout << data << " remove failed\n";
	}
}
TreeNode* Tree::find_Max(TreeNode* cur_node) {
	TreeNode* tmp = cur_node;
	TreeNode* prev = tmp;

	while (tmp->getRight()) {
		prev = tmp;
		tmp = tmp->getRight();
	}
	return prev;
}
