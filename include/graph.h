#ifndef GRAPHS_H_
#define GRAPHS_H_
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <time.h>
#include <queue>

#include "adjlist.h"
#include "fibonacci.h"

using namespace std;

enum selection
{
	ARRAY = 1,          // Simple Scheme implementation
	FIBONACCI = 2,      // Fibonacci heap implementation
	UI = 3,             // User input mode
	RAN = 4,            // Random mode
	FIBO_REMOVE_MIN = 5 // Fibonacci remove min method(Binomial Heap)
};

/* Class Graphs
 * It holds number of vertices and edges and the adjacency list
 * It also has all the operations required to modify the graph
 */
class Graphs
{
public:
	int num_vertices, num_edges;
	vector<Vertex*> adjlists;

	Graphs()
	{
		num_vertices = num_edges = 0;
		for (int i = 0; (unsigned)i < adjlists.size(); i++)
		{
			adjlists.push_back(0);
		}
	}
	~Graphs() { }

	/* Creates graph for the file input scheme
	 * @param file_name has the command line argument which gives the file name.
	 * This method creates the adjacency list
	 */
	void create_graph_from_file(char* file_name)
	{
		ifstream input_file (file_name);
		int m, n;

		if (input_file.is_open())
		{
			input_file>>m>>n;
			num_vertices = m;
			num_edges = n;

			// initializing the vertices
			for (int i = 0; i < num_vertices; i++)
			{
				adjlists.push_back(new Vertex(i, 0));
			}
			// fetching edges and storing in the adjacency list
			int v1, v2, cost;
			for (int i = 0; i < num_edges; i++)
			{
				input_file>>v1>>v2>>cost;
				if (check_vertex(v1) == 0 || check_vertex(v2) == 0)
				{
					cout << " Invalid vertex numbers.. Try another file.. " << endl;
					exit(1);
				}
				else
				{
					adjlists[v1]->adjlist = new Neighbor(v2, cost, v1, adjlists[v1]->adjlist);
					adjlists[v2]->adjlist = new Neighbor(v1, cost, v2, adjlists[v2]->adjlist);
				}
			}
			input_file.close();
		}
		else
		{
			cout << " Invalid File name " << endl;
		}
	}

	void prims(int ds, int mode = RAN);

	/* To create a graph for random scheme
	 * @param n gives the number of vertices entered on the command line
	 * @param d gives the density of edges entered on the command line
	 * This method creates the adjacency list
	 */
	void create_random_graph(int n, double d)
	{
		num_vertices = n;

		// initializing the vertices
		for (int i = 0; i < num_vertices; i++)
		{
			adjlists.push_back(new Vertex(i, 0));
		}

		// generating the edges
		srand (time(NULL));
		double density = d, x = 0;
		num_edges = ceil((density * n * (n-1))/200);
		
		if (num_edges < (num_vertices - 1))
		{
			cout << " Graph cannot be constructed.. Density is too low " << endl;
			exit(1);
		}

		while (x != num_edges)
		{
			int v1 = rand() % num_vertices;
			int v2 = rand() % num_vertices;
			int cost = rand() % 1000 + 1;
			
			// check if the edges are already present
			if (check_edges(v1, v2))
			{
				continue;
			}
			else
			{
				adjlists[v1]->adjlist = new Neighbor(v2, cost, v1, adjlists[v1]->adjlist);
				adjlists[v2]->adjlist = new Neighbor(v1, cost, v2, adjlists[v2]->adjlist);
				x++;
			}
		}
	}

	/* Check if the vertices of an edge are present in the graph in file input mode
	 * @param num1 is the vertex number
	 * returns 1 if it exists, otherwise returns 0
	 */
	int check_vertex(int num1)
	{
		int mid, flag = 0;
		mid = ceil(num_vertices/2);
		if (adjlists[mid]->v_num == num1) flag = 1;
		else if(adjlists[mid]->v_num < num1)
		{
			for (int i = mid+1; i < num_vertices; i++)
			{
				if (adjlists[i]->v_num == num1)
				{
					flag = 1;
				}
				
			}
		}
		else
		{
			for (int i = 0; i < mid; i++)
			{
				if (adjlists[i]->v_num == num1)
				{
					flag = 1;
				}
			}
		}
		return flag;
	}

	/* Check if the edge is already present in the graph in the random mode
	 * @param n1 gives the first vertex
	 * @param n2 gives the second vertex
	 * Returns 1 if the edge already exists
	 * Returns 0 if the edge is not present
	 */
	int check_edges(int n1, int n2)
	{
		if (n1 == n2)
		{
			return 1;
		}
		Neighbor* nbr = adjlists[n1]->adjlist;
		if (nbr == 0)
		{
			return 0;
		}
		while (nbr != 0)
		{
			// checking the source and destination 
			// of each node on the adjacency list
			if (nbr->dest == n2)
			{
				return 1;
			}
			nbr = nbr->next;
		}
		return 0;
	}

	/* to check if the graph created by random functions is connected or not
	 */
	int is_connected()
	{
		// num of edges should not be less than num of vertices - 1
		if (num_edges < num_vertices-1)
		{
			return 0;
		}

		int visited[num_vertices];
		for (int i = 0; i < num_vertices; i++) {
			visited[i] = 0;
		}
		int start_vertex = 0;
		int next_vertex;
		queue<int> temp;

		// start by visiting the start vertex and continue with its neighbors
		// if the graph is connected, all the vertices will be visited
		if (visited[start_vertex] != 1)
		{
			temp.push(start_vertex);
			while(!temp.empty())
			{
				next_vertex = temp.front();
				temp.pop();
				visited[next_vertex] = 1;
				for (Neighbor* nbr = adjlists[next_vertex]->adjlist; nbr != 0; nbr = nbr->next)
				{
					if(visited[nbr->dest] != 1)
					{
						temp.push(nbr->dest);
					}
				}
			}
		}
		for (int i = 0; i < num_vertices; i++)
		{
			// if visited of all vertices is not set
			// then the graph is not connected
			if (visited[i] != 1)
			{
				return 0;
			}
		}
		return 1;
	}

	/* Can be used to print the adjacency list
	 * For testing purposes
	 */
	void print_adjacency()
	{
		
		for (int i = 0; i < num_vertices; i++)
		{
			cout << adjlists[i]->v_num;
			for (Neighbor* nbr = adjlists[i]->adjlist; nbr != 0; nbr = nbr->next)
			{
				cout << " --> " << adjlists[nbr->dest]->v_num << " ( " << nbr->cost << " )";
			}
			cout << endl;
		}
	}
};

/* To apply Prim's algorithm to generate a Minimum Spanning Tree
 * @param ds gives the data structure to be used to implement the 
 * algorithm. It is based on the command line argument given.
 * -s will use an Array and -f will use a Fibonacci heap.
 */
void Graphs::prims(int ds, int mode)
{
	// visited will keep track of vertices already visited
	int visited[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		visited[i] = 0;
	}
	int mst[num_vertices][2];
	int mst_cost = 0;
	int selected_vertex = 0, source_vert = 0, count = 0;

	switch(ds)
	{
	case ARRAY: 
	{
		/* distance[] maintains distance of a vertex from a selected vertex in the MST
		 * nearest[] stores the source of the distance which is stored in the distance array
		 */
		int distance[num_vertices], nearest[num_vertices];
		for (int i = 0; i < num_vertices; i++)
		{
			distance[i] = 9999;
			nearest[i] = -1;
		}
		while (visited[selected_vertex] != 1 && count != (num_vertices-1))
		{
			int min_edge_cost = 9999;
			visited[selected_vertex] = 1;
			for (Neighbor* nbr = adjlists[selected_vertex]->adjlist; nbr != 0; nbr = nbr->next)
			{
				// Updating the distance and closest array entries for all the 
				// neighbors of the selected vertex
				if (visited[nbr->dest] != 1) {
					if (nbr->cost < distance[nbr->dest])
					{
						distance[nbr->dest] = nbr->cost;
						nearest[nbr->dest] = nbr->source;
					}
				}
			}

			// removing the min from the distance array
			for (int i = 0; i < num_vertices; i++)
			{
				if (distance[i] < min_edge_cost)
				{
					source_vert = nearest[i];
					selected_vertex = i;
					min_edge_cost = distance[i];
				}
			}
			distance[selected_vertex] = 9999;
			nearest[selected_vertex] = -1;
			// updating the MST
			mst[count][0] = source_vert;
			mst[count][1] = selected_vertex;
			count++;
			mst_cost += min_edge_cost;
		}
	}
	case FIBO_REMOVE_MIN:
	{
		// Fibonacci scheme uses a fibonacci heap to store the edges
		fiboHeap* fibo_heap = new fiboHeap();
		while (visited[selected_vertex] != 1 && count != (num_vertices-1))
		{
			int min_edge_cost = 9999;
			visited[selected_vertex] = 1;
			for (Neighbor* nbr = adjlists[selected_vertex]->adjlist; nbr != 0; nbr = nbr->next)
			{
				if (visited[nbr->dest] != 1) 
				{
					fibo_heap->insert(new fiboNode(nbr));
				}
			}
			// removing the minimum cost edge from the heap
			fiboNode* min_node = fibo_heap->remove_min();
			if (min_node == 0)
			{
				cout << " Oops :( Something went wrong, this wasn't supposed to happen.. " << endl;
				exit(1);
			}
			min_edge_cost = min_node->nbr->cost;
			selected_vertex = min_node->nbr->dest;
			source_vert = min_node->nbr->source;

			mst[count][0] = source_vert;
			mst[count][1] = selected_vertex;
			count++;
			mst_cost += min_edge_cost;
		}
	}

	/* A fibonacci heap of key values is created
	 * node_ptr[] holds the pointers to these fiboheap nodes
	 * key value is updated and min is chosen till all the vertices are visited
	 */
	case FIBONACCI:
	{
		// Fibonacci heap is used to store the distances
		fiboHeap* fibo_heap = new fiboHeap();
		fiboNode* node_ptr[num_vertices];
		for (int i = 0; i < num_vertices; i++)
		{
			node_ptr[i] = new fiboNode(0, 9999);
			fibo_heap->insert(node_ptr[i]);
		}
		while (visited[selected_vertex] != 1 && count != (num_vertices-1))
		{
			visited[selected_vertex] = 1;
			for (Neighbor* nbr = adjlists[selected_vertex]->adjlist; nbr != 0; nbr = nbr->next)
			{
				if (visited[nbr->dest] != 1) 
				{
					if (nbr->cost < node_ptr[nbr->dest]->key)
					{
						fibo_heap->decrease_key(node_ptr[nbr->dest], nbr->cost);
						node_ptr[nbr->dest]->key = nbr->cost;
						node_ptr[nbr->dest]->nbr = nbr;
					}
				}
			}
			fiboNode* min_node = fibo_heap->remove_min();
			if (min_node == 0)
			{
				cout << " Oops :( Something went wrong, this wasn't supposed to happen.. " << endl;
				exit(1);
			}
			int min_edge_cost = min_node->nbr->cost;
			selected_vertex = min_node->nbr->dest;
			source_vert = min_node->nbr->source;

			mst[count][0] = source_vert;
			mst[count][1] = selected_vertex;
			count++;
			mst_cost += min_edge_cost;		
		}
	}
	}

	// Output for the User input mode
	if (mode == UI)
	{
		cout << mst_cost << endl;
		for (int i = 0; i < count; i++)
		{
			cout << mst[i][0] << " " << mst[i][1] << endl;
		}
	}
}

#endif /*GRAPHS_H_*/
