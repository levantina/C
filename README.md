# C PROGRAMMING LANGUAGE

These files are written in C, some of them are exercises. The group of files in the “KCores" folder regard my Bachelor's Degree thesis, that is about a k-cores decomposition of a real graph vs random graph.

- awk ->
	Some code written in awk, useful to do some quick statistical analysis and 	data transformation.

- binaryTree ->
	It builds and analyzes a binary tree graph.

- heapfile.c ->
	Data Struct HEAP, a tree-based data structure. The heap is one maximally efficient implementation of an abstract data type called a priority queue.

- isingModel ->
	A group of programs and functions that allow to perform a simulation of an Ising Model (spins on a two-dimentional lattice) evolution in time, setting initial conditions such as temperature, and spin configuration. It 	uses the metropolis algorithm.

- kCores ->
	A group of function that perform and analyze the k-core decomposition of an undirected and not weighted graph. It uses the k-pruning algorithm, an iteration based on a recursive function.

    graforandN.c -> 
	It builds N random graph, given the integer value of <Number of verices>, <Average degree> and <N> (the number of iterations). It prints on file the 	result, with the standard data form of an adjacency list.
	
    kcoresfile.c -> 
        It reads a graph (as adjacency list) and it performs the k-cores decomposition, using the k-pruning algorithm (recursive). It can evaluate the degree distribution if requested. It prints on file the results.
        
    kcoresrand.c ->
        It reads from file the Dimes data that is a mapping of Internet. It produces a 	randomization of the edges of the graph and performs the k-cores decomposition (as before).

- quicksortvale.c ->
	A quick algorithm for sorting data.

- randomGraph.c -> 
	It creates a random Graph, given the number of vertices and a random seed. 	It gives also 	the number of clusters created.

- treeGraph.c -> 
	It creates a tree graph and it prints the result on file.

- weightedGraph -> 
	A group of programs that analyze a weighted graph in terms of shortest 	path.

    grafo_pesato6.c -> 
	It takes an undirected graph from file and two vertices, and it evaluates the shortest path between them.

    grafo2L.c ->
    	It creates a two-dimentional undirected graph, within a surface of LxL length units, that is a two-dimentional lattice. L is an initial value given from shell. It prints on file the resulting graph with the standard data format (adjacency list).
    	
    megagrafo.c -> 
	It allows to perform a data analysis, using the same things in grafo_pesato6.c, to iterate the creation of N         two-dimentional graphs, and it prints on file each time the shortest path.
	
    medie.awk -> 
	It evaluates the average (medie = averages) values of weights and number 
	of steps, for each value of L (the number of vertices in the lattice).


