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
    //bool **dominanceTable;  // I think we don't need it
    struct gtPoint *data;
    struct gtPoint *Sl;
    struct gtPoint *Sln;
    struct gtPoint *StwhSes;
    struct gtBucket *next;
    struct gtBucket *previous;
};

#endif
