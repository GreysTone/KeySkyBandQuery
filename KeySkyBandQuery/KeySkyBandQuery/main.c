//
//  main.cpp
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

//#include "Point.h"
//#include "Bucket.h"
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "PointVector.h"
#include "BucketVector.h"

int kValue;
int dataDimension;
int dataCount;
int *tmpInt;
int **tmpIntStar;

int *dominateBucket;

int tmpSize = 0;
int SSize = 0; //Total data size.
int StwhSize = 0, SesSize = 0, SgSize = 0;
struct gtPoint *tmpInput, *tmpHead, *tmpTail;
struct gtPoint *S, *SHead, *STail;
struct gtPoint *Stwh, *StwhHead, *StwhTail;
struct gtPoint *Ses, *SesHead, *SesTail;
struct gtPoint *Sg, *SgHead, *SgTail;
struct gtBucket *bucket;

void inputData(int dataDimension, int dataCount) {      // [!!!] Not catching failed
    int i, j;
    int bitValid;
    S = StartPoint(S, &SSize, &SHead, &STail, dataDimension);
    tmpIntStar = (int **)malloc(sizeof(int*) * dataCount);

    for (i = 0; i < dataCount; i++) {
        tmpInput = StartPoint(tmpInput, &tmpSize, &tmpHead, &tmpTail, dataDimension);
        tmpInput->dimension = dataDimension;

        tmpIntStar[i] = (int *)malloc(sizeof(int) * dataDimension);
        tmpInput->data = &(tmpIntStar[i]);

        for(j = 0; j < dataDimension; j++) {
            *(*(tmpInput->data) + j) = rand() % 30;             // Input Actual Data
            printf("%d ", *(*(tmpInput->data) + j));
        }
        printf("\n");

//        for(int j = 0; j < dataDimension; j++) {
//            printf("%d  ", *(*(tmpInput->data) + j));
//        }
//        printf("\n");

        tmpInput->bitmap = 0;

        for(j = 0; j < dataDimension; j++) {            // Set Bit Map
            bitValid = rand() % 2;
            printf("%d ", bitValid);
            if (bitValid != 1)  {
                *(*(tmpInput->data) + j) = 0;
                tmpInput->bitmap <<= 1;
            } else {
                tmpInput->bitmap <<= 1;
                tmpInput->bitmap |= 1;
            }
            //printf("%d ", tmpInput->bitmap);
        }
        printf("\n");

        //for(int j = 0; j < dataDimension; j++) {
        //    printf("%d\n", *(*(tmpInput->data) + j));
        //}

        PushPoint(tmpInput, &SSize, &STail);
        //S.push_back(tmpInput[i]);
    }

}

void printAllPoint() {
    int i, j;
    for (i = 0; i < SSize; i++) {
        tmpInput = GetPoint(i, SHead);
        for (j = 0; j < dataDimension; j++) {
            printf("%d", *(tmpInput->data[j]));
        }
    }
}

//Alan Ruijia has changed this function to suit vector. 
int isPoint1DominatePoint2(struct gtPoint *p1, struct gtPoint *p2) {
    int dimension = p1->dimension;
    int smallCount = 0;
    int atLeastOneSmall = 0; //C doesn't have bool type
    int i;
    int x1, x2;
    if (!p1 || !p2) return 0;
    for (i = 0; i < dimension; i++) {
        x1 = (*(p1->data)+i) == NULL? 0: *(*(p1->data)+i);
        x2 = (*(p2->data)+i) == NULL? 0: *(*(p2->data)+i);
        if (x1 <= x2) smallCount++;
        if (x1 < x2) atLeastOneSmall = 1;
    }
    if ((smallCount == dimension) && atLeastOneSmall) return 1;
    else return 0;
}

int gtSortAlgo(const struct gtPoint *v1, const struct gtPoint *v2) {
    return v1->domainatedCount > v2->domainatedCount;
}

void thicknessWarehouse(int dataDimension, int kValue) {
    int i, j, k, flag;
    int bucketSize = 0;
    int bucketCount = 1;
    int iterCount = 0;

    struct gtPoint *iterA;
    struct gtPoint *iterB;
    struct gtPoint *tmpPoint = NULL;
	struct gtPoint *tmpPoint2 = NULL;
	struct gtPoint *tmpPoint3 = NULL;
    struct gtPoint *tmpPointNext;
    struct gtPoint **tmpPointArray;
    struct gtBucket *tmpBucket = NULL;
    struct gtBucket *bucketHead, *bucketTail;

    Stwh = StartPoint(Stwh, &StwhSize, &StwhHead, &StwhTail, dataDimension);
    Ses = StartPoint(Ses, &SesSize, &SesHead, &SesTail, dataDimension);
    Sg = StartPoint(Sg, &SgSize, &SgHead, &SgTail, dataDimension);

    // [STEP 1] Push all points in S to every bucket according to bitmap
    for (i = 0; i < dataDimension; i++)
        bucketCount *= 2;   // !!!!!! Dimension should not larger then 30 !!!!!!!!

    ////////////////////////////////////////////////////
    // Origin: bucket = new gtBucket[bucketCount];
    bucket = StartBucket(bucket, &bucketSize, &bucketHead, &bucketTail, dataDimension);

    for (i = 0; i < bucketCount; i++) {
        tmpBucket = (struct gtBucket *)malloc(sizeof(struct gtBucket));
        InitBucket(tmpBucket, dataDimension);
        PushBucket(tmpBucket, &bucketSize, &bucketTail);
    }
    ////////////////////////////////////////////////////


    ////////////////////////////////////////////////////
    // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
    tmpPoint = S;
    tmpPointNext = tmpPoint->next;
    while (tmpPointNext != NULL) {
        tmpPoint = tmpPointNext;
        tmpPointNext = tmpPoint->next;
        tmpBucket = GetBucket(tmpPoint->bitmap, bucketHead);
        PushPoint(tmpPoint, &tmpBucket->dataSize, &tmpBucket->dataTail);
    }
    ////////////////////////////////////////////////////


    // [STEP 2] Divide points in every bucket into Sl and Sln
    tmpBucket = bucket;
    tmpPointArray = (struct gtPoint **)malloc(sizeof(struct gtPoint*) * tmpBucket->dataSize);
	
	for (i = 0; i < bucketCount; i++) {
        tmpBucket->bitmap = i;
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
            if (k == tmpBucket->dataSize) { // which means data[j] is not dominted more than k times, then put it into Sl.
                PushPoint(tmpPoint, &tmpBucket->SlSize, &tmpBucket->SlTail);
                // I think we do not need the above line code, because every point in Sl will be pushed
                // to Stwh, therefore, we can just put it to Stwh directly. is that right???

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
    tmpPointArray = (struct gtPoint **)malloc(sizeof(struct gtPoint*) * StwhSize);
    tmpInt = (int *)malloc(sizeof(int*) * StwhSize);
    tmpPoint = Stwh;
    tmpPointArray[0] = tmpPoint;
    tmpInt[0] = 1;
    for (i = 1; i < StwhSize; i++) {
        tmpPoint = tmpPoint->next;
        tmpPointArray[i] = tmpPoint;
        tmpInt[i] = 1;
    }
    for (int i = 1; i < StwhSize; i++) {
        iterA = tmpPointArray[i];
        if (!tmpInt[i]) continue;
        iterCount++;
        //iterCountB = 0;
        for (int j = StwhSize - 1; j >= 1; j--) {
            iterB = tmpPointArray[j];
            if (!tmpInt[j]) continue;
            //iterCountB++;
            if (iterA != iterB) {
                if (isPoint1DominatePoint2(iterB, iterA)) {
                    iterA->domainatedCount++;
                    if (iterA->domainatedCount >= kValue) {
                        DeletePoint(iterCount, &StwhHead, &StwhSize, &StwhTail);
                        PushPoint(iterA, &SesSize, &SesTail);
                        tmpInt[iterCount] = 0;
                        iterCount--;
                        break;
                    }
                }
                /*if (isPoint1DominatePoint2(iterA, iterB)) {
                    iterB->domainatedCount++;
                    if (iterB->domainatedCount >= kValue) {
                        DeletePoint(iterCountB, &StwhHead, &StwhSize, &StwhTail);
                        PushPoint(iterB, &SesSize, &SesTail);
                        tmpInt[iterCountB] = 0;
                    }
                }*/
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////


    //[STEP 5] (Stwh, Ses) -> Sg
    /////////////////////////////////////////////////////////////////////////////////////
    // Origin:
    // for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++) {
    //    for (itTail = Ses.begin(); itTail != Ses.end(); itTail++) {
    //        if (isPoint1DominatePoint2(*itTail, *itHead)) (*itHead)->domainatedCount ++;
    //        if ((*itHead)->domainatedCount > kValue) {
    //            Stwh.erase(itHead);
    //        }
    //    }
    // }
	/*
    iterCount = 0;
    iterA = Stwh;
    while (iterA->next != NULL) {
        iterA = iterA->next;
        iterCount++;
        iterB = Ses;
        while (iterB->next != NULL) {
            iterB = iterB->next;
            if (isPoint1DominatePoint2(iterB, iterA)) {
                iterA->domainatedCount++;
                if (iterA->domainatedCount >= kValue) {
                    DeletePoint(iterCount, &StwhHead, &StwhSize);
                    break;
                }
            }
        }
    }
	*/
    //////////////////////////////////////////////////////////////////////////////////////
	tmpPoint = Stwh->next;
	i = 1;
	
	do{ //To compare all the points in Stwh with all points in Stwh and Ses, to see whether they need to compare Slns in some Buckets.
		dominateBucket = (int *)malloc(bucketCount*sizeof(int));
		tmpPoint3 = tmpPoint->next;
		tmpPoint2 = Ses->next;
		while (tmpPoint2 != NULL){
			if (isPoint1DominatePoint2(tmpPoint2, tmpPoint)){
				tmpPoint->domainatedCount++;
				*(dominateBucket + tmpPoint2->bitmap)=1;
			}
			if (tmpPoint->domainatedCount >= kValue){
				tmpPoint->previous->next = tmpPoint->next;
				PushPoint(tmpPoint,SesSize,SesTail);
				free(dominateBucket);
				break;
			}
			tmpPoint2 = tmpPoint2->next;
		}
		if (tmpPoint2 == NULL){
			tmpPoint2 = Stwh->next;
			while (tmpPoint2 != NULL){
				if (isPoint1DominatePoint2(tmpPoint2, tmpPoint)){
					*(dominateBucket + tmpPoint2->bitmap) = 1;
				}
				tmpPoint2 = tmpPoint2->next;
			}
			for (j = 1; j < bucketCount; j++){
				if (*(dominateBucket + j) == 1){
					tmpPoint2 = (bucket + *(dominateBucket + j))->Sln->next;
					while (tmpPoint2 != NULL){
						if (isPoint1DominatePoint2(tmpPoint2, tmpPoint))
							tmpPoint->domainatedCount++;
						if (tmpPoint->domainatedCount >= kValue){
							tmpPoint->previous->next = tmpPoint->next;
							PushPoint(tmpPoint, SesSize, SesTail);
							free(dominateBucket);
							break;
						}
					}
					if (tmpPoint2 != NULL)
						break;
				}
			}
			if (j == bucketCount)
				PushPoint(tmpPoint, &SgSize, &SgTail);
		}
		tmpPoint = tmpPoint3;
	} while (tmpPoint);
    /*
    gtBucket *Stwh_b = new gtBucket [bucketCount];
    gtBucket *Ses_b  = new gtBucket [bucketCount];
    for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++)  (Stwh_b[(*itHead)->bitmap]).StwhSes.push_back((*itHead));
    for (itHead = Ses.begin(); itHead != Ses.end(); itHead++)  (Ses_b[(*itHead)->bitmap]).StwhSes.push_back((*itHead));

    for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++) {
        vector<gtPoint *> cmpS;
        for (int i = 0; i < bucketCount; i++) {
            if (i == (*itHead)->bitmap) continue;
            else {
                for (int j = 0; j < Stwh_b[i].StwhSes.size(); j++) cmpS.push_back(Stwh_b[i].StwhSes[j]);
                for (int j = 0; j < Ses_b[i].StwhSes.size(); j++) cmpS.push_back(Ses_b[i].StwhSes[j]);
            }
        }
        for (int i = 0; i < cmpS.size(); i++) {
            bool isIteratorErased = false;
            if (isPoint1DominatePoint2(*itHead, cmpS[i])) continue;
            else {
                for (int j = 0; j < bucket[cmpS[i]->bitmap].Sln.size(); j++) {
                    if (isPoint1DominatePoint2(bucket[cmpS[i]->bitmap].Sln[j], *itHead)) (*itHead)->domainatedCount ++;
                    if ((*itHead)->domainatedCount > kValue) {;
                        Stwh.erase(itHead);
                        isIteratorErased = true;
                        break;
                    }

                }
            }
            if (isIteratorErased) break;
        }
    }
    Sg = Stwh;
    */
}


int main(int argc, const char * argv[]) {
    /*
    cin >> dataCount >> kValue >> dataDimension;
    cout << "Count: [" << dataCount << "]data in " << dataDimension << "-Dimensions\n";
    cout << "K-Value: " << kValue << endl;
     */
    scanf("%d %d", &dataDimension, &dataCount);
    inputData(dataDimension, dataCount);
    kValue = 1;
    thicknessWarehouse(dataDimension, kValue);
    /*
    for (int i = 0; i < Sg.size(); i++) {
        Sg[i]->printPoint();
    }
    */
    return 0;
}
