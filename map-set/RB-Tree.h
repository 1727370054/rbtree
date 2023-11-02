#pragma once

#include <iostream>
#include <utility>
using namespace std;

enum Colour
{
	BLACK,
	RED,
};

template<class ValueType>
struct RBTreeNode
{
	RBTreeNode<ValueType>* _left;
	RBTreeNode<ValueType>* _right;
	RBTreeNode<ValueType>* _parent;

	ValueType _data;

	Colour _color;

	RBTreeNode(const ValueType& data)
		:_data(data), _left(nullptr), _right(nullptr), _parent(nullptr)
	{}
};

//迭代器
template<class ValueType>
class __TreeIterator
{
	typedef RBTreeNode<ValueType> Node;
	typedef __TreeIterator<ValueType> self;
	Node* _node;
public:
	__TreeIterator(Node* node)
		:_node(node)
	{}

	ValueType& operator*()
	{
		return _node->_data;
	}

	ValueType* operator->()
	{
		return &_node->_data;
	}

	self& operator++()
	{
		//1、右不为空，中序的下一个就是右子树的最左结点
		//2、右为空，说明该子树访问完毕，中序的下一个就是它的祖先
		if (_node->_right)
		{
			Node* SubRLeft = _node->_right;
			while (SubRLeft->_left)
			{
				SubRLeft = SubRLeft->_left;
			}
			_node = SubRLeft;
		}
		else
		{
			//cur是父亲的左，下一个遍历的节点就是父节点
			//cur是父亲的右，下一个遍历的就是父节点的父节点
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_right)
			{
				cur = parent;
				parent = parent->_parent;
			}

			_node = parent; 
		}
		return *this;
	}

	self& operator--()
	{
		return *this;
	}

	bool operator!=(const self& s)
	{
		return _node != s._node;
	}
};

template<class K, class ValueType, class KeyOfValue>
class RBTree
{
	typedef RBTreeNode<ValueType> Node;
public:
	typedef __TreeIterator<ValueType> iterator;

	iterator begin()
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}
		return iterator(cur);
	}

	iterator end()
	{
		return nullptr;
	}

	pair<iterator, bool> Insert(const ValueType& data)
	{
		//1.按搜索树的规则进行插入
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_color = BLACK;
			return make_pair(iterator(_root), true);
		}

		KeyOfValue kofv;
		Node* cur = _root;
		Node* parent = nullptr;

		while (cur)
		{
			if (kofv(cur->_data) < kofv(data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kofv(cur->_data) > kofv(data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return make_pair(iterator(cur), false);
			}
		}

		cur = new Node(data);
		Node* newNode = cur;
		if (kofv(parent->_data) < kofv(cur->_data))
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//新增结点设置为红
		cur->_color = RED;
		while (parent && parent->_color == RED)
		{
			//红黑树主要看uncle的颜色
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				//情况一、uncle存在且为红
				if (uncle && uncle->_color == RED)
				{
					parent->_color = uncle->_color = BLACK;
					grandfather->_color = RED;

					//继续往上处理
					cur = grandfather;
					parent = cur->_parent;
				}
				else //情况二 or 情况三：uncle不存在 or uncle存在且为黑
				{
					//情况三：双旋变单旋
					if (cur == parent->_right)
					{
						RotateL(parent);
						swap(parent, cur);
					}
					//情况二（也有可能是第三种情况变过来）
					RotateR(grandfather);
					grandfather->_color = RED;
					parent->_color = BLACK;

					break;
				}
			}
			else    // grandfather->_right == parent 
			{
				Node* uncle = grandfather->_left;
				//情况一、uncle存在且为红
				if (uncle && uncle->_color == RED)
				{
					parent->_color = uncle->_color = BLACK;
					grandfather->_color = RED;

					//继续往上处理
					cur = grandfather;
					parent = cur->_parent;
				}
				else  //情况二 or 情况三：uncle不存在 or uncle存在且为黑
				{
					//情况三：双旋变单旋
					if (cur == parent->_left)
					{
						RotateR(parent);
						swap(parent, cur);
					}
					//情况二（也有可能是第三种情况变过来）
					RotateL(grandfather);
					grandfather->_color = RED;
					parent->_color = BLACK;

					break;
				}
			}
		}

		_root->_color = BLACK; //保证根必须为黑
		return make_pair(iterator(newNode), true);
	}

	//左单旋
	void RotateL(Node* parent)
	{
		Node* SubR = parent->_right;
		Node* SubRL = SubR->_left;

		parent->_right = SubRL;
		if (SubRL)
			SubRL->_parent = parent;

		SubR->_left = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = SubR;

		if (_root == parent)
		{
			_root = SubR;
			SubR->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
				ppNode->_left = SubR;
			else
				ppNode->_right = SubR;

			SubR->_parent = ppNode;
		}
	}

	//右单旋
	void RotateR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;

		parent->_left = SubLR;
		if (SubLR)
			SubLR->_parent = parent;

		SubL->_right = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = SubL;

		if (_root == parent)
		{
			_root = SubL;
			SubL->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
				ppNode->_left = SubL;
			else
				ppNode->_right = SubL;

			SubL->_parent = ppNode;
		}
	}

	iterator Find(const K& key)
	{
		KeyOfValue kofv;
		Node* cur = _root;
		while (cur)
		{
			if (kofv(cur->_data) < key)
			{
				cur = cur->_right;
			}
			else if (kofv(cur->_data) > key)
			{
				cur = cur->_left;
			}
			else
			{
				return iterator(cur);
			}
		}
		return iterator(nullptr);
	}

private:
	Node* _root = nullptr;
};
