//
//  gtBucket.h
//  gtKeySkyBandQueryPlus
//
//  Created by GreysTone on 6/25/15.
//  Copyright (c) 2015 GreysTone. All rights reserved.
//

#ifndef __gtKeySkyBandQueryPlus__gtBucket__
#define __gtKeySkyBandQueryPlus__gtBucket__

#include <iostream>
#include <vector>
#include "gtPoint.h"

using namespace std;

#define MAX_DATA_AMOUNT (1000)

class gtBucket {
public:
    int32_t bitmap;
    vector <gtPoint *>data;
    bool **dominanceTable;
    vector <gtPoint *>Sl;
    vector <gtPoint *>Sln;
    vector <gtPoint *>StwhSes;
    
    gtBucket();
    virtual ~gtBucket();
    
    void printDominanceTable();
    void printSlSln();
};

#endif /* defined(__gtKeySkyBandQueryPlus__gtSet__) */
