#include <iostream>
#include <vector>
#include <LinkedList.h>

struct MyData
{
	std::string name;
	int age;
};

int main()
{
	LinkedList<MyData> myLinkedList;
	std::vector<MyData> myVector;
	MyData curData;
	curData.name = "Richard";
	curData.age = 42;
	myVector.push_back(curData);
	myLinkedList.addNode(curData);
	curData.name = "Christine";
	curData.age = 41;
	myVector.push_back(curData);
	myLinkedList.addNode(curData);
	curData.name = "Reid";
	curData.age = 12;
	myVector.push_back(curData);
	myLinkedList.addNode(curData);
	curData.name = "Luke";
	curData.age = 9;
	myVector.push_back(curData);
	myLinkedList.addNode(curData);
	curData.name = "Owen";
	curData.age = 7;
	myVector.push_back(curData);
	myLinkedList.addNode(curData);

	std::cout << "Vector output:" << std::endl;
	for (int i = 0; i < myVector.size(); i++)
	{
		std::cout << myVector[i].name << " is " << myVector[i].age << " years old" << std::endl;
	}

	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "LinkedList output:" << std::endl;
	myLinkedList.toHead();
	while (1)
	{
		curData = myLinkedList.getData();
		std::cout << curData.name << " is " << curData.age << " years old" << std::endl;
		if (myLinkedList.atTail())
			break;
		myLinkedList.toNext();
	}
	std::cout << "Hello, world!" << std::endl;
	return 0;
}