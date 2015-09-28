/*-------------------------------------------------
 *
 *  sky_domi_lit.c
 *  KSkyBandQuery
 *
 *  Created by Armour on 9/26/15.
 *  Copyright (c) 2015 Armour. All rights reserved.
 *
 *-------------------------------------------------
 */

#include "sky_domi_list.h"
#include <stdlib.h>

/*
 * Function: InitDomiTable
 * -------------------
 *   Initialize a dominate table
 *
 *   n: the size of this dominate list.
 *
 *   returns: void
 */

DomiTable *InitDomiTable(int n) {
    DomiTable *d;
    int i;
    d = (DomiTable *)malloc(sizeof(DomiTable));
    d->size = n;
    d->list = (DomiNode **)malloc(sizeof(DomiNode *) * d->size);
    for (i = 0; i < d->size; i++) {
        d->list[i] = (DomiNode *)malloc(sizeof(DomiNode));
        d->list[i]->index = -1;
        d->list[i]->next = NULL;
    }
    return d;
}

/*
 * Function: AddDomiPair
 * -------------------
 *   Insert into domitable d with a pair of nodes, which the node of index x dominate the node of index y
 *
 *   d: dominate table
 *   x: index of the dominate node
 *   y: index of the dominated node
 *
 *   returns: void
 */

void AddDomiPair(DomiTable *d, int x, int y) {
    DomiNode *p;
    DomiNode *new_node;
    new_node = (DomiNode *)malloc(sizeof(DomiNode));
    new_node->index = y;
    new_node->next = NULL;
    p = d->list[x];
    while (p->next != NULL)
        p = p->next;
    p->next = new_node;
}

/*
 * Function: IsDomiPairExist
 * -------------------
 *   Test if the node of index x dominate the node of node y exist in dominate table d
 *
 *   d: dominate table
 *   x: index of the dominate node
 *   y: index of the dominated node
 *
 *   returns: void
 */

int IsDomiPairExist(DomiTable *d, int x, int y) {
    DomiNode *p;
    p = d->list[x];
    while (p != NULL) {
        if (p->index == y) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}





