//
//  main.cpp
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 Armour. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PointList.h"
#include "BucketList.h"
#include "HashTable.h"

int kValue;
int dataDimension;
int dataCount;
int tmpSize, SSize, StwhSize, SesSize, SgSize;
int *tmpInt;
double **tmpDoubleStar;

struct skyPoint *tmpInput, *tmpTail;
struct skyPoint *SHead, *STail;
struct skyPoint *StwhHead, *StwhTail;
struct skyPoint *SesHead, *SesTail;
struct skyPoint *SgHead, *SgTail;
struct skyBucket *tmpBucket, *firstBucket, *lastBucket;
struct ListNode *tmpListNode;
struct HashTable *H;

FILE *fin, *fout;

void inputData(int dataCount, int dataDimension) {
    int i, j;
    int bitmap;
    SHead = StartPoint(&SSize, &SHead, &STail, dataDimension);

    tmpDoubleStar = (double **)malloc(sizeof(double*) * dataCount);

    for (i = 0; i < dataCount; i++) {
        tmpInput = StartPoint(&tmpSize, &tmpInput, &tmpTail, dataDimension);
        tmpDoubleStar[i] = (double *)malloc(sizeof(double) * dataDimension);
        tmpInput->data = &(tmpDoubleStar[i]);
        tmpInput->bitmap = (char *)malloc(sizeof(char) * dataDimension);

        for(j = 0; j < dataDimension; j++)
            fscanf(fin, "%lf", (*(tmpInput->data) + j));// Input Actual Data

        for(j = 0; j < dataDimension; j++) {            // Set Bit Map
            fscanf(fin, "%d", &bitmap);
            if (bitmap != 1)
                *(tmpInput->bitmap + j) = '0';
            else
                *(tmpInput->bitmap + j) = '1';
        }

        PushPoint(tmpInput, &SSize, &STail);        // Push point to S
    }
}

int isPoint1DominatePoint2(struct skyPoint *p1, struct skyPoint *p2) {
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
    const struct skyPoint **t1 = (const struct skyPoint **)v1;
    const struct skyPoint **t2 = (const struct skyPoint **)v2;
    return (**t2).dominatedCount - (**t1).dominatedCount;
}

void QsortStwh(int size) {
    int i;
    struct skyPoint *pointArray[size];
    struct skyPoint *tmpP;
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

    struct skyPoint *iterA;
    struct skyPoint *iterB;
    struct skyPoint *tmpPoint = NULL;
    struct skyPoint *tmpPoint2 = NULL;
    struct skyPoint *tmpPointNext;
    struct skyPoint **tmpPointArray;

    StwhHead = StartPoint(&StwhSize, &StwhHead, &StwhTail, dataDimension);
    SesHead = StartPoint(&SesSize, &SesHead, &SesTail, dataDimension);
    SgHead = StartPoint(&SgSize, &SgHead, &SgTail, dataDimension);

    // [STEP 1] Push all points in S to every bucket according to bitmap

    ////////////////////////////////////////////////////
    // Origin: bucket = new skyBucket[bucketCount];
    H = InitTable(dataCount);
    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
    firstBucket = NULL;
    tmpPoint = SHead;
    tmpPointNext = tmpPoint->next;
    while (tmpPointNext != NULL) {
        tmpPoint = tmpPointNext;
        tmpPointNext = tmpPoint->next;
        tmpListNode = Find(tmpPoint->bitmap, H, dataDimension);
        if (tmpListNode == NULL) {
            tmpBucket = (struct skyBucket *)malloc(sizeof(struct skyBucket));  // 1 bucket + 3 point => 232 bytes
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
    tmpPointArray = (struct skyPoint **)malloc(sizeof(struct skyPoint*) * tmpBucket->dataSize);
    while (tmpBucket != NULL) {
        tmpPoint = tmpBucket->dataHead;
        tmpPointArray[0] = tmpPoint;
        for (i = 0; i < tmpBucket->dataSize; i++) {
            tmpPoint = tmpPoint->next;
            tmpPointArray[i] = tmpPoint;
        }
        for (i = 0; i < tmpBucket->dataSize; i++) {
            tmpPoint = tmpPointArray[i];
            for (j = 0; j < tmpBucket->dataSize; j++) {
                tmpPoint2 = tmpPointArray[j];
                if (i != j ) {
                    if (isPoint1DominatePoint2(tmpPoint2, tmpPoint)) {
                        tmpPoint->dominatedCount++;
                        if (tmpPoint->dominatedCount>= kValue) {
                            PushPoint(tmpPoint, &tmpBucket->slnSize, &tmpBucket->slnTail);
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
    QsortStwh(StwhSize);

    /////////////////////////////////////////////////////////////////////////////////////
    // Origin:
    // vector<skyPoint *>::iterator itHead, itTail;
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
    iterA = StwhHead->next;
    while (iterA != NULL) {
        iterCount++;
        tmpPointNext = iterA->next;
        iterB = StwhTail;
        iterCountB = 0;
        while (iterB != StwhHead) {
            iterCountB++;
            tmpPoint = iterB->previous;
            if (SameBitmap(iterA->bitmap, iterB->bitmap, dataDimension))
                break;
            if (isPoint1DominatePoint2(iterB, iterA)) {
                iterA->dominatedCount++;
                if (iterA->dominatedCount >= kValue) {
                    DeletePoint(iterCount, &StwhSize, &StwhHead, &StwhTail);
                    PushPoint(iterA, &SesSize, &SesTail);
                    iterCount--;
                    break;
                }
            }
            if (isPoint1DominatePoint2(iterA, iterB)) {
                iterB->dominatedCount++;
                if (iterB->dominatedCount >= kValue) {
                    if (tmpPointNext == iterB) // if two nearby nodes, we delete the second, then update first node's next.
                        tmpPointNext = iterB->next;
                    DeletePoint(StwhSize - iterCountB + 1, &StwhSize, &StwhHead, &StwhTail);
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
    iterA = StwhHead->next;
    while (iterA != NULL) {
        iterCount++;
        tmpPointNext = iterA->next;
        iterB = SesHead->next;
        while (iterB != NULL) {
            if (isPoint1DominatePoint2(iterB, iterA)) {
                iterA->dominatedCount++;
                if (iterA->dominatedCount >= kValue) {
                    DeletePoint(iterCount, &StwhSize, &StwhHead, &StwhTail);
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
    iterA = StwhHead->next;
    while (iterA != NULL) {
        iterCount++;
        tmpPointNext = iterA->next;
        tmpBucket = firstBucket;
        while (tmpBucket != NULL) {
            iterB = tmpBucket->slnHead->next;
            while (iterB != NULL) {
                if (isPoint1DominatePoint2(iterB, iterA)) {
                    iterA->dominatedCount++;
                    if (iterA->dominatedCount >= kValue) {
                        DeletePoint(iterCount, &StwhSize, &StwhHead, &StwhTail);
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
    SgHead = StwhHead;
}


void init() {
    tmpSize = 0;
    SSize = 0;
    StwhSize = 0;
    SesSize = 0;
    SgSize = 0;
    firstBucket = NULL;
    lastBucket = NULL;
}

void input() {
    fin = fopen("/Users/armour/Desktop/KSkyBandQuery/KSkyBandQuery-C/KSkyBandQuery-C/Test/stdin.txt", "r+");
    fscanf(fin, "%d %d %d", &dataCount, &dataDimension, &kValue);
    inputData(dataCount, dataDimension);
    fclose(fin);
}

void kisb() {
    clock_t  start = clock(), diff;
    thicknessWarehouse(dataDimension, kValue);
    diff = clock() - start;
    unsigned long msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Skyline: %lus %lums\n", msec/1000, msec%1000);
}

void output() {
    int i, j;
    fout = fopen("/Users/armour/Desktop/KSkyBandQuery/KSkyBandQuery-C/KSkyBandQuery-C/Test/skylineout.txt", "w+");
    tmpInput = SgHead;
    for (i = 0; i < SgSize; i++) {
        tmpInput = tmpInput->next;
        for (j = 0; j < dataDimension; j++)
            fprintf(fout, "%.6lf ", *(*(tmpInput->data) + j));
        fprintf(fout, "\n");
    }
    fclose(fout);
}

int main(void) {
    init();
    input();
    kisb();
    output();
    return 0;
}
