#include <assert.h>
#include <stdio.h>
#include "skip.h"
#include "librng/rng.h"
#include "librng/xor32.h"

#include <list>

void skipnode_test()
{
    printf("Testing skipnode... ");
    fflush(stdout);
    for( int jj = 0; jj < 1024; ++jj ){
        skipnode* n = make_skipnode(-1L, SKIPLIST_MAX_LEVEL, sizeof(long));
        destroy_skipnode(n, sizeof(long));
    }
    printf("PASSED!\n");
}

void skiplist_test()
{
    printf("Testing skiplist creation/destruction... ");
    fflush(stdout);
    skiplist s = make_skiplist(sizeof(int));
    destroy_skiplist(s);
    printf("PASSED!\n");
}

void skiplist_increasing_key_test()
{
    printf("Testing increasing key insertion... ");
    fflush(stdout);

    XOR32 rng;
    skiplist s = make_skiplist(sizeof(int));

    for( int ii = 0; ii < 10; ++ii ) {
        int z = rng.get_int();
        insert(s, ii, &z, &rng);
    }

    destroy_skiplist(s);
    printf("PASSED!\n");
}

void skiplist_decreasing_key_test()
{
    printf("Testing decreasing key insertion... ");
    fflush(stdout);

    XOR32 rng;
    skiplist s = make_skiplist(sizeof(int));

    for( int ii = 0; ii < 10; ++ii ) {
        int z = rng.get_int();
        insert(s, 100000 - ii, &z, &rng);
    }

    destroy_skiplist(s);
    printf("PASSED!\n");
}



void skiplist_insert_find_test()
{
    printf("Testing insertion... ");
    fflush(stdout);

    XOR32 rng;
    skiplist s = make_skiplist(sizeof(int));

    unsigned long target_key = 0L;
    int target_value = 0;

    for( int ii = 0; ii < 1000; ++ii ) {

        if(ii == 77){
            target_key = rng.get_int();
            target_value = rng.get_int();
            insert(s, target_key, &target_value, &rng);
        } else {
            int z = rng.get_int();
            insert(s, rng.get_int(), &z, &rng);
        }
    }

    void* d = find(s, target_key);
    assert(d != NULL);
    assert(target_value == *((int*) d));

    destroy_skiplist(s);
    printf("PASSED!\n");
}

void skiplist_remove_test() {

    printf("Testing removal... ");
    fflush(stdout);

    XOR32 rng;
    skiplist s = make_skiplist(sizeof(int));
    unsigned long target_key;

    for( int ii = 0; ii < 1000; ++ii ) {
        unsigned long key = rng.get_int();
        if(ii == 244){
            target_key = key;
        }
            
        int z = rng.get_int();
        insert(s, key, &z, &rng);
    }

    void* dat = find(s, target_key);
    assert(dat != NULL);
    remove(s, target_key);
    dat = find(s, target_key);
    assert(dat == NULL);

    destroy_skiplist(s);
    printf("PASSED!\n");

}

void skiplist_load_test() {

    printf("Testing load... ");
    fflush(stdout);

    XOR32 rng;
    skiplist s = make_skiplist(sizeof(int));
    
    std::list<unsigned long> keys;

    for( int ii = 0; ii < 10000000; ++ii ) {
        unsigned long key = rng.get_int();
        if(rng.get_float() < 0.05){
            keys.push_back(key);
        }
            
        int z = rng.get_int();
        insert(s, key, &z, &rng);
    }

    std::list<unsigned long>::iterator itr = keys.begin();
    std::list<unsigned long>::iterator end = keys.end();

    for( ; itr != end ; itr++ ){
        find(s, *itr);
    }
    destroy_skiplist(s);
    printf("PASSED!\n");


}

int main(int argc, char** argv){

    skipnode_test();
    skiplist_test();
    skiplist_increasing_key_test();
    skiplist_decreasing_key_test();
    skiplist_insert_find_test();
    skiplist_remove_test();
//    skiplist_load_test();
    return 0;

}
