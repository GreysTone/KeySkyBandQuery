//
//  HashTable.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/23/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#ifndef KeySkyBandQuery_HashTable_h
#define KeySkyBandQuery_HashTable_h

#include <stdlib.h>
#include "BucketList.h"

struct ListNode {
    char *bitmap;                       // Bitmap of this listnode
    struct skyBucket *bucket;           // Stored bucket of this listnode
    struct ListNode *nextNode;          // Next node in the list
};

struct HashTable {
    int size;                           // The size of hashtable
    struct ListNode **list;             // The list of nodes where each one stores a bucket list
};

// Initialize hashtable with size n
struct HashTable *InitTable(int n);

// Find the list of nodes which has the same bitmap with b (from hashtable h)
struct ListNode *Find(char *b, struct HashTable *h, int d);

// Insert into hashtable h a bucket with bitmap b, make it the first node of a list
void Insert(char *b, struct HashTable *h, int d, struct skyBucket* bucket,
            struct skyBucket** firstBucket, struct skyBucket** lastBucket);

// Used to get the next prime number of n
int NextPrime(int n);

// Used to compare a to b to see if they have the same bitmap
int SameBitmap(char *a, char* b, int d);

// BKDR hash function
unsigned int BKDRHash(char *str, int m, int d);

#endif
