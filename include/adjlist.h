#ifndef ADJLIST_H_
#define ADJLIST_H_

#include <iostream>

using namespace std;

/* 
 * Class Neighbor defines objects of an edge.
 * It stores source vertex, destination vertex and 
 * the cost along with a pointer to the next edge node.
 */
class Neighbor
{
public:
	int dest;
	int cost;
	int source;
	Neighbor* next;
	Neighbor(int v, int c, int s, Neighbor* n)
		: dest(v),
		  cost(c),
	          source(s),
	          next(n) {}
	~Neighbor() {}
};

/* 
 * Class vertex defines each vertex.
 * Each vertex has an adjacent list which points to the neighboring vertices.
 */
class Vertex
{
public:
	int v_num;
	Neighbor* adjlist;
	Vertex() {}
	Vertex(int num, Neighbor* n)
		: v_num(num),
		  adjlist(n) {}
	~Vertex(){}
};

#endif /*ADJLIST_H_*/
