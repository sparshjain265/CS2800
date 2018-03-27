/*
	Program		: splayTree.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement splay trees
	Date			: March 20, 2018
*/

#include <iostream>
using namespace std;

void printMenu();

template <typename T>
class SplayTree;

template <typename T>
class SplayNode
{
 private:
	SplayNode *left;
	T key;
	SplayNode *parent;
	SplayNode *right;

 public:
	SplayNode()
	{
		left = NULL;
		parent = NULL;
		right = NULL;
	}

	SplayNode(T k)
	{
		left = NULL;
		key = k;
		parent = NULL;
		right = NULL;
	}

	SplayNode(T k, SplayNode<T> *p)
	{
		left = NULL;
		key = k;
		parent = p;
		right = NULL;
	}

	friend class SplayTree<T>;
};

template <typename T>
class SplayTree
{
 private:
	SplayNode<T> *root;
	int size;

 public:
	SplayTree()
	{
		root = NULL;
		size = 0;
	}

	~SplayTree()
	{
		while (size > 0)
			remove(root->key);
	}

	void zig(SplayNode<T> *x)
	{
		SplayNode<T> *p = x->parent;
		SplayNode<T> *g = p->parent;

		if (p != root)
		{
			if (p == g->left)
				g->left = x;
			else
				g->right = x;

			x->parent = g;
		}
		else
		{
			root = x;
			x->parent = NULL;
		}

		if (x == p->left)
		{
			p->left = x->right;
			if (x->right != NULL)
				x->right->parent = p;
			p->parent = x;
			x->right = p;
		}
		else
		{
			p->right = x->left;
			if (x->left != NULL)
				x->left->parent = p;
			p->parent = x;
			x->left = p;
		}
	}

	void zigzig(SplayNode<T> *x)
	{
		zig(x->parent);
		zig(x);
	}

	void zigzag(SplayNode<T> *x)
	{
		zig(x);
		zig(x);
	}

	//Splay in the entire tree
	void splay(SplayNode<T> *x)
	{
		if (root == NULL)
			return;

		if (x == NULL)
			return;

		if (x == root)
			return;

		if (x->parent == root)
			zig(x);
		else
		{
			SplayNode<T> *p = x->parent;
			SplayNode<T> *g = p->parent;

			if (p == g->left && x == p->left)
				zigzig(x);
			else if (p == g->right && x == p->right)
				zigzig(x);
			else
				zigzag(x);

			splay(x);
		}
	}

	//Splay in the subtree rooted at r
	void splay(SplayNode<T> *r, SplayNode<T> *x)
	{
		if (r == NULL)
			return;

		if (x == r)
			return;

		if (x->parent == r)
			zig(x);
		else
		{
			SplayNode<T> *p = x->parent;
			SplayNode<T> *g = p->parent;

			if (p == g->left && x == p->left)
				zigzig(x);
			else if (p == g->right && x == p->right)
				zigzig(x);
			else
				zigzag(x);

			splay(x);
		}
	}

	//Splay the maximum element in the subtree rooted at r if max is true
	//Else splay the minimum element in the subtree rooted at r
	void splay(SplayNode<T> *r, bool max)
	{
		if (r == NULL)
			return;

		if (max == true)
		{
			SplayNode<T> *x = r;
			while (x->right != NULL)
				x = x->right;
			splay(r, x);
		}
		else
		{
			SplayNode<T> *x = r;
			while (x->left != NULL)
				x = x->left;
			splay(r, x);
		}
	}

	//Search key in entire tree
	bool search(T key)
	{
		if (search(root, key))
		{
			cout << "Found!" << endl;
			return true;
		}
		else
		{
			cout << "Not Found!" << endl;
			return false;
		}
	}

	//Search key in subtree rooted at r
	bool search(SplayNode<T> *r, T key)
	{
		if (r == NULL)
			return false;

		if (r->key == key)
		{
			splay(r);
			return true;
		}

		if (key < r->key)
		{
			if (r->left != NULL)
				return search(r->left, key);
			splay(r);
			return false;
		}
		else
		{
			if (r->right != NULL)
				return search(r->right, key);
			splay(r);
			return false;
		}
	}

	//Insert key in the entire tree
	void insert(T key)
	{
		if (insert(root, key))
			cout << key << " already present!" << endl;
		else
			cout << key << " inserted!" << endl;
	}

	//Insert key in the subtree rooted at r
	bool insert(SplayNode<T> *r, T key)
	{
		if (r == NULL)
		{
			root = new SplayNode<T>(key);
			splay(root);
			return false;
		}

		if (r->key == key)
		{
			splay(r);
			size++;
			return true;
		}

		if (key < r->key)
		{
			if (r->left != NULL)
				return insert(r->left, key);

			r->left = new SplayNode<T>(key, r);
			splay(r->left);
			return false;
		}
		else
		{
			if (r->right != NULL)
				return insert(r->right, key);

			r->right = new SplayNode<T>(key, r);
			splay(r->right);
			return false;
		}
	}

	//Splits the tree about a given key and returns root of the tree with all elements larger than the key
	SplayNode<T> *split(T key)
	{
		search(key);
		if (root->key <= key)
		{
			SplayNode<T> *r = root->right;
			root->right = NULL;
			return r;
		}
		else
		{
			SplayNode<T> *r = root;
			root = r->left;
			r->left = NULL;
			return r;
		}
	}

	void join(SplayNode<T> *x, SplayNode<T> *y)
	{
		if (x != NULL)
		{
			splay(x, true);
			x->right = y;
		}
		else
			x = y;
	}

	void remove(T key)
	{
		if (remove(root, key))
			cout << key << " deleted!" << endl;
		else
			cout << key << " not present!" << endl;
	}

	bool remove(SplayNode<T> *r, T key)
	{
		if (r == NULL)
			return false;

		if (r->key == key)
		{
			join(r->left, r->right);
			if (r == root)
				root = r->left;
			else if (r == r->parent->left)
			{
				r->parent->left = r->left;
				r->left->parent = r->parent;
			}
			else
			{
				r->parent->right = r->right;
				r->right->parent = r->parent;
			}
			splay(r->parent);
			delete r;
			size--;
			return true;
		}

		if (key < r->key)
		{
			if (r->left != NULL)
				return remove(r->left, key);
			splay(r);
			return false;
		}
		else
		{
			if (r->right != NULL)
				return remove(r->right, key);
			splay(r);
			return false;
		}
	}

	void print()
	{
		print(root, 0);
	}

	void print(SplayNode<T> *r, int offset)
	{
		if (r == NULL)
		{
			cout << endl;
			return;
		}

		print(r->right, offset + 1);
		for (int i = 0; i < offset; i++)
			cout << "\t";
		cout << r->key << endl;
		print(r->left, offset + 1);
	}
};

int main()
{
	SplayTree<int> myTree;
	int choice;

	do
	{
		printMenu();
		cin >> choice;
		int key;
		switch (choice)
		{
		case 1: //Search
			cout << "Enter the key you want to search: ";
			cin >> key;
			myTree.search(key);
			break;
		case 2: //Insert
			cout << "Enter the key you want to insert: ";
			cin >> key;
			myTree.insert(key);
			break;
		case 3: //Delete
			cout << "Enter the key you want to delete: ";
			cin >> key;
			myTree.remove(key);
			break;
		case 4: //Print
			myTree.print();
			break;
		case 5: //Exit
			break;
		default:
			cout << "Please enter appropriate choice!" << endl;
		}
	} while (choice != 5);

	cout << endl;
	return 0;
}

void printMenu()
{
	cout << "Menu:" << endl;
	cout << "1. Search" << endl;
	cout << "2. Insert" << endl;
	cout << "3. Delete" << endl;
	cout << "4. Print" << endl;
	cout << "5. Exit" << endl;
	cout << "Enter your choice: ";
}