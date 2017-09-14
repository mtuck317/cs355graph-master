#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {35,37,39,41}; //connect to row pinouts 
byte colPins[COLS] = {43,45,47,49}; //connect to column pinouts

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char key=NO_KEY;

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

const uint16_t building = matrix.Color333(0,6,2);//10
const uint16_t fence = matrix.Color333(5,0,7);//2
const uint16_t water = matrix.Color333(0,3,6);//4
const uint16_t traceur = matrix.Color333(7,2,0);//
const uint16_t traversed = matrix.Color333(4,4,0);//
const uint16_t checked = matrix.Color333(1,1,1);//
const uint16_t blank = matrix.Color333(0,0,0);//
const uint16_t start_c = matrix.Color333(5,7,0);
const uint16_t end_c = matrix.Color333(5,0,0);

const unsigned char building_w = 20;
const unsigned char fence_w = 5;
const unsigned char water_w = 100;

const unsigned short int pause = 50;
	
const unsigned short int mapsize = 16;
const unsigned short int mapsquare = mapsize*mapsize;

unsigned char weight[mapsquare];
unsigned char cost[mapsquare];
bool prev1[mapsquare];
bool prev2[mapsquare];
bool queue[mapsquare];
char current_map = 0;
unsigned char start;
unsigned char end;

const unsigned char mapcount =3;

unsigned short int coord(unsigned short int x, unsigned short int y){
	return x*mapsize+y;
}

unsigned short int getX(unsigned short int crd){
	return crd/mapsize;
}

unsigned short int getY(unsigned short int crd){
	return crd%mapsize;
}

void draw4(unsigned short int x, unsigned short int y, uint16_t color){
	matrix.drawPixel(2*x,2*y,color);
	matrix.drawPixel(2*x+1,2*y,color);
	matrix.drawPixel(2*x,2*y+1,color);
	matrix.drawPixel(2*x+1,2*y+1,color);
}

bool Press(char button){
	bool result = false;
	while(digitalRead(button) == LOW){
		result = true;
	}
	delay(100);
	return result;
}

void resetWeights(){
	for(unsigned short int i=0;i<mapsquare;i++){
		weight[i]=1;
	}
}

void setup() {
	matrix.begin();
	//Serial.begin(9600);
	//pinMode(button1, INPUT);
}

void loop() {
	unsigned char w;
	
	//initialize vars
	for(unsigned short int x=0;x<mapsize;x++){
		for(unsigned short int y=0;y<mapsize;y++){
			weight[coord(x,y)] = 1;
			cost[coord(x,y)] = mapsquare-1;//represents infinity
			//previous[coord(x,y)] = 0;
			prev1[coord(x,y)] = 0;
			prev2[coord(x,y)] = 0;
			queue[coord(x,y)] = 0;
		}
	}
	
	//------------------------------------------------------------------
	
	key = NO_KEY;
	while(key != '*'){
	key = keypad.getKey();
	
	if(key=='6'){
		current_map=(current_map+1)%mapcount;
		matrix.fillScreen(blank);
		resetWeights();
	}
	if(key=='4'){
		if(current_map==0)
			current_map=mapcount-1;
		else
			current_map--;
		matrix.fillScreen(blank);
		resetWeights();
	}
	//------------------------------------------------------------------
	//current_map 1
	if(current_map==0){
		start = coord(0,0);
		end = coord(15,15);
		
		//C
		for(unsigned short int i=4;i<=5;i++){
			weight[coord(i,2)] = water_w;
			weight[coord(i,6)] = water_w;
		}
		for(unsigned short int i=2;i<=6;i++){
			weight[coord(3,i)] = water_w;
		}
		//.
		weight[coord(7,6)] = water_w;
		//S
		for(unsigned short int i=9;i<=11;i++){
			weight[coord(i,2)] = water_w;
			weight[coord(i,4)] = water_w;
			weight[coord(i,6)] = water_w;
		}
		weight[coord(9,3)] = water_w;
		weight[coord(11,5)] = water_w;
		//.
		weight[coord(13,6)] = water_w;
		//3
		for(unsigned short int i=3;i<=5;i++){
			weight[coord(i,9)] = water_w;
			weight[coord(i,11)] = water_w;
			weight[coord(i,13)] = water_w;
		}
		weight[coord(5,10)] = water_w;
		weight[coord(5,12)] = water_w;
		//5
		for(unsigned short int i=7;i<=9;i++){
			weight[coord(i,9)] = water_w;
			weight[coord(i,11)] = water_w;
			weight[coord(i,13)] = water_w;
		}
		weight[coord(7,10)] = water_w;
		weight[coord(9,12)] = water_w;
		//5
		for(unsigned short int i=11;i<=13;i++){
			weight[coord(i,9)] = water_w;
			weight[coord(i,11)] = water_w;
			weight[coord(i,13)] = water_w;
		}
		weight[coord(11,10)] = water_w;
		weight[coord(13,12)] = water_w;
	}
	//------------------------------------------------------------------
	//map 2
	else if(current_map==1){
		//water
		for(unsigned short int i=9;i<=10;i++){
			weight[coord(i,12)] = water_w;
			weight[coord(i,13)] = water_w;
			weight[coord(i,14)] = water_w;
			weight[coord(i,15)] = water_w;
		}
		for(unsigned short int i=11;i<=12;i++){
			weight[coord(i,12)] = water_w;
		}
		for(unsigned short int i=13;i<=14;i++){
			weight[coord(i,12)] = water_w;
			weight[coord(i,13)] = water_w;
			weight[coord(i,14)] = water_w;
		}
		for(unsigned short int i=9;i<=10;i++){
			weight[coord(14,i)] = water_w;
		}
		for(unsigned short int i=2;i<=4;i++){
			weight[coord(13,i)] = water_w;
		}
		for(unsigned short int i=6;i<=7;i++){
			weight[coord(i,1)] = water_w;
		}
		weight[coord(15,10)] = water_w;
		weight[coord(4,13)] = water_w;
		weight[coord(1,0)] = water_w;
		weight[coord(2,3)] = water_w;
		weight[coord(1,4)] = water_w;
		//fence
		for(unsigned short int i=3;i<=11;i++){
			weight[coord(i,3)] = fence_w;
			weight[coord(i,11)] = fence_w;
		}
		for(unsigned short int i=4;i<=10;i++){
			weight[coord(3,i)] = fence_w;
		}
		for(unsigned short int i=4;i<=6;i++){
			weight[coord(11,i)] = fence_w;
			weight[coord(11,i+4)] = fence_w;
		}
		//Buildings
		for(unsigned short int i=6;i<=8;i++){
			weight[coord(i,5)] = building_w;
			weight[coord(i,6)] = building_w;
		}
		for(unsigned short int i=5;i<=6;i++){
			weight[coord(i,8)] = building_w;
			weight[coord(i,9)] = building_w;
		}
		for(unsigned short int i=8;i<=9;i++){
			weight[coord(i,8)] = building_w;
			weight[coord(i,9)] = building_w;
		}
	}
	
	//------------------------------------------------------------------
	//current_map 3
	else if(current_map==2){
		start = coord(15,0);
		end = coord(3,3);
		
		for(unsigned short int i=1;i<=6;i++){
			weight[coord(1,i)] = water_w;
		}
		for(unsigned short int i=3;i<=4;i++){
			weight[coord(i,0)] = water_w;
		}
		for(unsigned short int i=0;i<=14;i++){
			weight[coord(8,i)] = water_w;
		}
		for(unsigned short int i=10;i<=11;i++){
			weight[coord(i,1)] = water_w;
		}
		for(unsigned short int i=3;i<=4;i++){
			weight[coord(10,i)] = water_w;
		}
		for(unsigned short int i=0;i<=7;i++){
			weight[coord(12,i)] = water_w;
		}
		for(unsigned short int i=1;i<=6;i++){
			weight[coord(6,i)] = water_w;
		}
		for(unsigned short int i=1;i<=2;i++){
			weight[coord(14,i)] = water_w;
		}
		for(unsigned short int i=2;i<=4;i++){
			weight[coord(i,2)] = water_w;
		}
		weight[coord(14,2)] = water_w;
		weight[coord(15,1)] = water_w;
		weight[coord(4,3)] = water_w;
		for(unsigned short int i=3;i<=4;i++){
			weight[coord(i,4)] = water_w;
		}
		weight[coord(7,4)] = water_w;
		weight[coord(9,4)] = water_w;
		for(unsigned short int i=13;i<=14;i++){
			weight[coord(i,4)] = water_w;
		}
		for(unsigned short int i=3;i<=5;i++){
			weight[coord(i,6)] = water_w;
		}
		for(unsigned short int i=10;i<=11;i++){
			weight[coord(i,6)] = water_w;
		}
		weight[coord(0,8)] = water_w;
		for(unsigned short int i=6;i<=9;i++){
			weight[coord(2,i)] = water_w;
		}
		for(unsigned short int i=4;i<=7;i++){
			weight[coord(i,8)] = water_w;
		}
		weight[coord(10,8)] = water_w;
		for(unsigned short int i=13;i<=14;i++){
			weight[coord(i,8)] = water_w;
		}
		for(unsigned short int i=10;i<=11;i++){
			weight[coord(i,9)] = water_w;
		}
		for(unsigned short int i=1;i<=6;i++){
			weight[coord(i,10)] = water_w;
		}
		for(unsigned short int i=10;i<=15;i++){
			weight[coord(i,10)] = water_w;
		}
		for(unsigned short int i=11;i<=14;i++){
			weight[coord(1,i)] = water_w;
			weight[coord(6,i)] = water_w;
		}
		weight[coord(5,11)] = water_w;
		for(unsigned short int i=12;i<=15;i++){
			weight[coord(3,i)] = water_w;
		}
		for(unsigned short int i=9;i<=14;i++){
			weight[coord(i,12)] = water_w;
		}
		weight[coord(4,13)] = water_w;
		weight[coord(4,15)] = water_w;
		for(unsigned short int i=10;i<=11;i++){
			weight[coord(i,14)] = water_w;
		}
		weight[coord(12,15)] = water_w;
		weight[coord(13,13)] = water_w;
		weight[coord(14,14)] = water_w;
	}
	
	//Print obstacles
	for(unsigned short int i=0;i<mapsize;i++){
		for(unsigned short int j=0;j<mapsize;j++){
			w = weight[coord(i,j)];
			if(w==building_w)
				draw4(i,j,building);
			else if(w==fence_w)
				draw4(i,j,fence);
			else if(w==water_w)
				draw4(i,j,water);
		}
	}
	}
	
	draw4(getX(start),getY(start),start_c);
	draw4(getX(end),getY(end),end_c);
	//------------------------------------------------------------------
	
	key = NO_KEY;
	while(key != '*'){
		key = keypad.getKey();
		
		w = weight[start];
		if(w==building_w)
			draw4(getX(start),getY(start),building);
		else if(w==fence_w)
			draw4(getX(start),getY(start),fence);
		else if(w==water_w)
			draw4(getX(start),getY(start),water);
		else
			draw4(getX(start),getY(start),blank);
				
		if(key=='2'){
			if(start%mapsize==0)
				start+=mapsize-1;
			else
				start--;
		}
		else if(key=='6')
			start=(start+mapsize)%mapsquare;
		else if(key=='4'){
			if(start/mapsize==0)
				start=mapsquare-mapsize+start;
			else
				start-=mapsize;
		}
		else if(key=='8'){
			if(start%mapsize==mapsize-1)
				start=start-mapsize+1;
			else
				start++;
		}
		draw4(getX(start),getY(start),start_c);
	}
	
	key = NO_KEY;
	while(key != '*'){
		if(key!=NO_KEY) Serial.println(key);
		key = keypad.getKey();
		
		w = weight[end];
		if(w==building_w)
			draw4(getX(end),getY(end),building);
		else if(w==fence_w)
			draw4(getX(end),getY(end),fence);
		else if(w==water_w)
			draw4(getX(end),getY(end),water);
		else
			draw4(getX(end),getY(end),blank);
				
		if(key=='2'){
			if(end%mapsize==0)
				end+=mapsize-1;
			else
				end--;
		}
		else if(key=='6')
			end=(end+mapsize)%mapsquare;
		else if(key=='4'){
			if(end/mapsize==0)
				end=mapsquare-mapsize+end;
			else
				end-=mapsize;
		}
		else if(key=='8'){
			if(end%mapsize==mapsize-1)
				end=end-mapsize+1;
			else
				end++;
		}
		draw4(getX(end),getY(end),end_c);
	}
	//------------------------------------------------------------------
	
	bool more = true;
	bool found = false;
	unsigned short int min;
	unsigned short int eyeball = start;
	unsigned short int neighbor;
	cost[start] = 0;
	
	while(more&&!found){
		
		if(weight[eyeball]==1)
			draw4(getX(eyeball),getY(eyeball),checked);
		delay(pause);
		
		if(eyeball == end){
			found = true;
		}
		
		neighbor = eyeball+mapsize;//check forward
		if(neighbor < mapsquare){
			if((cost[eyeball]+weight[neighbor]) < cost[neighbor]){
				queue[neighbor] = 1;
				cost[neighbor] = cost[eyeball] + weight[neighbor];
				//previous[neighbor] = eyeball;
				prev1[neighbor] = 0;
				prev2[neighbor] = 0;
			}
		}
		
		neighbor = eyeball+1;//check right
		if(neighbor/mapsize == eyeball/mapsize){
			if((cost[eyeball]+weight[neighbor]) < cost[neighbor]){
				queue[neighbor] = 1;
				cost[neighbor] = cost[eyeball] + weight[neighbor];
				//previous[neighbor] = eyeball;
				prev1[neighbor] = 0;
				prev2[neighbor] = 1;
			}
		}
		
		neighbor = eyeball-1;//check left
		if((neighbor/mapsize == eyeball/mapsize) && neighbor >= 0){
			if((cost[eyeball]+weight[neighbor]) < cost[neighbor]){
				queue[neighbor] = 1;
				cost[neighbor] = cost[eyeball] + weight[neighbor];
				//previous[neighbor] = eyeball;
				prev1[neighbor] = 1;
				prev2[neighbor] = 0;
			}
		}
		
		neighbor = eyeball-mapsize;//check backward
		if(neighbor >= 0){
			if((cost[eyeball]+weight[neighbor]) < cost[neighbor]){
				queue[neighbor] = 1;
				cost[neighbor] = cost[eyeball] + weight[neighbor];
				//previous[neighbor] = eyeball;
				prev1[neighbor] = 1;
				prev2[neighbor] = 1;
			}
		}
		
		more = false;
		min=end;
		for(unsigned short int i=0;i<mapsquare;i++){
			if(queue[i] == 1){
				more = true;
				if(cost[i] < cost[min])
					min = i;
			}
		}
		eyeball = min;
		queue[eyeball] = 0;
	}
	matrix.fillScreen(blank);
	
	//Print obstacles
	for(unsigned short int i=0;i<mapsize;i++){
		for(unsigned short int j=0;j<mapsize;j++){
			w = weight[coord(i,j)];
			if(w==building_w)
				draw4(i,j,building);
			else if(w==fence_w)
				draw4(i,j,fence);
			else if(w==water_w)
				draw4(i,j,water);
		}
	}
	
	unsigned short int backtrack = end;
	
	draw4(getX(backtrack),getY(backtrack),end_c);

	unsigned short int i=0;
	
	while(backtrack != start){
		if(prev1[backtrack]==0 && prev2[backtrack]==0)
			backtrack = backtrack - mapsize;
		else if(prev1[backtrack]==0 && prev2[backtrack]==1)
			backtrack = backtrack - 1;
		else if(prev1[backtrack]==1 && prev2[backtrack]==0)
			backtrack = backtrack + 1;
		else if(prev1[backtrack]==1 && prev2[backtrack]==1)
			backtrack = backtrack + mapsize;
		//backtrack = previous[backtrack];
		draw4(getX(backtrack),getY(backtrack),traceur);
	}
	
	draw4(getX(backtrack),getY(backtrack),start_c);
	
	key = NO_KEY;
	while(key == NO_KEY){
		key = keypad.getKey();
	}
	
	matrix.fillScreen(blank);
	
	for(unsigned short int i=0;i<mapsquare;i++){
		weight[i] = 1;
	}
	
	if(current_map==0)
		current_map=1;
	else
		current_map=0;

}
