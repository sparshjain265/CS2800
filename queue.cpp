#include <iostream>
using namespace std;

template <typename T>
class queue;

template <typename T>
class qNode
{
 private:
	T key;
	qNode<T> *next;

 public:
	qNode()
	{
		next = NULL;
	}

	qNode(T item)
	{
		key = item;
		next = NULL;
	}

	friend class queue<T>;
};

template <typename T>
class queue
{
 private:
	int size;
	qNode<T> *front;
	qNode<T> *end;

 public:
	queue()
	{
		size = 0;
		front = NULL;
		end = NULL;
	}

	void enqueue(T key)
	{
		qNode<T> *p = new qNode<T>(key);
		if (isEmpty())
		{
			front = p;
			end = p;
			size++;
		}
		else
		{
			end->next = p;
			end = p;
			size++;
		}
	}

	T dequeue()
	{
		if (!isEmpty())
		{
			qNode<T> *p = front;
			front = front->next;
			if (front == NULL)
				end = NULL;
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

	int getSize()
	{
		return size;
	}
};