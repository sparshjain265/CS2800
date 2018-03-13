/*
	Program Name	:	bellmanFord.cpp
	Programmer		:	Sparsh Jain
	Roll No			:	111601026
	Description		:	Implement Bellman-Ford Algorithm to find single source shortest distance in a graph with no negative weighted cycle
	Exception		:	Prints the negative weighted cycle in case the graph has one
	Date				:	March 5, 2018
*/
#include <iostream>
#include <limits.h>
using namespace std;

//Declaring class graph so it can be referred to in other classes
class graph;

//Class edge defined to handle edges in a graph
//Stores starting vertex as u, destination vertex as v and edge weight as weight
class edge
{
 private:
	int u, v, weight;

	//Declaring graph as friendly class for easy access to data
	friend class graph;
};

//Class vertex defined to handle vertices in a graph
class vertex
{
 private:
	int parent, parentWeight, distance; //Variables to store the parent in a path, weight of the edge to the parent and distance from the source vertex

 public:
	//Constructor to initialize distance as infinity, and parent as none
	vertex()
	{
		parent = -1;
		parentWeight = distance = INT_MAX;
	}

	//Declaring graph as a friendly function for easy access of private members
	friend class graph;
};

//Class graph to store the details of the graph
//Stores the number of vertices, number of edges and the list of edges
class graph
{
 private:
	int vertices, edges;
	vertex *v;
	edge *e;

 public:
	//Destructor to delete any dynamic memory
	~graph()
	{
		if (e != NULL)
			delete[] e;
		if (v != NULL)
			delete[] v;
	}

	graph();				  //Overloaded constructor to read the whole graph
	void bellmanFord(); //Function to find the shortest distance path from source vertex to all the vertices using bellman ford algorithm
	bool relax(edge e); //Function to perform relax operation to update the distance of a vertex
};

//Function bellmanFord to find the shortest distance path from source vertex to all the vertices
//Prints the negative cycle if present
void graph::bellmanFord()
{
	//Read the source and update it's credentials
	int source;
	cin >> source;
	v[source].distance = 0;
	v[source].parent = -1;
	v[source].parentWeight = 0;

	//Perform the relax operations on all edges n-1 times
	for (int i = 1; i < vertices; i++)
	{
		bool flag = false;
		for (int j = 0; j < edges; j++)
			if (relax(e[j]))
				flag = true;

		//If no update was made in this round, none will be made in further rounds
		if (!flag)
			break;
	}

	//check for negative weighted cycle
	//If a distance to the vertex is updated by some relax operation, it means a negative weighted cycle is present
	int updatedVertex = -1;
	for (int j = 0; j < edges; j++)
	{
		if (relax(e[j]))
			updatedVertex = e[j].v;
	}

	//If the updated vertex is not -1, a negative weighted cycle was encountered
	if (updatedVertex != -1)
	{
		//keep a track of visited vertices
		bool visited[vertices];
		for (int i = 0; i < vertices; i++)
			visited[i] = false;
		//Go up via parent, if a vertex is visited again, we've found a cycle
		while (!visited[updatedVertex])
		{
			visited[updatedVertex] = true;
			updatedVertex = v[updatedVertex].parent;
		}
		int cycle[vertices], current = v[updatedVertex].parent, j = 1, weight = 0;
		cycle[0] = updatedVertex;
		weight += v[updatedVertex].parentWeight;
		while (current != updatedVertex)
		{
			cycle[j] = current;
			j++;
			weight += v[current].parentWeight;
			current = v[current].parent;
		}
		cout << "The negative weighted cycle is: " << updatedVertex;
		while (j > 0)
		{
			j--;
			cout << "--->" << cycle[j];
		}
		cout << endl;
		cout << "Weight of the cycle is " << weight << endl;
	}
	//else print the shortest path from the source vertex to all the vertices
	else
	{
		for (int i = 0; i < vertices; i++)
		{
			cout << endl;
			if (v[i].distance != INT_MAX)
			{
				int current = i, path[vertices], j = 0;
				cout << "Shortest path from " << source << " to " << i << " : ";
				//Go up via parent and store the ancestors in the path array till you reach the source vertex
				//Update the weight of the path simultaneously
				while (current != source && current != -1)
				{
					path[j] = current;
					j++;
					current = v[current].parent;
				}
				path[j] = current;
				j++;

				// print the path array downwards to show the path from source to the vertex
				while (j > 1)
				{
					j--;
					cout << path[j] << "--->";
				}
				cout << path[0];
				cout << endl;
				//Print the weight of the path
				cout << "Weight: " << v[i].distance << endl;
			}
			else
				cout << i << " is not reachable from " << source << endl;
		}
	}
}

//Function relax to perform the relax operation
//Takes an edge e as input
//Returns true if including the edge reduced the distance of destination vertex, else returns false
bool graph::relax(edge e)
{
	if (v[e.u].distance != INT_MAX && v[e.u].distance + e.weight < v[e.v].distance)
	{
		v[e.v].distance = v[e.u].distance + e.weight;
		v[e.v].parent = e.u;
		v[e.v].parentWeight = e.weight;
		return true;
	}
	return false;
}

//Overloaded constructor to read the graph
graph::graph()
{
	cin >> vertices;
	cin >> edges;
	v = new vertex[vertices];
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
	graph g;			  //Create a graph g
	g.bellmanFord(); //Find the shortest distance path from the source vertex or print the negative cycle

	cout << endl;
	return 0;
}