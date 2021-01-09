#include <iostream>
using namespace std;

template <typename T>
class stack;

template <typename T>
class stackNode
{
 private:
	T key;
	stackNode<T> *next;

 public:
	stackNode()
	{
		next = NULL;
	}
	stackNode(T item)
	{
		key = item;
		next = NULL;
	}
	stackNode(T item, stackNode<T> *below)
	{
		key = item;
		next = NULL;
	}
	friend class stack<T>;
};

template <typename T>
class stack
{
 private:
	int size;
	stackNode<T> *top;

 public:
	stack()
	{
		size = 0;
		top = NULL;
	}

	void push(T key)
	{
		stackNode<T> *p = new stackNode<T>(key, top);
		top = p;
		size++;
	}

	T pop()
	{
		if (!isEmpty())
		{
			stackNode<T> *p = top;
			top = top->next;
			size--;

			T result = p->key;
			delete p;
			return result;
		}
		else
		{
			T result;
			return result;
		}
	}

	bool isEmpty()
	{
		if (size == 0)
			return true;
		else
			return false;
	}
};