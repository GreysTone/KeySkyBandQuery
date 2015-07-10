#include <stdio.h>
#include <stdlib.h>
#include "Point.h"

struct gtPoint *Tail;
struct gtPoint *Head;
//gtPoint *Now;

int TotalSize;

void Initialized(struct gtPoint *Node){
    Node -> dimension = 0;
    Node -> data = NULL;
    Node -> bitmap = 0;
    Node -> domainatedCount = 0;
    Node -> next = NULL;
    Node -> previous = NULL;
}

struct gtPoint *Start(struct gtPoint *StartNode, int TotalSize){
    StartNode = (struct gtPoint *)malloc(sizeof(struct gtPoint));
    //StartNode = (gtPoint *)palloc(sizeof(gtPoint));
    //if (StartNode == NULL)
    //	ereport(ERROR,(errcode(ERROR_OUT_OF_MEMORY_ERROR,errmsg("Cannot start a linked list."))));
    //else{
    Initialized(StartNode);
    Head = StartNode;
    Tail = StartNode;
    TotalSize = 1;
    //}
    return StartNode;
}

void PushPoint(struct gtPoint * NewElement, int TotalSize){
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

struct gtPoint * GetNode(int Position,struct gtPoint * Head){
    struct gtPoint * Pointer;
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

void DeleteNode(int Position, struct gtPoint * Head, int TotalSize){
    struct gtPoint * Pointer;
    int i = 0;
    Pointer = Head;
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
    TotalSize--;
    //pfree(Pointer);
}

void FreeAllPoints(struct gtPoint * Node){
    struct gtPoint * Pointer;
    struct gtPoint * FreePointer;
    Pointer = Node;
    FreePointer = Node;
    while (Pointer != NULL){
        Pointer = Pointer -> next;
        free(FreePointer);
        FreePointer = Pointer;
    }
    TotalSize = 0;
}
