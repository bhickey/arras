#ifndef __ARRAS_SKIPLIST_HEADER__
#define __ARRAY_SKIPLIST_HEADER__

#include <stdlib.h>
#include <string.h>
#include "librng/rng.h"

#define SKIPLIST_MAX_LEVEL 31

/*
 * Data lives at the head of the skipnode outside of the struct
 */
struct skipnode {
    uint64_t key;
    skipnode* next;
};

struct skiplist {
    size_t data_sz;
    skipnode *head;
};

// Operations on skip nodes
skipnode* make_skipnode(unsigned long, int, size_t);
void destroy_skipnode(skipnode*, size_t);
void* skipnode_data(skipnode*, size_t);
skipnode* next_skipnode(skipnode*, int);
void set_next_skipnode(skipnode*, int, skipnode*);
void articulate_nodes(skipnode*, skipnode*, skipnode*, int);

// Operations on skip lists
skiplist make_skiplist(size_t);
void destroy_skiplist(skiplist);

void insert(skiplist, unsigned long, void*, RNG*);
void insert_node(skipnode*, skipnode*, int);
void remove(skiplist, unsigned long);
void* find(skiplist, unsigned long);

int nodeLevel(RNG*);

#endif
