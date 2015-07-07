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

gtPoint *Start(gtPoint * StartNode,int TotalSize){
	StartNode = (gtPoint *)malloc(sizeof(gtPoint));
	//StartNode = (gtPoint *)palloc(sizeof(gtPoint));
	//if (StartNode == NULL)
	//	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Cannot start a linked list."))));
	//else{
	Initialized(StartNode);
	Head = StartNode;
	Tail = StartNode;
	TotalSize = 1;
	//}
}

void Initialized(gtPoint *Node){
	Node-> pDs = 0;
    Node -> data = NULL;
    Node -> bitmap = 0;
    Node -> domainatedCount = 0;
	Node -> next = NULL;
	Node -> previous = NULL;
}

void Push(gtPoint * NewElement,int TotalSize){
	//NewElement = (gtPoint *)malloc(sizeof(gtPoint));
	//NewElement = (gtPoint *)palloc(sizeof(gtPoint));
	//if (NewElement == NULL)
	//	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Cannot start a linked list."))));
	//else{
	//Initialized(NewElement);
	Tail -> next = NewElement;
	NewElement -> previous = Tail;
	NewElement -> next = NULL;
	Tail = NewElement;
	TotalSize++;
}

gtPoint * GetNode(int Position,gtPoint * Head){
	gtPoint * Pointer;
	int i = 0;
	Pointer = Head;
	for (i = 0;i < Position; i++){
		if (Pointer -> next != NULL || i == Position - 1)
			Pointer = Pointer -> next;
		else break;
	}
	//if (i != Position)
	//	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Illegal GetNode."))));
	return Pointer;
}

void DeleteNode(int Position, gtPoint * Head, int TotalSize){
	gtPoint * Pointer;
	gtPoint * PrePointer;
	int i = 0;
	Pointer = Head;
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
	TotalSize--;
	//pfree(Pointer);
}

void FreeAll(gtPoint * Node){
	gtPoint * Pointer;
	gtPoint * FreePointer;
	Pointer = Node;
	FreePointer = Node;
	while (Pointer != NULL){
		Pointer = Pointer -> next;
		free(FreePointer);
		FreePointer = Pointer;
	}
	TotalSize = 0;
}
