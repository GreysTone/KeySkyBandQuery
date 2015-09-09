//
//  BucketVector.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/14/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#ifndef KeySkyBandQuery_BucketVector_h
#define KeySkyBandQuery_BucketVector_h

#include "Point.h"
#include "Bucket.h"

// Initialize bucket with d dimension
void InitBucket(struct skyBucket *bucket, int d);

// Start a head bucket of new list with d dimesions
struct skyBucket *StartBucket(int *size, struct skyBucket **head, struct skyBucket **tail, int d);

// Push a bucket to a list, make it as tail
void PushBucket(struct skyBucket *bucket, int *size, struct skyBucket **tail);

#endif
