#include <iostream>

namespace zhy
{
	template<class T>
	struct __list_node
	{
		__list_node<T>* _next;        //指向前驱节点
		__list_node<T>* _prev;        //指向后继节点
		T _date;                      //存放数据
		__list_node(const T& value=T())  //初始化节点
			:_date(value)
			, _next(nullptr)
			, _prev(nullptr)
		{
		}
	};
	template<class T,class Ref,class Ptr>    //后面两个模板参数用来区别迭代器是可读可写的还是只读的
	struct __list_iterator
	{
		typedef __list_node<T> node;
		typedef __list_iterator<T, Ref, Ptr> self;  //根据实际传入的迭代器参数生成对应类型的迭代器
		node* _node;

		__list_iterator(node* node)
			:_node(node)
		{
		}
		//拷贝构造、赋值运算符重载、析构都用编译器默认生成的。因为迭代器的这些行为都是浅拷贝
		bool operator==(const self& x) const
		{
			return _node == x._node;
		}
		bool operator!=(const self& x) const
		{
			return _node != x._node;
		}
		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}
		self operator++(int)           //不能返回引用，因为返回的是局部变量，出了作用域就会被释放
		{
			self temp = *this;
			++(*this);
			return temp;
		}
		self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}
		self operator--(int)
		{
			self temp = *this;
			--(*this);
			return temp;
		}
		Ref operator*() const    //如果迭代器是const类型，则Ref代表const T&，外部不可修改
		{
			return _node->_date;    
		}
		Ptr operator->() const   //返回当前节点_date部分的地址，在外部以指针的方式来使用
		{
			return &_node->_date;
		}


	};


	template<class T>   
	class list
	{
		typedef __list_node<T> node;
	public:
		typedef __list_iterator<T, T&, T*> iterator;   //这样传参就代表是可读可写的迭代器，即iterator是可读可写迭代器
		typedef __list_iterator<T, const T&, const T*> const_iterator; //这样传参就代表是只读迭代器，即const_iterator是只读迭代器
		list()
		{
			_head = new node();     //开辟一个头结点
			_head->_next = _head;
			_head->_prev = _head;
		}
		list(const list<T>& lt)
		{
			_head = new node();
			_head->_next = _head;
			_head->_prev = _head;
			const_iterator it = lt.begin();
			while (it != lt.end())
			{
				push_back(*it);
				++it;
			}
		}
		template<class InputIterator>
		list(InputIterator first, InputIterator last)
		{
			_head = new node();                        //这里不能写node _head = new node();   新创建出来的_node是个局部变量，并没有为成员变量初始化
			_head->_next = _head;
			_head->_prev = _head;
			while (first != last)
			{
				std::cout << *first << std::endl;
				push_back(*first);
				++first;
			}
		}


		//现代写法
		//list(const list<T>& lt)
		//{
		//	node* _head = new node();               //这里也要初始化一个头节点，否则交换之后，temp拿到的是一个没有初始化的头结点，出作用域析构时就会出错
		//	_head->_next = _head;
		//	_head->_prev = _head;
		//	list<T> temp(lt.begin(), lt.end());
		//	std::swap(_head, temp._head);
		//}




		list<T> operator=(list<T> lt)
		{
			std::swap(_head, lt._head);
			return *this;
		}

		~list()
		{
			clean();
			delete _head;
			_head = nullptr;
		}
		void clean()
		{
			iterator it = (*this).begin();
			while (it != (*this).end())
			{
				it = erase(it);                       //erase后迭代器会失效，要给迭代器重新赋值    
				//erase(it++);  与上面同理
			}
		}
		bool empty()
		{
			return begin() == end();
		}
		iterator begin() 
		{
			return _head->_next;   //iterator b(_head->next);   return b;
		}
		iterator end()
		{
			return _head;          //iterator b(_head);   return b;
		}
		const_iterator begin() const   //const修饰的是this指针，表示调用该函数的对象是被const修饰的
		{
			return _head->_next;   //iterator b(_head->next);   return b;
		}
		const_iterator end() const
		{
			return _head;          //iterator b(_head);   return b;
		}
		void push_back(const T& value)
		{
			node* tail = _head->_prev;
			node* newnode = new node(value);
			tail->_next = newnode;
			newnode->_prev = tail;
			newnode->_next = _head;
			_head->_prev = newnode;
			//insert(end(), value);
		}
		void push_front(const T& x)
		{
			insert(begin(), x);
		}
		iterator insert(iterator pos, const T& x)
		{
			node* newnode = new node(x);
			node* cur = pos._node;
			node* prev = cur->_prev;
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;
			iterator ret(newnode);
			return ret;
		}
		void pop_back()
		{
			erase(begin());
		}
		void pop_front()
		{
			erase(--end());
		}
		iterator erase(iterator pos)
		{
			node* cur = pos._node;
			node* prev = cur->_prev;
			node* next = cur->_next;
			prev->_next = next;
			next->_prev = prev;
			delete cur;              //把要删除的空间释放掉，防止内存泄漏
			iterator ret(next);      //返回新位置的迭代器
			return ret;
		}
	private:
		node* _head;
	};
}

