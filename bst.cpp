/*
	Program		: bst.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement the Binary Search Tree data structure with all the necessary functions
	Date			: March 6, 2018
*/

#include <iostream>
using namespace std;

template <typename T>
class BST;

void printMenu();

template <typename T>
class node
{
 private:
	node *left;
	T key;
	node *parent;
	node *right;

 public:
	node()
	{
		left = NULL;
		parent = NULL;
		right = NULL;
	}

	friend class BST<T>;
};

template <typename T>
class BST
{
 private:
	node<T> *root;
	int size;

 public:
	BST()
	{
		root = NULL;
		size = 0;
	}

	~BST()
	{
		while (size > 0)
			deleteNode(root);
	}

	void find(T key)
	{
		node<T> *x = search(root, key);
		if (x == NULL)
			cout << "Not Found" << endl;
		else
			cout << "Found" << endl;
	}

	node<T> *search(node<T> *r, T key)
	{
		if (r == NULL || r->key == key)
			return r;

		if (key < r->key)
			return search(r->left, key);
		else
			return search(r->right, key);
	}

	void insertKey(T key)
	{
		node<T> *x = search(root, key);
		if (x == NULL)
		{
			insert(root, key);
			cout << key << " inserted" << endl;
		}
		else
			cout << key << " already present in the tree" << endl;
	}

	void insert(node<T> *r, T key)
	{
		if (r == NULL)
		{
			root = new node<T>;
			root->key = key;
			size++;
		}
		else if (key <= r->key)
		{
			if (r->left == NULL)
			{
				r->left = new node<T>;
				r->left->key = key;
				r->left->parent = r;
				size++;
			}
			else
				insert(r->left, key);
		}
		else
		{
			if (r->right == NULL)
			{
				r->right = new node<T>;
				r->right->key = key;
				r->right->parent = r;
				size++;
			}
			else
				insert(r->right, key);
		}
	}

	void findMinKey()
	{
		node<T> *x = findMin(root);
		if (x == NULL)
			cout << "Tree is empty" << endl;
		else
			cout << "Minimum Key: " << x->key << endl;
	}

	node<T> *findMin(node<T> *r)
	{
		if (r->left == NULL || r == NULL)
			return r;
		return findMin(r->left);
	}

	node<T> *findMax(node<T> *r)
	{
		if (r->right == NULL || r == NULL)
			return r;
		return findMax(r->right);
	}

	void printInOrderTraversal()
	{
		cout << "InOrder Traversal: ";
		inorder(root);
		cout << endl;
	}

	void inorder(node<T> *r)
	{
		if (r == NULL)
			return;
		inorder(r->left);
		cout << r->key << " ";
		inorder(r->right);
	}

	void preorder(node<T> *r)
	{
		if (r == NULL)
			return;
		cout << r->key << " ";
		preorder(r->left);
		preorder(r->right);
	}

	void printPostOrderTraversal()
	{
		cout << "PostOrder Traversal: ";
		postorder(root);
		cout << endl;
	}

	void postorder(node<T> *r)
	{
		if (r == NULL)
			return;
		postorder(r->left);
		postorder(r->right);
		cout << r->key << " ";
	}

	node<T> *findInorderSuccessor(node<T> *r)
	{
		if (r == NULL)
			return r;
		else if (r->right != NULL)
			return findMin(r->right);
		else
		{
			node<T> *current = r;
			node<T> *parent = r->parent;
			while (parent != NULL && current == parent->right)
			{
				current = parent;
				parent = current->parent;
			}
			return parent;
		}
	}

	void findPrecedingKey(T key)
	{
		node<T> *x = search(root, key);
		node<T> *y = findInorderPredecessor(x);
		if (y == NULL)
			cout << "No Predecessor" << endl;
		else
			cout << "Predecessor of " << key << " : " << y->key << endl;
	}

	node<T> *findInorderPredecessor(node<T> *r)
	{
		if (r == NULL)
			return r;
		else if (r->left != NULL)
			return findMax(r->left);
		else
		{
			node<T> *current = r;
			node<T> *parent = r->parent;
			while (parent != NULL && current == parent->left)
			{
				current = parent;
				parent = current->parent;
			}
			return parent;
		}
	}

	void deleteKey(T key)
	{
		node<T> *x = search(root, key);
		deleteNode(x);
		cout << "Deleted" << endl;
	}

	void deleteNode(node<T> *x)
	{
		if (x == NULL)
			return;
		else if (x->right == NULL && x->left == NULL)
		{
			node<T> *parent = x->parent;
			if (parent == NULL)
			{
				root = NULL;
				delete x;
				size--;
			}
			else
			{
				if (x == parent->left)
					parent->left = NULL;
				else
					parent->right = NULL;
				delete x;
				size--;
			}
		}
		else if (x->right != NULL && x->left == NULL)
		{
			if (x->parent == NULL)
			{
				root = x->right;
				root->parent = NULL;
			}
			else if (x == x->parent->right)
			{
				x->parent->right = x->right;
				x->right->parent = x->parent;
			}
			else
			{
				x->parent->left = x->right;
				x->right->parent = x->parent;
			}
			delete x;
			size--;
		}
		else if (x->right == NULL && x->left != NULL)
		{
			if (x->parent == NULL)
			{
				root = x->left;
				root->parent = NULL;
			}
			else if (x == x->parent->right)
			{
				x->parent->right = x->left;
				x->left->parent = x->parent;
			}
			else
			{
				x->parent->left = x->left;
				x->left->parent = x->parent;
			}
			delete x;
			size--;
		}
		else
		{
			node<T> *y = findInorderPredecessor(x);
			x->key = y->key;
			deleteNode(y);
		}
	}

	void print()
	{
		printTree(root, 0);
	}

	void printTree(node<T> *r, int offset)
	{
		if (r == NULL)
		{
			cout << endl;
			return;
		}

		printTree(r->right, offset + 1);
		for (int i = 0; i < offset; i++)
			cout << "\t";
		cout << r->key << endl;

		printTree(r->left, offset + 1);
	}
};

//main function to run the program
int main()
{
	BST<int> myTree;
	int choice;

	//doWhile loop to implement the menu
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
			myTree.find(key);
			break;
		case 2: //Insert
			cout << "Enter the key you want to insert: ";
			cin >> key;
			myTree.insertKey(key);
			break;
		case 3: //Find Minimum
			myTree.findMinKey();
			break;
		case 4: //InOrder Traversal
			myTree.printInOrderTraversal();
			break;
		case 5: //PostOrder Traversal
			myTree.printPostOrderTraversal();
			break;
		case 6: //Find Preceding Key
			cout << "Enter the key you want to search the preceding key for: ";
			cin >> key;
			myTree.findPrecedingKey(key);
			break;
		case 7: //Delete
			cout << "Enter the key you want to delete: ";
			cin >> key;
			myTree.deleteKey(key);
			break;
		case 8: //Print
			myTree.print();
			break;
		case 9: //Exit
			break;
		default: //Invalid Choice
			cout << "Please Enter Appropriate Choice!" << endl;
		}

	} while (choice != 9); //run until the user asks to exit

	cout << endl;
	return 0;
}

//prints the menu for user
void printMenu()
{
	cout << "Menu:" << endl;
	cout << "1. Search" << endl;
	cout << "2. Insert" << endl;
	cout << "3. Find Minimum" << endl;
	cout << "4. InOrder Traversal" << endl;
	cout << "5. PostOrder Traversal" << endl;
	cout << "6. Find Preceding Key" << endl;
	cout << "7. Delete" << endl;
	cout << "8. Print" << endl;
	cout << "9. Exit" << endl;
	cout << "Enter your choice: ";
}