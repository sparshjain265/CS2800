/*
   Program     : dijkstra.cpp
   Description : implementing dijkstra's algorithm on a graph of adjacency list using minHeap
   Programmer  : Sparsh Jain
   Roll  No    : 111601026
   Date        : January 30, 2018
*/

//Adding Required Libraries
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
using namespace std;

//class declarations so they can be referred to each other
class graph;
class vertex;
class minHeap;

//Structure path to store the currently known least distance to a vertex from the given source vertex
//minHeap based on this structure
struct path
{
	int pathTo; //Stores the index of the vertex
	int key;		//Stores the least known distance during the algorithm
};

/*
   This implementation uses an array to store the heap of size n from index 1 to n instead of the usual array implementation of 1 to n-1
   class minHeap to implement minHeap with necessary functions
   The maximum size of the heap can be 9999
*/
class minHeap
{
 private:
	path list[10000];	//Stores the heap as array
	int size;			  //Stores the size of the heap
	int indices[10000]; //Stores the index of ith vertex;

 public:
	//Constructor to initialize the size to be 0
	minHeap()
	{
		size = 0;
		list[0].pathTo = -1;
		list[0].key = -1;
	}

	//Returns the index of ith vertex
	int getIndex(int vertex)
	{
		if (vertex < 10000)
			return indices[vertex];
		else
			return -1;
	}

	//Returns the size of the heap
	int getSize()
	{
		return size;
	}

	//Returns the value at an index of the heap
	path getValue(int index)
	{
		if (index > 0 && index <= size)
			return list[index];
		else
		{
			cout << "Does not exist!!!" << endl;
			return list[0];
		}
	}

	//Returns the index of the left child if exists, returns -1 otherwise
	int left(int index)
	{
		if (2 * index <= size)
			return (2 * index);
		else
			return -1;
	}

	//Returns the index of the right child if exists, returns -1 otherwise
	int right(int index)
	{
		if (2 * index + 1 <= size)
			return (2 * index + 1);
		else
			return -1;
	}

	//Returns the index of the parent of index, -1 if the given index is out of bounds
	int parent(int index)
	{
		if (index < 1 || index > size)
			return -1;
		else
			return index / 2;
	}

	//Returns the minimum element from the heap
	path getMin()
	{
		if (size == 0)
		{
			cout << "Heap Empty!" << endl;
			return list[0];
		}
		return list[1];
	}

	//Destroys the heap
	void destroy()
	{
		size = 0;
		cout << "Destroyed entire heap!" << endl;
	}

	minHeap(int n);								 //Overloaded constructor to build a heap of a given size with all keys as INT_MAX
	minHeap(path array[], int n);				 //Overloaded constructor to build a heap from an array of definite size
	int minHeapify(int index);					 //Function minHeapify to correct any distortion between the given index and its decendents
	void build(path array[], int n);			 //Function build to create a heap from a given array of definite size
	int siftUp(int index);						 //Function siftUp to correct any distortion between the given index and its ancestors
	int siftUp(int index, int *indices);	 //Function siftUp overloaded to update the index of each vertex;
	void insert(path data);						 //Function insert to insert a new element in the heap
	path removeMin();								 //Function removeMin to remove the minimum element from the heap
	void heapSort();								 //Function heapSort to sort the given heap
	void showTree(int node, int offset);	 //Function to display the heap in a tree like format
	void printHeap();								 //Function to display the heap as an array
	int decreaseKey(int index, int newKey); //Function to decease the key at an index
};

//Constructor to build a heap of given size with all keys as INT_MAX
minHeap::minHeap(int n)
{
	size = n;
	for (int i = 0; i < size; i++)
	{
		list[i + 1].pathTo = i;
		list[i + 1].key = INT_MAX;
		indices[i] = i + 1;
	}
}

//Constructor to build a heap with a given array of a definite size
minHeap::minHeap(path array[], int n)
{
	build(array, n);
}

//minHeapify corrects any distortion between the given index and its decendents, returns the final index
int minHeap::minHeapify(int index)
{
	int small = index;
	path temp;

	//checking for boundary conditions, return -1 if not met
	if (index < 1 || index > size)
		return -1;

	//Updating the small index
	//First check if the child exists or not
	if (left(index) != -1 && list[left(index)].key < list[small].key)
		small = left(index);
	if (right(index) != -1 && list[right(index)].key < list[small].key)
		small = right(index);

	//if small index is changed, swap and call minHeapify on the new index, return the final index
	if (list[small].key != list[index].key)
	{
		int t = indices[list[small].pathTo];
		indices[list[small].pathTo] = indices[list[index].pathTo];
		indices[list[index].pathTo] = t;

		temp = list[small];
		list[small] = list[index];
		list[index] = temp;
		return minHeapify(small);
	}
	else //else return the same index
		return index;
}

//build creates the heap of a given size from a given array by calling minHeapify for all nodes with height more than 0 in ascending order of their heights
void minHeap::build(path array[], int n)
{
	int i;
	size = 0;

	//copy the array
	for (i = 0; i < n; i++)
	{
		size++;
		list[size] = array[i];
		//check for overflow
		if (size == 9999)
		{
			cout << "Heap overflow!" << endl;
			break;
		}
	}

	//call minHeapify as required
	for (i = size / 2; i > 0; i--)
		minHeapify(i);
}

//siftUp corrects any distortion between the given index and its ancenstors, returns the final index
int minHeap::siftUp(int index)
{
	//Check for boundary conditions, return -1 if not met
	if (index < 1 || index > size)
		return -1;

	if (index == 1) //return the index if it's the root
		return index;

	//If a distortion is found, swap and siftUp its parent, return the final index
	if (list[index].key < list[parent(index)].key)
	{
		int t = indices[list[parent(index)].pathTo];
		indices[list[parent(index)].pathTo] = indices[list[index].pathTo];
		indices[list[index].pathTo] = t;

		path temp = list[parent(index)];
		list[parent(index)] = list[index];
		list[index] = temp;
		return siftUp(parent(index));
	}
	else //else return the same index
		return index;
}

//insert a new element in the heap
void minHeap::insert(path data)
{
	//check for boundary condition
	if (size == 9999)
	{
		cout << "Heap already Full!" << endl;
		return;
	}

	//update the size and enter data in the end
	size++;
	list[size] = data;

	//arrange the heap by sifting Up the data
	siftUp(size);
}

//remove and return the minimum element from the heap
path minHeap::removeMin()
{
	path result;

	//check for boundary conditions
	if (size == 0)
	{
		cout << "Heap Empty!" << endl;
		return list[0];
	}

	//swap the minimum element with the last element and decrease the size
	int t = indices[list[1].pathTo];
	indices[list[1].pathTo] = indices[list[size].pathTo];
	indices[list[size].pathTo] = t;

	result = list[1];
	list[1] = list[size];
	list[size] = result;
	size--;
	//Call minheapify if the heap is non empty to correct any distortions due to swapping
	if (size > 0)
		minHeapify(1);
	//return the minimum element
	return result;
}

//display the heap in ascending order
void minHeap::heapSort()
{
	int n = size;
	path array[10000];

	//Use removeMin repeatedly to get ascending order
	for (int i = 1; i <= n; i++)
	{
		path num = removeMin();
		cout << "vertex: " << num.pathTo << ", distance: " << num.key << " ";
		array[i - 1] = num;
	}
	cout << endl;
	//rebuild the heap to retain the data
	build(array, n);
}

//print the heap in a tree like format from a given node
void minHeap::showTree(int node, int offset)
{
	//Check for boundary conditions
	if (size == 0)
	{
		cout << "Empty Heap! Nothing to display!" << endl;
		return;
	}

	//display the right branch of the tree from the node (if exists)
	int child = right(node);
	if (child != -1)
		showTree(child, offset + 1);

	//display the current node at appropriate spacing
	for (int i = 0; i < offset; i++)
		cout << "\t";
	cout << list[node].pathTo << "," << list[node].key << endl;

	//display the left branch of the tree from the node (if exists)
	child = left(node);
	if (child != -1)
		showTree(child, offset + 1);
}

//print the heap in the array format
void minHeap::printHeap()
{
	for (int i = 0; i < size; i++)
		cout << list[i + 1].pathTo << "," << list[i].key << " ";
	cout << endl;
}

//decrease the key of a given index to new key, and return it's new index
int minHeap::decreaseKey(int index, int newKey)
{
	//check for boundary conditions, return -1 if not met
	if (index < 1 || index > size)
		return -1;

	//if new key is higher, return the same index
	if (newKey >= list[index].key)
		return index;

	//else update to new key and siftUp to correct any distortions! return the updated index
	list[index].key = newKey;
	return siftUp(index);
}

//Structure node defined to implement a linked list on
struct node
{
	int edgeTo; //Stores the vertex to which an edge exists
	int weight; //Stores the weight of the edge
	node *next; //Stores the link to the next node
};

//Class link defined to implement a linked list with only required functions
class link
{
 private:
	int size;	//Stores the size of the list
	node *head; //Stores the head pointer of the list
	node *tail; //Stores the tail pointer of the list

 public:
	//Function to return the size of the list
	int getSize()
	{
		return size;
	}

	//Constructor to initialize the list with size 0 and pointers as NULL
	link()
	{
		size = 0;
		head = NULL;
		tail = NULL;
	}

	//Destructor to free any allocated memory
	~link()
	{
		while (head != NULL)
		{
			node *p = head;
			head = head->next;
			free(p);
		}
	}

	void addTail(int edgeTo, int weight); //Function to add a new neighbor in the list
	friend class graph;						  //Declaring graph as a friendly class to allow access to private members
};

//Function definition to add a new neighbor in the list
//Takes the index of the neighbor and edge weight as input
void link::addTail(int edgeTo, int weight)
{
	node *p = (node *)malloc(sizeof(node));
	p->edgeTo = edgeTo;
	p->weight = weight;
	p->next = NULL;
	if (size == 0)
		head = p;
	else
		tail->next = p;
	tail = p;
	size++;
}

//Class vertex defined to handle vertices in a graph
class vertex
{
 private:
	link list;									//A linked list containing it's neighbors and their edge-weights
	bool visited;								//A flag to mark whether the vertex is visited or not
	int parent, parentWeight, distance; //Variables to store the parent in a path, weight of the edge to the parent and distance from the source vertex

 public:
	//Constructor to initialize distance as infinity, parent as none, and visited as false
	vertex()
	{
		visited = false;
		parent = -1;
		parentWeight = distance = INT_MAX;
	}
	//Function Defined to add a new edge on the vertex
	void addEdge(int edgeTo, int weight)
	{
		list.addTail(edgeTo, weight);
	}

	//Declaring graph as a friendly function for easy access of private members
	friend class graph;
};

//Class graph defined for implementation of graph based algorithms using adjacency list
class graph
{
 private:
	vertex *v;						  //Dynamic array of vertices
	int vertices, edges, source; //Variables to store the number of vertices, edges, and the source vertex

 public:
	void readGraph(FILE *adjList); //Function to read a graph from a text file
	void printGraph();				 //Function to print the graph on screen just like the text file
	void dijkstra();					 //Function to call the dijkstra algorithm and find the minimum distance paths from the source vertex

	//Destructor to free the memory allocated for vertices
	~graph()
	{
		delete[] v;
	}
};

//Function Definition of the dijkstra's algorithm
void graph::dijkstra()
{
	//Heap created to store the distance, initialized with size equal to the number of vertices and all distance = inf
	minHeap distance = minHeap(vertices);

	//Reducing the distance of the source vertex to 0
	distance.decreaseKey(distance.getIndex(source), 0);

	//For all unvisited vertices
	while (distance.getSize() > 0)
	{
		path u = distance.removeMin();	//Take the nearest non-visited vertex from the source vertex
		v[u.pathTo].visited = true;		//Mark it as visited
		v[u.pathTo].distance = u.key;		//Update it's distance from the source vertex
		node *p = v[u.pathTo].list.head; //p to go through the adjacency list of this vertex
		while (p != NULL)
		{
			//If the adjacent vertex is unvisited AND visited it through this path reduces the distance from the source vertex
			if (!v[p->edgeTo].visited && distance.getValue(distance.getIndex(p->edgeTo)).key > u.key + p->weight)
			{
				distance.decreaseKey(distance.getIndex(p->edgeTo), u.key + p->weight); //Reduce the current distance
				v[p->edgeTo].parent = u.pathTo;													  //Update it's parent
				v[p->edgeTo].parentWeight = p->weight;											  //Update the weight of it's edge to it's parent
			}
			p = p->next; //Check for next adjacent vertex
		}
	}

	//Display the shortest path to all the vertices from the source vertex
	//v[source].parent = -1;
	cout << "Shortest path from " << source << "to each vertex:" << endl;
	for (int i = 0; i < vertices; i++)
	{
		cout << i;
		int p = i;
		while (v[p].parent != -1)
		{
			cout << " <--" << v[p].parentWeight << "-- " << v[p].parent;
			p = v[p].parent;
		}
		cout << " Distance: " << v[i].distance << endl;
	}
}

//Function defined to print the graph on screen as it is from the file
void graph::printGraph()
{
	cout << vertices << endl;
	cout << edges << endl;
	for (int i = 0; i < vertices; i++)
	{
		node *p = v[i].list.head;
		while (p != NULL)
		{
			if (i < p->edgeTo)
				cout << i << " " << p->edgeTo << " " << p->weight << endl;
			p = p->next;
		}
	}
}

//Function defined to read the graph from the file
void graph::readGraph(FILE *adjList)
{
	fscanf(adjList, " %d", &vertices);
	fscanf(adjList, " %d", &edges);
	v = new vertex[vertices];

	for (int i = 0; i < edges; i++)
	{
		int vertex1, vertex2, weight;
		fscanf(adjList, " %d %d %d", &vertex1, &vertex2, &weight);
		v[vertex1].addEdge(vertex2, weight);
		v[vertex2].addEdge(vertex1, weight);
	}

	fscanf(adjList, " %d", &source);
}

//main function to run the program
int main()
{
	//Read the file name and open the file
	char undirectedGraphName[100];
	cout << "Enter the name of input file (Containing Adjacency List of an Undirected Graph): ";
	cin >> undirectedGraphName;
	FILE *undirectedGraphFile = fopen(undirectedGraphName, "r");

	//Read the graph and print the paths
	graph undirectedGraph;
	undirectedGraph.readGraph(undirectedGraphFile);
	undirectedGraph.dijkstra();

	//Close the file before closing the program
	cout << endl;
	fclose(undirectedGraphFile);
	return 0;
}
