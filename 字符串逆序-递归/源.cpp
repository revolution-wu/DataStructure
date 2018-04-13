#define  _CRT_SECURE_NO_WARNINGS 
#include<iostream>
using namespace std;

int ReverseStr(char* str, char* outbuf)
{
	if (*str == '\0')
	{
		return 0; //退出递归的条件，不是说退出函数的条件
	}

	ReverseStr(str + 1,outbuf);

	cout << *str << endl;
	//strncat(outbuf, str, 1);
	return 0;
}
int main()
{
	char str[] = "abcdefg";
	char buf[128] = { 0 };
	ReverseStr(str, buf);

	//cout << buf << endl;;
	system("pause");
	return 0;
}