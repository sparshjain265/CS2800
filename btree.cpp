/*
	Program		: btree.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement insertion and search in B-Tree Data Structure
	Date			: March 13, 2018
*/

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

//Declaring class BTree to be referred by other classes
template <typename T>
class BTree;

//Class BNode to be used as a node in BTree Data Structure
template <typename T>
class BNode
{
 private:
	bool isLeaf;
	int n;
	T *key;
	BNode **child;

 public:
	BNode(int t)
	{
		key = new T[2 * t - 1];
		child = new BNode *[2 * t];
		n = 0;
		isLeaf = false;
	}

	~BNode()
	{
		delete[] key;
		delete[] child;
	}

	friend class BTree<T>;
};

template <typename T>
class searchResult
{
 private:
	BNode<T> *node;
	int i;

 public:
	searchResult()
	{
		node = NULL;
		i = -1;
	}

	friend class BTree<T>;
};

//Class BTree to implement search and insert
template <typename T>
class BTree
{
 private:
	int t;
	BNode<T> *root;

 public:
	BTree(int deg)
	{
		t = deg;
		root = new BNode<T>(t);
		root->isLeaf = true;
	}

	searchResult<T> search(BNode<T> *r, T key)
	{
		int i = 0, start = 0, end = r->n - 1;
		while (start != end)
		{
			int mid = (start + end) / 2;
			if (r->key[mid] == key)
			{
				start = mid;
				break;
			}
			else if (r->key[mid] < key)
				start = mid + 1;
			else
				end = mid;
		}

		if (r->key[start] == key)
		{
			searchResult<T> result = {r, start};
			return result;
		}

		if (r->isLeaf)
		{
			searchResult<T> result = {NULL, -1};
			return result;
		}

		if (r->key[start] < key)
			return search(r->child[start + 1], key);
		else
			return search(r->child[start], key);
	}

	searchResult<T> search(BNode<T> *r, T key, queue<int> *q)
	{
		int start = 0, end = r->n - 1;
		while (start != end)
		{
			int mid = (start + end) / 2;
			if (r->key[mid] == key)
			{
				start = mid;
				break;
			}
			else if (r->key[mid] < key)
				start = mid + 1;
			else
				end = mid;
		}

		if (r->key[start] == key)
		{
			searchResult<T> result;
			result.node = r;
			result.i = start;
			q->enqueue(start);
			return result;
		}

		if (r->isLeaf)
		{
			searchResult<T> result;
			result.node = NULL;
			result.i = -1;
			return result;
		}

		if (r->key[start] < key)
		{
			q->enqueue(start + 1);
			return search(r->child[start + 1], key, q);
		}
		else
		{
			q->enqueue(start);
			return search(r->child[start], key, q);
		}
	}

	void search(T key)
	{
		queue<int> *q = new queue<int>;
		searchResult<T> result = search(root, key, q);

		if (result.node != NULL)
		{
			cout << "root";
			while (q->getSize() > 1)
			{
				cout << "->child[" << q->dequeue() + 1 << "]";
			}
			cout << ", index[" << q->dequeue() + 1 << "]" << endl;
		}
		else
			cout << "key " << key << " is not found." << endl;
	}

	void split(BNode<T> *x, int i)
	{
		BNode<T> *z = new BNode<T>(t);
		BNode<T> *y = x->child[i];

		z->isLeaf = y->isLeaf;

		for (int j = t; j < 2 * t - 1; j++)
			z->key[j - t] = y->key[j];

		if (y->isLeaf == false)
			for (int j = t; j < 2 * t; j++)
				z->child[j - t] = y->child[j];

		y->n = t - 1;
		z->n = t - 1;

		for (int j = x->n - 1; j >= i; j--)
		{
			x->key[j + 1] = x->key[j];
			x->child[j + 2] = x->child[j + 1];
		}
		x->key[i] = y->key[t - 1];
		x->child[i + 1] = z;
		x->n++;
	}

	//assume x is not full
	void insert(BNode<T> *x, T k)
	{
		int i;
		if (x->isLeaf)
		{
			for (i = x->n - 1; i >= 0 && k < x->key[i]; i--)
				x->key[i + 1] = x->key[i];

			x->key[i + 1] = k;
			x->n++;
		}
		else
		{
			i = x->n - 1;
			while (i >= 0 && k < x->key[i])
				i--;
			i++;
			if (x->child[i]->n == 2 * t - 1)
			{
				split(x, i);
				if (k > x->key[i])
					i++;
			}
			insert(x->child[i], k);
		}
	}

	void insert(T k)
	{
		if (root->n == 2 * t - 1)
		{
			BNode<T> *r = new BNode<T>(t);
			r->child[0] = root;
			split(r, 0);
			root = r;
			insert(root, k);
		}
		else
			insert(root, k);
	}

	void print(BNode<T> *r, int offset)
	{
		if (r == NULL)
		{
			cout << endl;
			return;
		}

		for (int i = r->n; i > 0; i--)
		{
			if (!r->isLeaf)
				print(r->child[i], offset + 1);
			for (int j = 0; j < offset; j++)
				cout << "\t";
			cout << r->key[i - 1] << endl;
		}
		if (!r->isLeaf)
			print(r->child[0], offset + 1);
	}

	void print()
	{
		print(root, 0);
	}
};

int main()
{
	BTree<int> tree(3);

	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		int k;
		cin >> k;
		cout << endl;
		tree.insert(k);
		tree.print();
		cout << endl;
	}

	cin >> n;
	while (n >= 0)
	{
		tree.search(n);
		cin >> n;
	}

	return 0;
}