//
//  gtBucket.cpp
//  gtKeySkyBandQueryPlus
//
//  Created by GreysTone on 6/25/15.
//  Copyright (c) 2015 GreysTone. All rights reserved.
//

#include "gtBucket.h"

gtBucket::gtBucket() {
    dominanceTable = NULL;
}

gtBucket::~gtBucket() {
    
}

void gtBucket::printDominanceTable() {
    if(!dominanceTable) return;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data.size(); j++) {
            if(dominanceTable[i][j]) cout << "1";
            else cout << "0";
        }
        cout << endl;
    }
}

void gtBucket::printSlSln() {
    cout << "Bucket " << bitmap << endl;
    cout << "Sln" << endl;
    for (int j = 0; j < Sln.size(); j++) Sln[j]->printPoint();
    cout << "Sl" << endl;
    for (int j = 0; j < Sl.size(); j++) Sl[j]->printPoint();
}