//----------------------------------------------------------------------
//Map class which inherits from Graph class for command prompt
//implementation.
//
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//----------------------------------------------------------------------

#ifndef map_h
#define map_h
#include "graph.h"
#include <iostream>

//=============================================================================
// Class: Map - [Derived from Graph]
// Description: -Intro-
//				This class generates a grid map upon instantiation of a user
//				defined width and height. The map is a graph of size
//				width * height. Each grid square on the map is a node pointer 
//				that points back to a specific node in the graph's node
//				container.
//
//				-Map grid connections-
//				When a map is generated, it will connect all grid squares to
//				their adjacent squares. Currently, each grid will be connected
//				to its left, right, and above. This is so that the direction
//				of the map will generally flow from the top to bottom.
//
//				-Movement-
//				A cursor may navigate through the map by either going left,
//				right, or up. Diagonal movement is not supported at this time,
//				but it would not be difficult to add.
//
//				-Undirected Map-
//				I have currently not tested this map with undirected properties
//				To implement this, the constructor would need to be edited so
//				that all grid squares connect to every adjacent grid square.
//				More interesting maps could be built with a setup like this,
//				but I currently do not have time to implement.
//
// TODO: 1. Segfaults when initializing a map where length and width are not equivalent 
//	 2. Build a destructor
//	 3. Build more accessors as needed
//=============================================================================
class map : public graph{
	public:
		//Constructors
		map();
		map(int mapWidth, int mapHeight);
		
		//Mutators
		void InsertEdge(int originX, int originY, int destX, int destY, int weight);
		void changeSymbol(int xVal, int yVal, char newSymbol);
		void findPath(int originX, int originY, int destX, int destY);
		
		//Accessors
		void printMap() const;

	private:
		node ***nodeMap;
		int width;
		int height;
		char** symbol;
};
//=================================================================================================

//=================================================================================================
// Methods
//=================================================================================================

//=================================================================================================
// Class: Map
// Function: Default Constructor
// Parameters: (N/A)
// Description: Constructs an empty map
// TODO:
//=================================================================================================
map::map() : width(0), height(0) {}
//=================================================================================================


//=================================================================================================
// Class: Map
// Function: Overloaded Constructor
// Parameters: (N/A)
// Description: 
// TODO: Segfaults when building a map of non-proportional size. Length and width must be the same
//		 or else there will be segfaults. This needs to be fixed.
//=================================================================================================
map::map(int mapWidth, int mapHeight) : graph(mapWidth * mapHeight) {
	width = mapWidth;
	height = mapHeight;
	
	nodeMap = new node**[width];
	symbol = new char*[width];
	for (int i = 0; i < height; i ++){
		nodeMap[i] = new node*[height];	
		symbol[i] = new char[height];
	}
	int nodeIndex = 0;
	for (int w = 0; w < width; w++) {
		for (int h = 0; h < height; h++) {
			nodeMap[w][h] = NodeAccessor(nodeIndex);
			nodeMap[w][h]->location = nodeIndex;
			nodeIndex++;
			symbol[w][h] = ' '; //Default starting symbol - Feel free to change
		}
	}
	//Generating edges to connect all grids together
	//Grids are connected to their left, right, and above grids
	for (int h = 0; h < height; h++) {
		for (int w = width - 1; w >= 0; w--) {
			if(w+1 < width)
				InsertEdge(w, h, w+1, h, 1);//Connect to right node
			if(w-1 >= 0)
				InsertEdge(w, h, w-1, h, 1);//Connect to left node
			if(h+1 < height)
				InsertEdge(w, h, w, h+1, 1);//Connect to above node
			if(h-1 >= 0)
				InsertEdge(w, h, w, h-1, 1);//Connect to below node
		}
	}
}
//=================================================================================================


//=================================================================================================
// Class: Map
// Function: Prints the map
// Parameters: (N/A)
// Description: The print statement is inverted like this so that the graph will print out in
//				a more understandable format.
//				Ex:
//				X X X
//				X X X
//				X X X
//				The bottom center X's coordinates will be (0, 1)
//				This was originally inverted the other way so that (0, 0) would have been the 
//				top-left. I feel like this implementation is more clear.
// TODO: None
//=================================================================================================
void map::printMap() const {
	for (int h = height - 1; h >= 0; h--) {
		for (int w = 0; w < width; w++) {
			std::cout << symbol[w][h];
		}
		std::cout << std::endl;
	}
}
//=================================================================================================

//=================================================================================================
// Class: Map
// Function: InsertEdge [Overridden]
// Parameters: (int originX, int originY, int destX, destY, int weight)
// Description: In the map InsertEdge, simply pass the x,y coordinates of of the edge origin
//				and the x,y coordinates of the edge destination. The final parameter is the
//				weight that will be assigned to that edge. By default, all edges that are generated
//				with the map have a weight of one. Simply increase the weight to build obstacles
//				or other obstructions.
//
// TODO: None
//=================================================================================================
void map::InsertEdge(int originX, int originY, int destX, int destY, int weight){
	if(nodeMap[originX][originY]->location >= 0 
	   && nodeMap[originX][originY]->location < size 
	   && nodeMap[destX][destY]->location >= 0 
	   && nodeMap[destX][destY]->location < size && (weight > 0 || weight == -1)){ 
	   												//zero is not allowed as a weight 
	       matrix[nodeMap[originX][originY]->location][nodeMap[destX][destY]->location] = weight;
	}      //matrix[from][to]
}
//=================================================================================================


//=================================================================================================
// Class: Map
// Function: findPath
// Parameters: (int originX, int originY, int destX, destY)
// Description:
// TODO: None
//=================================================================================================
void map::findPath(int originX, int originY, int destX, int destY) {
	path route = ShortestPath(nodeMap[originX][originY]->location,
	                                nodeMap[destX][destY]->location);
	std::cout << "NOTE: Press ENTER to move the traceur" << std::endl << std::endl;
	printMap();
	std::cin.ignore();
	std::cout << std::endl;
	for (int i = 0; i < route.length; i++) {
		changeSymbol(NodeAccessor(route.location[i])->location/height,
					 NodeAccessor(route.location[i])->location%width,
					 'T' //Default traceur symbol - Feel free to change
					 );
		if (i > 0) {
			changeSymbol(NodeAccessor(route.location[i - 1])->location/height,
						 NodeAccessor(route.location[i - 1])->location%width,
					     '*' //Default traceur symbol - Feel free to change
					    );
		}
		printMap();
		std::cout << std::endl;
		std::cin.ignore(); 
	}
}
//=================================================================================================


//=================================================================================================
// Class: Map
// Function: changeSymbol
// Parameters: (int xVal, int yVal, char newSymbol)
// Description: Changes the symbol of an X, Y coordinate on the map. Symbols are currently
//				implemented as char's. Feel free to upgrade this if you have a better idea.
//				Symbols are purely arbitrary and allow the user to build a map however they like.
// TODO: None
//=================================================================================================
void map::changeSymbol(int xVal, int yVal, char newSymbol) {
	symbol[xVal][yVal] = newSymbol;
}
//=================================================================================================

#endif
