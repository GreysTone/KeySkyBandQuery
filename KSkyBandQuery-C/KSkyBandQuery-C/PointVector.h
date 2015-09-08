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

void InitPoint(struct gtPoint *Node);
struct gtPoint *StartPoint(int *TotalSize, struct gtPoint **PointHead, struct gtPoint **PointTail, int dataDimension);
void PushPoint(struct gtPoint *NewElement, int *TotalSize, struct gtPoint **PointTail);
void DeletePoint(int Position, struct gtPoint **PointHead, int *TotalSize, struct gtPoint **PointTail);

#endif
