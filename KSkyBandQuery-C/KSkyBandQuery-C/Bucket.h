//
//  Bucket.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

#ifndef KeySkyBandQuery_Bucket_h
#define KeySkyBandQuery_Bucket_h

#include "Point.h"

struct gtBucket {
    int dataSize;
    struct gtPoint *dataHead;
    struct gtPoint *dataTail;

	int SlnSize;
    struct gtPoint *SlnHead;
    struct gtPoint *SlnTail;

    struct gtBucket *next;
    struct gtBucket *previous;
};

#endif
