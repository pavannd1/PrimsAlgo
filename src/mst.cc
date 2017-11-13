#include <iostream>
#include <cstring>
#include <ctime>
#include <sstream>

#include "graphs.h"

using namespace std;

int main(int argc, char* argv[]) {
    // there are 3 types of running modes where they have
    // either 4 or 3 arguments
	if (argc != 4 && argc != 3) {
		cout << "\nUsage : mst -s <file_name> or mst -f <file_name> or mst -r num_vertices density_perc " << endl;
		exit(1);
	}

	// depending on the command line arguments do the necessary operations
	// random mode takes input of number of vertices and density of edges
	if (strcmp(argv[1],"-r") == 0) {
		clock_t start_simple;
        clock_t time_simple;
        clock_t start_fibo;
        clock_t time_fibo;
        clock_t start_bino;
        clock_t time_bino;

		// taking input from the command line arguments
		istringstream vec(argv[2]);
		int n;
		vec >> n;
		istringstream den(argv[3]);
		double d;
		den >> d;
		if (d > 100) {
			cout << "Invalid input for density.. Retry with a value less between 0 and 100" << endl;
			exit(1);
		}

		/* generating a random graph
		 * its connectivity is checked through BFS algorithm
		 * the process is repeated till we generate a connected graph
		 */
		Graphs* g;
		do {
			g = new Graphs();
			g->create_random_graph(n, d);
		} while(g->is_connected() != 1);

		cout << "Graph generated" << endl;

		// Simple array scheme
		start_simple = clock();
		g->prims(ARRAY);
		time_simple = clock() - start_simple;

		// Fibonacci scheme
		start_fibo = clock();
		g->prims(FIBONACCI);
		time_fibo = clock() - start_fibo;
		
		// Remove min scheme - Binomial heap
		start_bino = clock();
		g->prims(FIBO_REMOVE_MIN);
		time_bino = clock() - start_bino;

		cout << "Simple Scheme: " << time_simple/(CLOCKS_PER_SEC/1000) << " milliseconds\t( " << 1000*(time_simple/(CLOCKS_PER_SEC/1000)) << " microseconds) " << endl;
		cout << "Fibonacci Scheme: " << time_fibo/(CLOCKS_PER_SEC/1000) << " milliseconds\t( " << 1000*(time_fibo/(CLOCKS_PER_SEC/1000)) << " microseconds) " << endl;
		cout << "Binomial Scheme: " << time_bino/(CLOCKS_PER_SEC/1000) << " milliseconds\t( " << 1000*(time_bino/(CLOCKS_PER_SEC/1000)) << " microseconds) " << endl;

	} else if ((strcmp(argv[1], "-s") == 0) || (strcmp(argv[1], "-f") == 0) || (strcmp(argv[1], "-fr") == 0)) {
        // file input mode has two schemes, simple and fibonacci scheme
	    // -s simple scheme and -f fibonacci scheme
		Graphs* g;
		g = new Graphs();
		g->create_graph_from_file(argv[2]);
		if(g->is_connected() != 1) {
			cout << "Graph created from the file was not a connected graph.. Re-try another file.." << endl;
			exit(1);
		}

		// Running Prims for Simple array scheme
		if (strcmp(argv[1], "-s") == 0) {
			g->prims(ARRAY, UI);
		} else if (strcmp(argv[1], "-f") == 0) {
            // Running Prims for Fibonacci scheme
			g->prims(FIBONACCI, UI);
		} else if (strcmp(argv[1], "-fr") == 0) {
            // Running Prims for Fibonacci scheme using remove min method
			g->prims(FIBO_REMOVE_MIN, UI);
		}
	}
	return 0;
}
