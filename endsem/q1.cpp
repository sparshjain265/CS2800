/*
	Program		: q1.cpp
	Description	: Modified BST where each node contains information about the size of subtree rooted at itself and its rank in inorder traversal
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Date			: April 3, 2018
*/

#include<iostream>
using namespace std;

//class BST declared to be used as a freind to node
class BST;

//class node definied to be used as nodes of the BST
//Contains the key, pointers to its left and right children, pointer to its parent and size of the subtree rooted at itself
//Contains a function rank which returns its rank in inorder traversal of the BST the node is a part of in time O(h) where h is the height of the BST
class node
{
	private:
		node *left;
		int key;
		node *right;
		node *parent;
		int size;
	
	public:
		node()
		{
			left = NULL;
			right = NULL;
			parent = NULL;
			size = 1;
		}

		node(int k, node *p)
		{
			left = NULL;
			right = NULL;
			parent = p;
			key = k;
			size = 1;
		}

		int rank()
		{
			node *current = this;
			node *parent = current->parent;
			
			//Move up till the parent is NULL or current is parent's right child
			while(parent != NULL && current != parent->right)
			{
				current = parent;
				parent = current->parent;
			}

			//Rank = rank of such parent (0 if parent is NULL) + size of left subtree (0 if left subtree is NULL) + 1
			if(parent == NULL)
			{
				if(left == NULL)
					return 1;
				else 
					return left->size + 1;
			}
			else
			{
				if(left == NULL)
					return parent->rank() + 1;
				else
					return parent->rank() + left->size + 1;
			}
		}

		//Define friendly class BST
		friend class BST;
};

//Class BST with modifications to maintain the size property of each node
//Contains pointer to the root and size of the tree
class BST
{
	private:
		node *root;
		int size;
	
	public:
		//Constructor to initialize the empty tree
		BST()
		{
			root = NULL;
			size = 0;
		}

		//Destructor to deallocate the dynamicMemory
		~BST()
		{
			while(size > 0)
				deleteNode(root);
		}

		//user called search function
		//prints the size and rank if key is found, notifies otherwise
		void search(int key)
		{
			node *x = search(root, key);
			if(x == NULL)
				cout<<"Not Found"<<endl;
			else
				cout<<"Key Found with Size: "<<x->size<<", Rank: "<<x->rank()<<endl;
		}


		//Top Down Recursive Search Algorithm to find a key in the subtree rooted at r
		//Returns pointer to the node containing the key if found, NULL pointer otherwise
		node *search(node *r, int key)
		{
			if(r == NULL || r->key == key)
				return r;
			
			if(key < r->key)
				return search(r->left, key);
			else 
				return search(r->right, key);
		}


		//User Called insert function
		//Inserts and notifies the key if not present, notifies otherwise
		void insert(int key)
		{
			node *x = search(root, key);
			if(x == NULL)
			{
				insert(root, key);
				cout<<key<<" inserted"<<endl;
			}
			else
				cout<<key<<" already present in the tree"<<endl;
		}

		//Inserts a node with the given key in the subtree rooted at r and updates the size of the required nodes
		void insert(node *r, int key)
		{
			//if the tree is empty, create a new node at root and increment the size of the tree
			if(r == NULL)
			{
				root = new node(key, NULL);
				size++;
			}
			//Else if the key is to be inserted in the left subtree
			else if(key < r->key)
			{
				//If left subtree is null, create a new node and increment the size of the tree
				if(r->left == NULL)
				{
					r->left = new node(key, r);
					size++;

					//Increment the size property of individual nodes ancestral to the new node
					node *current = r;
					while(current != NULL)
					{
						current->size++;
						current = current->parent;
					}
				}
				//Else recursively insert the key in left subtree
				else
					insert(r->left, key);
			}
			//Else the key is to be inserted in the right subtree
			else
			{
				//If right subtree is null, create a new node and increment the size of the tree
				if(r->right == NULL)
				{
					r->right = new node(key, r);
					size++;

					//Increment the size property of individual nodes ancestral to the new node
					node *current = r;
					while(current != NULL)
					{
						current->size++;
						current = current->parent;
					}
				}
				//Else recursively insert the key in right subtree
				else
					insert(r->right, key);
			}
		}

		//User called inorder function
		//Prints the elements of the tree by inOrder Traversal
		void inorder()
		{
			cout<<"InOrder Traversal: ";
			inorder(root);
			cout<<endl;
		}

		//Recursive Algorithm to print the tree by inOrder Traversal
		void inorder(node *r)
		{
			if(r == NULL)
				return;
			
			inorder(r->left);
			cout<<r->key<<" ";
			inorder(r->right);
		}

		//User called preoder function
		//Prints the elements of the tree by preOrder Traversal
		void preorder()
		{
			cout<<"PreOrder Traversal: ";
			preorder(root);
			cout<<endl;
		}

		//Recursive Algorithm to print the tree by preOrder Traversal
		void preorder(node *r)
		{
			if(r == NULL)
				return;
			
			cout<<r->key<<" ";
			preorder(r->left);
			preorder(r->right);
		}


		//Function to return pointer to the inorder successor of a given node
		node *inSuccessor(node *r)
		{
			if(r == NULL)
				return r;
			else if(r->right != NULL)
				return findMin(r->right);
			else
			{
				node *current = r;
				node *parent = r->parent;
				while(parent != NULL && current == parent->right)
				{
					current = parent;
					parent = current->parent;
				}
				return parent;
			}
		}

		//Function to return pointer to the minimum element in the subtree rooted at r
		node *findMin(node *r)
		{
			if(r->left == NULL || r == NULL)
				return r;
			
			return findMin(r->left);
		}

		//User called function to delete a given key
		//Deletes the key if found, notifies otherwise
		void deleteNode(int key)
		{
			node *x = search(root, key);
			if(x != NULL)
			{
				deleteNode(x);
				cout<<key<<" deleted"<<endl;
			}
			else
				cout<<key<<" not present"<<endl;
		}

		//Algorithm to implement delete functionality and updating the size property of the affected nodes
		void deleteNode(node *x)
		{
			//If x is NULL, nothing to delete
			if(x == NULL)
				return;
			//If x is a leaf, delete x, make appropriate changes to it's parents
			//decrease the size of its ancestors by going up till you reach root
			else if(x->right == NULL && x->left == NULL)
			{
				node *parent = x->parent;
				if(parent == NULL)
				{
					root = NULL;
					delete x;
					size--;
				}
				else
				{
					if(x == parent->left)
						parent->left = NULL;
					else
						parent->right = NULL;
					delete x;
					size--;

					while(parent != NULL)
					{
						parent->size--;
						parent = parent->parent;
					}
				}
			}
			//If x has only right child, make appropriate changes to its child and parent before deleting the node
			//decrease the size of its ancentors by going up till you reach root
			else if(x->right != NULL && x->left == NULL)
			{
				node *parent = x->parent;
				if(parent == NULL)
				{
					root = x->right;
					root->parent = NULL;
				}
				else if(x == parent->right)
				{
					parent->right = x->right;
					x->right->parent = parent;
				}
				else
				{
					parent->left = x->right;
					x->right->parent = parent;
				}
				delete x;
				size--;

				while(parent != NULL)
				{
					parent->size--;
					parent = parent->parent;
				}
			}
			//If x has only left child, make appropriate changes to its child and parent before deleting the node
			//decrease the size of its ancestors by going up till you reach root
			else if(x->right == NULL && x->left != NULL)
			{
				node *parent = x->parent;
				if(parent == NULL)
				{
					root = x->left;
					root->parent = NULL;
				}
				else if(x == parent->right)
				{
					parent->right = x->left;
					x->left->parent = parent;
				}
				else
				{
					parent->left = x->left;
					x->left->parent = parent;
				}
				delete x;
				size--;

				while(parent != NULL)
				{
					parent->size--;
					parent = parent->parent;
				}
			}
			//Else replace x by its inorder successor and delete the successor
			//size property will be taken care while deleting the successor node
			else
			{
				node *y = inSuccessor(x);
				x->key = y->key;
				deleteNode(y);
			}
		}
};

int main()
{
	BST T;
	int n;

	//Phase 1: Insert
	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.insert(k);
	}

	//Phase 2: Search and print
	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.search(k);
	}
	T.inorder();
	T.preorder();

	//Phase 3: Delete
	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.deleteNode(k);
	}

	//Phase 4: Search and print
	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.search(k);
	}
	T.inorder();
	T.preorder();

	//Phase 5: Insert
	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.insert(k);
	}

	//Phase 6: Search and print
	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.search(k);
	}
	T.inorder();
	T.preorder();



	cout<<endl;
	return 0;
}