#pragma once
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <algorithm>
template<typename T>
struct AVLTreeNode
{
	AVLTreeNode* parent_;  //父节点
	AVLTreeNode* left_;    //左孩子
	AVLTreeNode* right_;    //右孩子
	T key_;                //key_用于比较大小
	int height_ = 1;
	AVLTreeNode(const T& key)
		:parent_(nullptr),
		left_(nullptr),
		right_(nullptr),
		key_(key)
	{}
	~AVLTreeNode()
	{
		printf("节点析构\n");

	}
};
template<typename T>
class AVLTree
{
public:
	AVLTree() {}
	~AVLTree() {}
	
	void insert(const T& key);
	void inorder()
	{
		inorder(root_);
	}

	void remove(const T& key)
	{
		AVLTreeNode<T>* deleteNode = search(key);
		if (deleteNode)
		{
			remove(deleteNode);
		}
	}
private:
	void inorder(const AVLTreeNode<T>* root)
	{
		if (!root)
		{
			return;
		}

		inorder(root->left_);
		printf("%d %d\n", root->key_, root->height_);
		inorder(root->right_);
	}

	bool isBalance(const AVLTreeNode<T>* node);
	//拿到平衡因子
	int balanceFactor(const AVLTreeNode<T>* node);
	//更新高度
	void updateHeight(AVLTreeNode<T>* node);

	void addNodeAndRebalance(AVLTreeNode<T>* node);
	//左旋
	void leftRotate(AVLTreeNode<T>* grand);
	//右旋
	void rightRotate(AVLTreeNode<T>* grand);
	//更新节点的父亲属性
	void updateParent(AVLTreeNode<T>* grand, AVLTreeNode<T>* parent, AVLTreeNode<T>* child);
	//重置平衡
	void reBalance(AVLTreeNode<T>* node);
	//拿到高度更高的孩子
	AVLTreeNode<T>* highterChild(AVLTreeNode<T>* node);
	bool isLeftChild(const AVLTreeNode<T>* node)
	{
		return node->parent_ != NULL && node == node->parent_->left_;
	}
	bool isRightChild(const AVLTreeNode<T>* node)
	{
		return node->parent_ != NULL && node == node->parent_->right_;
	}

	AVLTreeNode<T>* search(const T& key);
	bool hasTwoChild(const AVLTreeNode<T>* node)
	{
		return node->left_ != nullptr && node->right_ != nullptr;
	}
	void remove(AVLTreeNode<T>* removeNode);
	AVLTreeNode<T>* predecessor(AVLTreeNode<T>* node);
	void afterRemoveRebalance(AVLTreeNode<T>* node);
	AVLTreeNode<T>* root_;
};

template<typename T>
void AVLTree<T>::insert(const T& key)
{
	if (!root_)
	{
		AVLTreeNode<T>* newNode = new AVLTreeNode<T>(key);
		root_ =  newNode;
		//addNodeAndRebalance(newNode);
		return;
	}
	
	AVLTreeNode<T>* parent = NULL;
	AVLTreeNode<T>* serch = root_;
	while (serch)
	{
		parent = serch;
		if (serch->key_ < key)
		{
			serch = serch->right_;
		}
		else if (serch->key_ > key)
		{
			serch = serch->left_;
		}
		else //相等直接返回
			return;
	}
	assert(parent);
	AVLTreeNode<T>* newNode = new AVLTreeNode<T>(key);
	newNode->parent_ = parent;
	if (parent->key_ < key)
	{
		parent->right_ = newNode;
	}
	else if (parent->key_ > key)
	{
		parent->left_ = newNode;
	}
	
	addNodeAndRebalance(newNode);
}

template<typename T>
bool AVLTree<T>::isBalance(const AVLTreeNode<T>* node)
{
	return -1<= balanceFactor(node) && balanceFactor(node) <= 1;
}

template<typename T>
int AVLTree<T>::balanceFactor(const AVLTreeNode<T>* node)
{
	assert(node);
	//拿到做子树的高度
	int leftHight = node->left_ == nullptr ? 0 : node->left_->height_;
	//拿到右子树的高度
	int rightHight = node->right_ == nullptr ? 0 : node->right_->height_;
	return leftHight - rightHight;
}

template<typename T>
void AVLTree<T>::updateHeight(AVLTreeNode<T>* node)
{
	int leftHight = node->left_ == nullptr ? 0 : node->left_->height_;
	int rightHight = node->right_ == nullptr ? 0 : node->right_->height_;
	node->height_ = std::max(leftHight , rightHight ) + 1 ;
}
template<typename T>
void AVLTree<T>::addNodeAndRebalance(AVLTreeNode<T>* node)
{
	assert(node);
	AVLTreeNode<T>* parent = node->parent_;
	while (parent)
	{
		if (isBalance(parent))
		{
			updateHeight(parent);
			
		}
		else
		{
			reBalance(parent);
			//printf("is not balance: %d\n", parent->height_);
			break;
		}
		parent = parent->parent_;
	
	}
}
template<typename T>
void AVLTree<T>::leftRotate(AVLTreeNode<T>* grand)
{
	AVLTreeNode<T>* newParent = grand->right_;
	AVLTreeNode<T>* leftChild = newParent->left_;
	grand->right_ = leftChild;
	newParent->left_ = grand;

	updateParent(grand,newParent,leftChild);
	updateHeight(grand);
	updateHeight(newParent);
}
template<typename T>
void AVLTree<T>::rightRotate(AVLTreeNode<T>* grand)
{
	AVLTreeNode<T>* newParent = grand->left_;
	AVLTreeNode<T>* rightChild = newParent->right_;
	grand->left_ = rightChild;
	newParent->right_ = grand;
	updateParent(grand, newParent, rightChild);
	updateHeight(grand);
	updateHeight(newParent);
}
template<typename T>
void AVLTree<T>::updateParent(AVLTreeNode<T>* grand, AVLTreeNode<T>* parent, AVLTreeNode<T>* child)
{
	parent->parent_ = grand->parent_;
	if (isLeftChild(grand))
	{
		grand->parent_->left_ = parent;
	}
	else if (isRightChild(grand))
	{
		grand->parent_->right_ = parent;
	}
	else
	{
		root_ = parent;
	}

	grand->parent_ = parent;
	if (child)
		child->parent_ = grand;
}
template<typename T>
void AVLTree<T>::reBalance(AVLTreeNode<T>* node)
{
	//node代表祖父节点
	AVLTreeNode<T>* parent = highterChild(node);
	AVLTreeNode<T>* child = highterChild(parent);
	if (isLeftChild(parent))
	{
		if (isLeftChild(child))
		{
			rightRotate(node);
		}
		else
		{
			leftRotate(parent);
			rightRotate(node);
		}
	}
	else
	{
		if (isRightChild(child))
		{
			leftRotate(node);
		}
		else
		{
			rightRotate(parent);
			leftRotate(node);
		}
	}
}
template<typename T>
AVLTreeNode<T>* AVLTree<T>::highterChild(AVLTreeNode<T>* node)
{
	assert(node);
	int leftHight = node->left_ == nullptr ? 0 : node->left_->height_;
	int rightHight = node->right_ == nullptr ? 0 : node->right_->height_;
	if (leftHight > rightHight)
	{
		return node->left_;
	}
	if (rightHight > leftHight)
	{
		return node->right_;
	}
	return isLeftChild(node) ? node->left_ : node->right_;
}
template<typename T>
AVLTreeNode<T>* AVLTree<T>::search(const T& key)
{
	AVLTreeNode<T>* p = root_;
	while (p)
	{
		if (p->key_ < key)
		{
			p = p->right_;
		}
		else if (p->key_ > key)
		{
			p = p->left_;
		}
		else
		{
			return p;
		}
	}

	return p;
}
template<typename T>
AVLTreeNode<T>* AVLTree<T>::predecessor(AVLTreeNode<T>* node)
{
	AVLTreeNode<T>* leftChild = node->left_;
	if (leftChild)
	{
		while (leftChild->right_)
		{
			leftChild = leftChild->right_;
		}
		return leftChild;
	}

	AVLTreeNode<T>* parent = node->parent_;
	while (parent && node == parent->left_)
	{
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}
template<typename T>
void AVLTree<T>::remove( AVLTreeNode<T>* removeNode)
{
	assert(removeNode);
	if (hasTwoChild(removeNode))
	{
		AVLTreeNode<T>* pre = predecessor(removeNode);
		removeNode->key_ = pre->key_;
		removeNode = pre;
	}

	AVLTreeNode<T>* child = removeNode->left_ != nullptr ? removeNode->left_ : removeNode->right_;
	AVLTreeNode<T>* parent = removeNode->parent_;
	AVLTreeNode<T>* node = removeNode;
	if (child)
	{
		child->parent_ = parent;
		if (removeNode == parent->left_)
		{
			parent->left_ = child;
			delete removeNode;
			removeNode = nullptr;
		}
		else if (removeNode == parent->right_)
		{
			parent->right_ = child;
			delete removeNode;
			removeNode = nullptr;
		}
		else
		{
			root_ = child;
			delete removeNode;
			removeNode = nullptr;
		}
		afterRemoveRebalance(child);

	}
	else
	{
		printf("is leaf \n");
		if (removeNode == parent->left_)
		{
			parent->left_ = nullptr;
			afterRemoveRebalance(node);

			delete removeNode;
			removeNode = nullptr;
		}
		else if (removeNode == parent->right_)
		{
			parent->right_ = nullptr;
			afterRemoveRebalance(node);

			delete removeNode;
			removeNode = nullptr;
		}
		else
		{
			root_ = nullptr;
			delete removeNode;
			removeNode = nullptr;
		}
		//afterRemoveRebalance(node);
	}
}

template<typename T>
void AVLTree<T>::afterRemoveRebalance(AVLTreeNode<T>* node)
{
	
	if (!node)
	{
		return;
	}
	AVLTreeNode<T>* parent = node->parent_;
	while (parent)
	{
		if (isBalance(parent))
		{
			updateHeight(parent);
		}
		else
		{
			reBalance(parent);
		}

		parent = parent->parent_;
	}
}