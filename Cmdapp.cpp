//----------------------------------------------------------------------
//Command prompt application that uses our back-end algorithm.
//
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//----------------------------------------------------------------------

#include <iostream>
#include "map.h"

using namespace std;

//Digital Parkour Example Driver

int main() {
	
	std::cout << "Legend: \n";
	std::cout << "\tT = Traceur\n";
	std::cout << "\t* = Traceur's path\n";
	std::cout << "\tH = locked door / gate\n";
	std::cout << "\t- and I = Walls\n";
	std::cout << "\t'=' = fences and big walls\n\n";
	std::cout << "ENTER to continue";
	std::cin.ignore();
	std::cout << std::endl;
	
	
	//Building the Map
	
	//NOTE: There is currently a bug with non-proportional maps
	//		 Length and width must be equivalent. See map.h
	//		 for details on the bug.
	
	//Map generate with length X height
	int mapsize = 10;

	map parkourMap(mapsize, mapsize);
	
	//Generating building and obstacles on the map
	parkourMap.changeSymbol(3, 2, 'H'); //Locked Door
	parkourMap.InsertEdge(3, 1, 3, 2, 2);//Break through door
	parkourMap.changeSymbol(2, 2, '-'); //Wall
	parkourMap.InsertEdge(3, 2, 4, 2, 1000);//Impossible
	parkourMap.InsertEdge(3, 2, 2, 2, 1000);//Impossible
	parkourMap.InsertEdge(2, 1, 2, 2, 10);//Climbing a wall
	parkourMap.changeSymbol(4, 2, '-');//Wall
	parkourMap.InsertEdge(4, 1, 4, 2, 10);//Climbing a wall
	parkourMap.changeSymbol(5, 2, '-');//Wall
	parkourMap.InsertEdge(5, 1, 5, 2, 10);//Climbing a wall
	parkourMap.changeSymbol(1, 2, '-');//Wall
	parkourMap.InsertEdge(1, 1, 1, 2, 10);//Climbing a wall
	
	parkourMap.changeSymbol(3, 4, 'H'); //Locked Door
	parkourMap.InsertEdge(3, 4, 2, 4, 1000);//Impossible
	parkourMap.InsertEdge(3, 4, 4, 4, 1000);
	parkourMap.InsertEdge(3, 3, 3, 4, 2);//Break through door
	
	parkourMap.changeSymbol(1, 4, '-');//Interior Wall
	parkourMap.InsertEdge(0, 4, 1, 4, 20);//Break through wall
	
	parkourMap.changeSymbol(2, 4, '-');//Interior Wall
	parkourMap.InsertEdge(2, 3, 2, 4, 20);//Break through wall
	
	parkourMap.changeSymbol(4, 4, '-');//Interior Wall
	parkourMap.InsertEdge(4, 3, 4, 4, 20);//Break through wall
	
	parkourMap.changeSymbol(5, 4, '-');//Interior Wall
	parkourMap.InsertEdge(5, 3, 5, 4, 20);//Break through wall
	
	parkourMap.changeSymbol(1, 3, 'I');//Side Wall
	parkourMap.InsertEdge(0, 3, 1, 3, 10);//Climb left wall
	parkourMap.InsertEdge(2, 3, 1, 3, 20);//Break through wall from interior
	
	parkourMap.changeSymbol(5, 3, 'I');//Side Wall
	parkourMap.InsertEdge(6, 3, 5, 3, 10);//Climb wall exterior
	parkourMap.InsertEdge(4, 3, 5, 3, 20);//Break through wall from interior
	
	parkourMap.changeSymbol(2, 3, '.');//Floor
	parkourMap.changeSymbol(3, 3, '.');//Floor
	parkourMap.changeSymbol(4, 3, '.');//Floor
	
	parkourMap.changeSymbol(6, 3, '=');//Fence
	parkourMap.InsertEdge(6, 2, 6, 3, 5);//Climb Fence
	
	parkourMap.changeSymbol(7, 3, '=');//Fence
	parkourMap.InsertEdge(7, 2, 7, 3, 5);//Climb Fence
	
	parkourMap.changeSymbol(8, 3, '=');//Fence
	parkourMap.InsertEdge(8, 2, 8, 3, 5);//Climb Fence
	
	parkourMap.changeSymbol(9, 3, '=');//Fence
	parkourMap.InsertEdge(9, 2, 9, 3, 5);//Climb Fence
	
	parkourMap.changeSymbol(0, 7, '=');//Big Fence
	parkourMap.InsertEdge(0, 6, 0, 7, 10);//Climb Fence

	parkourMap.changeSymbol(1, 7, '=');//Big Fence
	parkourMap.InsertEdge(1, 6, 1, 7, 10);//Climb Fence
	
	parkourMap.changeSymbol(2, 7, '=');//Big Fence
	parkourMap.InsertEdge(2, 6, 2, 7, 10);//Climb Fence	

	parkourMap.changeSymbol(3, 7, '=');//Big Fence
	parkourMap.InsertEdge(3, 6, 3, 7, 10);//Climb Fence

	parkourMap.changeSymbol(4, 7, '=');//Big Fence
	parkourMap.InsertEdge(4, 6, 4, 7, 10);//Climb Fence

	parkourMap.changeSymbol(5, 7, 'H');//Big Gate
	parkourMap.InsertEdge(5, 7, 4, 7, 1000);//Impossible
	parkourMap.InsertEdge(5, 7, 6, 7, 1000);//Impossible
	parkourMap.InsertEdge(5, 6, 5, 7, 5);//Lockpick the gate

	parkourMap.changeSymbol(6, 7, '=');//Big Fence
	parkourMap.InsertEdge(6, 6, 6, 7, 10);//Climb Fence

	parkourMap.changeSymbol(7, 7, '=');//Big Fence
	parkourMap.InsertEdge(7, 6, 7, 7, 10);//Climb Fence

	parkourMap.changeSymbol(8, 7, '=');//Big Fence
	parkourMap.InsertEdge(8, 6, 8, 7, 10);//Climb Fence

	parkourMap.changeSymbol(9, 7, '=');//Big Fence
	parkourMap.InsertEdge(9, 6, 9, 7, 10);//Climb Fence
	
	int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
	
	parkourMap.printMap();
	
	cout << "Enter x and y coordinates between 0 and 10 for start point (ex. 4 0):" << endl;
		cin >> start_x >> start_y;
		
	cout << "Enter x and y coordinates between 0 and 10 for end point (ex. 2 10):" << endl;
		cin >> end_x >> end_y;
	
	cout << "Here is the shortest path:" << endl << endl;
	
	//Insert Path Here
	parkourMap.findPath(start_x, start_y, end_x, end_y);

	parkourMap.printMap();
	
	return 0;
}


