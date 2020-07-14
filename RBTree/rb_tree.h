#pragma once
#include <stdio.h>
#include <assert.h>
#include <memory>
#include <iterator>

int num = 0;
template<typename VALUE>
struct RBTreeNode
{
	typedef RBTreeNode<VALUE> RBNode;
	typedef RBNode* RBTreeNodePtr;
	RBTreeNode() {}

	RBTreeNode(const VALUE& v)
		:left(nullptr),
		right(nullptr),
		parent(nullptr),
		value(v),
		color(RED)
	{}

	~RBTreeNode() = default;

	RBTreeNodePtr left;
	RBTreeNodePtr right;
	RBTreeNodePtr parent;
	VALUE value;
	bool color;

	static const bool RED = true;
	static const bool BLACK = false;
};

template<typename VALUE, typename Ptr, typename Ref>
struct RBTreeIterator:public std::iterator<std::bidirectional_iterator_tag, VALUE>
{
	typedef Ptr pointor;
	typedef Ref reference;
	/*typedef VALUE* pointor;
	typedef VALUE& reference;*/
	typedef RBTreeNode<VALUE> RBNode;
	typedef RBTreeNode<VALUE>* RBTreeNodePtr;
	RBTreeIterator() {}
	RBTreeIterator(RBTreeNodePtr node) :nodePtr(node) {}
	//RBTreeIterator(const RBTreeIterator& iter) :nodePtr(iter.nodePtr) {}

	pointor operator->()
	{
		return &nodePtr->value;
	}
	reference& operator*()
	{
		return *(operator->());
	}

	RBTreeIterator& operator++()
	{
		increment();
		return *this;
	}

	RBTreeIterator operator++(int)
	{
		RBTreeIterator temp(*this);
		increment();
		return temp;
	}

	RBTreeIterator& operator--()
	{
		decrement();
		return *this;
	}

	RBTreeIterator operator--(int)
	{
		RBTreeIterator temp(*this);
		decrement();
		return temp;
	}

	bool operator==(const RBTreeIterator& iter) const { return nodePtr == iter.nodePtr; }
	bool operator!=(const RBTreeIterator& iter) const { return nodePtr != iter.nodePtr; }

	void increment()
	{
		if (nodePtr->right)
		{
			nodePtr = nodePtr->right;
			while (nodePtr->left)
			{
				nodePtr = nodePtr->left;
			}
		}
		else
		{
			//如果nodePtr指向根节点，并且根节点没有右节点的话，那么根节点就是最大
			//那么，m_header就是其父亲，m_header的右节点必然是根节点，就会进入循环
			//循环结束以后，nodePtr来到了m_header， 父节点就是根节点，因此退出循环
			//这时nodePtr的右指向的就是根节点,如果相等则代表到了end()，不会更新指向了
			RBTreeNodePtr parent = nodePtr->parent;
			while (nodePtr == parent->right)
			{
				nodePtr = parent;
				parent = parent->parent;
			}

			if (nodePtr->right != parent)
			{
				nodePtr = parent;
			}
		}
	}

	void decrement()
	{
		//如果是end()，end()就是m_header节点，那么与根节点的parent互相指向
		//m_header节点必须是红色，假如是黑色，如果nodePtr指向根节点，那么后面的parent属性互相指向成立
		//那么--操作就不会是最大的，而是左边最大的
		if (nodePtr->color == RBNode::RED && nodePtr == nodePtr->parent->parent)
		{
			nodePtr = nodePtr->right;
			return;
		}
		if (nodePtr->left)
		{
			nodePtr = nodePtr->left;
			while (nodePtr->right)
			{
				nodePtr = nodePtr->right;
			}
		}
		else
		{
			RBTreeNodePtr parent = nodePtr->parent;
			while (nodePtr == parent->left)
			{
				nodePtr = parent;
				parent = parent->parent;
			}
			//指向根节点，且根节点的左为空，那么会进入上面的循环，循环完之后nodePtr就是header节点
			if(nodePtr->left != parent) 
				nodePtr = parent;
		}
	}

	RBTreeNodePtr nodePtr;
};

template<typename T, typename VALUE, typename Compare = std::less<T>>
class RBTree
{
public:
	typedef T key_type;
	typedef size_t size_type;
	typedef std::pair<T, VALUE> value_type;
	//TODO:如果类模板RBTreeIterator<VALUE>只有一个模板参数，怎么设计const_iterator？
	typedef RBTreeIterator<value_type, value_type*, value_type&> iterator;
	typedef RBTreeIterator<value_type, const value_type*, const value_type&> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef typename iterator::difference_type difference_type;

	typedef RBTreeNode<value_type> Node;
	typedef typename iterator::RBTreeNodePtr RBTreeNodePtr;

	//typedef RBTreeNode<const value_type>* constRBTreeNodePtr;
	//typedef select1st<value_type> KeyOfValue;
	//typedef RBTree<T, VALUE, Compare> RBTree;
	RBTree()
        :m_size(0)
	{
		//m_header.reset(new Node());
		init();
	}
	
	~RBTree()
	{
		if (m_header)
		{
			destroy(m_header->parent);
			delete m_header;
			m_header = nullptr;
		}
		
	}
	RBTree(const RBTree& tree);
	RBTree& operator=(const RBTree& tree);
	RBTree(RBTree&&);
	RBTree& operator=(RBTree&&);
	void inorder()
	{
		inorder(m_header->parent);
	}

	iterator begin()
	{
		iterator iterator(m_header->left);
		return iterator;
	}
	iterator end() { return iterator(m_header); }
	const_iterator cbegin() const{ return const_iterator(m_header->left); }
	const_iterator cend() const{ return const_iterator(m_header); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator crbegin() { return const_reverse_iterator(end()); }
	const_reverse_iterator crend() { return const_reverse_iterator(begin()); }

	iterator insert(const value_type& value);
	iterator insert(iterator positon, const value_type& value);
	template<typename InputIterator>
	iterator insert(InputIterator first, InputIterator last);
	size_t size() const { return m_size; }

	size_t erase(const T& key);
	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);

	void clear() 
	{ 
		destroy(m_header->parent); 
		m_header->parent = nullptr;
		m_header->left = m_header;
		m_header->right = m_header;
		m_size = 0;
	}
	iterator find(const T& key);
	iterator lower_bound(const T& key);
	iterator upper_bound(const T& key);
	std::pair<iterator, iterator> equal_range(const T& key);

	const_iterator lower_bound(const T& key) const;
	const_iterator upper_bound(const T& key) const;
	const_iterator find(const T& key) const;
	size_t count(const T& key)
	{
		iterator it = find(key);
		if (it != end()) return 1;
		return 0;
	}
	VALUE& operator[](const T& key)
	{
		return insert(value_type(key, VALUE()))->second;
	}

	void swap(RBTree& tree)
	{
		std::swap(m_header, tree.m_header);
		std::swap(m_size, tree.m_size);
	}
private:
	void init()
	{
		//初始化m_header节点
		m_header = new Node();
		m_header->parent = nullptr;
		m_header->left = m_header;
		m_header->right = m_header;
		m_header->color = Node::RED;
	}

	bool isLeftChild(const RBTreeNodePtr node)
	{
		//assert(node);
		return node->parent != nullptr && node == node->parent->left;
	}

	bool isRightChild(const RBTreeNodePtr node)
	{
		//assert(node);
		return node->parent != nullptr && node == node->parent->right;
	}

	bool hasTwoChild(const RBTreeNodePtr node)
	{
		return node->left != nullptr && node->right != nullptr;
	}
	void setBlack(RBTreeNodePtr node)
	{
		assert(node);
		node->color = RBTreeNode<T>::BLACK;
	}

	void setRed(RBTreeNodePtr node)
	{
		assert(node);
		node->color = RBTreeNode<T>::RED;
	}

	bool nodeColor(const RBTreeNodePtr node)
	{
		return node == nullptr ? RBTreeNode<T>::BLACK : node->color;
	}
	bool isRed(const RBTreeNodePtr node)
	{
		return nodeColor(node) == RBTreeNode<T>::RED;
	}

	RBTreeNodePtr setColor(RBTreeNodePtr node, bool color)
	{
		node->color = color;
		return node;
	}
	bool isBlack(const RBTreeNodePtr node)
	{
		return nodeColor(node) == RBTreeNode<T>::BLACK;
	}

	RBTreeNodePtr brotherNode(const RBTreeNodePtr node)
	{
		if (node)
		{
			if (isLeftChild(node))
			{
				return node->parent->right;
			}
			if (isRightChild(node))
			{
				return node->parent->left;
			}
		}
		return nullptr;
	}
	RBTreeNodePtr uncleNode(const RBTreeNodePtr node)
	{
		if (node)
			return brotherNode(node->parent);
		return nullptr;
	}
	
	void inorder(RBTreeNodePtr node)
	{
		if (!node)
			return;

		inorder(node->left);
		const char* color = node->color == RBTreeNode<T>::RED ? "red" : "black";
		if (node->parent->parent == node && node->parent->color == RBTreeNode<T>::RED)
			printf("根节点; %d  %s\n", node->value.first, color);
		else
		{
			const char* leftright = node == node->parent->left ? "isleft" : "isright";
			printf("%d %s %s 父亲：%d\n", node->value.first, color, leftright, node->parent->value.first);

		}
		inorder(node->right);
	}

	void destroy(RBTreeNodePtr node)
	{
		if (!node) return;
		destroy(node->left);
		destroy(node->right);
		delete node;
	}

	void addNodeAndRebalance(RBTreeNodePtr node);
	void leftRotate(RBTreeNodePtr node);
	void rightRotate(RBTreeNodePtr node);
	void updateParent(RBTreeNodePtr oldParent, RBTreeNodePtr newParent, RBTreeNodePtr child);
	void removeAndRebalance(RBTreeNodePtr node);
	void remove(RBTreeNodePtr removeNode);
	RBTreeNodePtr predecessor(RBTreeNodePtr node);
	RBTreeNodePtr rightmost() const { return m_header->right; }
	RBTreeNodePtr leftmost() const { return m_header->left; }
	RBTreeNodePtr root() const { return m_header->parent; }
	static RBTreeNodePtr findMinNode(RBTreeNodePtr node);
	static RBTreeNodePtr findMaxNode(RBTreeNodePtr node);
	void copy(const RBTree& tree);
private:
	//std::unique_ptr<Node> m_header;
	RBTreeNodePtr m_header;
	size_t m_size;
};


template<typename T, typename VALUE, typename Compare>
inline RBTree<T, VALUE, Compare>::RBTree(const RBTree & tree)
{
	init();
	copy(tree);
	m_size = tree.m_size;
}

template<typename T, typename VALUE, typename Compare>
RBTree<T, VALUE, Compare> & RBTree<T, VALUE, Compare>::operator=(const RBTree & tree)
{
	if (m_header)
	{
		clear();
		delete m_header;
		m_header = nullptr;
	}
	init();
	copy(tree);
	m_size = tree.m_size;
	return *this;
}

template<typename T, typename VALUE, typename Compare>
RBTree<T, VALUE, Compare>::RBTree(RBTree && tree)
{
	m_header = tree.m_header;
	m_size = tree.m_size;
	tree.m_header = nullptr;
}

template<typename T, typename VALUE, typename Compare>
RBTree<T, VALUE, Compare> & RBTree<T, VALUE, Compare>::operator=(RBTree && tree)
{
	std::swap(tree.m_header, m_header);
	std::swap(tree.m_size, m_size);
	return *this;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T,VALUE, Compare>::iterator RBTree<T, VALUE, Compare>::insert(const value_type& value)
{
	//这是根节点
	if (!m_header->parent)
	{
		RBTreeNodePtr newNode = new Node(value);
		//m_header的parent指向根节点
		m_header->parent = newNode;
		//根节点的parent指向m_header
		newNode->parent = m_header;
		m_header->left = newNode;
		m_header->right = newNode;
		addNodeAndRebalance(newNode);
		++m_size;
		return iterator(newNode);
	}

	RBTreeNodePtr parent = NULL;
	RBTreeNodePtr serch = m_header->parent;
	while (serch)
	{
		parent = serch;
		if (Compare()(serch->value.first, value.first))
		{
			serch = serch->right;
		}
		else if (Compare()(value.first, serch->value.first))
		{
			serch = serch->left;
		}
		else
			return iterator(serch);
	}

	assert(parent);
	RBTreeNodePtr newNode = new Node(value);
	newNode->parent = parent;

	if (Compare()(parent->value.first, value.first))
	{
		parent->right = newNode;
		if (parent == rightmost()) m_header->right = newNode;
	}
	else if (Compare()(value.first, parent->value.first))
	{
		parent->left = newNode;
		if (parent == leftmost()) m_header->left = newNode;
	}

	++m_size;
	addNodeAndRebalance(newNode);
	return iterator(newNode);
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::iterator RBTree<T, VALUE, Compare>::insert(iterator position, const value_type & value)
{
	if (m_header->parent == nullptr)
	{
		RBTreeNodePtr newNode = new Node(value);
		m_header->parent = newNode;
		newNode->parent = m_header;
		m_header->left = newNode;
		m_header->right = newNode;
		addNodeAndRebalance(newNode);
		return iterator(newNode);
	}

	if (position == end()) --position;

	if (Compare()(value.first, position->first) )
	{
		if (position.nodePtr == leftmost())
		{
			RBTreeNodePtr newNode = new Node(value);
			newNode->parent = m_header->left;
			m_header->left->left = newNode;
			m_header->left = newNode;
			addNodeAndRebalance(newNode);
            ++m_size;
			return iterator(newNode);
		}
		return insert(value);	
	}
	else if (Compare()(position->first,value.first))
	{
		if (position == rightmost())
		{
			RBTreeNodePtr newNode = new Node(value);
			newNode->parent = m_header->right;
			m_header->right->right = newNode;
			m_header->right = newNode;
            ++m_size;
			addNodeAndRebalance(newNode);
			return iterator(newNode);
		}
		return insert(value);
	}
	else return position;
}

template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE,Compare>::addNodeAndRebalance(RBTreeNodePtr node)
{
	RBTreeNodePtr parent = node->parent;

	//node的父亲是m_header，代表node是根节点， node不是根节点且父亲为红色
	while (root() != node &&  isRed(parent))
	{
	
		//递归调用的时候，这句话必须在之前处理，若之后处理的话，父亲是黑色，也会旋转的
		//if (isBlack(parent)) return;

		//叔叔节点
		//祖父节点
		RBTreeNodePtr grand = parent->parent;
		RBTreeNodePtr uncle = uncleNode(node);
		//叔叔是红色, 红黑红的情况
		if (isRed(uncle))
		{
			//父亲染黑，叔叔染黑
			setBlack(parent);
			setBlack(uncle);
			//祖父节点染红
			setRed(grand);
			//addNodeAndRebalance(grand);
			//parent = grand->parent;
			//return;
			//将祖父节点当做新节点，重新处理
			node = grand;
			parent = node->parent;
		}
		else //叔叔不是红色
		{
			if (isLeftChild(parent)) //erase
			{
				if (isLeftChild(node))//LL
				{
					setBlack(parent);
					rightRotate(grand);
				}
				else                  //LR
				{
					setBlack(node);
					leftRotate(parent);
					rightRotate(grand);
					//node成为父节点
					//setRed(grand);	
				}
				setRed(grand);
				break;
			}
			else //R
			{
				if (isRightChild(node))//RR
				{
					setBlack(parent);
					leftRotate(grand);
				}
				else                   //RL
				{
					setBlack(node);
					rightRotate(parent);
					leftRotate(grand);
					//node成为父节点
				}
				setRed(grand);
				break;
			}
		}
	}
	root()->color = Node::BLACK;
}

template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE, Compare>::leftRotate(RBTreeNodePtr node)
{
	assert(node);
	//右孩子称为自己的父亲
	RBTreeNodePtr newParent = node->right;
	//右孩子的左孩子成为自己的右孩子
	RBTreeNodePtr leftChild = newParent->left;
	node->right = leftChild;
	//自己成为新父亲的左节点
	newParent->left = node;
	updateParent(node, newParent, leftChild);
}

template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE, Compare>::rightRotate(RBTreeNodePtr node)
{
	//左孩子成为自己的父亲
	RBTreeNodePtr newParent = node->left;
	//左孩子的右孩子称为自己的左孩子
	RBTreeNodePtr rightChild = newParent->right;
	node->left = rightChild;
	//自己成为新父亲的右节点
	newParent->right = node;
	updateParent(node, newParent, rightChild);
}

template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE, Compare>::updateParent(RBTreeNodePtr oldParent, RBTreeNodePtr newParent, RBTreeNodePtr child)
{

	RBTreeNodePtr parent = oldParent->parent;
	//原来父亲节点的父亲是新父亲的父亲
	newParent->parent = parent;
	//parent->parent = newParent; //写到这儿会错误，
	if (oldParent->parent->parent == oldParent)
	{
		//只有是根节点的时候，才更新相互的父亲属性
		m_header->parent = newParent;

	}
	else if (isLeftChild(oldParent))
	{
		parent->left = newParent;
	}
	else if (isRightChild(oldParent))
	{
		parent->right = newParent;
	}

	//原来节点的父亲是新节点
	oldParent->parent = newParent;
	if (child)
	{
		//孩子节点，指的是，原来左孩子的右孩子，或者是右孩子的左孩子
		//孩子节点的父亲是新孩子
		child->parent = oldParent;
	}
}

//删除后恢复平衡，并且是度为1或者0的节点
template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE, Compare>::removeAndRebalance(RBTreeNodePtr node)
{

	//删除的是红色节点或者删除黑色节点有一个红色节点的
	if (isRed(node) /*|| node == root()*/)
	{
		setBlack(node);
		return;
	}
	
	//接下来删除的节点是黑色的，并且是叶子节点
	while (node != root() && node->color == Node::BLACK)
	{
		RBTreeNodePtr parent = node->parent;
		bool left = isLeftChild(node);
		RBTreeNodePtr brother = left ? parent->right : parent->left;
		assert(brother);
		//删除的是左节点，兄弟是右节点
		if (left)
		{
			//兄弟是红色
			if (isRed(brother))
			{
				//cout << " brother is red " << endl;
				//兄弟染黑
				setBlack(brother);
				//父亲染红

				setRed(parent);
				leftRotate(parent);
				//兄弟重新成为父亲的右
				brother = parent->right;
				assert(brother);

			}
			//兄弟是黑色且兄弟的子节点都是黑色
			if (isBlack(brother->left) && isBlack(brother->right))
			{
				//cout << " brother left child and right child is black " << endl;
				//bool parentBlack = isBlack(parent);

				setRed(brother);

				node = parent;
				continue;
				//setBlack(parent);
				/*if (parentBlack)
				{
					if (parent == root()) return;
					removeAndRebalance(parent);
				}*/
			}
			else  //兄弟至少有一个红色子节点
			{
				//cout << " brother least has a red child " << endl;
				if (isBlack(brother->right))
				{
					rightRotate(brother);
					brother = parent->right;
				}
				assert(parent);
				assert(brother);

				leftRotate(parent);
				setBlack(brother->right);
				setBlack(parent);
				setColor(brother, parent->color);
				break;
			}	
		}
		else  //删除节点是右节点，兄弟节点是左节点
		{
			//兄弟节点是红色
			if (isRed(brother))
			{
				setBlack(brother);
				setRed(parent);
				rightRotate(parent);
				brother = parent->left;
				assert(brother);

			}
			//兄弟节点是黑色并且左右子节点都是黑色，代表兄弟不能借元素给自己，去父亲借
			if (isBlack(brother->left) && isBlack(brother->right))
			{
				//bool parentBlack = isBlack(parent);
				setRed(brother);  //兄弟染红，父亲染黑
				
				//如果父亲是黑色的，则代表父亲也不能借
				
				node = parent;
				continue;
				//setBlack(parent);
				/*if (parentBlack)
				{
					if (parent == root()) return;
					removeAndRebalance(parent);
				}*/
			}
			else  //至少有一个红色子节点
			{
				if (isBlack(brother->left)) // L
				{
					leftRotate(brother);  //LR
					brother = parent->left;
				}
				assert(brother);
				assert(parent);
				assert(brother->left);
				//新的中心节点的左右都染黑       // LL
				setBlack(brother->left);
				setBlack(parent);
				//中心节点跟随父亲的节点
				setColor(brother, parent->color);
				rightRotate(parent);
				break;
			}
		}
	}
	node->color = Node::BLACK;
}

template<typename T, typename VALUE, typename Compare>
size_t RBTree<T, VALUE, Compare>::erase(const T& key)
{
	size_t element_count = 0;
	iterator removeNode = find(key);
	if (removeNode != end())
	{
		remove(removeNode.nodePtr);
		return ++element_count;
	}
	return element_count;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::iterator RBTree<T, VALUE, Compare>::erase(iterator position)
{
	if (position == end()) return position;
	iterator temp = position;
	++position;
	remove(temp.nodePtr);
	return position;	
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::iterator RBTree<T, VALUE, Compare>::erase(iterator first, iterator last)
{
	if (first == begin() && last == end())
	{
		clear();
		return begin();
	}
	else
	{
		while (first != last)
		{
			erase(first++);
		}
	}
	return first;
}

template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE, Compare>::remove(RBTreeNodePtr removeNode)
{
	assert(removeNode);
	if (hasTwoChild(removeNode))
	{
		RBTreeNodePtr pre = predecessor(removeNode);
		removeNode->value = pre->value;
		removeNode = pre;
	}

	RBTreeNodePtr child = removeNode->left != nullptr ? removeNode->left : removeNode->right;
	RBTreeNodePtr parent = removeNode->parent;
	RBTreeNodePtr node = removeNode;
	//读为1或者度为0的节点
	if (child)  //度为1的节点
	{
		child->parent = parent;
		//如果删除节点是根节点
		if (root() == removeNode)
		{
			//根节点在最左
			if (removeNode == leftmost())
			{
				parent->left = child;
			}
			else  //根节点在最右
			{
				parent->right = child;
			}	
			m_header->parent = child;
			delete removeNode;
			removeNode = nullptr;
		}
		else if (removeNode == parent->left)
		{
			//最左的节点存在一个右孩子
			if (removeNode == leftmost()) m_header->left = child;
			parent->left = child;
			delete removeNode;
			removeNode = nullptr;
		}
		else if (removeNode == parent->right)
		{
			//最右的节点存在一个左孩子
			if (removeNode == rightmost()) m_header->right = child;
			parent->right = child;
			delete removeNode;
			removeNode = nullptr;
		}
		
		removeAndRebalance(child);

	}
	else  //叶子节点
	{
		removeAndRebalance(node);
		//是根节点且仅有一个根节点
		if (removeNode == parent->left && removeNode == parent->right)
		{
			m_header->parent = nullptr;
			m_header->left = m_header;
			m_header->right = m_header;
			delete removeNode;
			removeNode = nullptr;
		}
		else if (removeNode == parent->left)
		{
			//如果删除的是最左的叶子节点，那么删除完之后，最小就是他的后继就是其父亲
			if (removeNode == leftmost()) m_header->left = parent;
			//removeAndRebalance(node);
			delete removeNode;
			
			parent->left = nullptr;
		}
		else if (removeNode == parent->right)
		{
			//如果删除的是最右的叶子节点，那么删除完之后，最大就是他的前驱就是其父亲
			if (removeNode == rightmost()) m_header->right = parent;
			//removeAndRebalance(node);

			delete removeNode;
			parent->right = nullptr;
			
		}

		//afterRemoveRebalance(node);
	}
	--m_size;
}

template<typename T, typename VALUE,typename Compare>
typename RBTree<T, VALUE, Compare>::RBTreeNodePtr RBTree<T, VALUE, Compare>::predecessor(RBTreeNodePtr node)
{
	RBTreeNodePtr pre = node->left;
	if (pre)
	{
		return findMaxNode(pre);
	}
	RBTreeNodePtr parent = node->parent;
	while (parent && node == parent->left)
	{
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::iterator  RBTree<T, VALUE, Compare>::find(const T& key)
{
	RBTreeNodePtr p = m_header->parent;
	while (p)
	{
		if (Compare()(p->value.first, key))
		{
			p = p->right;
		}
		else if (Compare()(key, p->value.first))
		{
			p = p->left;
		}
		else
		{
			return p;//发生隐式转换
		}
	}
	return end();
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::iterator RBTree<T, VALUE, Compare>::lower_bound(const T & key)
{
	RBTreeNodePtr x = root();
	//j要查找的值
	RBTreeNodePtr j = m_header;
	while (x)
	{
		//找左边界，key小于等于的去左边寻找，因为中间后面的肯定都大于key了
		if (!Compare()(x->value.first, key))
		{
			j = x;
			x = x->left;
		}
		else
			x = x->right;
	}

	iterator searched = iterator(j);
	return  searched;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::iterator  RBTree<T, VALUE, Compare>::upper_bound(const T & key)
{
	RBTreeNodePtr x = root();
	//j要查找的值
	RBTreeNodePtr j = m_header;
	while (x)
	{
		//找左边界，key小于等于的去左边寻找，因为中间后面的肯定都大于key了
		if (!Compare()(key, x->value.first))	
			x = x->right;
		else
		{
			j = x;
			x = x->left;
		}		
	}
	iterator searched = iterator(j);
	return searched;
}

template<typename T, typename VALUE, typename Compare>
std::pair<typename RBTree<T, VALUE, Compare>::iterator, typename RBTree<T, VALUE, Compare>::iterator>
RBTree<T, VALUE, Compare>::equal_range(const T & key)
{
	iterator gt = lower_bound(key);
	iterator gteater = upper_bound(key);
	return std::pair<iterator, iterator>(gt, gteater);
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::const_iterator RBTree<T, VALUE, Compare>::lower_bound(const T & key) const
{
	RBTreeNodePtr rootNode = root();
	RBTreeNodePtr headNode = m_header;
	while (rootNode)
	{
		//如果父节点的值小于key，那意味着父节点的左子树的值都小于key，去右子树找
		if(Compare()(rootNode->value.first, key))
		{ 
			rootNode = rootNode->right;
		}
		else
		{
			headNode = rootNode;
			rootNode = rootNode->left;
		}
	}
	const_iterator constIter(headNode);
	return constIter;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::const_iterator RBTree<T, VALUE, Compare>::upper_bound(const T & key) const
{
	RBTreeNodePtr rootNode = root();
	//j要查找的值
	RBTreeNodePtr headNode = m_header;
	while (rootNode)
	{
		//key小于父节点的值，意味着，key小于右子树所有的值，那么去左子树寻可能的第一个大于自己的值的节点
		if (Compare()(key, rootNode->value.first))
		{
			headNode = rootNode;
			rootNode = rootNode->left;
		}
		else  //key大于等于父节点的值，去右节点寻找大于自己的
		{
			rootNode = rootNode->right;
		}
	}
	const_iterator constIter(headNode);
	return constIter;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::const_iterator RBTree<T, VALUE, Compare>::find(const T & key) const
{
	const_iterator constIter = lower_bound(key);
	return (constIter == cend() || Compare()(key, constIter->first)) ? cend() : constIter;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::RBTreeNodePtr RBTree<T, VALUE, Compare>::findMinNode(RBTreeNodePtr node)
{
	while (node->left)
	{
		node = node->left;
	}
	return node;
}

template<typename T, typename VALUE, typename Compare>
typename RBTree<T, VALUE, Compare>::RBTreeNodePtr RBTree<T, VALUE, Compare>::findMaxNode(RBTreeNodePtr node)
{
	while (node->right)
	{
		node = node->right;
	}
	return node;
}

template<typename T, typename VALUE, typename Compare>
void RBTree<T, VALUE, Compare>::copy(const RBTree& tree)
{
	for (RBTree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it)
	{
		insert(*it);
	}
}

template<typename T, typename VALUE, typename Compare>
template<typename InputIterator>
typename RBTree<T, VALUE, Compare>::iterator RBTree<T, VALUE, Compare>::insert(InputIterator first, InputIterator last)
{
	iterator position;
	InputIterator result = first;
	while (++first != last)
	{
		if (*result < *first)
			position = insert(end(), *result);
		else if (*first < *result)
			position = insert(begin(), *result);
		result = first;
	}

	/*while (first != last)
	{
		position = insert(*first++);
	}*/
	return position;
}
