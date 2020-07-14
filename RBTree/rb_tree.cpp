#define  _CRT_SECURE_NO_WARNINGS 


#include <string.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <iostream>

#include <string>
#include <vector>
#include <algorithm>
#include "rb_tree.h"
#include "Timer.h"
using std::cout;
using std::endl;
class Person
{
public:
	//Person() {}
	Person(int a) :age_(a) {}

	bool operator< (const Person& p1) const
	{
		return this->age_ < p1.age_;
	}
	void setAge(int n) { age_ = n; }
	int getAge() const { return age_; }
private:
	int age_;
};

void testBaseFunction()
{
	typedef RBTree<int, std::string> RBTree;
	RBTree b;
	// 5 3 1 2 4 7 6 9 10 8 12 13
	b.insert(std::make_pair(5, "55555"));
	b.insert(std::make_pair(3, "33333"));
	b.insert(std::make_pair(1, "11111"));
	b.insert(std::make_pair(2, "22222"));
	b.insert(std::make_pair(4, "44444"));
	b.insert(std::make_pair(7, "77777"));
	b.insert(std::make_pair(6, "66666"));
	b.insert(std::make_pair(9, "99999"));
	b.insert(std::make_pair(10, "101010"));
	b.insert(std::make_pair(8, "88888"));
	b.insert(std::make_pair(12, "12"));
	b.insert(std::make_pair(13, "13"));

	b.inorder();
	for (RBTree::iterator it = b.begin(); it != b.end(); ++it)
	{
		std::cout << " " << it.nodePtr->value.first;
	}
	cout << endl;
	cout << " after reverse " << endl;
	for (RBTree::reverse_iterator it = b.rbegin(); it != b.rend(); ++it)
	{
		cout << "frist: " << it->first << " second: " << it->second << endl;
	}
	std::cout << std::endl;

	auto it = b.lower_bound(2);
	assert(it->second == "22222");
	it = b.lower_bound(13);
	assert(it->second == "13");
	it = b.lower_bound(4);
	assert(it->second == "44444");
	b.erase(4);
	it = b.lower_bound(4);
	assert(it->second == "55555");

	it = b.upper_bound(8);
	assert(it->second == "99999");
	b[5] = "55555+5";
	assert(b[5] == "55555+5");
	//std::pair<RBTree::iterator, RBTree::iterator> range = b.equal_range(6);
	auto range = b.equal_range(6);
	assert(range.first->second == "66666");
	assert(range.second->second == "77777");
}
void testInsertRange(int size)
{
	typedef RBTree<int, int> RBTree;
	RBTree b;
	std::map<int, int> m;
	{
		Timer t;
		cout << "insert " << size << " element at end(): ";
		for (int i = 0; i < size; ++i)
		{
			b.insert(b.end(), std::make_pair(i, i + 10));
		}
	}

	{
		Timer t;
		cout << "map insert " << size << " element at end(): ";
		for (int i = 0; i < size; ++i)
		{
			m.insert(m.end(), std::make_pair(i, i + 10));
		}
	}

	b.clear();
	m.clear();

	{
		Timer t;
		cout << "insert " << size << " element at begin(): ";
		for (int i = size - 1; i >= 0; --i)
		{
			b.insert(b.begin(), std::make_pair(i, i + 10));
		}
	}

	{
		Timer t;
		cout << "map insert " << size << " element at begin(): ";
		for (int i = size - 1; i >= 0; --i)
		{
			m.insert(m.begin(), std::make_pair(i, i + 10));
		}
	}

	b.clear();
	m.clear();

	{
		Timer t;
		cout << "insert " << size << " element at root(): ";
		for (int i = 0; i < size; ++i)
		{
			b.insert(std::make_pair(i, i + 10));
		}
	}

	{
		Timer t;
		cout << "map insert " << size << " element at root(): ";
		for (int i = 0; i < size; ++i)
		{
			m.insert(std::make_pair(i, i + 10));
		}
	}

	b.clear();
	m.clear();

	std::vector<std::pair<int, int>> v;
	for (int i = 0; i < size; ++i)
	{
		v.push_back(std::make_pair(i , i + 10));
	}

	{
		Timer t;
		cout << "insert " << size << " element at [first, last) and asc : ";
		b.insert(v.begin(), v.end());
	}
	
	{
		Timer t;
		cout << "map insert " << size << " element at [first, last) and asc : ";
		m.insert(v.begin(),v.end());
	}

	b.clear();
	v.clear();
	m.clear();

	for (int i = size - 1 ; i >= 0; --i)
	{
		v.push_back(std::make_pair(i, i + 10));
	}

	{
		Timer t;
		cout << "insert " << size << " element at [first, last) and desc: ";
		b.insert(v.begin(), v.end());
	}

	{
		Timer t;
		cout << "map insert " << size << " element at [first, last) and desc: ";
		m.insert(v.begin(), v.end());
	}

	b.clear();
	m.clear();

	std::random_shuffle(v.begin(), v.end());
	{
		Timer t;
		cout << "insert " << size << " element at [first, last) and random: ";
		b.insert(v.begin(), v.end());
	}

	{
		Timer t;
		cout << "map insert " << size << " element at [first, last) and random: ";
		m.insert(v.begin(), v.end());
	}
	//优化后的[first, last) 花费时间:
	/*insert 10000000 element at end() : 11168ms
		insert 10000000 element at begin() : 9675ms
		insert 10000000 element at root() : 17511ms
		insert 10000000 element at[first, last) and asc : 13762ms
		insert 10000000 element at[first, last) and desc: 13437ms
		insert 10000000 element at[first, last) and random: 30720ms*/
	//没有优化后花费的时间
	/*insert 10000000 element at end() : 10967ms
		insert 10000000 element at begin() : 9581ms
		insert 10000000 element at root() : 17485ms
		insert 10000000 element at[first, last) and asc : 24246ms
		insert 10000000 element at[first, last) and desc: 30152ms
		insert 10000000 element at[first, last) and random: 31733ms*/
}
void testStd()
{
	cout << "--------------test refer to std::algorithm: " << endl;
	typedef RBTree<int, int> RBTree;
	RBTree b;
	for (int i = 0; i < 10; ++i)
	{
		b.insert(std::make_pair(i, i + 10));
	}
	cout << "--------------test refer to std::find--------------" << endl;
	auto it = std::find(b.begin(), b.end(), std::pair<int, int>(1,11));
	cout << "frist: "<< it->first << " second: "<< it->second << endl;
	
	std::vector<std::pair<int, int>> v;
	cout << "--------------test refer to std::copy--------------" << endl;
	std::copy(b.begin(), b.end(), std::back_inserter(v));
	for (const auto& it2 : v)
	{
		cout << "frist: " << it2.first << " second: " << it2.second << endl;
	}
}

void testTime(int size)
{
	cout << "--------------test rbtree--------------" << endl;
	typedef RBTree<int, int> RBTree;
	RBTree rb;
	{
		Timer t;
		cout << "rbtree insert " << size << " size: ";

		for (int i = 0; i < size; ++i)
		{
			rb.insert(std::make_pair(i, i + 10));
		}
	}
	
	assert(rb.size() == static_cast<size_t>(size));
	{
		Timer t(true);
		cout << "rbtree find : " << size / 2 << ": ";
		rb.find(size / 2);
	}

	auto start = std::chrono::system_clock::now();
	RBTree rb2;
	rb2 = rb;
	auto end = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "rb copy " << size << " element time: " << duration.count() << "ms" << endl;

	start = std::chrono::system_clock::now();
	RBTree rb3(std::move(rb));
	end = std::chrono::system_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "rb move " << size << " element time: " << duration.count() << "ms" << endl;
	auto it = rb3.find(size / 2);

	{
		Timer t;
		
		/*cout << "rbtree erase [0, " << size / 2 << "]: ";
		rb2.erase(rb2.begin(), it);*/
		cout << "rbtree erase " << size / 2 << " : ";
		rb3.erase(it);
	}

	cout << "--------------test map--------------" << endl;
	std::map<int, int> m;
	{
		Timer t;
		cout << "map insert " << size << " size: ";

		for (int i = 0; i < size; ++i)
		{
			m.insert(std::make_pair(i, i + 10));
		}
	}
	
    cout << "map size: " << m.size() << endl;
	{
		Timer t(true);
		cout << "map find : " << size / 2 << ": ";
		m.find(size / 2);
	}

	start = std::chrono::system_clock::now();
	std::map<int, int> m2(m);
	end = std::chrono::system_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "map copy " << size << " element time: " << duration.count() << "ms" << endl;

	auto mapIt = m.find(size / 2);
	{
		Timer t;
		/*cout << "map erase [0, " << size / 2 << "]: ";
		m.erase(m.begin(), mapIt);*/
		cout << "map erase " << size / 2 << " : ";
		m.erase(mapIt);
	}
	cout << "--------------test vector--------------" << endl;
	std::vector<std::pair<int ,int>> vec;

	{
		Timer t;
		cout << "vector insert " << size << " size: ";

		for (int i = 0; i < size; ++i)
		{
			vec.push_back(std::make_pair(i, i + 10));
		}
	}
	std::pair<int, int> value(size / 2, size / 2 + 10);
	{
		Timer t(true);
		cout << "vector find : " << size / 2 << ": ";
		std::find(vec.begin(), vec.end(), value);
	}

	start = std::chrono::system_clock::now();
	std::vector<std::pair<int, int>> vec2(vec);
	end = std::chrono::system_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "vector copy " << size << " element time: " << duration.count() << "ms" << endl;
	auto vecIt = std::find(vec.begin(), vec.end(), value);
	{
		Timer t;
		/*cout << "vector erase [0, " << size / 2 << "]: ";
		vec.erase(vec.begin(), vecIt);*/
		cout << "vector erase " << size / 2 << " : ";
		vec.erase(vecIt);
	}
}


int main(int argc, char* argv[])
{
	testBaseFunction();
	testStd();
	testInsertRange(5000000);
	testTime(1000000);

	return 0;
}
