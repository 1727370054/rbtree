#pragma once
#include "RB-Tree.h"

namespace stl
{
	template<class K, class V>
	class map
	{
		typedef pair<K, V> ValueType;
		struct MapKeyOf_T  
		{
			const K& operator()(const ValueType& v)
			{
				return v.first;
			}
		};
	public:
		typedef typename RBTree<K, ValueType, MapKeyOf_T>::iterator iterator;

		iterator begin()
		{
			return _t.begin();
		}

		iterator end()
		{
			return _t.end();
		}

		pair<iterator, bool> insert(const ValueType& v)
		{
			return _t.Insert(v);
		}

		iterator find(const K& key)
		{
			return _t.Find(key);
		}

		V& operator[](const K& key)
		{
			// V()������Ĭ�Ϲ��죬����һ��ȱʡֵ(int���͸�0��string���ǿմ�)
			pair<iterator, bool> ret = _t.Insert(make_pair(key, V()));
			return ret.first->second;
		}

	private:
		RBTree<K, ValueType, MapKeyOf_T> _t;
	};
}
