/*
   Program     : topologicalSorting.cpp
   Description : Determines a topologicalSorting in a DAG, intimates the user if graph is not a DAG
   Programmer  : Sparsh Jain
   Roll  No    : 111601026
   Date        : January 16, 2018
*/

#include<iostream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

//Self referencial structure for dynamic memory data types
struct node
{
   int data;
   node *next;
};

//Class link implementing linked list
class link
{
public:
   int size;
   link ();
   void addTail(int data);
   node *head;
   node *tail;
};

//Constructor of linked list, initializes the head and tail pointer to NULL and sets the size to 0
link::link()
{
   size = 0;
   head = NULL;
   tail = NULL;
}

//addTail function of the linked list, adds a new node at the end of the list and increments it's size
//takes integer data as input
void link::addTail(int data)
{
   node *p = (node *)malloc(sizeof(node));
   p->data = data;
   p->next = NULL;
   if(size == 0)
      head = p;
   else tail->next = p;
   tail = p;
   size++;
}

//Class graph to store a graph and it's related information and functions
class graph
{
private:
   //dynamic array of linked lists to store the adjacency list of the graph
   link *list;
public:
    //variable to store relevant data
    //size stores the number of nodes in the graph
    //visited keeps track whether a node is visited or not, start and end note the time when it was visited with respect to the clock
    //topologicalOrder maintains the record of the topological order of the graph, cycle maintains the first directed cycle found in the graph
    //position is used as a counter to update topological order or cycle, repeat is the first vertex found to have a back edge
    //isDAG stores whether the given graph is a DAG or not
   int size, *visited, *start, *end, clock, *topologicalOrder, position, *cycle, repeat;
   bool isDAG;
   void readGraph(FILE *adjList);
   void printGraph();
   void dfs(int current);
   void dfsForest();
   void printTopologicalOrder();
   void printDirectedCycle();
};

//function to print the Directed Cycle of the Non DAG
void graph::printDirectedCycle()
{
    cout<<endl;
    cout<<repeat;
    for(position = size - 1; position >=0; position--)
    {
        if(cycle[position] == -1)   continue;
        else    cout<<" "<<cycle[position];
    }
    cout<<endl;
}

//function to print the topological Order of the DAG
void graph::printTopologicalOrder()
{
   cout<<endl;
   for(position = 0; position < size; position++)
      cout<<topologicalOrder[position]<<" ";
   cout<<endl;
}

//function to create a dfsForest to determine the topological sorting
void graph::dfsForest()
{
   int i;
   clock = 0;
   isDAG = true;
   position = size - 1;

    //discontinue if the graph is not a DAG, not required anymore
   for(i = 0; i < size && isDAG; i++)
      if(visited[i] == -1)
         dfs(i);

}

//function to create a dfs tree from a node taken as input
//sets the start and end time with respect to the clock
//marks nodes as visited or unvisited
void graph::dfs(int current)
{
   node *p;
   start[current] = clock;
   clock++;
   visited[current] = 1;
   p = list[current].head;
   while (p != NULL)
   {
      if(visited[p->data] == -1)
      {
         //printf("\n Tree Edge: %d-%d", current, p->data);
         dfs(p->data);
         if(!isDAG)
         {
             if(position == -1)  return;
             else if(current != repeat)
             {
                cycle[position] = current;
                position++;
                return;
             }
            else return;
         }
      }
      else if(p->data == current)
      {
         //printf("\n Self Edge: %d-%d", current, p->data);
         isDAG = false;
         //cout<<"Self Edge on "<<current<< "found!"<<endl;
         //cout<<"Graph is not a DAG!"<<endl;
         cout<<"A Self Loop Detected on "<<current<<endl;
         position = -1;
         return;
      }
      else if(start[p->data] > start[current]);
         //printf("\n Forward Edge: %d-%d", current, p->data);
      else if(end[p->data] == -1)
      {
         //printf("\n Back Edge: %d-%d", current, p->data);
         isDAG = false;
         //cout<<"Back Edge "<<current<<"-"<<p->data<<" found!"<<endl;
         //cout<<"Graph is not a DAG!"<<endl;
         cout<<"A directed cycle is detected"<<endl;
         repeat = current;
         position = 0;
         cycle[position] = current;
         position++;
         return;
      }
      else; //printf("\n Cross Edge: %d-%d", current, p->data);
      p = p->next;
   }
   end[current] = clock;                                 //Push into stack here
   clock++;
   topologicalOrder[position] = current;
   position--;
}

//function to read the graph as adjacency list from a file
//takes the file pointer as input
//initializes the dynamic arrays according to the graph size, setting them to -1
void graph::readGraph(FILE *adjList)
{
   int i = 0, v;

   fscanf(adjList, " %d", &size);
   list = (link *)calloc(size, sizeof(link));   //Constructor of link should be automatically called
   visited = (int *)calloc(size, sizeof(int));
   start = (int*)calloc(size, sizeof(int));
   end = (int*)calloc(size, sizeof(int));
   topologicalOrder = (int *)calloc(size, sizeof(int));
   cycle = (int *)calloc(size, sizeof(int));
   position = size - 1;
   for(i = 0; i < size; i++)
      cycle[i] = topologicalOrder[i] = visited[i] = start[i] = end[i] = -1;
    repeat = -1;
   i = 0;
   while(i < size)
   {
      fscanf(adjList, " %d", &v);
      if (v != -1)
         list[i].addTail(v);
      else i++;
   }
}

//function to print the adjacency list on the screen
void graph::printGraph()
{
   int i;
   node *p;

   for(i = 0; i < size; i++)
   {
      p = list[i].head;
      while(p != NULL)
      {
         cout<<p->data<<" ";
         p = p->next;
      }
      cout<<endl;
   }
}

int main()
{
   graph g;
   FILE *adjList;
   char adjListName[50];

   cout<<"Enter the name of the file: ";
   cin>>adjListName;
   adjList = fopen(adjListName, "r");

   if(adjList == NULL)
   {
      cout<<"File Not Found!"<<endl;
      return -1;
   }

   g.readGraph(adjList);
   //g.printGraph();
   g.dfsForest();
   if(g.isDAG)  g.printTopologicalOrder();
   else g.printDirectedCycle();

   cout<<endl<<endl;
   fclose(adjList);
   return 0;
}
