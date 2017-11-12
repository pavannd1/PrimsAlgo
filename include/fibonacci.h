#ifndef FIBONACCI_H_
#define FIBONACCI_H_

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>

#include "adjlist.h"

using namespace std;

class fiboHeap;

/* fiboNode class defines objects of each node type in a fiboHeap
 * it holds the key which is the cost of an edge along with other basic attributes and the edge itself
 */
class fiboNode
{
public:
	int degree, child_cut, marked, key;
	Neighbor* nbr;
	fiboNode *child, *parent, *left, *right;

	// default constructor
	fiboNode()
	{
		degree = child_cut = marked = key = 0;
		nbr = 0;
		child = parent = left = right = 0;
	}

	// initializes the edge node into the node
	fiboNode(Neighbor* n)
	{
		degree = child_cut = marked = 0;
		nbr = n;
		key = nbr->cost;
		child = parent = left = right = 0;
	}
	
	// for initializing edge node and key
	fiboNode(Neighbor* n, int k)
	{
		degree = child_cut = marked = 0;
		nbr = n;
		key = k;
		child = parent = left = right = 0;
	}

	~fiboNode() { }

	// compares the cost of two nodes and returns true if if the caller is lesser
	int min_cost(fiboNode* node)
	{
		if(key < node->key) return 1;
		return 0;
	}

	// inserts the passed node as the child node of the caller and returns the parent node
	fiboNode* insert_child(fiboNode* child_node)
	{
		if(child == 0)
		{
			child = child_node;
			child->parent = this;
			child->left = child->right = child;
			degree++;
			this->child_cut = 0;
		}
		else
		{
			child_node->right = child->right;
			child_node->left = child;
			child_node->right->left = child_node;
			child->right = child_node;
			child_node->parent = this;
			degree++;
			this->child_cut = 0;
		}
		return this;
	}
};

/* fiboHeap class defines the fibonacci heap
 * it has a min pointer which points to minimum element of the heap
 * it also stores the numNodes for calculation purposes
 */
class fiboHeap
{
public:
	fiboNode *min;
	int numNodes;

	// default constructor
	fiboHeap()
	{
		min = 0;
		numNodes = 0;
	}

	// continues remove min until all nodes are removed
	~fiboHeap() 
	{
		fiboNode* temp;
		while (min != 0)
		{
			temp = remove_min();
			delete temp;
		}
	}

	/* To insert a new node into the heap
	 * @param newNode is the node to be inserted
	 * each node is inserted to the right of the min element
	 */
	void insert(fiboNode* newNode)
	{
		if (newNode == 0) return;

		// if the heap is empty, the new node becomes the root
		if (min == 0)
		{
			min = newNode->left = newNode->right = newNode;
		}

		else {
			// inserting the new node between min and min->right
			newNode->left = min;
			newNode->right = min->right;

			// setting pointers to new node
			newNode->left->right = newNode;
			newNode->right->left = newNode;
		}

		// new node becomes min, if cost is less than min
		if (newNode->min_cost(min))
		{
			min = newNode;
		}

		// num of nodes increases
		numNodes++;
		newNode->parent = 0;
	}

	/* To meld two heaps
	 * @param newHeap gives the heap which has to be joined to the current one
	 * the joining happens with the merger of two circular lists at the root level
	 */
	void meld(fiboHeap* newHeap)
	{
		fiboNode *min1, *min2, *next1, *next2;
		if (newHeap == 0 || newHeap->min == 0)
		{
			return;
		}

		// we join the two heaps by cutting between min and its next node
		min1 = min;
		min2 = newHeap->min;
		next1 = min1->right;
		next2 = min2->right;

		// joining min nodes to next nodes on other heaps
		min1->right = next2;
		next2->left = min1;
		min2->right = next1;
		next1->left = min2;

		// updating min
		if (min2->min_cost(min1))
		{
			min = min2;
		}

		// deleting the other heap
		newHeap->min = 0;
		delete newHeap;
	}

	/* returns the min pointer
	 */
	fiboNode* get_min()
	{
		return min;
	}

	/* Removes all links of a node
	 * @param node is the node which has to be disconnected from the circular list
	 */
	fiboNode* remove_node(fiboNode* node)
	{
		node->left->right = node->right;
		node->right->left = node->left;
		node->left = node->right = 0;
		return node;
	}

	/* Remove min extracts the minimum element
	 * returns the min element node
	 * after the min is removed it does a pairwise combine of all the trees
	 * where trees of same degrees are combined
	 */
	fiboNode* remove_min()
	{
		fiboNode* result_min;
		fiboHeap* child_heap = 0;

		if ((result_min = get_min()) == 0) return 0;

		// removing the min and setting it to the next node
		min = result_min->right;
		result_min->right->left = result_min->left;
		result_min->left->right = result_min->right;
		result_min->right = result_min->left = 0;

		numNodes--;
		result_min->degree = 0;

		//we need to attach the child list to the root level list
		// if no child list, nothing to do
		if (result_min->child == 0)
		{
			if (min == result_min)
			{
				min = 0;
			}
		}

		// if there was only one node in the root level
		// then the min will be the child of the resulted min
		else if (min == result_min)
		{
			min = result_min->child;
		}
		
		// if min was different, the child list has to be merged with root level list
		// copying it to temp list and then melding with the root list
		else 
		{
			child_heap = new fiboHeap();
			child_heap->min = result_min->child;
		}

		// remove all pointers to the resulting min
		if (result_min->child != 0)
		{
			result_min->child->parent = 0;
		}
		result_min->child = result_min->parent = 0;

		// merging the child list with root level list
		if(child_heap)
		{
			meld(child_heap);
		}

		// need to do pairwise combine
		if (min != 0 && min->right != min)
		{
			pairwise_combine();
		}

		// updating the min after pairwise combine
		fiboNode* iter = min;
		do {
			if (iter->key < min->key)
			{
				min = iter;
			}
			iter = iter->right;
		}while (iter != min);

		// return the resulting min
		return result_min;
	}

	/* Printing the heap
	 * only for testing purposes
	 */
	void print()
	{
		fiboNode* temp = min;
		if (min == 0)
		{
			cout << "Empty heap " << endl;
		}
		else {
			cout << "Num nodes: "<< numNodes << endl;
			cout << "The heap "<< endl;
			do {
				cout << temp->key << " ";
				temp = temp->right;
			}while (temp != min);
			cout << endl;
		}
	}

	/* This combines all the trees of same degree into one tree
     * degree table is maintained which holds the degrees of trees traversed
	 */
	void pairwise_combine()
	{
		fiboNode *node1, *node2, *iter;

		// the size is log to base 2 of num of nodes
		int size = (ceil(log10(numNodes)/log10(1.62)));
		vector<fiboNode*> degree_table(size);
		
		for (int i = 0; i < size; i++)
		{
			degree_table.push_back(0);
		}

		// traverse through the root level nodes combining them
		iter = min;
		while (iter->marked != 1)
		{
			int curr_degree = iter->degree;
			// if no other node with same degree has been found
			if (degree_table[curr_degree] == 0)
			{
				degree_table[curr_degree] = iter;
				iter->marked = 1;
				if (iter->key < min->key)
				{
					min = iter;
				}
				iter = iter->right;
			}

			// if there already exists a node with same degree
			else
			{
				iter->marked = 0;
				node1 = degree_table[curr_degree];
				node2 = iter;

				// check which of the nodes has min cost and make it the root of the new tree
				if (node1->min_cost(node2))
				{
					node2 = remove_node(node2);
					iter = node1->insert_child(node2);
					degree_table[curr_degree] = 0;
					iter->marked = 0;
					min = iter;
				}
				else
				{
					node1 = remove_node(node1);
					iter = node2->insert_child(node1);
					degree_table[curr_degree] = 0;
					iter->marked = 0;
					min = iter;
				}
			}
		}
	}

	/* Decreases the key of a node
	 * does appropriate changes to the heap
	 * @param node is the node whose key is being decreased
	 * @param key is the new key to be set
	 */
	void decrease_key(fiboNode* node, int key)
	{
		if (key > node->key)
		{
			cout << " Invalid key.. value is greater than current key "<<endl;
		}
		node->key = key;
		fiboNode* parent = node->parent;
		if (parent != 0 && node->key < parent->key)
		{
			cut(node, parent);
			cascading_cut(parent);
		}
		if (node->min_cost(min))
		{
			min = node;
		}
	}

	/* Cuts the child from the parent and adds it to the root list
	 * also sets the child cut value to null and decreases degree of the parent
	 * @param node is the child node
	 * @param parent is the parent node
	 */
	void cut(fiboNode* node, fiboNode* parent)
	{
		if (parent->child == node)
		{
			// if node is the only child of the parent
			// remove it and set child pointer to null
			if (node->right == node && node->left == node)
			{
				parent->child = 0;
			}
			// if there are other children then assign pointers appropriately
			else
			{
				node->left->right = node->right;
				node->right->left = node->left;
				parent->child = node->right;
				node->left = node->right = 0;
			}
		}
		// if its somewhere in the list, then remove and set pointers appropriately
		else
		{
			node->left->right = node->right;
			node->right->left = node->left;
			node->left = node->right = 0;
		}
		// set parent pointer null and decrease parent's degree
		node->parent = 0;
		parent->degree = parent->degree - 1;

		// add the child to the root list
		// adding to the right of the min element
		node->left = min;
		node->right = min->right;

		// setting pointers to node
		node->left->right = node;
		node->right->left = node;
		node->child_cut = 0;
	}

	/* Does cascading cut recursively
	 * @param node is a parent of a node whose key was decreased and
	 * it was moved to the root list
	 */
	void cascading_cut(fiboNode* node)
	{
		fiboNode* parent = node->parent;
		if (parent != 0)
		{
			if (!node->child_cut)
			{
				node->child_cut = 1;
				return;
			}
			else
			{
				cut(node,parent);
				cascading_cut(parent);
			}
		}
		return;
	}

};
#endif /*FIBONACCI_H_*/
