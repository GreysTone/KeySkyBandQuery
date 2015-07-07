#include <stdio.h>

gtPoint *Tail;

gtPoint *Head;

//gtPoint *Now;

int TotalSize;

struct gtPoint {
				int pDs;            // point dimension
				int **data;
				int bitmap;
				int domainatedCount;
				gtPoint *next;
				gtPoint *previous;
};

struct gtBucket {
				int bitmap;
				vector <gtPoint *>data;
				bool **dominanceTable;
				vector <gtPoint *>Sl;
				vector <gtPoint *>Sln;
				vector <gtPoint *>StwhSes;
				gtBucket * next;
				gtBucket * previous;
}

gtBucket *StartBucket(gtBucket * StartBucket,int Size, gtBucket * BucketHead, gtBucket * BucketTail){
				StartBucket = (gtPoint *)malloc(sizeof(gtBucket));
					
				//StartNode = (gtPoint *)palloc(sizeof(gtPoint));
				//if (StartNode == NULL)
				//	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Cannot start a linked list."))));
				//else{
				InitBucket(StartBucket);
				BucketHead = StartBucket;
				BucketTail = StartBucket;
				TotalSize = 1;
				//}
}

void InitBucket(gtBucket *Bucket){
}

void Push(gtBucket * NewBucket,int Size, gtBucket *BucketTail){
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

gtBucket * GetBucket(int Position,gtBucket * BucketHead){
	gtBucket * Pointer;
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

void DeleteBucket(int Position, gtBucket * BucketHead, int Size){
	gtBucket * Pointer;
	gtBucket * PrePointer;
	int i = 0;
	Pointer = BucketHead;
	for (i = 0;i < Position; i++){
		if (Pointer = NULL){
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

void FreeAll(gtBucket * Bucket,int Size){
	gtBucket * Pointer;
	gtBucket * FreePointer;
	Pointer = Bucket;
	FreePointer = Bucket;
	while (Pointer != NULL){
		Pointer = Pointer -> next;
		free(FreePointer);
		FreePointer = Pointer;
	}
	Size = 0;
}
