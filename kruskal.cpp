/*
	Program Name: kruskal.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement kruskal's algorithm to find MST using Union-Find
	Date			: February 13, 2018
*/

#include <iostream>
using namespace std;

//Declaring classes so they can be declared friendly later
class unionFind;
class minHeap;

//Class node for each vertex to implement the unionFind
//Each node contains it's rank, it's index(for referencing), and it's parent
class node
{
 private:
	int rank;
	int index;
	int parent;

 public:
	//Constructor to initialize with rank 0 and parent as itself
	node()
	{
		rank = 0;
		index = 0;
		parent = index;
	}

	friend class unionFind; //declaring unionFind class as friendly so that it can access members easily
};

//Class unionFind to implement unionFind data structure using trees, rank and path compression
//Contains the total number of elements and a dynamic array of vertices
class unionFind
{
 private:
	int size;
	node *vertex;

 public:
	//Default Constructor to initialize an empty set
	unionFind()
	{
		size = 0;
		vertex = NULL;
	}

	//Overloaded constructor to call a specified makeSet operations
	unionFind(int n)
	{
		size = n;
		vertex = new node[n];
		for (int i = 0; i < n; i++)
			makeSet(i);
	}

	//makeSet Function to insitialize each vertex as a separate set by setting it's parent to itself, it's rank to be 0
	//Also sets the vertex's index to be it's index in the array for easy referencing
	void makeSet(int index)
	{
		vertex[index].parent = index;
		vertex[index].rank = 0;
		vertex[index].index = index;
	}

	//Destructor to delete the dynamically allocated memory
	~unionFind()
	{
		delete[] vertex;
	}

	int find(int index);			  //Find Function to return the index of the representative of the set
	void setUnion(int u, int v); //setUnion Function to union two sets
};

//Find Function to return the index of the representative of the set
//Updates the parents of all the intermediate nodes to it's root for path compression
int unionFind::find(int index)
{
	if (vertex[index].parent == index)
		return index;

	int v = find(vertex[index].parent);
	vertex[index].parent = v;
	return v;
}

//setUnion function to unite two sets
//uses find function to get the representative of the set to compare
void unionFind::setUnion(int u, int v)
{
	int x = find(u);
	int y = find(v);
	if (x != y)
	{
		if (vertex[x].rank == vertex[y].rank)
		{
			vertex[y].parent = x;
			vertex[x].rank++;
		}
		else if (vertex[x].rank > vertex[y].rank)
			vertex[y].parent = x;
		else
			vertex[x].parent = y;
	}
}

//Class Edge to store an edge of undirected graph
//Stores it's end points and weight
//Class graph and minHeap are friendly classes to allow easy access to its members
class edge
{
 private:
	int u, v, weight;

 public:
	int getWeight()
	{
		return weight;
	}
	friend class graph;
	friend class minHeap;
};

//Class minHeap to implement on a dynamic list of edges
class minHeap
{
 private:
	edge *list;
	int size;

 public:
	//Default constructor overloaded to initialize with size 0
	minHeap()
	{
		size = 0;
		list = NULL;
	}

	//Destructor to delete any remaining list using dynamic memory
	~minHeap()
	{
		if (list != NULL)
			delete[] list;
	}

	//Function left to return the index of left child if exists, -1 if doesn't
	int left(int index)
	{
		if (2 * index <= size)
			return 2 * index;
		else
			return -1;
	}

	//Function right to return the index of right child if exists, -1 if doesn't
	int right(int index)
	{
		if (2 * index + 1 <= size)
			return 2 * index + 1;
		else
			return -1;
	}

	void buildHeap(int edges, edge e[]); //Function buildheap to create a heap from a given array
	void minHeapify(int index);			 //Function minheapify to correct any distortions in the heap
	edge removeMin();							 //Function to delete and return the minimum element of the heap
};

//Function definition of builHeap to create a heap from a given list of edges
void minHeap::buildHeap(int edges, edge e[])
{
	size = 0;
	if (list != NULL)
		delete[] list;
	list = new edge[edges + 1];
	for (int i = 0; i < edges; i++)
	{
		size++;
		list[size] = e[i];
	}
	for (int i = size / 2; i > 0; i--)
		minHeapify(i);
}

//Function definition of minHeapify to correct any distortion in the heap from a node to it's children
void minHeap::minHeapify(int index)
{
	int small = index;
	if (index < 1 || index > size)
		return;

	if (left(index) != -1 && list[left(index)].getWeight() < list[small].getWeight())
		small = left(index);
	if (right(index) != -1 && list[right(index)].getWeight() < list[small].getWeight())
		small = right(index);

	if (small != index)
	{
		edge temp = list[small];
		list[small] = list[index];
		list[index] = temp;
		minHeapify(small);
	}
}

//Function definition of removeMin to delete and return the minimum element from the heap
edge minHeap::removeMin()
{
	edge result;
	if (size == 0)
	{
		cout << "Heap Empty!" << endl;
		result.u = -1;
		result.v = -1;
		result.weight = -1;
		return result;
	}
	result = list[1];
	list[1] = list[size];
	size--;
	if (size > 0)
		minHeapify(1);

	return result;
}

//Class graph to store the details of the graph
//Stores the number of vertices, number of edges and the list of edges
class graph
{
 private:
	int vertices, edges;
	edge *e;

 public:
	//Destructor to delete any dynamic memory
	~graph()
	{
		if (e != NULL)
			delete[] e;
	}

	graph();			 //Overloaded constructor to read the whole graph
	void kruskal(); //Function kruskal to find the MST using kruskal's algorithm
};

//Function definition of kruskal to find an MST in the graph using kruskal's algorithm
//Prints the list of edges in the MST
//Prints the weight of the MST
void graph::kruskal()
{
	//Create minHeap to get sorted Edges
	minHeap sortedEdges;
	sortedEdges.buildHeap(edges, e);
	edge MST[vertices - 1]; //Array of edges to store the edges of the MST
	int mstSize = 0;			//Initially the list is empty

	unionFind set(vertices); //Creating the set of individual vertices

	while (mstSize < vertices - 1) //While the MST is incomplete
	{
		edge f = sortedEdges.removeMin(); //Remove a minimum edge

		//If it is an edge from one component to another, unite the two components and add the edge in the MST
		if (set.find(f.u) != set.find(f.v))
		{
			set.setUnion(f.u, f.v);
			MST[mstSize] = f;
			mstSize++;
		}
	}

	//Initialize the total weight of the MST to 0
	//Print the edges in the MST while updating the total weight
	//Print the total weight of the MST
	int totalWeight = 0;
	cout << "Edges in the MST are: " << endl;
	for (int i = 0; i < mstSize; i++)
	{
		cout << MST[i].u << "," << MST[i].v << " " << MST[i].weight << endl;
		totalWeight += MST[i].weight;
	}
	cout << "Total Weight of the MST: " << totalWeight << endl;
}

//Overloaded constructor to read the graph
graph::graph()
{
	cin >> vertices;
	cin >> edges;
	e = new edge[edges];
	for (int i = 0; i < edges; i++)
	{
		cin >> e[i].u;
		cin >> e[i].v;
		cin >> e[i].weight;
	}
}

//Main function to run the program
int main()
{
	graph g;		 //Create a graph g
	g.kruskal(); //Call kruskal to find the MST

	cout << endl;
	return 0;
}