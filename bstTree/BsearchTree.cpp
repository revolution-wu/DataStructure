#include "BsearchTree.h"
#include <stdlib.h>
#include <unordered_set>
#include <iostream>
BSTree<int> * test01()
{
	BSTree<int> * b = new BSTree<int>();

	b->insert(5);
	b->insert(3);
	b->insert(1);
	b->insert(2);
	b->insert(4);
	b->insert(0);
	b->insert(7);
	b->insert(6);
	b->insert(9);
	//b->insert(10);
	b->insert(8);
	//b->insert(12);
	

	std::vector<int> v;
	b->inorderTraversal(v);
	for (const auto& it : v)
	{
		std::cout << it << std::endl;
	}
	//b->preorderTraversal();
	int size = 0;
	size_t h = b->height(&size);
	printf("高度: %lu\n", h);
	printf("高度: %d\n", b->getHeight());
	
	printf("叶子节点数量:%d\n", size);
	if (b->isComplete())
	{
		printf("是完全二叉树\n");
	}
	else
	{
		printf("不是完全二叉树\n");

	}
	//b->reverTree();

	b->preorderTraversal();
	printf("\n");
	b->postorderTraversal();
	printf("\n");

	printf("%ld\n", sizeof(std::unordered_set<int>));
	printf("size: %ld\n", b->size());
	b->InOrder();
	b->preOrder();
	b->postOrder();
	b->reverseTree();
	b->preOrder();
	b->reverseTree();
	b->preOrder();
	BSTreeNode<int>* serch = b->getNodeByKey(5);
	if (serch)
	{
		printf("查找节点的值: %d\n", serch->key_);
	}
	BSTreeNode<int>* pre = b->predecessor(serch);
	if (pre)
	{
		printf("前驱节点的值: %d\n", pre->key_);

	}
	BSTreeNode<int>* sucessor = b->successor(serch);
	if (sucessor)
	{
		printf("后驱节点的值: %d\n", sucessor->key_);

	}
	b->InOrder();
	b->remove(4);
	b->InOrder();
	printf("------------------------------------------------\n");

	b->remove(5);
	b->InOrder();
	b->remove(10);
	b->remove(12);
	b->remove(7);
	b->remove(1);
	b->remove(3);
	b->InOrder();
	b->remove(12);
	b->remove(0);
	b->remove(2);
	b->InOrder();
	b->remove(6);
	b->InOrder();
	b->remove(8);
	b->remove(9);
	b->InOrder();

	return b;
}

void test02()
{
	BSTree<int> b;

	b.insert(3);
	b.insert(1);
	b.insert(2);
	b.insert(5);
	b.insert(4);
	b.insert(0);
	b.insert(7);
	b.insert(6);
	b.insert(9);
	//b.insert(10);
	b.insert(8);
	//b.insert(12);
	b.clear();
	printf("删除后size: %lu\n", b.size());
	b.InOrder();
	b.remove(4);
	b.InOrder();
}
int main()
{
	BSTree<int>* b = test01();
	delete b;
	//test02();
	system("pause");

}
