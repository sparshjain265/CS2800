/*
	Program Name: kruskal.cpp
	Programmer	: Sparsh Jain
	Roll No		: 111601026
	Description	: Implement kruskal's algorithm to find MST using Union-Find
	Date			: February 13, 2018
*/

#include<iostream>

class unionFind;

class node
{
	private:
		int rank;
		int index;
		int parent;
	public:
		node()
		{
			rank = 0;
			index = 0;
			parent = index;
		}

		friend class unionFind;
};

class unionFind
{
	private:
		int size;
		node *vertex;
	public:
		unionFind()
		{
			size = 0;
			vertex = NULL;
		}

		unionFind(int n)
		{
			size = n;
			vertex = new node[n];
			for(i = 0; i < n; i++)
				makeSet(i);
		}

		void makeSet(int index)
		{
			vertex[index].parent = index;
			vertex[index].rank	= 0;
			vertex[index].index	= index;
		}

		~unionFind()
		{
			delete[] vertex;
		}

		int find(int index);
		void setUnion(int u, int v);

};

int unionFind::find(int index)
{
	if(vertex[index].parent == index)
		return index;
	
	int v = find(vertex[index].parent);
	vertex[index].parent = v;
	return v;
}

void unionFind::setUnion(int u, int v)
{
	int x = find(u);
	int y = find(v);
	if(x != y)
	{
		if(vertex[x].rank == vertex[y].rank)
		{
			vertex[y].parent = x;
			vertex[x].rank++;
		}
		else if(vertex[x].rank > vertex[y].rank)
			vertex[y].parent = x;
		else vertex[x].parent = y;
	}
}

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
}

class graph
{
	private:
		int vertices, edges;
		edge *e;
	public:
		graph();
		void kruskal();
}

void graph::kruskal()
{
	minHeap sortedEdges;
	sortedEdges.buildHeap(edges, e);
	edge MST[vertices-1];
	int mstSize = 0;

	unionFind set(vertices);

	while (i < vertices - 1)
	{
		edge f = sortedEdges.removeMin();
		
		if(set.find[f.u] != set.find[f.v])
		{
			set.setUnion(f.u, f.v);
			MST[i] = f;
			i++;
		}
	}

	cout<<"Edges in the MST are: "<<endl;
	for(i = 0; i < vertices - 1; i++)
		cout<<MST[i].u<<","<<MST[i].v<<" "<<MST[i].weight<<endl;
}

graph::graph()
{
	cin>>vertices;
	cin>>edges;
	e = new edge[edges];
	for(int i = 0; i < edges; i++)
	{
		cin>>e[i].u;
		cin>>e[i].v;
		cin>>e[i].weight;
	}
}

class minHeap
{
	private:
		edge *list;
		int size;
	
	public:
		minHeap()
		{
			size = 0;
			list = NULL;
		}

		~minHeap()
		{
			if(list != NULL)
				delete[] list;
		}

		int left(int index)
		{
			if(2*index <= size)
				return 2*index;
			else return -1;
		}

		int right(int index)
		{
			if(2*index +1 <= size)
				return 2*index+1;
			else return -1;
		}

		void buildHeap(int edges, edge e[]);
		void minHeapify(int index);
		edge removeMin();
}

void minHeap::buildHeap(int edges, edge e[])
{
	size = 0;
	if(list != NULL)
		delete[] list;
	list = new edge[edges + 1];
	for(int i = 0; i < edges; i++)
	{
		size++;
		list[size] = e[i];
		// list[size].u = e[i].u;
		// list[size].v = e[i].v;
		// list[size].weight = e[i].weight;
	}
	for(int i = size/2; i > 0; i--)
		minHeapify(i);
}
void minHeap::minHeapify(int index)
{
	int small = index;
	if(index < 1 || index > size)
		return;
	
	if(left(index) != -1 && list[left(index)].getWeight() < list[small].getWeight())
		small = left(index);
	if(right(index) != -1 && list[right(index)].getWeight() < list[small].getWeight())
		small = right(index);
	
	if(small != index)
	{
		edge temp = list[small];
		list[small] = list[index];
		list[index] = temp;
		minHeapify(small);
	}
}
edge minHeap::removeMin()
{
	edge result;
	if(size == 0)
	{
		cout<<"Heap Empty!"<<endl;
		result.u = -1;
		result.v = -1;
		result.weight = -1;
		return result;
	}
	result = list[1];
	list[1] = list[size];
	size--;
	if(size > 0)
		minHeapify(1);
	
	return result;
}