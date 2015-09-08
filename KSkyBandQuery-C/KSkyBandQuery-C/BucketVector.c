#include <stdlib.h>
#include "PointVector.h"
#include "BucketVector.h"

void InitBucket(struct gtBucket *Bucket, int Dimension) {
    Bucket -> dataSize = 0;
    Bucket -> dataTail = NULL;
    Bucket -> dataHead = StartPoint(&Bucket->dataSize, &Bucket->dataHead, &Bucket->dataTail, Dimension);
	Bucket -> SlnHead = StartPoint(&Bucket->SlnSize, &Bucket->SlnHead, &Bucket->SlnTail, Dimension);;
    Bucket -> next = NULL;
    Bucket -> previous = NULL;
}

struct gtBucket *StartBucket(int *TotalSize, struct gtBucket **BucketHead, struct gtBucket **BucketTail, int Dimension) {
    *BucketHead = (struct gtBucket *)malloc(sizeof(struct gtBucket));
    //if (StartBucket == NULL)
    //    ereport(ERROR, (errcode(ERRCODE_OUT_OF_MEMORY), errmsg("Cannot start a linked list.")));
    //else{
	InitBucket(*BucketHead, Dimension);
	*BucketTail = *BucketHead;
    *TotalSize = 1;
	//}
    return *BucketHead;
}

void PushBucket(struct gtBucket *NewBucket, int *TotalSize, struct gtBucket **BucketTail) {
	(*BucketTail) -> next = NewBucket;
    NewBucket -> previous = *BucketTail;
	NewBucket -> next = NULL;
    *BucketTail = NewBucket;
	*TotalSize = *TotalSize + 1;
}

void DeleteBucket(int Position, struct gtBucket **BucketHead, int *TotalSize) {
    struct gtBucket *Pointer;
    int i = 0;
    Pointer = *BucketHead;
    for (i = 0; i < Position; i++) {
        if (Pointer->next != NULL){
            Pointer = Pointer -> next;
        }
        else break;
    }
    //if (i != Position)
    //    ereport(ERROR, (errcode(ERRCODE_OUT_OF_MEMORY), errmsg("Illegal GetNode.")));
    Pointer -> previous -> next = Pointer -> next;
    Pointer -> next = NULL;
    Pointer -> previous = NULL;
    free(Pointer);
    *TotalSize = *TotalSize - 1;
}
