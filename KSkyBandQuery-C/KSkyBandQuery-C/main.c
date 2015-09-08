//
//  main.cpp
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PointVector.h"
#include "BucketVector.h"
#include "HashTable.h"

int kValue;
int dataDimension;
int dataCount;
int *tmpInt;
double **tmpDoubleStar;
int tmpSize;
int SSize, StwhSize, SesSize, SgSize;

FILE *fin, *fout;

struct gtPoint *tmpInput, *tmpHead, *tmpTail;
struct gtPoint *S, *SHead, *STail;
struct gtPoint *Stwh, *StwhHead, *StwhTail;
struct gtPoint *Ses, *SesHead, *SesTail;
struct gtPoint *Sg, *SgHead, *SgTail;
struct gtBucket *tmpBucket;
struct gtBucket *firstBucket, *lastBucket;
struct ListNode *tmpListNode;
struct HashTable *H;

void inputData(int dataCount, int dataDimension) {
    int i, j;
    int bitValid;
    S = StartPoint(S, &SSize, &SHead, &STail, dataDimension);

    tmpDoubleStar = (double **)malloc(sizeof(double*) * dataCount);

    for (i = 0; i < dataCount; i++) {
        tmpInput = StartPoint(tmpInput, &tmpSize, &tmpHead, &tmpTail, dataDimension);
        tmpDoubleStar[i] = (double *)malloc(sizeof(double) * dataDimension);
        tmpInput->data = &(tmpDoubleStar[i]);
        tmpInput->bitmap = (char *)malloc(sizeof(char) * dataDimension);

        for(j = 0; j < dataDimension; j++)
            fscanf(fin, "%lf", (*(tmpInput->data) + j));// Input Actual Data

        for(j = 0; j < dataDimension; j++) {            // Set Bit Map
            fscanf(fin, "%d", &bitValid);
            if (bitValid != 1)
                *(tmpInput->bitmap + j) = '0';
            else
                *(tmpInput->bitmap + j) = '1';
        }

        PushPoint(tmpInput, &SSize, &STail);        // Push point to S
    }
}

int isPoint1DominatePoint2(struct gtPoint *p1, struct gtPoint *p2) {
    double x1, x2;
    int i;
    int dimension = p1->dimension;
    int smallCount = 0;
    int atLeastOneSmall = 0;
    int x1IsNull, x2IsNull;
    if (!p1 || !p2) return 0;
    for (i = 0; i < dimension; i++) {
        x1 = *(*(p1->data)+i);
        x2 = *(*(p2->data)+i);
        x1IsNull = (*(p1->bitmap + i)) == '0';
        x2IsNull = (*(p2->bitmap + i)) == '0';
        if (x1IsNull || x2IsNull) {
            smallCount++;
        } else {
            if (x1 <= x2) smallCount++;
            if (x1 < x2) atLeastOneSmall = 1;
        }
    }
    if ((smallCount == dimension) && atLeastOneSmall)
        return 1;
    else
        return 0;
}

int cmpFunc(const void *v1, const void *v2) {
    const struct gtPoint **t1 = (const struct gtPoint **)v1;
    const struct gtPoint **t2 = (const struct gtPoint **)v2;
    return (**t2).domainatedCount - (**t1).domainatedCount;
}

void QsortStwh(int size) {
    int i;
    struct gtPoint *pointArray[size];
    struct gtPoint *tmpP;
    if (kValue > 1) {
        tmpP = StwhHead;
        for (i = 0; i < size; i++) {
            pointArray[i] = tmpP->next;
            tmpP = tmpP->next;
        }
        qsort(pointArray, size, sizeof(pointArray[0]), cmpFunc);
        StwhHead->next = pointArray[0];
        pointArray[0]->previous = StwhHead;
        pointArray[0]->next = NULL;
        StwhTail = pointArray[0];
        for (i = 1; i < size; i++) {
            StwhTail->next = pointArray[i];
            pointArray[i]->previous = StwhTail;
            pointArray[i]->next = NULL;
            StwhTail = pointArray[i];
        }
    }
}

void thicknessWarehouse(int dataDimension, int kValue) {
    int i, j;
    int iterCount, iterCountB;

    struct gtPoint *iterA;
    struct gtPoint *iterB;
    struct gtPoint *tmpPoint = NULL;
    struct gtPoint *tmpPoint2 = NULL;
    struct gtPoint *tmpPointNext;
    struct gtPoint **tmpPointArray;

    Stwh = StartPoint(Stwh, &StwhSize, &StwhHead, &StwhTail, dataDimension);
    Ses = StartPoint(Ses, &SesSize, &SesHead, &SesTail, dataDimension);
    Sg = StartPoint(Sg, &SgSize, &SgHead, &SgTail, dataDimension);

    // [STEP 1] Push all points in S to every bucket according to bitmap

    ////////////////////////////////////////////////////
    // Origin: bucket = new gtBucket[bucketCount];
    H = InitializeTable(dataCount);
    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
    firstBucket = NULL;
    tmpPoint = S;
    tmpPointNext = tmpPoint->next;
    while (tmpPointNext != NULL) {
        tmpPoint = tmpPointNext;
        tmpPointNext = tmpPoint->next;
        tmpListNode = Find(tmpPoint->bitmap, H, dataDimension);
        if (tmpListNode == NULL) {
            tmpBucket = (struct gtBucket *)malloc(sizeof(struct gtBucket));  // 1 bucket + 3 point => 232 bytes
            InitBucket(tmpBucket, dataDimension);
            Insert(tmpPoint->bitmap, H, dataDimension, tmpBucket, &firstBucket, &lastBucket);
        } else {
            tmpBucket = tmpListNode->bucket;
        }
        PushPoint(tmpPoint, &tmpBucket->dataSize, &tmpBucket->dataTail);
    }
    ////////////////////////////////////////////////////

    // [STEP 2] Divide points in every bucket into Sl and Sln
    tmpBucket = firstBucket;
    tmpPointArray = (struct gtPoint **)malloc(sizeof(struct gtPoint*) * tmpBucket->dataSize);
    //if (tmpPointArray == NULL)
    //    ereport(ERROR, (errcode(ERRCODE_OUT_OF_MEMORY), errmsg("Step2: Cannot palloc PointArray for insert point into Sln")));
    while (tmpBucket != NULL) {
        tmpPoint = tmpBucket->data;
        tmpPointArray[0] = tmpPoint;
        for (i = 1; i < tmpBucket->dataSize; i++) {
            tmpPoint = tmpPoint->next;
            tmpPointArray[i] = tmpPoint;
        }
        for (i = 1; i < tmpBucket->dataSize; i++) {
            tmpPoint = tmpPointArray[i];
            for (j = 1; j < tmpBucket->dataSize; j++) {
                tmpPoint2 = tmpPointArray[j];
                if (i != j ) {
                    if (isPoint1DominatePoint2(tmpPoint2, tmpPoint)) {
                        tmpPoint->domainatedCount++;
                        if (tmpPoint->domainatedCount >= kValue) {
                            PushPoint(tmpPoint, &tmpBucket->SlnSize, &tmpBucket->SlnTail);
                            break;
                        }
                    }
                }
            }
            if (j == tmpBucket->dataSize) //which means data[j] is not dominted more than k times, then put it into Sl.
                PushPoint(tmpPoint, &StwhSize, &StwhTail);
        }
        tmpBucket = tmpBucket->next;
    }

    free(tmpPointArray);

    // [STEP 4] Push Swth -> Ses
    QsortStwh(StwhSize - 1);

    /////////////////////////////////////////////////////////////////////////////////////
    // Origin:
    // vector<gtPoint *>::iterator itHead, itTail;
    // for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++) {
    //    if(!*itHead) continue;
    //    for (itTail = Stwh.end(); itTail != Stwh.begin(); itTail--) {
    //        if(!*itTail) continue;
    //        if (isPoint1DominatePoint2(*itTail, *itHead)) (*itHead)->domainatedCount ++;
    //        if ((*itHead)->domainatedCount > kValue) {
    //            Ses.push_back(*itHead);
    //            Stwh.erase(itHead);
    //            break;
    //        }
    //        if (isPoint1DominatePoint2(*itHead, *itTail)) (*itTail)->domainatedCount ++;
    //        if ((*itTail)->domainatedCount > kValue) {
    //            Ses.push_back(*itTail);
    //            Stwh.erase(itTail);
    //        }
    //    }
    // }

    iterCount = 0;
    iterA = Stwh->next;
    while (iterA != NULL) {
        iterCount++;
        tmpPointNext = iterA->next;
        iterB = StwhTail;
        iterCountB = 0;
        while (iterB != StwhHead) {
            iterCountB++;
            tmpPoint = iterB->previous;
            if (sameBitmap(iterA->bitmap, iterB->bitmap, dataDimension))
                break;
            if (isPoint1DominatePoint2(iterB, iterA)) {
                iterA->domainatedCount++;
                if (iterA->domainatedCount >= kValue) {
                    DeletePoint(iterCount, &StwhHead, &StwhSize, &StwhTail);
                    PushPoint(iterA, &SesSize, &SesTail);
                    iterCount--;
                    break;
                }
            }
            if (isPoint1DominatePoint2(iterA, iterB)) {
                iterB->domainatedCount++;
                if (iterB->domainatedCount >= kValue) {
                    if (tmpPointNext == iterB) // if two nearby nodes, we delete the second, then update first node's next.
                        tmpPointNext = iterB->next;
                    DeletePoint(StwhSize - iterCountB, &StwhHead, &StwhSize, &StwhTail);
                    PushPoint(iterB, &SesSize, &SesTail);
                    iterCountB--;
                }
            }
            iterB = tmpPoint;
        }
        iterA = tmpPointNext;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Test:
    //
    // int test = 0;
    // tmpBucket = firstBucket;
    // while (tmpBucket != NULL) {
    //     test += tmpBucket->SlnSize - 1;
    //     tmpBucket = tmpBucket->next;
    // }
    // printf("%d %d %d", test, StwhSize - 1, SesSize - 1);    // sum should be total number of point  // success!
    //
    /////////////////////////////////////////////////////////////////////////////

    //[STEP 5] (Stwh, Ses) -> Sg
    /////////////////////////////////////////////////////////////////////////////////////

    // Stwh VS Ses
    iterCount = 0;
    iterA = Stwh->next;
    while (iterA != NULL) {
        iterCount++;
        tmpPointNext = iterA->next;
        iterB = Ses->next;
        while (iterB != NULL) {
            if (isPoint1DominatePoint2(iterB, iterA)) {
                iterA->domainatedCount++;
                if (iterA->domainatedCount >= kValue) {
                    DeletePoint(iterCount, &StwhHead, &StwhSize, &StwhTail);
                    iterCount--;
                    break;
                }
            }
            iterB = iterB->next;
        }
        iterA = tmpPointNext;
    }

    //////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Test:
    //
    // int test = 0;
    // tmpBucket = bucket;
    // for (i = 0; i < bucketCount; i++) {
    //     test += tmpBucket->SlnSize - 1;
    //     tmpBucket = tmpBucket->next;
    // }
    // printf("%d %d %d %d", test, deleted, StwhSize - 1, SesSize - 1);    // sum should be total number of point  // success!
    //
    /////////////////////////////////////////////////////////////////////////////

    // Stwh VS Sln
    iterCount = 0;
    iterA = Stwh->next;
    while (iterA != NULL) {
        iterCount++;
        tmpPointNext = iterA->next;
        tmpBucket = firstBucket;
        while (tmpBucket != NULL) {
            iterB = tmpBucket->Sln->next;
            while (iterB != NULL) {
                if (isPoint1DominatePoint2(iterB, iterA)) {
                    iterA->domainatedCount++;
                    if (iterA->domainatedCount >= kValue) {
                        DeletePoint(iterCount, &StwhHead, &StwhSize, &StwhTail);
                        iterCount--;
                        break;
                    }
                }
                iterB = iterB->next;
            }
            if (iterB != NULL) break;
            tmpBucket = tmpBucket->next;
        }
        iterA = tmpPointNext;
    }

    SgSize = StwhSize;
    SgHead = StwhHead;
    SgTail = StwhTail;
    Sg = Stwh;
}


int main(void) {
    int i, j;

    tmpSize = 0;
    SSize = 0;
    StwhSize = 0;
    SesSize = 0;
    SgSize = 0;
    firstBucket = NULL;
    lastBucket = NULL;

    fin = fopen("/Users/armour/Desktop/KSkyBandQuery/KSkyBandQuery-C/KSkyBandQuery-C/Test/stdin.txt", "r+");
    fscanf(fin, "%d %d %d", &dataCount, &dataDimension, &kValue);
    inputData(dataCount, dataDimension);
    fclose(fin);

    clock_t  start = clock(), diff;
    thicknessWarehouse(dataDimension, kValue);
    diff = clock() - start;
    unsigned long msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Skyline: %lu.%lus\n", msec/1000, msec%1000);

    fout = fopen("/Users/armour/Desktop/KSkyBandQuery/KSkyBandQuery-C/KSkyBandQuery-C/Test/skylineout.txt", "w+");
    for (i = 1; i < SgSize; i++) {
        tmpInput = GetPoint(i, SgHead);
        for (j = 0; j < dataDimension; j++)
            fprintf(fout, "%.6lf ", *(*(tmpInput->data) + j));
        fprintf(fout, "\n");
    }
    fclose(fout);

    return 0;
}
