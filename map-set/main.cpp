#include "stl-map.h"
#include "stl-set.h"
using namespace stl;

void testset()
{
	set<int> s;
	s.insert(0);
	s.insert(5);
	s.insert(4);
	s.insert(6);
	s.insert(7);
	s.insert(1);

	set<int>::iterator it = s.begin();
	while (it != s.end())
	{
		cout << *it << " ";
		++it;
	}

	set<int>::iterator pos = s.find(0);
	if (pos != s.end())
	{
		cout <<"查找到key：" << *pos << endl;
	}
	else
	{
		cout << "未查找到！" << endl;
	}
}

void testmap()
{
	/*map<int, int> m;
	m.insert(make_pair(1, 2));
	m.insert(make_pair(2, 5));
	m.insert(make_pair(3, 8));
	m.insert(make_pair(9, 8));
	m.insert(make_pair(0, 8));


	map<int, int>::iterator it = m.begin();*/
	//while (it != m.end())
	//{
	//	cout << it->first << ":" << it->second << " ";
	//	++it;
	//}
	//cout << endl;

	/*for (auto kv : m)
	{
		cout << kv.first << ":" << kv.second << " ";
	}

	map<int, int>::iterator pos = m.find(9);
	if (pos != m.end())
	{
		cout <<"查找到key: " << pos->first << ":" << pos->second << " ";
	}
	else
	{
		cout << "未查找到！" << endl;
	}*/

	string strs[] = { "西瓜","樱桃","西瓜","苹果","西瓜","西瓜","香蕉" };
	map<string, int> countMap;
	countMap["哈希"] = 10;
	for (auto& str : strs)
	{
		countMap[str]++;
	}

	for (auto kv : countMap)
	{
		cout << kv.first << ":" << kv.second << " ";
	}
}

int main(void)
{
	// testset();
	cout << endl;
	testmap(); 

	system("pause");
	return 0;
}

