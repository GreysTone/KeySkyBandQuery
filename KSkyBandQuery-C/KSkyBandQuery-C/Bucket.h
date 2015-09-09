//
//  Bucket.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#ifndef KeySkyBandQuery_Bucket_h
#define KeySkyBandQuery_Bucket_h

#include "Point.h"

struct skyBucket {
    int dataSize;
    struct skyPoint *dataHead;
    struct skyPoint *dataTail;       // Used to store points in this bucket

	int slnSize;
    struct skyPoint *slnHead;
    struct skyPoint *slnTail;        // Store Sln points in this bucket

    struct skyBucket *next;
    struct skyBucket *previous;      // Stored as a list
};

#endif
