#define  _CRT_SECURE_NO_WARNINGS 
#include<iostream>
using namespace std;

int bar(int c, int d)
{
	int e = c + d;
	return e;
}

int foo(int a, int b)
{
	return bar(a, b);
}


int main()
{
	foo(2, 3);
	system("pause");
	return 0;
}