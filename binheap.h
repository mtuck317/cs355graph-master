//----------------------------------------------------------------------
//Binary Heap Class
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//
//----------------------------------------------------------------------

#ifndef BINHEAP_H
#define BINHEAP_H
#include "node.h"

class binheap {
    private:
		//the heap:
        //double asterisk because this is an array of pointers
		//(pointer to pointers)
        node** heap;
        int size;//size of heap
        int last;//index of last item in heap
        void SwapNode(int i, int j);
        void Copy(const binheap& source);//Helper for copy constructor/assignment operator
    public:
		binheap():heap(0),size(0),last(-1){};
		binheap(int heap_size);
		binheap(const binheap& source);
		binheap& operator=(const binheap& source);
		~binheap();
        void Insert(node* n);
        node* Remove();
        node* Min()const;
        bool empty();
};

//swaps nodes in heap at given indeces
void binheap::SwapNode(int i, int j){
	node* temp = heap[i];
	heap[i] = heap [j];
	heap[j] = temp;
}

void binheap::Copy(const binheap& source){
	size = source.size;
	last = source.last;
	heap = new node*[size];
	for(int i=0;i<size;i++){
		heap[i] = source.heap[i];
	}
}

binheap::binheap(const binheap& source){
	Copy(source);
}

binheap& binheap::operator=(const binheap& source){
	delete[] heap;
	Copy(source);
	return *this;
}

//overloaded constructor
binheap::binheap(int heap_size){
	if (heap_size < 0){
		heap = 0;
		size = 0;
	}
	else {
		size = heap_size;
		heap = new node*[size];
	}
	last = -1;
}

binheap::~binheap(){
	delete[] heap;
	heap = 0;
}

//returns item at top of heap
node* binheap::Min()const{
    if(heap && size > 0)
        return heap[0];
    else
		return 0;
}

//inserts an item into heap, maintaining heap property (parent <= children)
void binheap::Insert(node* n){
	bool done = false;
	int current = last + 1;//item will start at bottom of heap
	int parent = 0;//keeps track of index of parent node
	//1. start by inserting item at bottom of heap
	heap[current] = n;//inserts item at bottom of heap
	last++;//update location of last item
	//2. Swap with parent until it is larger than parent
	//or it becomes root (current==0)
	while(current > 0 && !done){
		//identify parent
		if(current%2==0)//if index is even, this is a right child or root
			parent = (current-2)/2;
		else//if index is odd
			parent = (current-1)/2;
		if(heap[current]->cost<heap[parent]->cost){//if item is smaller than parent
			SwapNode(current,parent);//swap values in item and parent
			current = parent;//now our item is at parent spot, so we need to keep up with it
		}
		else//item is greater than or equal to parent (what we want)
			done = true;//fall out of the loop
	}
}

node* binheap::Remove(){
	node* result = 0;
	if(last>=0){
		bool done = false;
		result = heap[0];//pass node pointer to variable to be returned
		heap[0] = heap[last];//Place last item at top of heap
		heap[last] = 0;//Remove the last item from former location
		last--;//Now last item is one lower in heap
		int current = 0;//keeps track of the location of node we are moving
		int left = 0;//var for left child
		int right = 0;//var for right child
		int minchild = 0;//temp variable for comparing children
		while(!done){
			left = current*2+1;
			right = current*2+2;
			if(left <= last){//if it has a left child
				if(right <= last){//if it has a right child
					if(heap[left]->cost < heap[right]->cost)//find which child is smaller
						minchild = left;
					else
						minchild = right;
					if(heap[minchild]->cost < heap[current]->cost){//compare current with smallest child
						SwapNode(current,minchild);
						current = minchild;
					}
					else//if smaller child is not smaller than current, we're done
						done = true;
				}
				else if(heap[left]->cost < heap[current]->cost){//if left less than current, swap
					SwapNode(current, left);
					current = left;//our current node is now where left was
				}
				else
					done = true;//if left but no right and left not less than current, we're done
			}
			else//if no left child, can't have right child, so we're done
				done = true;
		}
	}
	return result;
}

//sees if heap is empty by where the last index is
bool binheap::empty(){
	return (last == -1);
}

#endif
