//
//  gtPoint.cpp
//  gtKeySkyBandQueryPlus
//
//  Created by GreysTone on 6/25/15.
//  Copyright (c) 2015 GreysTone. All rights reserved.
//

#include "gtPoint.h"

gtPoint::gtPoint() {
    pDs = 0;
    data = NULL;
    bitmap = 0;
    domainatedCount = 0;
}

gtPoint::~gtPoint() {
    delete data;
}

void gtPoint::printPoint() {
    cout << "Point (";
    for (int32_t i=0; i < pDs - 1; i++) {
        if (data[i]) cout << *(data[i]) << ", ";
        else cout << "-, ";
    }
    if (data[pDs - 1]) cout << *(data[pDs - 1]) << ")";
    else cout << "-)";
    
    cout << " BIT: " << bitmap << endl;
}