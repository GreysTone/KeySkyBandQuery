#include <stdio.h>
#include <stdlib.h>
#include "Point.h"
#include "Bucket.h"

struct gtPoint *Tail;
struct gtPoint *Head;
//gtPoint *Now;

int TotalSize;

void InitBucket(struct gtBucket *Bucket){
}

struct gtBucket *StartBucket(struct gtBucket * StartBucket,int Size, struct gtBucket * BucketHead, struct gtBucket * BucketTail){
    StartBucket = (struct gtBucket *)malloc(sizeof(struct gtBucket));
	//StartNode = (gtPoint *)palloc(sizeof(gtPoint));
	//if (StartNode == NULL)
	//	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Cannot start a linked list."))));
	//else{
	InitBucket(StartBucket);
	BucketHead = StartBucket;
	BucketTail = StartBucket;
    TotalSize = 1;
	//}
    return StartBucket;
}

void PushBucket(struct gtBucket * NewBucket,int Size, struct gtBucket *BucketTail){
	//NewBucket = (gtPoint *)malloc(sizeof(gtPoint));
	//NewBucket = (gtPoint *)palloc(sizeof(gtPoint));
	//if (NewBucket == NULL)
	//	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Cannot start a linked list."))));
	//else{
	//Initialized(NewBucket);
	BucketTail -> next = NewBucket;
    NewBucket -> previous = BucketTail;
	NewBucket -> next = NULL;
    BucketTail = NewBucket;
	Size++;
}

struct gtBucket * GetBucket(int Position,struct gtBucket * BucketHead){
    struct gtBucket * Pointer;
    int i = 0;
    Pointer = BucketHead;
    for (i = 0;i < Position; i++){
        if (Pointer != NULL)
            Pointer = Pointer -> next;
        else break;
    }
    //if (i != Position)
    //ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Illegal GetNode."))));
    return Pointer;
}

void DeleteBucket(int Position, struct gtBucket * BucketHead, int Size){
    struct gtBucket * Pointer;
    int i = 0;
    Pointer = BucketHead;
    for (i = 0;i < Position; i++){
        if (Pointer == NULL){
            Pointer = Pointer -> next;
        }
        else break;
    }
    //if (i != Position)
    //	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Illegal GetNode."))));
    Pointer -> previous -> next = Pointer -> next;
    Pointer -> next = NULL;
    Pointer -> previous = NULL;
    free(Pointer);
    Size--;
    //pfree(Pointer);
}

void FreeAllBuckets(struct gtBucket * Bucket,int Size){
    struct gtBucket * Pointer;
    struct gtBucket * FreePointer;
    Pointer = Bucket;
    FreePointer = Bucket;
    while (Pointer != NULL){
        Pointer = Pointer -> next;
        free(FreePointer);
        FreePointer = Pointer;
    }
    Size = 0;
}
