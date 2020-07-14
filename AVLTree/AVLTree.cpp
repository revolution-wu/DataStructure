#include "AVLTree.h"
#include <stdlib.h>

void test01()
{
	
	
	AVLTree<int> * b = new AVLTree<int>();

	b->insert(5);
	b->insert(3);
	b->insert(1);
	b->insert(2);
	b->insert(4);
	b->insert(0);
	b->insert(7);
	b->insert(6);
	b->insert(9);
	b->insert(10);
	b->insert(8);
	b->insert(12);
	b->insert(13);
	b->inorder();

	b->remove(8);
	b->inorder();
	b->remove(10);
	b->inorder();
}
int main()
{
	test01();
	system("pause");
}