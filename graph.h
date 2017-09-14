//----------------------------------------------------------------------
//Graph Class
//
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//----------------------------------------------------------------------

#ifndef GRAPH_H
#define GRAPH_H
#include "node.h"
#include "path.h"
#include "binheap.h"

//---------------------------------------------------------------------
//Graph
//---------------------------------------------------------------------

class graph{
	private:
		node* nodes;//container for all nodes in graph
		binheap* heap;//choose a better name?
		void Copy(const graph& source);//Helper for copy constructor/assignment operator
		void ClearPath();//Resets nodes' prev and cost but not location
	protected:
		int** matrix;
		int size;   //nodes in graph
		node * NodeAccessor(int location);
	public:
		graph():size(0){};
		graph(int graph_size);
		graph(const graph& source);
		graph& operator=(const graph& source);
		~graph();
		path ShortestPath(int A, int B);
		void InsertEdge(int from, int to, int weight); //This also updates existing edges
		void ClearEdge(int from, int to);//removes a single matrix spot
		void ClearEdges();//resets entire matrix
};

void graph::Copy(const graph& source){
	size = source.size;
	nodes = new node[size];
	matrix = new int*[size];
	for(int i=0;i<size;i++){
		nodes[i].cost = -1;           
		nodes[i].prev = 0;
		nodes[i].location = i;
		matrix[i] = source.matrix[i];
	}
	heap = new binheap(size);
	*heap = *source.heap;
}

void graph::ClearPath(){
	for(int i=0;i<size;i++){
		nodes[i].cost = -1;
		nodes[i].prev = 0;
	}
}

//overloaded constructor
graph::graph(int graph_size){
	size = graph_size;
	nodes = new node[size];
	matrix = new int*[size];
	heap = new binheap(size);

	for(int i=0;i<size;i++){
		nodes[i].cost = -1;           
		nodes[i].prev = 0;
		nodes[i].location = i;
		matrix[i] = new int[size];
		for(int j=0;j<size;j++){
			matrix[i][j] = -1;
		}
	}
}

graph::graph(const graph& source){
	Copy(source);
}

graph& graph::operator=(const graph& source){
	delete nodes;
	delete matrix;
	delete heap;
	Copy(source);
	return *this;
}

graph::~graph(){
	delete nodes;
	delete matrix;
	delete heap;
	nodes = 0;
	matrix = 0;
	heap = 0;
}

//input: indeces of nodes in matrix for begin and end point of desired path
//output: for now, array of indeces could be set to object's "path" array for shortest path... maybe change to LinkedList?
path graph::ShortestPath(int start,int end){
	path returnPath;  //moved to top for return statement
	if(start < size && start >= 0 && end < size && end >= 0){
		nodes[start].cost = 0;//Setting start cost to 0 because: (1.) logically makes sense (2.) will keep final cost from being off by one (3.) marks the origin conveniently
		node* eyeball = &nodes[start];//Our eye starts at the start point of course
		heap->Insert(eyeball);
		while(!heap->empty() && &(nodes[end]) != heap->Min()){//min method will take a node as a parameter and determine if that node is the root
			
			eyeball = heap->Remove();//go to next item in the heap, and pop it off the heap    
			for(int k=0; k < size; k++){//Iterate through the matrix to find any connections
				
				if(matrix[eyeball->location][k] != -1){//if there's a connection from the eye to the indexed node. Two if statements are for clarity
					if(nodes[eyeball->location].cost + matrix[eyeball->location][k] < nodes[k].cost || nodes[k].cost == -1){//if the eye's distance from orgin + endgelength  less than what the node currently costs
						
							nodes[k].cost = nodes[eyeball->location].cost + matrix[eyeball->location][k];//Change cost of eye node to new total cost
							nodes[k].prev = eyeball;//So eye node will know who changed it
							heap->Insert(&nodes[k]);//Add this to our heap to become our eye at a later point
					}
				}
			}
		}
		
		int current = end;//Now we will trace path from end to beginning
		
		//make sure a path was actually found
		if(nodes[current].prev!=0){
			int* backtrack = new int[size];//This is where we'll store the final path
			int pathlen = 0;
			while(current != start){//when current is beginning
				backtrack[pathlen] = current;
				pathlen++;
				current = nodes[current].prev->location;
			}
			backtrack[pathlen] = start;//Don't forget to include the first node!
			pathlen++;
		
			path successPath(pathlen);
			returnPath = successPath;
			for(int i=0;i<pathlen;i++){
				returnPath.location[i] = backtrack[(pathlen-i-1)];
			}
		}
		else{
			path failPath(0);
			returnPath = failPath;
		}
		
		ClearPath();//Clear the path made from the graph
	}
	return returnPath;
}

void graph::InsertEdge(int from, int to, int weight){
	if(from >= 0 && from < size && to >= 0 && to < size && (weight > 0 || weight == -1)){ //zero is not allowed as a weight 
		matrix[from][to] = weight;
	}
}

node * graph::NodeAccessor(int location) {
	return &nodes[location];
}

#endif
