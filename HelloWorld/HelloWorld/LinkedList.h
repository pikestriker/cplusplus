#pragma once

template <typename T>
class LinkedList
{
private:
	T data;
	LinkedList<T>* headNode;
	LinkedList<T>* nextNode;
	LinkedList<T>* curNode;
	int numNodes;
	LinkedList<T>* findPrevious();
public:
	LinkedList();
	~LinkedList();
	int getNumNodes();
	void destroy();
	void addNode(T &data);
	T getData();
	void setData(T &data);
	void toNextNodeLoop();
	void toHead();
	void toNext();
	void deleteNode();
	bool atHead();
	bool atTail();
};

template <typename T>
LinkedList<T>::LinkedList() : headNode(0), nextNode(0), curNode(0), numNodes(0)
{
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	destroy();
}

template <typename T>
void LinkedList<T>::destroy()
{
	//do my cleanup of the linked list
	toHead();
	while (curNode)
	{
		deleteNode();
	}
}

template <typename T>
LinkedList<T>* LinkedList<T>::findPrevious()
{
	if (curNode == headNode || !curNode)
		return 0;

	LinkedList* tempNode = headNode;

	while (tempNode)
	{
		if (tempNode->nextNode == curNode)
			return tempNode;

		tempNode = tempNode->nextNode;
	}

	return 0;
}

//adds a node
template <typename T>
void LinkedList<T>::addNode(T &data)
{
	LinkedList* newNode = new LinkedList();
	newNode->data = data;
	if (!headNode)		//non-initialized list
	{
		headNode = newNode;
	}
	else
	{
		//at this point the curNode should always be set when the head node is set
		newNode->nextNode = curNode->nextNode;		//this works even if the next node is null
		curNode->nextNode = newNode;
	}

	curNode = newNode;
	numNodes++;
}

template <typename T>
int LinkedList<T>::getNumNodes()
{
	return numNodes;
}

template <typename T>
T LinkedList<T>::getData()
{
	if (curNode)
		return curNode->data;

	return data;
}

template <typename T>
void LinkedList<T>::setData(T &data)
{
	if (curNode)
		curNode->data = data;
}

template <typename T>
void LinkedList<T>::toNextNodeLoop()
{
	if (curNode->nextNode)
		curNode = curNode->nextNode;
	else
		curNode = headNode;
}

template <typename T>
void LinkedList<T>::toHead()
{
	curNode = headNode;
}

template <typename T>
void LinkedList<T>::toNext()
{
	if (curNode)
	{
		if (curNode->nextNode)
			curNode = curNode->nextNode;
	}
}

template <typename T>
void LinkedList<T>::deleteNode()
{
	if (curNode)
	{
		LinkedList* tempNode = curNode;
		LinkedList* prevNode = findPrevious();
		curNode = curNode->nextNode;

		if (prevNode)
			prevNode->nextNode = curNode;
		else
			headNode = curNode;
		delete tempNode;
	}

	numNodes--;
}

template <typename T>
bool LinkedList<T>::atHead()
{
	return curNode == headNode ? true : false;
}

template <typename T>
bool LinkedList<T>::atTail()
{
	if ((curNode && !curNode->nextNode) || !curNode)
		return true;
	return false;
}