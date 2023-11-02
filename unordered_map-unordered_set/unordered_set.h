#pragma once

#include "HashTable.h"

namespace stl
{
	template<class K, class Hash = _Hash<K>>
	class unordered_set
	{
	private:
		typedef K ValueType;
		struct SetKeyOf_T
		{
			const K& operator()(const ValueType& key)
			{
				return key;
			}
		};
	public:
		typedef typename HashTable<K, ValueType, SetKeyOf_T, Hash>::iterator iterator;

		iterator begin()
		{
			return _hash.begin();
		}
		iterator end()
		{
			return _hash.end();
		}

		std::pair<iterator, bool> insert(const ValueType& key)
		{
			return _hash.Insert(key);
		}
	private:
		HashTable<K, ValueType, SetKeyOf_T, Hash> _hash;
	};
}