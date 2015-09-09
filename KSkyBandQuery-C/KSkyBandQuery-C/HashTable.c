//
//  HashTable.c
//  KeySkyBandQuery
//
//  Created by Armour on 7/23/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#include "HashTable.h"

/*
 * Function: NextPrime
 * -------------------
 *   Give a prime number as hashtable size that is suitble for n
 *
 *   n: the maximal points that may uesd in hashtable
 *
 *   returns: prime number
 */

int NextPrime(int n) {
    if (n < 53)
        return 53;
    else if (n < 97)
        return 97;
    else if (n < 193)
        return 193;
    else if (n < 389)
        return 389;
    else if (n < 769)
        return 769;
    else if (n < 1543)
        return 1543;
    else if (n < 3079)
        return 3079;
    else if (n < 6151)
        return 6151;
    else if (n < 12289)
        return 12289;
    else if (n < 24593)
        return 24593;
    else if (n < 49157)
        return 49157;
    else if (n < 98317)
        return 98317;
    else if (n < 196613)
        return 196613;
    else if (n < 393241)
        return 393241;
    else if (n < 786433)
        return 786433;
    else if (n < 1572869)
        return 1572869;
    else if (n < 3145739)
        return 3145739;
    else if (n < 6291469)
        return 6291469;
    else
        return 12582917;
}

/*
 * Function: BKDRHash
 * -------------------
 *   Hash function that used to calculate a hashValue for str with dimension d and limitation m
 *
 *   str: the character array that used to hash
 *   m: the limitation of hashValue
 *   d: the dimension of str (in other words, called "length")
 *
 *   returns: hashed value
 */

unsigned int BKDRHash(char *str, int m, int d) {
    unsigned int seed = 131;            // Seed like 31 131 1313 13131 131313 etc..
    unsigned int hashValue = 0;
    int i = 0;
    while (i < d) {
        hashValue = hashValue * seed + *(str + i);
        i++;
        if (hashValue >= m)
            hashValue %= m;
    }
    return(hashValue % m);
}

/*
 * Function: SameBitmap
 * -------------------
 *   Compare to see weather a has the same bitmap with b or not
 *
 *   a: the compared characters array
 *   b: the compared characters array
 *   d: the dimension of a and b (in other words, called "length")
 *
 *   returns: if same returns 1, else 0
 */

int SameBitmap(char *a, char* b, int d) {
    int i;
    for (i = 0; i < d; i++)
        if (*(a + i) != *(b + i))
            return 0;
    return  1;
}

/*
 * Function: InitializeTable
 * -------------------
 *   Initialize a hashtable with size of n
 *
 *   n: size of the new hashtable
 *
 *   returns: the pointer to the new hashtable
 */

struct HashTable* InitTable(int n) {
    struct HashTable* h;
    int i;
    h = (struct HashTable *)malloc(sizeof(struct HashTable));
    h->size = NextPrime(n);                                         // Better be prime
    h->list = (struct ListNode **)malloc(sizeof(struct ListNode *) * h->size);
    for( i = 0; i < h->size; i++ ) {                                // Allocate list headers
        h->list[i] = (struct ListNode *)malloc(sizeof(struct ListNode));
        h->list[i]->nextNode = NULL;
    }
    return h;
}

/*
 * Function: Find
 * -------------------
 *   Find the list that has the bitmap b and return the first node of this list
 *
 *   b: bitmap that want to find in hashtable
 *   h: hashtable used to find things
 *   d: the dimension of bitmap b (in other words, called "length")
 *
 *   returns: the pointer to the new hashtable
 */

struct ListNode *Find(char *b, struct HashTable *h, int d) {
    struct ListNode *p;
    p = h->list[BKDRHash(b, h->size, d)]->nextNode;
    while (p != NULL && !SameBitmap(p->bitmap, b, d))
        p = p->nextNode;
    return p;
}

/*
 * Function: Insert
 * -------------------
 *   Insert the list that has the bitmap b and make it head node of that list
 *
 *   b: bitmap that want to insert in hashtable
 *   h: hashtable used to insert things
 *   d: the dimension of bitmap b (in other words, called "length")
 *
 *   returns: void
 */

void Insert(char *b, struct HashTable *h, int d, struct skyBucket* bucket,
            struct skyBucket** firstBucket, struct skyBucket** lastBucket) {
    int i;
    struct ListNode *p;                 // A new listnode uesd to insert into hashtable
    p = (struct ListNode *)malloc(sizeof(struct ListNode));
    p->bucket = bucket;
    p->nextNode = h->list[BKDRHash(b, h->size, d)]->nextNode;       // Set nextNode of this new listnode
    h->list[BKDRHash(b, h->size, d)]->nextNode = p;
    p->bitmap = (char *)malloc(sizeof(char) * d);       // Set bitmap of this new listnode
    for (i = 0; i < d; i++)
        *((p->bitmap) + i) = *(b + i);
    if (*firstBucket == NULL) {             // Check if it is the first bucket
        *firstBucket = p->bucket;
        *lastBucket = p->bucket;
    } else {
        (*lastBucket)->next = p->bucket;
        *lastBucket = p->bucket;
    }
}
