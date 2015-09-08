#include <stdlib.h>
#include "PointVector.h"

void InitPoint(struct gtPoint *Node) {
    Node -> dimension = 0;
    Node -> data = NULL;
    Node -> bitmap = NULL;
    Node -> domainatedCount = 0;
    Node -> next = NULL;
    Node -> previous = NULL;
}

struct gtPoint *StartPoint(int *TotalSize, struct gtPoint **PointHead, struct gtPoint **PointTail, int dataDimension) {
    *PointHead = (struct gtPoint *)malloc(sizeof(struct gtPoint));
    //if (StartNode == NULL)
    //    ereport(ERROR, (errcode(ERRCODE_OUT_OF_MEMORY), errmsg("Cannot start a linked list.")));
    //else {
    InitPoint(*PointHead);
    (*PointHead)->dimension = dataDimension;
    *PointTail = *PointHead;
    *TotalSize = 1;
    //}
    return *PointHead;
}

void PushPoint(struct gtPoint *NewElement, int *TotalSize, struct gtPoint **PointTail) {
    if (*PointTail == NULL) return;
    (*PointTail) -> next = NewElement;
    NewElement -> previous = *PointTail;
    NewElement -> next = NULL;
    *PointTail = NewElement;
    *TotalSize = *TotalSize + 1;
}

void DeletePoint(int Position, struct gtPoint **PointHead, int *TotalSize, struct gtPoint **PointTail) {
    struct gtPoint *Pointer;
    int i = 0;
    Pointer = *PointHead;
    for (i = 0; i < Position; i++) {
        if (Pointer -> next != NULL)
            Pointer = Pointer -> next;
        else
            break;
    }
    if (Pointer -> previous != NULL) {  // if not the first node
        if (Pointer -> next == NULL)      // if delete the last node
            *PointTail = (*PointTail)->previous;
        //if (i != Position)
        //    ereport(ERROR, (errcode(ERRCODE_OUT_OF_MEMORY), errmsg("Illegal GetNode.")));
        if (Pointer -> previous != NULL)
            Pointer -> previous -> next = Pointer -> next;
        if (Pointer -> next != NULL)
            Pointer -> next -> previous = Pointer -> previous;
        Pointer -> next = NULL;
        Pointer -> previous = NULL;
        *TotalSize = *TotalSize - 1;
    }
}