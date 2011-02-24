#include <assert.h>
#include <stdio.h>
#include "skip.h"

skipnode* make_skipnode(unsigned long key, int level, size_t data_sz)
{
    size_t alloc_sz = data_sz + sizeof(skipnode) + sizeof(skipnode*) * (level);
    char* space = (char*) calloc(1, alloc_sz);
    skipnode* s = (skipnode*) (space + data_sz);
    s->key   = key;

    return s;
}

void destroy_skipnode(skipnode* node, size_t data_sz)
{ free(skipnode_data(node, data_sz)); }

void* skipnode_data(skipnode* node, size_t data_sz)
{ return (void*) ((char*) node - data_sz); }

skipnode* next_skipnode(skipnode* node, int level)
{ return *(&(node->next) + level); }

void set_next_skipnode(skipnode* node, int level, skipnode* next)
{ 
    skipnode** slot = &(node->next) + level;
    *slot = next; 
}

int nodeLevel(MTRand* rng)
{
    unsigned long b = 0x7FFFFFFF & rng->randInt() & rng->randInt();
    b = ((b + 1) ^ b) - 1;
    return __builtin_popcount(b);
}

void insert(skiplist sk, unsigned long key, void* data, MTRand* rng)
{
    int insertion_level = nodeLevel(rng);
    skipnode* new_node = make_skipnode(key, insertion_level, sk.data_sz);
    memcpy(data, ((char*) new_node - sk.data_sz), sk.data_sz);

    skipnode* search_node = sk.head;
    int search_level = SKIPLIST_MAX_LEVEL;

    // INVARIANT:   search_node.key < key
    //              search_level never increases
    while( search_level >= 0 ){

        skipnode* next_node = next_skipnode(search_node, search_level);

        if( !next_node ){
            if( search_level <= insertion_level ){
                set_next_skipnode(search_node, search_level, new_node);
                set_next_skipnode(new_node,search_level, NULL);
            }
    } else {
        if( key > next_node->key ){
            search_node = next_node;
            continue;
        }

        if( search_level <= insertion_level ){
            set_next_skipnode(new_node, search_level, next_node);
            set_next_skipnode(search_node, search_level, new_node);
        }
    }

    --search_level;
}
}    

void* find(skiplist sk, unsigned long key)
{
    skipnode* search_node = sk.head;
    int search_level = SKIPLIST_MAX_LEVEL;
    do {
        skipnode* next_node = next_skipnode(search_node, search_level); 
        if( next_node ){
            if( key < next_node->key){ --search_level; } 
            else if( key > next_node->key){ search_node = next_node; } 
            else if( key == next_node->key ){
                return skipnode_data(next_node, sk.data_sz);
            }
        } else {
            --search_level;
        }
    } while( search_level >= 0 );
    return NULL;
}


void remove(skiplist sk, unsigned long key)
{
    skipnode* search_node = sk.head;
    int search_level = SKIPLIST_MAX_LEVEL;
    do {
        skipnode* next_node = next_skipnode(search_node, search_level); 
        if( next_node ){
            if( key < next_node->key){ --search_level; } 
            else if( key > next_node->key){ search_node = next_node; } 
            else if( key == next_node->key ){
                skipnode* next_next_node = next_skipnode(next_node, search_level);
                set_next_skipnode(search_node, search_level, next_next_node);
                if( search_level == 0 ){ destroy_skipnode(next_node, sk.data_sz); }
                --search_level;
            }
        } else {
            --search_level;
        }
    } while( search_level >= 0 );
}



skiplist make_skiplist(size_t data_sz)
{
    skiplist s;
    s.head = make_skipnode(-1L, SKIPLIST_MAX_LEVEL, 0);
    s.data_sz = data_sz;
    return s;
}

void destroy_skiplist(skiplist s)
{
    skipnode* curr = s.head;
    skipnode* next = next_skipnode(curr, 0);
    destroy_skipnode(s.head, 0);

    while( next ){
        curr = next;
        next = next_skipnode(curr, 0);
        destroy_skipnode(curr, s.data_sz);
    }
}

