#pragma once
#include "RB-Tree.h"

namespace stl
{
	template<class K>
	class set
	{
		typedef K ValueType;
		struct SetKeyOf_T
		{
			const K& operator()(const ValueType& key)
			{
				return key;
			}
		};
	public:
		typedef typename RBTree<K, ValueType, SetKeyOf_T>::iterator iterator;

		iterator begin()
		{
			return _t.begin();
		}

		iterator end()
		{
			return _t.end();
		}

		pair<iterator, bool> insert(const ValueType& key)
		{
			return _t.Insert(key);
		}

		iterator find(const K& key)
		{
			return _t.Find(key);
		}
	private:
		RBTree<K, ValueType, SetKeyOf_T> _t;
	};
}
