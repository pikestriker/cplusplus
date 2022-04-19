#include <iostream>
#include <vector>
#include <string>
#include "LinkedList.h"

struct Person
{
	std::string name;
	int age;
};

void printList(LinkedList<Person>& input)
{
	input.toHead();
	Person curPerson;
	std::vector<Person> nodes;

	//****************
	//*  * name   *  *  ->
	//*  * age    *  *  <-
	//****************
	int maxSize = 0;

	for (int i = 0; i < input.count(); i++)
	{
		curPerson = input.getData();
		nodes.push_back(curPerson);
		if (curPerson.name.size() > maxSize)
		{
			maxSize = curPerson.name.size();
		}
		input.toNext();
	}

	//std::cout << "****" << std::string(maxSize + 4, '*') << "****    ";
	for (int i = 0; i < nodes.size(); i++)
	{
		std::cout << "****" << std::string(maxSize + 4, '*') << "****    ";
	}

	std::cout << std::endl;

	for (int i = 0; i < nodes.size(); i++)
	{
		std::string curN = nodes[i].name;
		std::cout << "*  * " << curN << std::string(maxSize - curN.size() + 3, ' ') << "*  * -> ";
	}

	std::cout << std::endl;

	for (int i = 0; i < nodes.size(); i++)
	{
		std::string ageS = std::to_string(nodes[i].age);
		std::cout << "*  * " << ageS << std::string(maxSize - ageS.size() + 3, ' ') << "*  * <- ";
	}

	std::cout << std::endl;

	for (int i = 0; i < nodes.size(); i++)
	{
		std::cout << "****" << std::string(maxSize + 4, '*') << "****    ";
	}
	/*while (!input.atTail())
	{
		curPerson = input.getData();
		std::cout << "Name: " << curPerson.name << std::endl;
		std::cout << "Age : " << curPerson.age << std::endl;
		input.toNext();
	}

	curPerson = input.getData();
	std::cout << "Name: " << curPerson.name << std::endl;
	std::cout << "Age : " << curPerson.age << std::endl;*/
}

int main()
{
	LinkedList<Person> family;
	Person curPerson;
	curPerson.name = "Richard";
	curPerson.age = 43;
	family.addNode(curPerson);
	curPerson.name = "Christine";
	curPerson.age = 42;
	family.addNode(curPerson);
	curPerson.name = "Reid";
	curPerson.age = 13;
	family.addNode(curPerson);
	curPerson.name = "Luke";
	curPerson.age = 10;
	family.addNode(curPerson);
	curPerson.name = "Owen";
	curPerson.age = 8;
	family.addNode(curPerson);

	printList(family);

	family.toHead();
	family.removeNode();

	std::cout << std::endl;

	printList(family);

	std::cout << std::endl;

	family.toHead();
	family.toNext();
	family.removeNode();
	printList(family);

	std::cout << std::endl;

	family.toTail();
	family.removeNode();
	printList(family);

	return 0;
}