#pragma once

#include "HashTable.h"
#include <utility>

namespace stl
{
	template<class K, class V, class Hash = _Hash<K>>
	class unordered_map
	{
	private:
		typedef std::pair<K, V> ValueType;
		struct MapKeyOf_T
		{
			const K& operator()(const ValueType& kv)
			{
				return kv.first;
			}
		};
	public:
		typedef typename HashTable<K, ValueType, MapKeyOf_T, Hash>::iterator iterator;

		iterator begin()
		{
			return _hash.begin();
		}
		iterator end()
		{
			return _hash.end();
		}

		std::pair<iterator, bool> insert(const ValueType& kv)
		{
			return _hash.Insert(kv);
		}

		V& operator[](const K& key)
		{
			std::pair<iterator, bool> ret = _hash.Insert(std::make_pair(key, V()));
			return ret.first->second;
		}

	private:
		HashTable<K, ValueType, MapKeyOf_T, Hash> _hash;
	};
}