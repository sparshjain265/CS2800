/*
	Program		: q2.cpp
	Description	: ---
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Date			: February 27, 2018
*/

//Include required libraries
#include<iostream>
#include<limits.h>
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
   This implementation uses a dynamic array to store the heap of size n from index 1 to n instead of the usual array implementation of 1 to n-1
   class minHeap to implement minHeap with only necessary functions
*/
class minHeap
{
 private:
	path *list;	//Stores the heap as array
	int size;			  //Stores the size of the heap
	int *indices; //Stores the index of ith vertex;

 public:
	//Default constructor to initialize the size to be 0
	minHeap()
	{
		size = 0;
		list = new path[size + 1];
		list[0].pathTo = -1;
		list[0].key = -1;
		indices = new int[size + 1];
	}

	//Destructor to deallocate dynamic memory
	~minHeap()
	{
		if(list != NULL)
			delete[] list;
		if(indices != NULL)
			delete[] indices;
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

	minHeap(int n);								 //Overloaded constructor to build a heap of a given size with all keys as INT_MAX
	int minHeapify(int index);					 //Function minHeapify to correct any distortion between the given index and its decendents
	int siftUp(int index);						 //Function siftUp to correct any distortion between the given index and its ancestors
	path removeMin();								 //Function removeMin to remove the minimum element from the heap
	int decreaseKey(int index, int newKey); //Function to decease the key at an index
};

//Constructor to build a heap of given size with all keys as INT_MAX
minHeap::minHeap(int n)
{
	size = n;
	list = new path[size + 1];
	indices = new int[size + 1];
	for (int i = 0; i < size; i++)
	{
		list[i + 1].pathTo = i;
		list[i + 1].key = INT_MAX;
		indices[i] = i + 1;
	}
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
//Stores information of an edge
struct node
{
	int edgeTo; //Stores the vertex to which an edge exists
	int weight; //Stores the weight of the edge
	node *next; //Stores the link to the next node
};

//Class link defined to implement a linked list with only required functions
//Used to store adjacency list of a vertex
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
			delete p;
		}
	}

	void addTail(int edgeTo, int weight); //Function to add a new neighbor in the list
	friend class graph;						  //Declaring graph as a friendly class to allow access to private members
};

//Function definition to add a new neighbor in the list
//Takes the index of the neighbor and edge weight as input
void link::addTail(int edgeTo, int weight)
{
	//node *p = (node *)malloc(sizeof(node));
	node *p = new node;
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
	link list;									//A linked (adjacency) list containing it's neighbors and their edge-weights
	bool visited;								//A flag to mark whether the vertex is visited or not
	int parent, parentWeight, distance; //Variables to store the parent in a path, weight of the edge to the parent and distance from the source vertex

 public:
	//Default constructor to initialize distance as infinity, parent as none, and visited as false
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
	vertex *v;							 					//Dynamic array of vertices
	int vertices, edges;				 					//Variables to store the number of vertices, edges, and the source vertex

 public:
	void readGraph(); 									//Function to read a graph
	void dijkstra(int source, int destination);	//Function to call the dijkstra algorithm and find the minimum distance paths from the source vertex
	void drive();						 					//Function to give a shortest route to the driver according to the requirements

	//Destructor to free the memory allocated for vertices
	~graph()
	{
		delete[] v;
	}
};

//Function Definition of the drive to provide proper route to the drivers
void graph::drive()
{
	int rounds;								
	cin >> rounds;							//Reading number of rounds

	//Perform the same method for given number of rounds
	for(int i = 0; i < rounds; i++)
	{
		int repair;
		cin >> repair;						//Reading the number of roads under repair
		//Read the roads under repair
		for(int j = 0; j < repair; j++)
		{
			int source, destination;
			cin >> source;
			cin >> destination;
			node *p = v[source].list.head;	//p to go through the adjacency list of the source vertex
			//marking the road under repair
			while(p != NULL)
			{
				if(p->edgeTo == destination)
				{
					p->weight = -1*p->weight;
					break;
				}
				else p = p->next;
			}
		}

		//Reading the request of the driver
		int source, destination;
		cin >> source;
		cin >> destination;

		//Calling dijkstra to find the appropriate path for the driver
		dijkstra(source, destination);

		//Unmark the roads for next round
		//Also reset all the parent, parentWeight, and distances
		for(int j = 0; j < vertices; j++)
		{
			node *p = v[j].list.head;	//p to go through the adjacency list of jth vertex
			//unmark all the roads under repair which start from jth city
			while(p != NULL)
			{
				if(p->weight < 0)
					p->weight = -1*p->weight;
				p = p->next;
			}

			v[j].visited = false;
			v[j].parent = -1;
			v[j].parentWeight = INT_MAX;
			v[j].distance = INT_MAX;
		}
	}
}


//Function Definition of the dijkstra's algorithm
//Takes source and destination vertices as input
//Prints the shortest path from the source to destination
//Automatically disregards any negative edge
void graph::dijkstra(int source, int destination)
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
		//If this is the destination, no need to continue the algorithm
		if(u.pathTo == destination)
			break;
		node *p = v[u.pathTo].list.head; //p to go through the adjacency list of this vertex
		while (p != NULL)
		{	
			//Negative edge weight means the road is under repair and the edge cannot be considered
			//If the adjacent vertex is unvisited AND the edge weight is non-negative AND visiting it through this path reduces the distance from the source vertex
			if ((!v[p->edgeTo].visited) && (p->weight >= 0) && (distance.getValue(distance.getIndex(p->edgeTo)).key > u.key + p->weight))
			{
				distance.decreaseKey(distance.getIndex(p->edgeTo), u.key + p->weight); //Reduce the current distance
				v[p->edgeTo].parent = u.pathTo;													  //Update it's parent
				v[p->edgeTo].parentWeight = p->weight;											  //Update the weight of it's edge to it's parent
			}
			p = p->next; //Check for next adjacent vertex
		}
	}

	//Display the shortest path to destination vertex from the source vertex
	//cout << "Shortest path from " << source << "to each vertex:" << endl;
	int i = 0, path[vertices];
	int current = destination;
	//If no path found, parent of destination would be -1
	if(v[destination].parent == -1)
	{
		cout<<"no route found"<<endl;
		return;
	}
	
	//Print the path if exists
	cout<<"Minimum Path is ";
	path[i] = destination;
	i++;
	while(v[current].parent != -1)
	{
		path[i] = v[current].parent;
		i++;
		current = v[current].parent;
	}
	while(i > 0)
	{
		i--;
		cout<<path[i];
		if(i > 0)
			cout<<"-->";
	}
	cout<<endl;
	cout<<"Minimum Path Distance is "<<v[destination].distance<<endl;
}

//Function defined to read the graph from the file
void graph::readGraph()
{
	cin >> vertices;
	cin >> edges;
	v = new vertex[vertices];

	for (int i = 0; i < edges; i++)
	{
		int source, destination, weight;
		cin >> source;
		cin >> destination;
		cin >> weight;
		v[source].addEdge(destination, weight);
	}
}

//main function to run the program
int main()
{
	//Read the graph and print the paths
	graph city;
	city.readGraph();
	
	//Call the drive function to report the required paths
	city.drive();

	//Bring the cursor to newLine before exiting
	cout << endl;
	return 0;
}