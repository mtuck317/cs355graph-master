//----------------------------------------------------------------------
//Path struct for returning final path for use in front-end applications
//
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//----------------------------------------------------------------------

#ifndef PATH_H
#define PATH_H

struct path{
	int length;
	int* location;
	path():length(0),location(0){}
	path(int l){
		length = l;
		location = new int[length];
	}
	path(const path& source){
		length = source.length;
		location = new int[length];
		for(int i=0;i<length;i++)
			location[i] = source.location[i];
	}
	path& operator =(const path& source){
		delete[] location;
		length = source.length;
		location = new int[length];
		for(int i=0;i<length;i++)
			location[i] = source.location[i];
		return *this;
	}
	~path(){
		delete[] location;
		location = 0;
		length = 0;
	}
};

#endif
