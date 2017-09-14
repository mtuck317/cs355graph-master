//----------------------------------------------------------------------
//This is our HTMl web page implementation of our back-end algorithm.
//
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//----------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <fstream>
#include "binheap.h"
#include "graph.h"
#include "path.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <stdio.h> 
const int OBSTACLES = 9;
using namespace std;
template <typename T>
//got function from http://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
string to_string ( T Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }



int main(){
	srand (time(NULL));
	int left = 0;
	int top = 0;
	int id = 0;
	int weights[OBSTACLES]; 
	graph map(OBSTACLES);
	string person = "<img class=\"person\" src=\"images/person.jpg\">";
	string stairs =  "<img class=\"object\" src=\"images/stairs.png\">";
	string fence = "<img class=\"object\" src=\"images/fence.jpg\">";
	string grass = "<img class=\"object\" src=\"images/grass.png\">";
	string line;
	string connections[OBSTACLES];
	string html = "";
	fstream myfile;
	fstream start;
	fstream end;
	fstream end2;
	myfile.open("Webapp.html");
	start.open("html/start.html");
	
	while ( getline (start,line) )
    {
    	html += line+"\n";
    }
    start.close();
	
	for(id = 0; id < OBSTACLES; id++){
		connections[id] = "";
	}
	
	weights[0] = 6;
	weights[1] = 7;
	weights[2] = 6;
	weights[3] = 6;
	weights[4] = 1;
	weights[5] = 1;
	weights[6] = 7;
	weights[7] = 1;
	weights[8] = 1; 
	
	
	
	map.InsertEdge(0,1,weights[1]);
	connections[0] += to_string(1)+",";
	map.InsertEdge(0,2,weights[2]);
	connections[0] += to_string(2)+",";
	map.InsertEdge(0,3,weights[3]);
	connections[0] += to_string(3)+",";
	map.InsertEdge(1,6,weights[6]);
	connections[1] += to_string(6)+",";
	map.InsertEdge(2,4,weights[4]);
	connections[2] += to_string(4)+",";
	map.InsertEdge(3,5,weights[5]);
	connections[3] += to_string(5)+",";
	map.InsertEdge(4,5,weights[5]);
	connections[4] += to_string(5)+",";
	map.InsertEdge(4,6,weights[6]);
	connections[4] += to_string(6)+",";
	map.InsertEdge(5,7,weights[7]);
	connections[5] += to_string(7)+",";
	map.InsertEdge(6,8,weights[8]);
	connections[6] += to_string(8)+",";
	map.InsertEdge(7,8,weights[8]);
	connections[7] += to_string(8)+",";
	
	
	for(id = 0; id <OBSTACLES; id++){
		
		switch(id){
			case 0: left=10; top=10; break;
			case 1: left = 240; top =10; break;
			case 2: left = 240; top = 120; break;
			case 3: left = 240; top = 240; break;
			case 4: left = 480; top = 120; break;
			case 5: left = 480; top = 240; break;
			case 6: left = 720; top = 10; break;
			case 7: left = 720; top = 240; break;
			case 8: left = 960; top = 120; break;
		}
		
		string cur ="";
		cur = "<div id=\""+to_string(id)+"\" class=\"item\" connection=\""+connections[id]+"\" style=\"left:"+to_string(left)+"px; top:"+to_string(top)+"px;\">"+to_string(id);
		html +=cur;
		if(weights[id] == 1){
			html += grass;
		}
		else if(weights[id] == 6){
			html += fence;
		}
		else if(weights[id] == 7){
			html += stairs;
		}
		html += person;
		html += "</div>\n";
		

		
	}
	
	end.open("html/end.html");
	while ( getline (end,line) )
    {
    	html += line+"\n";
    }
    end.close();
    
    int begin;
    int finish;
    
    cout<<"Where would you like to start?\n";
    cin>>begin;
    cout<<"Where would you like to  finish?\n";
    cin>>finish;
    path route = map.ShortestPath(begin,finish);
    string routstr = "var path = [";
    for(int i =0; i<route.length;i++){
    	if(i != route.length-1)
    		routstr += "\""+to_string(route.location[i])+"\",";
    	else
    		routstr += "\""+to_string(route.location[i])+"\"";
    }
    html += routstr;
    html+= "];\n";
    html +="var end =\""+to_string(finish)+"\";\n";
    
    end2.open("html/end2.html");
	while ( getline (end2,line) )
    {
    	html += line+"\n";
    }
    end2.close();
    
    
    
	
	myfile<<html;
	myfile.close();
	cin.ignore();
	const string opendoc = "open.bat";
	system(opendoc.c_str());
	
	
	
	
	
	return 0;
}
