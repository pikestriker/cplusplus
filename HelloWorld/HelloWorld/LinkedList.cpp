#include <LinkedList.h>

template <class T>

LinkedList::LinkedList() : headNode(0), curNode(0), nextNode(0)
{
}

LinkedList::~LinkedList()
{
	destroy();
}