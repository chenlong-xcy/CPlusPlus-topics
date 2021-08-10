#pragma once
#include "Date.h"
#include <iostream>
#include <assert.h>
using namespace std;

namespace cl
{
	template<class T>
	struct _list_node
	{
		//���캯��
		_list_node(const T& val = T())
			:_val(val)
			, _prev(nullptr)
			, _next(nullptr)
		{}

		T _val;
		_list_node<T>* _prev;
		_list_node<T>* _next;
	};

	template<class T, class Ref, class Ptr>
	struct _list_iterator
	{
		typedef _list_node<T> node;
		typedef _list_iterator<T, Ref, Ptr> self;

		//���캯��
		_list_iterator(node* pnode)
			:_pnode(pnode)
		{}
		//ǰ��++
		self operator++()
		{
			_pnode = _pnode->_next; //�ý��ָ��ָ���һ�����
			return *this; //����������Ľ��ָ��
		}
		//ǰ��--
		self operator--()
		{
			_pnode = _pnode->_prev; //�ý��ָ��ָ��ǰһ�����
			return *this; //�����Լ���Ľ��ָ��
		}
		//����++
		self operator++(int)
		{
			self tmp(*this); //��¼��ǰ���ָ���ָ��
			_pnode = _pnode->_next; //�ý��ָ��ָ���һ�����
			return tmp; //��������ǰ�Ľ��ָ��
		}
		//����--
		self operator--(int)
		{
			self tmp(*this); //��¼��ǰ���ָ���ָ��
			_pnode = _pnode->_prev; //�ý��ָ��ָ��ǰһ�����
			return tmp; //�����Լ�ǰ�Ľ��ָ��
		}
		Ref operator*()
		{
			return _pnode->_val; //���ؽ��ָ����ָ��������
		}
		Ptr operator->()
		{
			return &_pnode->_val; //���ؽ��ָ����ָ�������ݵĵ�ַ
		}
		bool operator==(const self& s) const
		{
			return _pnode == s._pnode; //�ж��������ָ��ָ���Ƿ���ͬ
		}
		bool operator!=(const self& s) const
		{
			return _pnode != s._pnode; //�ж��������ָ��ָ���Ƿ�ͬ
		}
		node* _pnode;
	};

	template<class T>
	class list
	{
	public:
		typedef _list_node<T> node;
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, const T&, const T*> const_iterator;
		//���캯��
		list()
		{
			_head = new node; //����һ��ͷ���
			_head->_next = _head; //ͷ���ĺ��ָ��ָ���Լ�
			_head->_prev = _head; //ͷ����ǰ��ָ��ָ���Լ�
		}
		//�������캯��
		list(const list<T>& lt)
		{
			_head = new node; //����һ��ͷ���
			_head->_next = _head; //ͷ���ĺ��ָ��ָ���Լ�
			_head->_prev = _head; //ͷ����ǰ��ָ��ָ���Լ�
			for (const auto& e : lt)
			{
				push_back(e); //������lt���е�����һ����β�嵽��������
			}
		}
		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}
		////��ͳд��
		//list<T>& operator=(const list<T>& lt)
		//{
		//	if (this != &lt) //�����Լ����Լ���ֵ
		//	{
		//		clear(); //�������
		//		for (const auto& e : lt)
		//		{
		//			push_back(e); //������lt���е�����һ����β�嵽��������
		//		}
		//	}
		//	return *this; //֧��������ֵ
		//}
		void swap(list<T>& lt)
		{
			::swap(_head, lt._head);
		}
		//��ֵ��������غ���
		//�ִ�д��
		list<T>& operator=(list<T> lt) //������������ֵ��ʱ���Զ������俽�����캯��
		{
			swap(lt); //��������������
			return *this; //֧��������ֵ
		}
		//��������
		~list()
		{
			clear(); //��������
			delete _head; //�ͷ�ͷ���
			_head = nullptr; //ͷָ���ÿ�
		}
		iterator begin()
		{
			//����ʹ��ͷ����һ�����ĵ�ַ�����������ͨ������
			return iterator(_head->_next);
		}
		iterator end()
		{
			//����ʹ��ͷ���ĵ�ַ�����������ͨ������
			return iterator(_head);
		}
		const_iterator begin() const
		{
			//����ʹ��ͷ����һ�����ĵ�ַ���������const������
			return const_iterator(_head->_next);
		}
		const_iterator end() const
		{
			//����ʹ��ͷ���ĵ�ַ�����������ͨconst������
			return const_iterator(_head);
		}
		//void push_back(const T& x)
		//{
		//	node* newnode = new node(x);
		//	node* tail = _head->_prev;
		//	tail->_next = newnode;
		//	newnode->_prev = tail;
		//	_head->_prev = newnode;
		//	newnode->_next = _head;
		//}
		void insert(iterator pos, const T& x)
		{
			assert(pos._pnode);

			node* cur = pos._pnode;
			node* prev = cur->_prev;
			node* newnode = new node(x);

			newnode->_next = cur;
			cur->_prev = newnode;
			newnode->_prev = prev;
			prev->_next = newnode;
		}
		void push_back(const T& x)
		{
			insert(end(), x);
		}
		void push_front(const T& x)
		{
			insert(begin(), x);
		}
		iterator erase(iterator pos)
		{
			assert(pos._pnode);
			assert(pos != end());

			node* cur = pos._pnode;
			node* prev = cur->_prev;
			node* next = cur->_next;

			delete cur;
			prev->_next = next;
			next->_prev = prev;
			
			return iterator(next);
		}
		void pop_back()
		{
			erase(--end());
		}
		void pop_front()
		{
			erase(begin());
		}
		bool empty() const
		{
			return begin() == end();
		}
		size_t size() const
		{
			size_t sz = 0;
			const_iterator it = begin();
			while (it != end())
			{
				sz++;
				it++;
			}
			return sz;
		}
		T& front()
		{
			return *begin();
		}
		const T& front() const
		{
			return *begin();
		}
		T& back()
		{
			return *(--end());
		}
		const T& back() const
		{
			return *(--end());
		}
		void resize(size_t n, const T& val = T())
		{
			iterator i = begin();
			size_t len = 0;
			while (len < n&&i != end())
			{
				len++;
				i++;
			}
			if (len == n)
			{
				while (i != end())
				{
					i = erase(i);
				}
			}
			else
			{
				while (len < n)
				{
					push_back(val);
					len++;
				}
			}

		}
	private:
		node* _head;
	};
	void test1()
	{
		list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);
		lt1.push_back(5);
		for (auto e : lt1)
		{
			cout << e << " ";
		}
		cout << endl;
		list<int> lt2(lt1);
		lt2.pop_back();
		swap(lt2, lt1);
		for (auto e : lt1)
		{
			cout << e << " ";
		}
		cout << endl;
		for (auto e : lt2)
		{
			cout << e << " ";
		}
		cout << endl;
		cout << lt1.size() << endl;
		cout << lt2.size() << endl;
	}
	void test2()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_back(5);
		cout << ++lt.front() << endl;
		cout << ++lt.back() << endl;
	}
	void test3()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_back(5);
		lt.resize(10);
		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
		lt.resize(15, 9);
		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
		lt.resize(5);
		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;
	}
	void test4()
	{
		list<Date> lt;
		Date d1(2021, 8, 10);
		Date d2(1980, 4, 3);
		Date d3(1931, 6, 29);
		lt.push_back(d1);
		lt.push_back(d2);
		lt.push_back(d3);
		list<Date>::iterator pos = lt.begin();
		cout << pos->_year << endl; //�����һ�����ڵ����
	}
}



//namespace cl
//{
//	//ģ��ʵ��list���еĽ����
//	template<class T>
//	struct _list_node
//	{
//		//��Ա����
//		_list_node(const T& val = T()); //���캯��
//
//		//��Ա����
//		T _val;                 //������
//		_list_node<T>* _next;   //���ָ��
//		_list_node<T>* _prev;   //ǰ��ָ��
//	};
//
//	//ģ��ʵ��list������
//	template<class T, class Ref, class Ptr>
//	struct _list_iterator
//	{
//		typedef _list_node<T> node;
//		typedef _list_iterator<T, Ref, Ptr> self;
//
//		_list_iterator(node* pnode);  //���캯��
//
//		//������������غ���
//		self operator++();
//		self operator--();
//		self operator++(int);
//		self operator--(int);
//		bool operator==(const self& s) const;
//		bool operator!=(const self& s) const;
//		Ref operator*();
//		Ptr operator->();
//
//		//��Ա����
//		node* _pnode; //һ��ָ�����ָ��
//	};
//
//	//ģ��ʵ��list
//	template<class T>
//	class list
//	{
//	public:
//		typedef _list_node<T> node;
//		typedef _list_iterator<T, T&, T*> iterator;
//		typedef _list_iterator<T, const T&, const T*> const_iterator;
//
//		//Ĭ�ϳ�Ա����
//		list();
//		list(const list<T>& lt);
//		list<T>& operator=(const list<T>& lt);
//		~list();
//
//		//��������غ���
//		iterator begin();
//		iterator end();
//		const_iterator begin() const;
//		const_iterator end() const;
//
//		//����������غ���
//		T& front();
//		T& back();
//		const T& front() const;
//		const T& back() const;
//
//		//���롢ɾ������
//		void insert(iterator pos, const T& x);
//		iterator erase(iterator pos);
//		void push_back(const T& x);
//		void pop_back();
//		void push_front(const T& x);
//		void pop_front();
//
//		//��������
//		size_t size() const;
//		void resize(size_t n, const T& val = T());
//		void clear();
//		bool empty() const;
//		void swap(list<T>& lt);
//
//	private:
//		node* _head; //ָ������ͷ����ָ��
//	};
//}