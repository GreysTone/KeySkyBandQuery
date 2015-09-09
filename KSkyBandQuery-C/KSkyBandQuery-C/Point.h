//
//  Point.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#ifndef KeySkyBandQuery_Point_h
#define KeySkyBandQuery_Point_h

struct skyPoint {
    int dimension;              // Point dimension
    double **data;              // Stored data of this point
    char *bitmap;               // Bitmap of this point
    int dominatedCount;         // The number of dominated times of this point
    struct skyPoint *next;
    struct skyPoint *previous;   // Stored as a list
};

#endif
