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

struct ListNode;
struct HashTable;

struct ListNode {
    char *bitmap;
    struct skyBucket *bucket;
    struct skyBucket *nextBucket;
    struct ListNode *Next;
};

struct HashTable {
    int TableSize;
    struct ListNode **TheLists;
};

struct HashTable *InitializeTable(int TableSize);
struct ListNode *Find(char *bitmap, struct HashTable *H, int dataDimension);
void Insert(char *bitmap, struct HashTable *H, int dataDimension, struct skyBucket* bucket, struct skyBucket** firstBucket, struct skyBucket** lastBucket);
int NextPrime(int size);
int sameBitmap(char *A, char* B, int dataDimension);
unsigned int BKDRHash(char *str, int M, int dataDimension);

#endif
