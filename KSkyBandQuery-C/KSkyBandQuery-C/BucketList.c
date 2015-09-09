//
//  BucketVector.c
//  KeySkyBandQuery
//
//  Created by Armour on 7/14/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#include <stdlib.h>
#include "PointList.h"
#include "BucketList.h"

/*
 * Function: InitBucket
 * -------------------
 *   Initialize bucket
 *
 *   bucket: the bucket that needs to be initialized
 *   d: the dimension of this bucket list.
 *
 *   returns: void
 */

void InitBucket(struct skyBucket *bucket, int d) {
    bucket -> dataSize = 0;                 // init data list in bucket
    bucket -> dataTail = NULL;
    bucket -> dataHead = StartPoint(&bucket->dataSize, &bucket->dataHead, &bucket->dataTail, d);
    bucket -> slnSize = 0;                  // init sln list in bucket
    bucket -> slnTail = NULL;
    bucket -> slnHead = StartPoint(&bucket->slnSize, &bucket->slnHead, &bucket->slnTail, d);;
    bucket -> next = NULL;
    bucket -> previous = NULL;
}

/*
 * Function: StartBucket
 * -------------------
 *   Start a new bucket with d dimesions, and then make it as the head of new list
 *
 *   size: the total size of this list, it equals to zero at first (ignored head point)
 *   head: the head of this bucket list, need malloc at first
 *   tail: the tail of this bucket list, equals to head at first
 *   d: the dimension of this bucket list.
 *
 *   returns: the head bucket of new list, with d dimensions
 */

struct skyBucket *StartBucket(int *size, struct skyBucket **head, struct skyBucket **tail, int d) {
    *head = (struct skyBucket *)malloc(sizeof(struct skyBucket));
	InitBucket(*head, d);
	*tail = *head;              // because there is only one head point
    *size = 0;
    return *head;
}

/*
 * Function: PushBucket
 * -------------------
 *   Pushing new bucket to the end of the list
 *
 *   bucket: the new bucket which need to push into list
 *   size: the total size of this list, here we just need to plus 1
 *   tail: the tail of this point list, will be updated to the new bucket
 *
 *   returns: void
 */

void PushBucket(struct skyBucket *bucket, int *size, struct skyBucket **tail) {
	(*tail) -> next = bucket;
    bucket -> previous = *tail;
	bucket -> next = NULL;
    *tail = bucket;
	*size = *size + 1;
}
