#pragma once

template <class T>
class LinkedList
{
private:
	LinkedList* head;
	LinkedList* next;
	LinkedList* prev;
	LinkedList* tail;
	LinkedList* cur;
	int numNodes;

	T data;

public:
	LinkedList();
	~LinkedList();
	void destroy();

	void initList();
	void setData(T data);
	T getData();
	void addNode(T data);
	void toPrev();
	void toNext();
	bool atHead();
	bool atTail();
	bool isInitialized();
	void removeNode();
	void toHead();
	void toTail();
	int count();

};

template <class T>
void LinkedList<T>::destroy()
{
	//LinkedList* cur = head;
	toHead();

	if (isInitialized())
	{
		while (cur->next != 0)
		{
			LinkedList* temp = cur->next;
			delete cur;
			cur = temp;
		}

		delete cur;

		cur = 0;
		head = 0;
		tail = 0;
	}
}

template <class T>
LinkedList<T>::~LinkedList()
{
	destroy();
}

template <class T>
LinkedList<T>::LinkedList() : head(0), tail(0), cur(0), next(0), prev(0), numNodes(0)
{
	//initList();
}

template <class T>
void LinkedList<T>::initList()
{
	head = this;
	tail = this;
	cur = this;
	next = 0;
	prev = 0;
}

template <class T>
void LinkedList<T>::setData(T data)
{
	this->data = data;
}

template <class T>
T LinkedList<T>::getData()
{
	return cur->data;
}

template <class T>
void LinkedList<T>::addNode(T data)
{
	LinkedList* newNode = new LinkedList();
	newNode->setData(data);

	if (!isInitialized())
	{
		head = newNode;
		cur = newNode;
		tail = newNode;
	}
	else
	{
		LinkedList* temp = cur->next;
		cur->next = newNode;
		newNode->prev = cur;
		cur = newNode;

		if (temp != 0)
		{
			temp->prev = newNode;
		}
		else
		{
			tail = newNode;
		}
	}

	numNodes++;
}

template <class T>
bool LinkedList<T>::atHead()
{
	return cur == head;
}

template <class T>
bool LinkedList<T>::atTail()
{
	return cur == tail;
}

template <class T>
void LinkedList<T>::toPrev()
{
	if (isInitialized())
	{
		if (cur->prev == 0)
			cur = tail;
		else
			cur = cur->prev;
	}
}

template <class T>
void LinkedList<T>::toNext()
{
	if (isInitialized())
	{
		if (cur->next == 0)
			cur = head;
		else
			cur = cur->next;
	}
}

template <class T>
bool LinkedList<T>::isInitialized()
{
	return cur != 0;
}

template <class T>
void LinkedList<T>::removeNode()
{
	if (isInitialized())
	{
		//removing the current node
		LinkedList* tempPrev = cur->prev;
		LinkedList* tempNext = cur->next;
		LinkedList* tempCur = cur;

		if (tempPrev != 0)
		{
			tempPrev->next = tempNext;
			cur = tempPrev;
		}

		if (tempNext != 0)
		{
			tempNext->prev = tempPrev;
			cur = tempNext;
		}

		if (head == tempCur)
			head = cur;

		if (tail == tempCur)
			tail = cur;

		delete tempCur;
		numNodes--;
	}
}

template <class T>
int LinkedList<T>::count()
{
	return numNodes;
}

template <class T>
void LinkedList<T>::toHead()
{
	cur = head;
}

template <class T>
void LinkedList<T>::toTail()
{
	cur = tail;
}