#define  _CRT_SECURE_NO_WARNINGS 
#include<iostream>
using namespace std;

void up_and_down(int n)
{
	cout << "Level :n " << n << " location " << &n << endl; /* 1 */
	if (n<4)
		up_and_down(n + 1);
	cout << "Level :n " << n << " location " << &n << endl; ; /* 2 */
	 
}
void test01()
{	 
	up_and_down(1);	
}
int main()
{
	test01();
	system("pause");
	return 0;
}