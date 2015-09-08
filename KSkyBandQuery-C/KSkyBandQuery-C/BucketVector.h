//
//  BucketVector.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/14/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

#ifndef KeySkyBandQuery_BucketVector_h
#define KeySkyBandQuery_BucketVector_h

#include "Point.h"
#include "Bucket.h"

void InitBucket(struct gtBucket *Bucket, int Dimension);
struct gtBucket *StartBucket(int *TotalSize, struct gtBucket **BucketHead, struct gtBucket **BucketTail, int Dimension);
void PushBucket(struct gtBucket *NewBucket, int *TotalSize, struct gtBucket **BucketTail);
void DeleteBucket(int Position, struct gtBucket **BucketHead, int *TotalSize);

#endif
