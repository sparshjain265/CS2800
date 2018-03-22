/*
	Program		: bTree.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement B-Tree Data Structure
	Date			: March 20, 2018
*/

#include <iostream>
using namespace std;

void printMenu();

//Declaring class queue to be referred later
template <typename T>
class queue;

//class qNode to be used as node for queue
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

	//declaring friend class for easy access
	friend class queue<T>;
};

//Defining class queue for FIFO operations
template <typename T>
class queue
{
 private:
	int size;
	qNode<T> *front;
	qNode<T> *end;

 public:
	//Constructor to initialize pointers and set size 0
	queue()
	{
		size = 0;
		front = NULL;
		end = NULL;
	}

	//Function to enqueue a key
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

	//Function to dequeue a key
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

	//Function to check whether queue is empty
	bool isEmpty()
	{
		if (size == 0)
			return true;
		else
			return false;
	}

	//Function to get the size of the queue
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
	//Constructor to create a new node of a given degree parameter
	BNode(int t)
	{
		key = new T[2 * t - 1];
		child = new BNode *[2 * t];
		n = 0;
		isLeaf = false;
	}

	//Destructor to delete dynamically allocated memory
	~BNode()
	{
		delete[] key;
		delete[] child;
	}

	//Declaring BTree as friendly class for easy access
	friend class BTree<T>;
};

//Class search result for BTree to store pointer to the node and index of the key
template <typename T>
class searchResult
{
 private:
	BNode<T> *node;
	int i;

 public:
	//Constructor to initialize with default not found result
	searchResult()
	{
		node = NULL;
		i = -1;
	}

	//Declaring BTree as freind class for easy access
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
	//Constructor takes degree parameter as input and creates an empty tree
	BTree(int deg)
	{
		t = deg;
		root = new BNode<T>(t);
		root->isLeaf = true;
	}

	//default search algorithm in BTree implementing binary search within array going downwards
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

	//search algorithm which enqueues the path taken in a queue
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

	//search function which calls the search algorithm with queue and prints the path
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

	//Function split to split the ith child of a given node x (assuming x is non-full)
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

	//insert a key k in the subtree rooted at x assuming x is not full
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

	//insert a key k in the BTree, root may or may not be full
	void insert(T k)
	{
		//If root is full, split and create a new root, then call insert in non-full subtree else call insert directly
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

	void rotate(BNode<T> *p, int i)
	{
		BNode<T> *x;
		BNode<T> *s;
		if (p->child[i]->n == t - 1)
		{
			x = p->child[i];
			s = p->child[i + 1];

			x->key[x->n] = p->key[i];
			x->child[x->n + 1] = s->child[0];
			x->n++;
			p->key[i] = s->key[0];
			for (int i = 0; i < s->n - 1; i++)
			{
				s->child[i] = s->child[i + 1];
				s->key[i] = s->key[i + 1];
			}
			s->child[s->n - 1] = s->child[s->n];
			s->n--;
		}
		else
		{
			x = p->child[i + 1];
			s = p->child[i];

			x->child[x->n + 1] = x->child[x->n];
			for (int i = x->n; i > 0; i--)
			{
				x->key[i] = x->key[i - 1];
				x->child[i] = x->child[i - 1];
			}
			x->n++;
			x->key[0] = p->key[i];
			x->child[0] = s->child[s->n];
			p->key[i] = s->key[s->n - 1];
			s->n--;
		}
	}

	void merge(BNode<T> *p, int i)
	{
		BNode<T> *x = p->child[i];
		BNode<T> *y = p->child[i + 1];

		x->key[t - 1] = p->key[i];
		for (int j = t; j < 2 * t - 1; j++)
		{
			x->child[j] = y->child[j - t];
			x->key[j] = y->key[j - t];
		}
		x->child[2 * t - 1] = y->child[t - 1];
		x->n = 2 * t - 1;

		delete y;

		for (int j = i; j < p->n - 1; j++)
		{
			p->key[j] = p->key[j + 1];
			p->child[j + 1] = p->child[j + 2];
		}
		p->n--;

		if (p->n == 0)
		{
			root = x;
			delete p;
		}
	}

	T deleteMin(BNode<T> *r)
	{
		if (r->isLeaf)
		{
			T result = r->key[0];
			for (int i = 0; i < r->n - 1; i++)
				r->key[i] = r->key[i + 1];
			r->n--;
			return result;
		}
		else if (r->child[0]->n == t - 1)
		{
			if (r->child[1]->n == t - 1)
				merge(r, 0);
			else
				rotate(r, 0);

			return deleteMin(r->child[0]);
		}
		else
			return deleteMin(r->child[0]);
	}

	T deleteMax(BNode<T> *r)
	{
		if (r->isLeaf)
		{
			r->n--;
			return r->key[r->n];
		}
		else if (r->child[r->n]->n == t - 1)
		{
			if (r->child[r->n - 1]->n == t - 1)
				merge(r, r->n - 1);
			else
				rotate(r, r->n - 1);

			return deleteMax(r->child[r->n]);
		}
		else
			return deleteMax(r->child[r->n]);
	}

	bool deleteKey(BNode<T> *x, T k)
	{
		int start = 0, end = x->n - 1;
		while (start != end)
		{
			int mid = (start + end) / 2;
			if (x->key[mid] == k)
			{
				start = mid;
				break;
			}
			else if (x->key[mid] < k)
				start = mid + 1;
			else
				end = mid;
		}

		if (x->key[start] == k)
		{
			if (x->isLeaf)
			{
				for (int i = start; i < x->n - 1; i++)
					x->key[i] = x->key[i + 1];
				x->n--;
			}
			else
			{
				if (x->child[start]->n > t - 1)
				{
					T pred = deleteMax(x->child[start]);
					x->key[start] = pred;
				}
				else if (x->child[start + 1]->n > t - 1)
				{
					T succ = deleteMin(x->child[start + 1]);
					x->key[start] = succ;
				}
				else
				{
					merge(x, start);
					return deleteKey(x->child[start], k);
				}
			}
			return true;
		}

		if (x->isLeaf)
			return false;

		if (x->key[start] < k)
		{
			BNode<T> *y = x->child[start + 1];
			if (y->n == t - 1)
			{
				if (x->child[start]->n > t - 1)
					rotate(x, start);
				else if (start + 1 < 2 * t - 1 && x->child[start + 2]->n > t - 1)
					rotate(x, start + 1);
				else
				{
					merge(x, start);
					y = x->child[start];
				}
			}
			return deleteKey(y, k);
		}
		else
		{
			BNode<T> *y = x->child[start];
			if (y->n == t - 1)
			{
				if (x->child[start + 1]->n > t - 1)
					rotate(x, start);
				else if (start > 0 && x->child[start - 1]->n > t - 1)
					rotate(x, start - 1);
				else
					merge(x, start);
			}
			return deleteKey(x->child[start], k);
		}
	}

	void deleteKey(T key)
	{
		if (deleteKey(root, key))
			cout << key << " deleted!" << endl;
		else
			cout << key << " not found!" << endl;
	}

	//Print a sub-BTree rotated 90deg anti-clockwise i.e left to right on screen after a given offset
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

	//Print the whole BTree, with no offset for root
	void print()
	{
		print(root, 0);
	}
};

int main()
{
	//BTree with degree parameter 3
	BTree<int> tree(3);

	//Input for number of insertions
	cout << "Enter the number of initial insertions: ";
	int n;
	cin >> n;

	//n insertions with printing current state of BTree after each insert
	for (int i = 0; i < n; i++)
	{
		cout << "Enter element #" << i + 1 << ": ";
		int k;
		cin >> k;
		cout << endl;
		tree.insert(k);
		tree.print();
		cout << endl;
	}

	int choice;
	do
	{
		printMenu();
		cin >> choice;
		switch (choice)
		{
		case 1: //Insert
			cout << "Enter element: ";
			cin >> n;
			cout << endl;
			tree.insert(n);
			tree.print();
			cout << endl;
			break;
		case 2: //Delete
			cout << "Enter element: ";
			cin >> n;
			tree.deleteKey(n);
			cout << endl;
			tree.print();
			cout << endl;
			break;
		case 3: //Search
			cout << "Enter element: ";
			cin >> n;
			tree.search(n);
			break;
		case 4: //Exit
			break;
		default:
			cout << "Please Enter Appropriate Choice!" << endl;
		}
	} while (choice != 4);

	cout << endl;

	return 0;
}

//prints the menu for user
void printMenu()
{
	cout << "Menu:" << endl;
	cout << "1. Insert" << endl;
	cout << "2. Delete" << endl;
	cout << "3. Search" << endl;
	cout << "4. Exit" << endl;
	cout << "Enter your choice: ";
}