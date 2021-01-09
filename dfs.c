/*
Program		: dfs.c
Programmer	: Sparsh Jain
Roll No		: 111601026
Description	: Identifies different types of Edges using dfs in adjacency list
*/

//Including required libraries
#include<stdio.h>
#include<stdlib.h>

//Introducing global variables for easy access across functions
int *visited, *start, *end, clock = 0;

//Struct node defined to keep a node of the graph
struct node
{
	int data;
	struct node *next;
};

//Struct link defined to manage a linked list of the nodes
struct link
{
	int size;
	struct node *head;
	struct node *tail;
};

//Function createList to initialize a linked list
//Takes pointer to a list as input
void createList (struct link *l)
{
	l->size = 0;
	l->head = NULL;
	l->tail = NULL;
}

//Function addTail to add a new node in a linked list in its tail
//Takes pointer to the list and integer data as input
void addTail (struct link *l, int data)
{
	struct node *p = (struct node *)malloc(sizeof(struct node));
	p->data = data;
	p->next = NULL;
	if(l->size == 0)
		l->head = p;
	else l->tail->next = p;
	l->tail = p;
	l->size++;
}

/*
	Structure graph to store the adjacency list of a graph
	Assuming the adjList file to have number of vertices in first line and adjacent vertices in the subsequent lines
	Starting node will be assumed to be 0
*/
struct graph
{
	int size;
	struct link *list;
};

//Function readGraph to read a text file containing adjacency list of a graph
//Takes pointer to a file as input
//Returns a structure of type graph
struct graph readGraph(FILE *adjList)
{
	struct graph g;
	int i, v;
	
	fscanf(adjList, " %d", &g.size);
	g.list = (struct link *)calloc(g.size, sizeof(struct link));
	for(i = 0; i < g.size; i++)
		createList(&g.list[i]);
	
	i = 0;
	while(i < g.size)
	{
		fscanf(adjList, " %d", &v);
		if(v != -1)
			addTail(&g.list[i], v);
		else i++;
	}
	
	return g;
}

//Function printGraph to print the given graph in the form of adjacency list
//Takes a graph as input
void printGraph(struct graph g)
{
	int i;
	struct node *p;

	for(i = 0; i < g.size; i++)
	{
		p = g.list[i].head;
		
		while(p != NULL)
		{
			printf("%d ", p->data);
			p = p->next;
		}
		printf("\n");
	}
}

//Function dfs to implement dfs on a graph given as adjacency list
//At each edge, it also identifies the type of edge
//Takes a graph and current or starting point of the dfs as input
//Recursive implementation
void dfs(struct graph g, int current)
{
	struct node *p;
	start[current] = clock;
	clock++;
	visited[current] = 1;
	p = g.list[current].head;
	while(p != NULL)
	{
		if(visited[p->data] == -1)
		{
			printf("\n Tree Edge: %d-%d", current, p->data);
			dfs(g, p->data);
		}
		else if(p->data == current)
			printf("\n Self Edge: %d-%d", current, p->data);
		else if(start[p->data] > start[current])
			printf("\n Forward Edge: %d-%d", current, p->data);
		else if(end[p->data] == -1)
			printf("\n Back Edge: %d-%d", current, p->data);
		else printf("\n Cross Edge: %d-%d", current, p->data);
		p = p->next;
	}
	end[current] = clock;
	clock++;
}

//Function track to track the type of edges, start and end times of node in dfs of the graph starting with the least unvisited node
//Takes a graph as input
void track(struct graph g)
{
	int i;
	
	visited = (int *)calloc(g.size, sizeof(int));
	start = (int *)calloc(g.size, sizeof(int));
	end = (int *)calloc(g.size, sizeof(int));

	for(i = 0; i < g.size; i++)
	{
		visited[i] = -1;
		start[i] = -1;
		end[i] = -1;
	}

	for(i = 0; i < g.size; i++)
		if(visited[i] == -1)
			dfs(g, i);

	for(i = 0; i < g.size; i++)
		printf("\n Node: %d, Start: %d, End: %d", i, start[i], end[i]);	

}

//Main function defined to read a graph file given in adjacency list format and identify the type of edges and start/end timings of each node
//Returns -1 if the entered file is not found, 0 for successful completion for the program
int main()
{
	struct graph g;
	FILE *adjList;
	char adjListName[50];
	
	printf("\nEnter the name of the file: ");
	scanf(" %s", adjListName);
	adjList = fopen(adjListName, "r");
	
	if(adjList == NULL)
	{
		printf("\n File Not Found!");
		return -1;
	}

	g = readGraph(adjList);
	printGraph(g);
	
	track(g);

	printf("\n\n");
	fclose(adjList);
	return 0;
}
