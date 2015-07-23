//
//  main.cpp
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "PointVector.h"
#include "BucketVector.h"
#include "HashTable.h"

int kValue;
int dataDimension;
int dataCount;
int *tmpInt;
int **tmpIntStar;
int *dominateBucket;
char *bitmapStr;

struct HashTable *H;

int tmpSize = 0;
int SSize = 0; //Total data size.
int StwhSize = 0, SesSize = 0, SgSize = 0;
struct gtPoint *tmpInput, *tmpHead, *tmpTail;
struct gtPoint *S, *SHead, *STail;
struct gtPoint *Stwh, *StwhHead, *StwhTail;
struct gtPoint *Ses, *SesHead, *SesTail;
struct gtPoint *Sg, *SgHead, *SgTail;
struct gtBucket *tmpBucket = NULL;
struct gtBucket *firstBucket = NULL, *lastBucket = NULL;
struct ListNode *tmpListNode;


void inputData(int dataDimension, int dataCount) {
    int i, j;
    int bitValid;
    S = StartPoint(S, &SSize, &SHead, &STail, dataDimension);
    tmpIntStar = (int **)malloc(sizeof(int*) * dataCount);

    for (i = 0; i < dataCount; i++) {
        tmpInput = StartPoint(tmpInput, &tmpSize, &tmpHead, &tmpTail, dataDimension);
        tmpIntStar[i] = (int *)malloc(sizeof(int) * dataDimension);
        tmpInput->data = &(tmpIntStar[i]);
        tmpInput->bitmap = (char *)malloc(sizeof(char) * dataDimension);

        for(j = 0; j < dataDimension; j++) {
            *(*(tmpInput->data) + j) = rand() % 30;             // Input Actual Data
            //printf("%d ", *(*(tmpInput->data) + j));
        }
        //printf("\n");

        for(j = 0; j < dataDimension; j++) {            // Set Bit Map
            bitValid = rand() % 2;
            //printf("%d  ", bitValid);
            if (bitValid != 1)
                *(tmpInput->bitmap + j) = '0';
            else
                *(tmpInput->bitmap + j) = '1';
        }
        //printf("\n");

        PushPoint(tmpInput, &SSize, &STail);        // Push point to S
    }
}

int isPoint1DominatePoint2(struct gtPoint *p1, struct gtPoint *p2) {
    int dimension = p1->dimension;
    int smallCount = 0;
    int atLeastOneSmall = 0;
    int i, x1, x2;
    int x1IsNull, x2IsNull;
    if (!p1 || !p2) return 0;
    for (i = 0; i < dimension; i++) {
        x1 = *(*(p1->data)+i);
        x2 = *(*(p2->data)+i);
        x1IsNull = !(*(p1->bitmap + i));
        x2IsNull = !(*(p2->bitmap + i));
        if (x1IsNull || x2IsNull) {
            smallCount++;
        } else {
            if (x1 <= x2) smallCount++;
            if (x1 < x2) atLeastOneSmall = 1;
        }
    }
    if ((smallCount == dimension) && atLeastOneSmall) return 1;
    else return 0;
}

int gtSortAlgo(const struct gtPoint *v1, const struct gtPoint *v2) {
    return v1->domainatedCount > v2->domainatedCount;
}

void buildBucketHashTable(int depth) {
    if (depth == dataDimension) {
        tmpBucket = (struct gtBucket *)malloc(sizeof(struct gtBucket));  // 1 bucket + 3 point => 232 bytes
        InitBucket(tmpBucket, dataDimension);
        Insert(bitmapStr, H, dataDimension, tmpBucket, &firstBucket, &lastBucket);
        return;
    }
    *(bitmapStr + depth) = '0';
    buildBucketHashTable(depth + 1);
    *(bitmapStr + depth) = '1';
    buildBucketHashTable(depth + 1);
}

void thicknessWarehouse(int dataDimension, int kValue) {
    int i, j, k;
    int iterCount = 0, iterCountB;

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
    bitmapStr = (char *)malloc(sizeof(char) * dataDimension);
    //buildBucketHashTable(0);
    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
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
	
	while (tmpBucket != NULL) {
        tmpPoint = tmpBucket->data;
        tmpPointArray[0] = tmpPoint;
        for (j = 1; j < tmpBucket->dataSize; j++) {
            tmpPoint = tmpPoint->next;
            tmpPointArray[j] = tmpPoint;
        }
        for (j = 1; j < tmpBucket->dataSize; j++) {
            tmpPoint = tmpPointArray[j];
            for (k = 1; k < tmpBucket->dataSize; k++) {
                tmpPoint2 = tmpPointArray[k];
                if (j != k ) {
                    if (isPoint1DominatePoint2(tmpPoint2, tmpPoint)) {
                        tmpPoint->domainatedCount++;
                        if (tmpPoint->domainatedCount >= kValue) {
                            PushPoint(tmpPoint, &tmpBucket->SlnSize, &tmpBucket->SlnTail);
                            break;
                        }
                    }
                }
			}
            if (k == tmpBucket->dataSize) {
                //////////////////////////////////////////////////////////////////////////////////////////////////
                // [STEP 3]  Push Bucket.Sl -> Stwh
                // Origin:
                // for (int j = 0; j < bucket[i].SlSize(); j++) {
                //    PushPoint(bucket[i].Sl[j],&StwhSize,&StwhTail);
                // }
                PushPoint(tmpPoint, &StwhSize, &StwhTail);
                //////////////////////////////////////////////////////////////////////////////////////////////////
            }
		}
        tmpBucket = tmpBucket->next;
    }


    free(tmpPointArray);


    // [STEP 4] Push Swth -> Ses
    // std::sort(Stwh.begin(), Stwh.end(), gtSortAlgo);


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
            if (iterA->bitmap == iterB->bitmap ) {
                iterB = tmpPoint;
                continue;
            }
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
    // tmpBucket = bucket;
    // for (i = 0; i < bucketCount; i++) {
    //     test += tmpBucket->SlnSize - 1;
    //     tmpBucket = tmpBucket->next;
    // }
    // printf("%d %d %d", test, StwhSize - 1, SesSize - 1);    // sum should be total number of point  // success!
    //
    /////////////////////////////////////////////////////////////////////////////

    i = StwhSize;
    tmpPoint = Stwh;

    //[STEP 5] (Stwh, Ses) -> Sg
    /////////////////////////////////////////////////////////////////////////////////////

    int deleted = 0;

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
                    deleted++;  // for test
                    free(iterA);
                    iterCount--;
                    break;
                }
            }
            iterB = iterB->next;
        }
        iterA = tmpPointNext;
    }

    i = StwhSize;
    tmpPoint = Stwh;

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
                        free(iterA);
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


int main(int argc, const char * argv[]) {
    int i, j;

    scanf("%d %d", &dataDimension, &dataCount);
    inputData(dataDimension, dataCount);
    kValue = 1;
    thicknessWarehouse(dataDimension, kValue);

    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!\n");
    for (i = 1; i < SgSize; i++) {
        tmpInput = GetPoint(i, SgHead);
        for (j = 0; j < dataDimension; j++) {
            printf("%d ", *(*(tmpInput->data) + j));
        }
        printf("\n");
    }
    printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");

    return 0;
}
