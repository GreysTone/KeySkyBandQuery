//
//  gtPoint.h
//  gtKeySkyBandQueryPlus
//
//  Created by GreysTone on 6/25/15.
//  Copyright (c) 2015 GreysTone. All rights reserved.
//

#ifndef __gtKeySkyBandQueryPlus__gtPoint__
#define __gtKeySkyBandQueryPlus__gtPoint__

#include <iostream>

using namespace std;

#define MAX_DIMENSION 5

class gtPoint {
    
public:
    int32_t pDs;            // point dimension
    int32_t **data;
    //int32_t data[MAX_DIMENSION];
    int32_t bitmap;
    int32_t domainatedCount;
    
    gtPoint();
    virtual ~gtPoint();
    
    void printPoint();
};

#endif /* defined(__gtKeySkyBandQueryPlus__gtPoint__) */
