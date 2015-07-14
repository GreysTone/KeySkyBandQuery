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
    int bitmap;
    int dataSize;
    struct gtPoint *data;
    struct gtPoint *dataTail;
    struct gtPoint *Sl;
    struct gtPoint *Sln;
    struct gtPoint *Stwh;
    struct gtPoint *Ses;
    struct gtBucket *next;
    struct gtBucket *previous;
};

#endif
