Performance evaluation of Prim's algorithm using Binomial heaps, Fibonacci heaps and arrays.
===========================================================================================

Implemented Binomial heaps and Fibonacci heaps to compare the performance of Prim's Algorithm to generate the minimum spanning tree along with a simple array implementation. This also has random graph generation and implementation of BFS to confirm connectivity of generated graph. These graphs are stored using Adjacency lists.

To compile the code use "make" or "g++ mst.cc –o mst".

There are two ways of running the code:
1. Using a text file as input and generating a graph using the text file. The text file needs to have the following format: 
n m // The number of vertices and edges respectively in the first line
v1 v2 c1 // the edge (v1, v2) with cost c1
v2 v3 c2 // the edge (v2, v3) with cost c2
… // a total of m edges

$mst -s file-name // read the input from a file ‘file-name’ for simple scheme
$mst -f file-name // read the input from a file ‘file-name’ for f-heap scheme
$mst -fr file-name // read the input from a file 'file-name' for binomial heap scheme

2. Using the random graph generator.

$ mst –r n d // run in a random connected graph with n vertices and d% of density.
