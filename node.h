//----------------------------------------------------------------------
//Node struct used by Binary Heap and Graph
//
//by Victor Rogers, Martin Tuck, & Tyler Yasaka
//----------------------------------------------------------------------

#ifndef NODE_H
#define NODE_H

struct node{
    int cost;//total cost to get here from origin of path
    node* prev;//previous node in tentative path
    int location;//index of this node on matrix
};

#endif 
