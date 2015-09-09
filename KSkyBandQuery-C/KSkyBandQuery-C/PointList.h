//
//  PointVector.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/14/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

#ifndef KeySkyBandQuery_PointVector_h
#define KeySkyBandQuery_PointVector_h

#include "Point.h"

// Initialize point
void InitPoint(struct skyPoint *point);

// Start a head point of new list with d dimesions
struct skyPoint *StartPoint(int *size, struct skyPoint **head, struct skyPoint **tail, int d);

// Push a point to a list, make it as tail
void PushPoint(struct skyPoint *point, int *size, struct skyPoint **tail);

// Delete point in any position of the list
void DeletePoint(int position, int *size, struct skyPoint **head, struct skyPoint **tail);

#endif
