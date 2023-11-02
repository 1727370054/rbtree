#pragma once

#include <iostream>
#include <string>
#include <vector>

template<class ValueType>
struct HashNode
{
	ValueType _data;
	HashNode<ValueType>* _next;
	HashNode(const ValueType& data):_data(data), _next(nullptr){}
};

template <class K>
struct _Hash
{
	const K& operator()(const K& key)
	{
		return key;
	}
};

//模板的特化
template <>
struct _Hash<std::string>
{
	size_t operator()(const std::string& key)
	{
		// BKDR hash
		size_t hash = 0;
		for (size_t i = 0; i < key.size(); i++)
		{
			hash *= 131;
			hash += key[i];
		}

		return hash;
	}
};

// 迭代器实现
//////////////////////////////////////////////////////////////////////////
template<class K, class ValueType, class KeyOfValue, class Hash>
class HashTable;

template<class K, class ValueType, class KeyOfValue, class Hash>
struct __HashTableIterator
{
	typedef __HashTableIterator<K, ValueType, KeyOfValue, Hash> Self;
	typedef HashTable<K, ValueType, KeyOfValue, Hash> HT;
	typedef HashNode<ValueType> Node;
	Node* _node;
	HT* _pHT;

	__HashTableIterator(Node* node, HT* pHT)
		:_node(node), _pHT(pHT)
	{}

	ValueType& operator*()
	{
		return _node->_data;
	}

	ValueType* operator->()
	{
		return &_node->_data;
	}

	Self& operator++()
	{
		if (_node->_next)
		{
			_node = _node->_next;
		}
		else
		{
			//找到下一个哈希桶
			KeyOfValue kofv;
			size_t i = _pHT->HashFunc(kofv(_node->_data)) % _pHT->_tables.size();
			i++;
			for (; i < _pHT->_tables.size(); i++)
			{
				Node* cur = _pHT->_tables[i];
				if (cur)
				{
					_node = cur;
					return *this;
				}
			}
			_node = nullptr;
		}
		return *this;
	}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}
};

///////////////////////////////////////////////////////////////////////////

template<class K, class ValueType, class KeyOfValue, class Hash>
class HashTable
{
	typedef HashNode<ValueType> Node;
public:
	friend struct __HashTableIterator<K, ValueType, KeyOfValue, Hash>;
	typedef __HashTableIterator<K, ValueType, KeyOfValue, Hash> iterator;

	iterator begin()
	{
		for (size_t i = 0; i < _tables.size(); i++)
		{
			if (_tables[i])
			{
				return iterator(_tables[i], this);
			}
		}
		return end();
	}

	iterator end()
	{
		return iterator(nullptr, this);
	}

	size_t HashFunc(const K& key)
	{
		Hash hash;
		return hash(key);
	}

	size_t GetNextPrime(size_t prime)
	{
		const int PRIMECOUNT = 28;
		const static size_t primeList[PRIMECOUNT] =
		{
		 53ul, 97ul, 193ul, 389ul, 769ul,
		 1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
		 49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
		 1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
		 50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
		 1610612741ul, 3221225473ul, 4294967291ul
		};

		for (size_t i = 0; i < PRIMECOUNT; ++i)
		{
			if (primeList[i] > prime)
				return primeList[i];
		}

		return primeList[PRIMECOUNT - 1];
	}

	std::pair<iterator, bool> Insert(const ValueType& data)
	{
		KeyOfValue kofv;

		if (_num == _tables.size())
		{
			//扩容
			//1、开二倍大小的新表
			//2、遍历旧表的数据，重新计算新表的位置
			//3、释放旧表
			std::vector<Node*> newTables;
			// size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
			size_t newSize = GetNextPrime(_tables.size());
			newTables.resize(newSize);
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;
					size_t index = HashFunc(kofv(cur->_data)) % newTables.size();
					cur->_next = newTables[index];
					newTables[index] = cur;
					cur = next;
				}
				_tables[i] = nullptr;
			}
			_tables.swap(newTables);
		}

		size_t index = HashFunc(kofv(data)) % _tables.size();
		//查找这个值在不在表中
		Node* cur = _tables[index];
		while (cur)
		{
			if (kofv(cur->_data) == kofv(data))
				return std::make_pair(iterator(cur, this), false);
			else
				cur = cur->_next;
		}

		Node* newNode = new Node(data);
		newNode->_next = _tables[index];
		_tables[index] = newNode;
		_num++;

		return std::make_pair(iterator(newNode, this), true);
	}

	Node* Find(const K& key)
	{
		KeyOfValue kofv;
		size_t index = HashFunc(key) % _tables.size();
		Node* cur = _tables[index];
		while (cur)
		{
			if (kofv(cur->_data) == key)
				return cur;
			else
				cur = cur->_next;
		}
		return nullptr;
	}

	bool Erase(const K& key)
	{
		KeyOfValue kofv;
		size_t index = HashFunc(key) % _tables.size();
		Node* prev = nullptr;
		Node* cur = _tables[index];
		while (cur)
		{
			if (kofv(cur->_data) == key)
			{
				if (prev == nullptr) //删除的是第一个结点
					_tables[index] = cur->_next;
				else
					prev->_next = cur->_next;

				delete cur;
				return true;
			}
			else
			{
				prev = cur;
				cur = cur->_next;
			}
		}
		return false;
	}

	void Clear()
	{
		for (int i = 0; i < _tables.size(); i++)
		{
			Node* cur = _tables[i];
			while (cur)
			{
				Node* next = cur->_next;
				delete cur;
				cur = next;
			}
			_tables[i] = nullptr;
		}
	}

	~HashTable()
	{
		Clear();
	}

private:
	std::vector<Node*> _tables;
	int _num = 0;
};

//template<class K>
//struct SetKeyofValue
//{
//	const K& operator()(const K& key)
//	{
//		return key;
//	}
//};
//
//void Test_Hash()
//{
//	HashTable<int, int, SetKeyofValue<int>, _Hash<int>> hash;
//	hash.Insert(4);
//	hash.Insert(14);
//	hash.Insert(24);
//	hash.Insert(5);
//	hash.Insert(15);
//	hash.Insert(25);
//	hash.Insert(6);
//	hash.Insert(16);
//	hash.Insert(26);
//	hash.Insert(36);
//	hash.Insert(33);
//	hash.Insert(44);
//
//	HashTable<std::string, std::string, SetKeyofValue<std::string>,_Hash<std::string>> hash1;
//	hash1.Insert("sort");
//
//	std::cout << hash1.HashFunc("aadd") << std::endl;
//	std::cout << hash1.HashFunc("abcd") << std::endl;
//}
