#pragma once
#include <stdio.h>
#include <assert.h>
#include <queue>
#include <vector>
#include <stack>

template<typename T>
struct BSTreeNode
{
	BSTreeNode* parent_;  //父节点
	BSTreeNode* left_;    //左孩子
	BSTreeNode* right_;    //右孩子
	T key_;                //key_用于比较大小
	BSTreeNode(const T& key)
		:parent_(nullptr),
		left_(nullptr),
		right_(nullptr),
		key_(key)
	{}
	~BSTreeNode()
	{
		printf("节点析构\n");

	}
};

template<typename T>
class BSTree
{
public:
	BSTree() :root_(nullptr),size_(0)
	{
		printf("构造\n");
	}
	~BSTree() 
	{
		printf("析构\n");

		destroy(root_);
	}
	void insert(const T& key);
	//递归实现遍历
	void inorderTraversal(std::vector<T>& v) const;
	void preorderTraversal() const;
	void postorderTraversal() const;
	//非递归实现遍历
	void preOrder() const;
	void InOrder() const;
	void postOrder() const;

	//求树的高度，sizeNum是叶子节点的数量
	size_t height(int* sizeNum) const;
	int getHeight() const { return getHeight(root_); }
	void reverseTree() { reverseTree(root_); }
	bool isEmpty() const { return size_ == 0; }
	size_t size() const { return size_; }
	//判断是否是完全二叉树
	bool isComplete() const;
	//查找一个节点的前驱节点
	BSTreeNode<T>* predecessor(BSTreeNode<T>* node);
	//查找一个节点的后继节点
	BSTreeNode<T>* successor(BSTreeNode<T>* node);
	BSTreeNode<T>* getNodeByKey(const T& key);

	//删除节点
	void remove(const T& key) ;
	void clear() { destroy(root_); size_ = 0; }
	
private:
	//内部插入节点
	void insert(BSTreeNode<T>** root, BSTreeNode<T>* z);
	//中序遍历, vector容器保存遍历后的关键字值
	void inorderTraversal(const BSTreeNode<T>* tree, std::vector<T>& v) const;
	//前序序遍历
	void preorderTraversal(const BSTreeNode<T>* root) const;
	//后序遍历
	void postorderTraversal(const BSTreeNode<T>* root) const;
	int getHeight(BSTreeNode<T>* root) const;
	void destroy(BSTreeNode<T>* root);
	//是否是叶子节点
	bool isLeaf(const BSTreeNode<T>* node) const 
	{ 
		return node->left_ == nullptr && node->right_ == nullptr; 
	}
	bool hasTwoChild(const BSTreeNode<T>* node) const 
	{ 
		return node->left_ != nullptr && node->right_ != nullptr;
	}
	void reverseTree(BSTreeNode<T>* root);

	BSTreeNode<T>* findMaxNode(BSTreeNode<T>* node);
	BSTreeNode<T>* findMinNode(BSTreeNode<T>* node);

	void remove(BSTreeNode<T>* removeNode);
	BSTreeNode<T> * root_;  //根节点
	size_t size_;
};
template<typename T>
void BSTree<T>::insert(const T& key)
{
	BSTreeNode<T> *newNode = new BSTreeNode<T>(key);
	insert(&root_,newNode);
	size_++;
}

template<typename T>
void BSTree<T>::insert(BSTreeNode<T>** root, BSTreeNode<T>* newNode)
{
	BSTreeNode<T>* parent = nullptr;
	BSTreeNode<T>* rootNode = *root;
	//找父节点，当退出循环的时候parent就是父节点
	while (rootNode != nullptr)
	{
		//parent必须在这里先指向
		parent = rootNode;
		if (rootNode->key_ < newNode->key_)
		{
			rootNode = rootNode->right_;
		}
		else if(rootNode->key_ > newNode->key_)
		{
			rootNode = rootNode->left_;
		}
		else
		{
			delete newNode;
			newNode = nullptr;
		}
	}

	newNode->parent_ = parent;
	//如果为空，则是根节点，所以直接让root指向新节点
	if (parent == nullptr)
	{
		*root = newNode;
		return;
	}
	
	
	if (parent->key_ < newNode->key_)
	{
		parent->right_ = newNode;
	}
	else if (parent->key_ > newNode->key_)
	{
		parent->left_ = newNode;
	}
	
}
template<typename T>
void BSTree<T>::inorderTraversal(const BSTreeNode<T>* tree,std::vector<T>& v) const
{
	if (tree == nullptr)
	{
		return;
	}
	inorderTraversal(tree->left_,v);
	v.push_back(tree->key_);
	inorderTraversal(tree->right_, v);
}
template<typename T>
void BSTree<T>::inorderTraversal(std::vector<T>& v) const
{
	inorderTraversal(root_, v);
}

template<typename T>
void BSTree<T>::preorderTraversal(const BSTreeNode<T>* root) const
{
	if (root == nullptr)
	{
		return;
	}
	printf("%d ", root->key_);
	preorderTraversal(root->left_);
	preorderTraversal(root->right_);
}
template<typename T>
void BSTree<T>::preorderTraversal() const
{
	preorderTraversal(root_);
}

template<typename T>
void BSTree<T>::postorderTraversal(const BSTreeNode<T>* root) const
{
	if (!root)
	{
		return;
	}
	postorderTraversal(root->left_);
	postorderTraversal(root->right_);
	printf(" %d", root->key_);
}

template<typename T>
void BSTree<T>::postorderTraversal() const
{
	postorderTraversal(root_);
}

template<typename T>
void BSTree<T>::preOrder() const
{
	if (!root_) return;
	std::stack<BSTreeNode<T>*> s;
	s.push(root_);
	while (!s.empty())
	{
		BSTreeNode<T>* node = s.top();
		s.pop();
		printf(" %d", node->key_);

		if (node->right_)
		{
			s.push(node->right_);
		}

		if (node->left_)
		{
			s.push(node->left_);
		}
	}

	printf("\n");
}

template<typename T>
void BSTree<T>::InOrder() const
{
	if (!root_) return;
	std::stack<BSTreeNode<T>*> s;
	BSTreeNode<T>* cur = root_;

	while (cur || !s.empty())
	{
		while (cur)
		{
			s.push(cur);
			cur = cur->left_;
		}

		BSTreeNode<T> * top = s.top();
		s.pop();
		printf(" %d", top->key_);
		if (top->right_)
		{
			cur = top->right_;
		}
	}
	printf("\n");
}

template<typename T>
void BSTree<T>::postOrder() const
{
	if (!root_) return;
	std::stack<BSTreeNode<T>*> s;
	BSTreeNode<T> * cur = root_;
	BSTreeNode<T>* after = nullptr;

	while (cur || !s.empty())
	{
		while (cur)
		{
			s.push(cur);
			cur = cur->left_;
		}

		BSTreeNode<T>* top = s.top();
		
		//after保存已经被访问的节点
		//并且当右孩子为NULL，或者是after等于了当前节点的右孩子，证明右孩子被遍历过
		if (top-> right_ == nullptr || after == top->right_)
		{
			printf(" %d", top->key_);
			s.pop();
			after = top;
		}
		else
		{
			cur = top->right_;
		}
	}
	printf("\n");
}
//求一个数的高度，通用算法
template<typename T>
size_t BSTree<T>::height(int * sizeNum) const
{
	if (!root_) return 0;
	std::queue<BSTreeNode<T>*> q;
	size_t tree_height = 0;
	//保存着每层的数量
	size_t level_size = 1;
	q.push(root_);
	while (!q.empty())
	{
		BSTreeNode<T>* node = q.front();
		if (isLeaf(node))
		{
			(*sizeNum)++;
		}
		q.pop();
		level_size--;
		if (node->left_ != nullptr)
		{
			q.push(node->left_);

		}
		if (node->right_ != nullptr)
		{
			q.push(node->right_);
		}
		//每层的最后一个元素已经出队，size为0，也意味着这个元素的子节点入队，
		//因此这时队列里面保存的就是全部的下一层元素
		//这时让size等于下一层的数量，也就是队列元素的数量
		if (level_size == 0)
		{
			level_size = q.size();
			tree_height++;
		}
	}

	return tree_height;
}

template<typename T>
int BSTree<T>::getHeight(BSTreeNode<T>* root) const
{
	if (!root)
	{
		return 0;
	}
	return 1 + std::max(getHeight(root->left_), getHeight(root->right_));
}

template<typename T>
bool BSTree<T>::isComplete() const
{
	if (!root_)
	{
		return false;
	}

	std::queue<BSTreeNode<T>*> q;
	q.push(root_);
	//叶子节点的标记位
	bool leaf = false;
	while (!q.empty())
	{
		BSTreeNode<T> *node = q.front();
		q.pop();
		
		if (leaf && !isLeaf(node))
		{
			return false;
		}
		
		if (node->left_ != nullptr)
		{
			q.push(node->left_);
		}
		else if (node->right_ != nullptr)
		{
			//node->left == NULL && node->right != NULL，一定不是完全二叉树
			return false;
		}

		if (node->right_ != nullptr)
		{
			q.push(node->right_);
		}
		else
		{
			//到了这个条件:node->right == NULL && node->left != NULL 或者node->right == NULL && node->left == NULL
			//那之后的节点必然是叶子节点
			leaf = true;
		}
	}

	return true;
}

template<typename T>
void BSTree<T>::reverseTree(BSTreeNode<T>* root)
{
	if (!root)
	{
		return;
	}

	std::swap(root->left_, root->right_);
	reverseTree(root->left_);
	reverseTree(root->right_);
}

template<typename T>
BSTreeNode<T>* BSTree<T>::predecessor(BSTreeNode<T>* node)
{
	if (!node) return nullptr;
	//1、若查找的节点左子树不为空，则node = node->right->right...直到node->right为空
	BSTreeNode<T>* p = node->left_;
	if (p)
	{
		while (p->right_)
		{
			p = p->right_;
		}

		return p;
	}
	//2、若查找的节点左子树为空，需要找父节点
	//若查找的节点是其父节点的右孩子，则它的父节点就是其前驱节点
	//若查找的节点是其父节点的左孩子，则找到父节点的最低父节点，并且查找的节点是父亲的右孩子，
	//3、既没有左子树，父节点也为空，则没有前驱节点
	BSTreeNode<T>* parent = node->parent_;
	while (parent && node == parent->left_)
	{
		node = parent;
		parent = parent->parent_;
	}

	return parent;
}

template<typename T>
BSTreeNode<T>* BSTree<T>::successor(BSTreeNode<T>* node)
{
	if (!node) return nullptr;
	//1、若查找的节点右子树不为空，则node = node->left->left...直到node->left为空
	BSTreeNode<T>* p = node->right_;
	if (p)
	{
		BSTreeNode<T>* maxNode = findMinNode(p);
		return maxNode;
	}
	//2、若查找的节点右子树为空，需要找父节点
	//若查找的节点是其父节点的左孩子，则它的父节点就是其后继节点
	//若查找的节点是其父节点的右孩子，则找到父节点的最低父节点，并且查找的节点是父亲的左孩子，
	//3、既没有左子树，父节点也为空，则没有前驱节点
	BSTreeNode<T>* parent = node->parent_;
	while (parent && node == parent->right_)
	{
		node = parent;
		parent = parent->parent_;
	}

	return parent;
}
template<typename T>
BSTreeNode<T>* BSTree<T>:: getNodeByKey(const T& key)
{
	if (!root_) return nullptr;
	BSTreeNode<T>* serch = root_;
	while (serch)
	{
		if (serch->key_ == key)
		{
			return serch;
		}
		else if (serch->key_ > key)
		{
			serch = serch->left_;
		}
		else
		{
			serch = serch->right_;
		}
	}

	return serch;
}

template<typename T>
BSTreeNode<T>* BSTree<T>::findMaxNode(BSTreeNode<T>* node)
{
	if (!node)
	{
		return node;
	}

	while (node->right_)
	{
		node = node->right_;
	}
	return node;
}

template<typename T>
inline BSTreeNode<T>* BSTree<T>::findMinNode(BSTreeNode<T>* node)
{
	if (!node)
	{
		return nullptr;
	}

	while (node->left_)
	{
		node = node->left_;
	}
	return node;
}

template<typename T>
void BSTree<T>::remove(const T& key) 
{
	BSTreeNode<T>* deleted = getNodeByKey(key);
	if(deleted)
		remove(deleted); 
}

template<typename T>
void BSTree<T>::remove(BSTreeNode<T>* removeNode)
{
	assert(removeNode);
	if (hasTwoChild(removeNode))
	{
		//找前驱节点，或者也可以找后继节点
		BSTreeNode<T>* pre = predecessor(removeNode);
		//如果是度为2的节点，则必然有前驱和后继
		assert(pre);
		//将前驱或者后继的节点的值覆盖掉被删除的节点的值
		removeNode->key_ = pre->key_;
		//让被删除节点指向前驱或者后继节点，而前驱节点或者后继节点必然是度为1或者0的点
		removeNode = pre;
	}

	//拿到左孩子或者右孩子
	BSTreeNode<T>* replaceNode = removeNode->left_ != nullptr ? removeNode->left_ : removeNode->right_;
	//拿到被删除节点的父亲
	BSTreeNode<T>* parent = removeNode->parent_;
	if (replaceNode) //度为1
	{
		//被删除节点的孩子的父亲指向被删除节点的父亲
		replaceNode->parent_ = parent;
		if (parent == nullptr)
		{
			root_ = replaceNode;
			delete removeNode;
			removeNode = nullptr;
		}
		else if (removeNode == parent->left_)
		{
			parent->left_ = replaceNode;
			delete removeNode;
			removeNode = nullptr;
		}
		else
		{
			parent->right_ = replaceNode;
			delete removeNode;
			removeNode = nullptr;
		}
	}
	else //度为0
	{
		if (parent == nullptr)
		{
			delete removeNode;
			removeNode = nullptr;
			root_ = nullptr;
		}
		else
		{
			if (removeNode == parent->left_)
			{
				parent->left_ = nullptr;
				delete removeNode;
				removeNode = nullptr;
			}
			else
			{
				parent->right_ = nullptr;
				delete removeNode;
				removeNode = nullptr;
			}
		}
	}
}
//通过后序遍历删除节点，不能通过前序和中序
//前序直接就断开了，不会有左右子节点了，
//中序是当左走完以后，直接删除掉，操作了一个删除掉的节点的右节点
template<typename T>
void BSTree<T>::destroy(BSTreeNode<T>* root)
{
	if (root == NULL)
	{
		return;
	}

	destroy(root->left_);
	destroy(root->right_);
	delete root;
	root = nullptr;
}
