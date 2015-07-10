//
//  Point.h
//  gtKeySkyBandQueryPlus
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 GreysTone. All rights reserved.
//

#ifndef gtKeySkyBandQueryPlus_Point_h
#define gtKeySkyBandQueryPlus_Point_h

struct gtPoint {
    int dimension;            // point dimension
    int **data;
    int bitmap;
    int domainatedCount;
    struct gtPoint *next;
    struct gtPoint *previous;
};

#endif
