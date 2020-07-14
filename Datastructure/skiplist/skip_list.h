#pragma once

//template<typename Key, typename Value>
#include <vector>
#include <algorithm>

namespace structure
{
	const static double p = 0.25;

	template<typename Value>
	struct SkipListNode
	{
		Value value;
		SkipListNode** nexts; //节点指针的数组
		size_t level;  //每个节点的层数
		SkipListNode() {};
		SkipListNode(const Value& valueArg, size_t levelArg)
			:value(valueArg),level(levelArg)
		{}
	};

	//前向迭代器,没有--
	template<typename Value, typename Ptr, typename Ref>
	class SkipListIterator:public std::iterator<std::forward_iterator_tag, Value>
	{
	public:
		typedef Ptr pointer;
		typedef Ref reference;
		typedef SkipListNode<Value>* NodePtr;

		explicit SkipListIterator(NodePtr node) :m_node(node) {}
		pointer operator->()
		{
			return &m_node->value;
		}

		reference operator*()
		{
			return m_node->value;
		}

		

		SkipListIterator(const SkipListIterator& it) :m_node(it.m_node) {}
		SkipListIterator& operator=(const SkipListIterator& it)
		{
			m_node = it.m_node;
			return *this;
		}

		SkipListIterator operator++(int)
		{
			SkipListIterator temp(*this);
			*this++;
			return temp;
		}

		SkipListIterator& operator++()
		{
			m_node = m_node->nexts[0];
			return *this;
		}
		bool operator==(SkipListIterator it)
		{
			return m_node == it.m_node;
		}

		bool operator!=(SkipListIterator it)
		{
			return m_node != it.m_node;
		}
		NodePtr m_node;
	};
	
	//单向链表的跳表
	template<typename Key, typename Value, typename Compare = std::less<Key>>
	class SkipList
	{
	public:
		typedef std::pair<Key, Value> value_type;
		typedef SkipListIterator<value_type, value_type*, value_type&> iterator;
		typedef SkipListNode<value_type>* SkipListNodePtr;
		//struct SkipListNode;
		SkipList() :m_size(0), m_level(0)
		{
			init();
		}

		~SkipList()
		{
			if (m_header)
			{
				clear();
				delete[] m_header->nexts;
				m_header->nexts = nullptr;
				delete m_header;
				m_header = nullptr;
			}
		}
		iterator begin() 
		{
			iterator it(m_header->nexts[0]);
			return it;
		}

		iterator end()
		{
			iterator it(nullptr);
			return it;
		}
		iterator insert(const Key& key, const Value& value);
		void remove(const Key& key);
		void clear();
		iterator find(const Key& key);
		int level() const { return m_level; }
		size_t size() const { return m_size; }
		bool empty() const { return m_size == 0; }
	private:

		void init()
		{
			m_header = new SkipListNode<value_type>(value_type(), kFixSize);
			m_header->nexts = new SkipListNodePtr[kFixSize];
			reset(m_header->nexts, kFixSize);
		}
		//得到小数点后两位的[0, 1)之间的小数
		double random()
		{
			return  rand() % (N + 1) / static_cast<double>(N + 1);
		}

		//得到随机的层数
		int randomLevel()
		{
			int level = 1;
			while (random() < p && level < kFixSize)
			{
				++level;
			}

			return level;
		}

		//将数组的各个指针置为null;
		void reset(SkipListNodePtr* &node, size_t len)
		{
			for (int i = 0; i < len; ++i)
				node[i] = nullptr;
		}

		//比较函数
		//return: -1 key1 < key2, 
		//         1 key1 > key2, 
		//         0 ke1 == key2;
		int compare(const Key& key1, const Key& key2) const
		{
			if (m_cmp(key1, key2))
				return  -1;
			else if (m_cmp(key2, key1))
				return 1;
			else
				return 0;
		}

		//拿到value_type的key值
		Key keyofValue(const value_type& value) const
		{
			return value.first;
		}

	private:
		size_t m_size;
		int m_level; //表的当前最大层数
		Compare m_cmp;
		SkipListNodePtr m_header;
		const static int N = 99;
		const static int kFixSize = 32;  //每个节点的含有的最大层数
	};


	template<typename Key, typename Value, typename Compare>
	typename SkipList<Key, Value, Compare>::iterator SkipList<Key, Value, Compare>::find(const Key& key)
	{
		SkipListNodePtr findNode = m_header;

		int cmp;
		for (int i = m_level - 1; i >= 0; --i)
		{
			//如果要查找的节点的值小于key值，那么一直往后找
			while (findNode->nexts[i])
			{
				Key findKey = keyofValue(findNode->nexts[i]->value);
				if ((cmp = compare(findKey, key)) < 0)
					findNode = findNode->nexts[i];
				else
					break;
			}
			if (cmp == 0)
			{
				iterator it(findNode->nexts[i]);
				return it;
			}	
		}

		return end();
	}

	template<typename Key, typename Value, typename Compare>
	typename SkipList<Key, Value, Compare>::iterator SkipList<Key, Value, Compare>::insert(const Key& key, const Value& value)
	{
		SkipListNodePtr node = m_header;
		std::vector<SkipListNodePtr> prevArr;
		prevArr.resize(m_level);
		int cmp;
		for (int i = m_level - 1; i >= 0; --i)
		{
			while (node->nexts[i])
			{
				Key findKey = keyofValue(node->nexts[i]->value);
				if((cmp = compare(findKey, key)) < 0)
					node = node->nexts[i];
				else
					break;
			}

			if (cmp == 0)
			{
				iterator it(node->nexts[i]);
				return it;
			}

			prevArr[i] = node;
		}

		int newLevel = randomLevel();
		value_type newValue(key, value);
		SkipListNodePtr newNode = new SkipListNode<value_type>(newValue, newLevel);
		newNode->nexts = new SkipListNodePtr[newLevel];
		reset(newNode->nexts, newLevel);

		for (int i = 0; i < newLevel; ++i)
		{
			//如果新节点的层数大于当前表中最大的层数,那么当i大于等于最大的层数的时候，让头结点的第i层指向新节点
			if (m_level <= i)
				//
				m_header->nexts[i] = newNode;
			else //如果新节点的层数小于等于最大层数，让前驱数节点的数组中的节点依次指向新节点(不是指向节点中的nexts数组)
			{
				newNode->nexts[i] = prevArr[i]->nexts[i];
				prevArr[i]->nexts[i] = newNode;
			}

		}

		++m_size;
		m_level = std::max(m_level, newLevel);
		iterator it(newNode);
		return it;
	}

	template<typename Key, typename Value, typename Compare>
	void SkipList<Key, Value, Compare>::remove(const Key& key)
	{
		SkipListNodePtr node = m_header;
		//被删除节点的前驱节点的数组
		std::vector<SkipListNodePtr> prevArr;
		prevArr.resize(m_level);
		int cmp;
		//判断是键值为key的节点否存在
		bool exist = false;
		for (int i = m_level - 1; i >= 0; --i)
		{
			while (node->nexts[i])
			{
				Key findKey = keyofValue(node->nexts[i]->value);
				if ((cmp = compare(findKey, key)) < 0)
					node = node->nexts[i];
				else
					break;
			}
			//如果等于0， 证明节点存在
			if (cmp == 0) exist = true;
			prevArr[i] = node;
		}

		if (!exist) return;
		SkipListNodePtr removeNode = node->nexts[0];
		//先更改前驱的指向
		for (int i = 0; i < removeNode->level; ++i)
		{
			prevArr[i]->nexts[i] = removeNode->nexts[i];
		}
		//再删除
		delete[] removeNode->nexts;
		removeNode->nexts = nullptr;
		delete removeNode;
		removeNode = nullptr;
		//更新表的层数
		int newLevel = m_level;
		while (--newLevel >= 0 && m_header->nexts[newLevel] == nullptr)
		{
			m_level = newLevel;
		}

		--m_size;
	}

	template<typename Key, typename Value, typename Compare>
	void SkipList<Key, Value, Compare>::clear()
	{
		SkipListNodePtr node = nullptr;
		SkipListNodePtr temp = m_header->nexts[0];
		while(temp)
		{
			node = temp;
			temp = temp->nexts[0];
			m_header->nexts[0] = node;
			delete[] node->nexts;
			node->nexts = nullptr;
			delete node;
			//node = nullptr;
			m_header->nexts[0] = nullptr;
		}

		m_level = 0;
		m_size = 0;
	}

}

