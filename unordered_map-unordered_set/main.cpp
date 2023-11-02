#include <iostream>
#include "unordered_map.h"
#include "unordered_set.h"

void test_unordered_set()
{
	stl::unordered_set<int> s;
	s.insert(10);
	s.insert(20);
	s.insert(30);
	s.insert(40);

	stl::unordered_set<int>::iterator it = s.begin();
	while (it != s.end())
	{
		std::cout << *it << " ";
		++it;
	}
}

void test_unordered_map()
{
	stl::unordered_map<std::string, std::string> dict;
	dict.insert(std::make_pair("sort", "����"));
	dict.insert(std::make_pair("hash", "��ϣ"));
	dict.insert(std::make_pair("hello", "���"));
	dict.insert(std::make_pair("message", "��Ϣ"));
	dict["left"] = "���";
	dict["message"] = "����";

	stl::unordered_map<std::string, std::string>::iterator it = dict.begin();
	while (it != dict.end())
	{
		std::cout << it->first << ":" << it->second<< " ";
		++it;
	}
}

int main()
{
	//Test_Hash();
	test_unordered_set();
	std::cout << "\n";
	test_unordered_map();
	std::cout << "\n";
	return 0;
}