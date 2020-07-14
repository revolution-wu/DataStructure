#define  _CRT_SECURE_NO_WARNINGS 

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include "skip_list.h"
#include "Timer.h"
//#include <time.h>
using std::cout;
using std::endl;
using namespace structure;

const int kSize = 1000 * 1000 * 10 * 1;
class Person
{
public:
	Person(int i):m_i(i) {}
	int m_i;
};

void testIterator()
{
	typedef SkipList<int, int> SkipList;
	SkipList skip;
	skip.insert(1, 2);
	skip.insert(3, 4);
	skip.insert(2, 3);
	skip.insert(5, 6);
	skip.insert(4, 5);
	skip.insert(6, 7);
	for (auto it : skip)
	{
		cout << it.first << " " << it.second << endl;
	}
}

void testTimer(int size)
{
	cout << "element size: " << size << endl;
	cout << "--------- test SkipList ---------\n";

	SkipList<int, int> skip;

	{
		Timer t;
		cout << "insert size: " << size << ": ";
		for (int i = 0; i < size - 1; ++i)
		{
			skip.insert(i, i + 10);
		}
	}

	{
		cout << "insert one: ";
		Timer t(true);
		skip.insert(size - 1, size - 1 + 10);
	}

	
	{
		cout << "SkipList::find: ";
		Timer t(true);
		assert(skip.find(size / 2)->second == size / 2 + 10);
	}
	
	{
		cout << "std::find(): ";
		Timer t;
		std::pair<int, int> value(size / 2, (size >> 1) + 10);
		assert(std::find(skip.begin(), skip.end(), value) != skip.end());
	}
	cout << "skiplist: level: " << skip.level() << endl;
	{
		Timer t(true);
		skip.remove(size -1 );
	}
	
	cout << "--------- test std::list ---------\n";
	std::list<std::pair<int, int>> l;
	{
		Timer t;
		for (int i = 0; i < size; ++i)
		{
			l.push_back(std::make_pair(i, i + 10));
		}
	}

	std::pair<int, int> value(size / 2 , size / 2 + 10);
	{
		cout << "std::list:find: ";
		Timer t;
		assert(std::find(l.begin(), l.end(), value) != l.end());
	}


	{
		Timer t;
		l.remove(value);
	}
	//cout << "---------test std::map ---------\n";
	//std::map<int, int> m;
	//{
	//	Timer t;
	//	for (int i = 0; i < size; ++i)
	//	{
	//		m.insert(std::make_pair(i, i + 10));
	//	}
	//}

	//{
	//	cout << "std::map:find: ";
	//	Timer t(true);
	//	assert(m.find(size / 2)->second == size / 2 + 10);
	//}


	//{
	//	Timer t(true);
	//	m.erase(size / 2);
	//}
	skip.clear();
}
int main(int argc, char* argv[])
{
	testTimer(kSize);
	testIterator();
	cout << "---------------------" << endl;
	system("pause");
	return 0;
}