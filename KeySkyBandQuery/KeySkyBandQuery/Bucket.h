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
    gtPoint * data;   //  这几个vector我在思考怎么弄
    gtPoint *  Sl;
    gtPoint * Sln;
    gtPoint * StwhSes;
    struct gtBucket *next;
    struct gtBucket *previous;
};

#endif
