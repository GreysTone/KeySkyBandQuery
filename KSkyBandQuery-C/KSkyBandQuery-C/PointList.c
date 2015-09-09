//
//  PointVector.h
//  KeySkyBandQuery
//
//  Created by Armour on 7/14/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#include <stdlib.h>
#include "PointList.h"

/*
 * Function: InitPoint
 * -------------------
 *   Initialize point
 *
 *   point: the point that needs to be initialized
 *
 *   returns: void
 */

void InitPoint(struct skyPoint *point) {
    point -> data = NULL;
    point -> bitmap = NULL;
    point -> dominatedCount = 0;             // dominatedCount should be zero at first
    point -> next = NULL;
    point -> previous = NULL;
}

/*
 * Function: StartPoint
 * -------------------
 *   Start a new point with d dimesions, and then make it as the head of new list
 *
 *   size: the total size of this list, it equals to zero at first (ignored head point)
 *   head: the head of this point list, need malloc at first
 *   tail: the tail of this point list, equals to head at first
 *   d: the dimension of this point list.
 *
 *   returns: the head point of new list, with d dimensions
 */

struct skyPoint *StartPoint(int *size, struct skyPoint **head, struct skyPoint **tail, int d) {
    *head = (struct skyPoint *)malloc(sizeof(struct skyPoint));
    (*head)->dimension = d;
    InitPoint(*head);
    *tail = *head;          // because there is only one head point
    *size = 0;
    return *head;
}

/*
 * Function: PushPoint
 * -------------------
 *   Pushing new point to the end of the list
 *
 *   point: the new point which need to push into list
 *   size: the total size of this list, here we just need to plus 1
 *   tail: the tail of this point list, will be updated to the new point
 *
 *   returns: void
 */

void PushPoint(struct skyPoint *point, int *size, struct skyPoint **tail) {
    if (*tail == NULL) return;
    (*tail) -> next = point;
    point -> previous = *tail;
    point -> next = NULL;
    *tail = point;
    *size = *size + 1;
}

/*
 * Function: DeletePoint
 * -------------------
 *   Deleting the point in position p in the list
 *
 *   p: the position of the point which we need to delete
 *   size: the total size of this list, here we just need to minus 1
 *   head: the head of this point list
 *   tail: the tail of this point list
 *
 *   returns: void
 */

void DeletePoint(int p, int *size, struct skyPoint **head, struct skyPoint **tail) {
    struct skyPoint *point;                  // a temp point
    int i = 0;
    point = *head;
    for (i = 0; i < p; i++)                 // to find the point in the right position
        if (point -> next != NULL)
            point = point -> next;
        else
            break;
    if (point -> previous != NULL) {        // if deleted point is not the first point
        if (point -> next == NULL)          // if deleted point is the last point, then update tail
            *tail = (*tail)->previous;
        if (point -> previous != NULL)      // update previous point of deleted point
            point -> previous -> next = point -> next;
        if (point -> next != NULL)          // update next point of deleted point
            point -> next -> previous = point -> previous;
        point -> next = NULL;               // delete this point
        point -> previous = NULL;
        *size = *size - 1;
    }
}
