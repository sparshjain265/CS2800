/*
	Program		: q1.cpp
	Description	: ---
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Date			: April 3, 2018
*/

#include<iostream>
using namespace std;

class BST;

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
			
			while(parent != NULL && current != parent->right)
			{
				current = parent;
				parent = current->parent;
			}

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

		friend class BST;
};

class BST
{
	private:
		node *root;
		int size;
	
	public:
		BST()
		{
			root = NULL;
			size = 0;
		}

		~BST()
		{
			while(size > 0)
				deleteNode(root);
		}

		void search(int key)
		{
			node *x = search(root, key);
			if(x == NULL)
				cout<<"Not Found"<<endl;
			else
				cout<<"Key Found with Size: "<<x->size<<", Rank: "<<x->rank()<<endl;
		}

		node *search(node *r, int key)
		{
			if(r == NULL || r->key == key)
				return r;
			
			if(key < r->key)
				return search(r->left, key);
			else 
				return search(r->right, key);
		}

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

		void insert(node *r, int key)
		{
			if(r == NULL)
			{
				root = new node(key, NULL);
				size++;
			}
			else if(key < r->key)
			{
				if(r->left == NULL)
				{
					r->left = new node(key, r);
					size++;
					node *current = r;
					while(current != NULL)
					{
						current->size++;
						current = current->parent;
					}
				}
				else
					insert(r->left, key);
			}
			else
			{
				if(r->right == NULL)
				{
					r->right = new node(key, r);
					size++;
					node *current = r;
					while(current != NULL)
					{
						current->size++;
						current = current->parent;
					}
				}
				else
					insert(r->right, key);
			}
		}

		void inorder()
		{
			cout<<"InOrder Traversal: ";
			inorder(root);
			cout<<endl;
		}

		void inorder(node *r)
		{
			if(r == NULL)
				return;
			
			inorder(r->left);
			cout<<r->key<<" ";
			inorder(r->right);
		}

		void preorder()
		{
			cout<<"PreOrder Traversal: ";
			preorder(root);
			cout<<endl;
		}

		void preorder(node *r)
		{
			if(r == NULL)
				return;
			
			cout<<r->key<<" ";
			preorder(r->left);
			preorder(r->right);
		}

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

		node *findMin(node *r)
		{
			if(r->left == NULL || r == NULL)
				return r;
			
			return findMin(r->left);
		}

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

		void deleteNode(node *x)
		{
			if(x == NULL)
				return;
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

	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.insert(k);
	}

	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.search(k);
	}
	T.inorder();
	T.preorder();

	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.deleteNode(k);
	}

	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.search(k);
	}
	T.inorder();
	T.preorder();

	cin>>n;
	for(int i = 0; i < n; i++)
	{
		int k;
		cin>>k;
		T.insert(k);
	}

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