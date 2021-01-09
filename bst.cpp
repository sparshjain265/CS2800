/*
	Program		: bst.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement the Binary Search Tree data structure with all the necessary functions
	Date			: March 6, 2018
*/

#include <iostream>
using namespace std;

//Declaring class to be referred by other classes
template <typename T>
class BST;

//Function declaration to print the menu
void printMenu();

//class node with a key, parent pointer, left and right child pointers
//To be used for BST, hence has BST as friendly class to allow easy access
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

//Class BST containing the root node and size
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

	//Constructor to free dynamic memory
	~BST()
	{
		while (size > 0)
			deleteNode(root);
	}

	//Overloaded function search to search for a key
	//Takes the key to be searched as input, and calls the search algorithm on root node of the tree
	//Prints Found or Not Found respectively
	void search(T key)
	{
		node<T> *x = search(root, key);
		if (x == NULL)
			cout << "Not Found" << endl;
		else
			cout << "Found" << endl;
	}

	//Function search to implement search algorithm in BST
	//Takes root of the tree/subtree and the key as inputs
	//Returns pointer to the node holding the key if found, NULL otherwise
	node<T> *search(node<T> *r, T key)
	{
		//If the tree/subtree is empty, or the key resides in the root, return the root
		if (r == NULL || r->key == key)
			return r;

		//Recursively call search algorithm on it's left or right child depending on the key value
		if (key < r->key)
			return search(r->left, key);
		else
			return search(r->right, key);
	}

	//Overloaded function to insert a key in the BST
	//Takes the key as input and prints inserted after the job is done
	//Notifies if the key is already present to maintain uniqueness in the BST
	void insert(T key)
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

	//Function insert to implement insertion algorithm in BST
	//Takes root node of the tree/subtree and key as input
	void insert(node<T> *r, T key)
	{
		//If the root is empty, create a new node and point root to it
		if (r == NULL)
		{
			root = new node<T>;
			root->key = key;
			size++;
		}
		//Else if the key is to be added in left subtree
		else if (key <= r->key)
		{
			//If left child does not exist, create new node and point left child to it
			if (r->left == NULL)
			{
				r->left = new node<T>;
				r->left->key = key;
				r->left->parent = r;
				size++;
			}
			//Else recursively call the insert algorithm on left child
			else
				insert(r->left, key);
		}
		//Else the key is to be added in the right subtree
		else
		{
			//If right child does not exist, create a new node and point right child to it
			if (r->right == NULL)
			{
				r->right = new node<T>;
				r->right->key = key;
				r->right->parent = r;
				size++;
			}
			//Else recursively call the insert algorithm on right child
			else
				insert(r->right, key);
		}
	}

	//Overloaded functiont to find the minimum in the BST
	//Prints the minimum key if exists
	void findMin()
	{
		//Call the findMin algorithm on root
		node<T> *x = findMin(root);
		if (x == NULL)
			cout << "Tree is empty" << endl;
		else
			cout << "Minimum Key: " << x->key << endl;
	}

	//Function findMin to find the minimum in a tree/subtree of BST
	//Takes the root of tree/subtree as input
	//Returns pointer to the minimum element
	node<T> *findMin(node<T> *r)
	{
		//If left child is null, root is the minimum element
		//If root is null, minimum element does not exist (tree is empty)
		//Return root
		if (r->left == NULL || r == NULL)
			return r;
		//Else find minimum in left subtree
		return findMin(r->left);
	}

	//Function findMax to find the maximum in a tree/subtree of BST
	//Takes the root of tree/subtree as input
	//Returns pointer to the maximum element
	node<T> *findMax(node<T> *r)
	{
		//If right child is null, root is the maximum element
		//If root is null, maximum element does not exist (tree is empty)
		//Return root
		if (r->right == NULL || r == NULL)
			return r;
		//Else fidn maximum in right subtree
		return findMax(r->right);
	}

	//Overloaded function to traverse (print) BST inOrder
	void inorder()
	{
		cout << "InOrder Traversal: ";
		inorder(root); //Call the inorder traversal algorithm on root
		cout << endl;
	}

	//Function inorder to traverse the tree/subtree of BST inOrder
	//Takes root of the tree as input, prints tree according to traversal
	void inorder(node<T> *r)
	{
		//If root is NULL, nothing to be done, return
		if (r == NULL)
			return;
		//Recursively call inorder traversal on left child, print the current node and call inorder traversal on right subtree
		inorder(r->left);
		cout << r->key << " ";
		inorder(r->right);
	}

	//Function preorder to traverse the tree/subtree of BST preOrder
	//Takes root of the tree as input, prints tree accroding to traversal
	void preorder(node<T> *r)
	{
		//If root is null, nothing to be done
		if (r == NULL)
			return;
		//Print the current node, and call preorder traversal on left and right subtree
		cout << r->key << " ";
		preorder(r->left);
		preorder(r->right);
	}

	//Overloaded function to traverse (print) BST postOrder
	void postorder()
	{
		cout << "PostOrder Traversal: ";
		postorder(root); //Call the postOrder traversal on root
		cout << endl;
	}

	//Function postorder to traverse the tree/subtree of BST postOrder
	//Takes root of the tree as input, prints tree according to traversal
	void postorder(node<T> *r)
	{
		//If root is nul, nothing to be done
		if (r == NULL)
			return;
		//postOrder traverse through left and right subtree before printing the current node
		postorder(r->left);
		postorder(r->right);
		cout << r->key << " ";
	}

	//Function to find the successor in inOrder traversal of the BST
	//Takes pointer to the node as input
	//Returns pointer to the successor
	node<T> *findInorderSuccessor(node<T> *r)
	{
		//If root is null, there is no successor, return r
		if (r == NULL)
			return r;
		//Else if right child exists, minimum of that is successor
		else if (r->right != NULL)
			return findMin(r->right);
		//Else Traverse upwards till we find the successor
		else
		{
			node<T> *current = r;
			node<T> *parent = r->parent;
			//Continue traversing upwards till the node is the right child or is root itself
			while (parent != NULL && current == parent->right)
			{
				current = parent;
				parent = current->parent;
			}
			//In both cases, parent (NULL in case of root) is the successor
			return parent;
		}
	}

	//Overloaded function to find the inorder predecessor in the BST
	//Takes a key as input
	//Prints it's predecessor if exists, else notifies
	void findInorderPredecessor(T key)
	{
		node<T> *x = search(root, key);			 //Find the pointer to the key
		node<T> *y = findInorderPredecessor(x); //Call the predecessor algorithm to get the predecessor
		//Print if exists, notify otherwise
		if (y == NULL)
			cout << "No Predecessor" << endl;
		else
			cout << "Predecessor of " << key << " : " << y->key << endl;
	}

	//Function to find the predecessor in inOrder traversal of BST
	//Takes pointer to node as input
	//Returns pointer to the predecessor
	node<T> *findInorderPredecessor(node<T> *r)
	{
		//If root is NULL, there is no predecessor, return r
		if (r == NULL)
			return r;
		//Else if left child exists, maximum of that is successor
		else if (r->left != NULL)
			return findMax(r->left);
		//Else Traverse upwards till we find a predecessor
		else
		{
			node<T> *current = r;
			node<T> *parent = r->parent;
			//Continue traversing upwards till the node is left child or is root itself
			while (parent != NULL && current == parent->left)
			{
				current = parent;
				parent = current->parent;
			}
			//In both cases, parent (NULL in case of root) is the predecessor
			return parent;
		}
	}

	//Overloaded function to delete a node from BST
	//Takes the key value as input
	//Informs if the key was not preset, or notify if deleted
	void deleteNode(T key)
	{
		//Find the node with the key
		node<T> *x = search(root, key);
		//Delete and notify if found
		if (x != NULL)
		{
			deleteNode(x);
			cout << "Deleted" << endl;
		}
		//Inform if not present
		else
			cout << key << " not present." << endl;
	}

	//Function to delete a node from BST
	//Takes the pointer to the node as input
	//Deletes and deallocates the dynamically allocated memory
	void deleteNode(node<T> *x)
	{
		//If the node does not exist, there is nothing to be done
		if (x == NULL)
			return;
		//If the node is a leaf, delete the node and deallocate the memory
		//Update necessary pointers
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
		//Else if x has only right child, make it the corresponding child of the parent and delete the node
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
		//Else if x has only left child, make it the corresponding child of the parent and delete the node
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
		//Else replace the key of it's inorder predecessor and delete that node recursively
		else
		{
			node<T> *y = findInorderPredecessor(x);
			x->key = y->key;
			deleteNode(y);
		}
	}

	//Overloaded function to print the BST
	//Calls the first instance of print algorithm on root with offset 0
	void print()
	{
		print(root, 0);
	}

	//Function to print the tree/subtree
	//Takes pointer to the root and offset (from left) as input
	void print(node<T> *r, int offset)
	{
		//If the tree is empty, print blank line and return
		if (r == NULL)
		{
			cout << endl;
			return;
		}

		//Recursively print the right subtree with 1 additional offset
		print(r->right, offset + 1);
		//Print the current key with required offset
		for (int i = 0; i < offset; i++)
			cout << "\t";
		cout << r->key << endl;
		//Recursively print the left subtree with 1 additional offset
		print(r->left, offset + 1);
	}
};

//main function to run the program
int main()
{
	BST<int> myTree; //Create a BST on integer
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
			myTree.search(key);
			break;
		case 2: //Insert
			cout << "Enter the key you want to insert: ";
			cin >> key;
			myTree.insert(key);
			break;
		case 3: //Find Minimum
			myTree.findMin();
			break;
		case 4: //InOrder Traversal
			myTree.inorder();
			break;
		case 5: //PostOrder Traversal
			myTree.postorder();
			break;
		case 6: //Find Preceding Key
			cout << "Enter the key you want to search the preceding key for: ";
			cin >> key;
			myTree.findInorderPredecessor(key);
			break;
		case 7: //Delete
			cout << "Enter the key you want to delete: ";
			cin >> key;
			myTree.deleteNode(key);
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