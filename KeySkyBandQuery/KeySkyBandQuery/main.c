//
//  main.cpp
//  KeySkyBandQuery
//
//  Created by Armour on 7/10/15.
//  Copyright (c) 2015 ZJU. All rights reserved.
//

//#include "Point.h"
//#include "Bucket.h"
#include "stdio.h"
#include "stdlib.h"
#include "PointVector.h"
#include "BucketVector.h"

int kValue;
int dataDimension;
int dataCount;

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
		int i,j;
    int bitValid;
    S = StartPoint(S, &SSize, &SHead, &STail, dataDimension);
    tmpInput = StartPoint(tmpInput, &tmpSize, &tmpHead, &tmpTail, dataDimension);

    for (i = 0; i < dataCount; i++) {
        tmpInput->dimension = dataDimension;

        //*(tmpInput->data) = (int *)malloc(sizeof(int) * dataDimension);
        for(j = 0; j < dataDimension; j++) {
            *(*(tmpInput->data) + j) = 233;             // Input Actual Data
            printf("%d \n", *(*(tmpInput->data) + j));
        }

        //for(int j = 0; j < dataDimension; j++) {
        //    printf("%d \n", *(*(tmpInput->data) + j));
        //}

        tmpInput->bitmap = 0;

        for(j = 0; j < dataDimension; j++) {            // Set Bit Map
            //cin >> bitValid;
            bitValid = j % 2;
            if (bitValid != 1)  {
                *(*(tmpInput->data) + j) = 0;
                tmpInput->bitmap <<= 1;
                //tmpInput->bitmap &= 0;
            } else {
                tmpInput->bitmap <<= 1;
                tmpInput->bitmap |= 1;
            }
            //printf("%d ", tmpInput->bitmap);
        }

        //for(int j = 0; j < dataDimension; j++) {
        //    printf("%d\n", *(*(tmpInput->data) + j));
        //}

        PushPoint(tmpInput, &SSize, &STail);
        //S.push_back(tmpInput[i]);
    }
}

void printAllPoint() {
		int i,j;
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
        x1 = (p1->data[i]) == NULL? 0: *(p1->data[i]);
        x2 = (p2->data[i]) == NULL? 0: *(p2->data[i]);
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
		int i,j,k;
    struct gtPoint *tmpPoint = NULL;
    struct gtBucket *tmpBucket;

    int bucketSize = 0;
    int bucketCount = 1;
    struct gtBucket *bucketHead, *bucketTail;

    Stwh = StartPoint(Stwh, &StwhSize, &StwhHead, &StwhTail, dataDimension);
    Ses = StartPoint(Ses, &SesSize, &SesHead, &SesTail, dataDimension);
    Sg = StartPoint(Sg, &SgSize, &SgHead, &SgTail, dataDimension);

    // [STEP 1]
    for (i = 0; i < dataDimension; i++)
        bucketCount *= 2;

    ////////////////////////////////////////////////////
    // Origin: bucket = new gtBucket[bucketCount];
    bucket = StartBucket(bucket, &bucketSize, &bucketHead, &bucketTail);

    for (i = 0; i < bucketCount; i++) {
        tmpBucket = (struct gtBucket *)malloc(sizeof(struct gtBucket));
        InitBucket(tmpBucket);
        PushBucket(tmpBucket, &bucketSize, &bucketTail);
    }
    ////////////////////////////////////////////////////
		/*
    for (int i = 0; i < SSize; i++){
        ////////////////////////////////////////////////////
        // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
        tmpPoint = GetPoint(i, SHead);
        tmpBucket = GetBucket(tmpPoint->bitmap, bucketHead);
        //Armour: tmpBucket->data = tmpPoint;There seems to have some problems.
				tmpBucket->data->PushPoint(tmpPoint,&i,&bucketTail);   
				////////////////////////////////////////////////////
		}*/
    
		for (i = 0; i < SSize; ){ //i will automatically increment in tmpBucket->data->PushPint();
        ////////////////////////////////////////////////////
        // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
        tmpPoint = GetPoint(i, SHead);
        tmpBucket = GetBucket(tmpPoint->bitmap, bucketHead);
        //Armour: tmpBucket->data = tmpPoint;There seems to have some problems.
				//tmpBucket->data->PushPoint(tmpPoint,&i,&bucketTail);   
				//PushPoint(tmpPoint,&i,&bucketTail);   
				////////////////////////////////////////////////////

		}

    // [STEP 2]
    for (i = 0; i < bucketCount; i++) {
        ////////////////////////////////////////////////////
        // Origin: bucket[i].bitmap = i;
        tmpBucket = GetBucket(i, bucketHead);
        tmpBucket->bitmap = i;                      // Set Bitmap
        ////////////////////////////////////////////////////
/* Original:
        //size_t dataSize = bucket[i].data.size();
        //bucket[i].dominanceTable = new bool *[dataSize];
        //for (int j = 0; j < dataSize; j++) bucket[i].dominanceTable[j] = new bool [dataSize];
        for (int j = 0; j < dataSize; j++) {
            for (int k = j+1; k < dataSize; k++) {
                bucket[i].dominanceTable[j][k] = isPoint1DominatePoint2(&bucket[i].data[j], &bucket[i].data[k]);
            }
            bucket[i].dominanceTable[j][j] = 0;
        }
*/
        for (j = 0; j < SSize; j++) {
            for (k = j+1; k < SSize; k++) {
                bucket[i].dominanceTable[j][k] = isPoint1DominatePoint2(&bucket[i].data[j], &bucket[i].data[k]);
                bucket[i].dominanceTable[k][j] = isPoint1DominatePoint2(&bucket[i].data[k], &bucket[i].data[j]);
            }
            bucket[i].dominanceTable[j][j] = 0;
        }
/* Original
        //Calculate Dominance Count
        for (int j = 0; j < dataSize; j++) {
            for (int k = j+1; k < dataSize; k++) {
                if (bucket[i].dominanceTable[j][k]) {
                    bucket[i].data[k]->domainatedCount++;
                }
            }
        }
*/ 
        //Calculate Dominance Count
        for (j = 0; j < SSize; j++) {
            for (k = 0; k < SSize; k++) { //Changed
                if (bucket[i].dominanceTable[j][k]) {
                    bucket[i].data[k]->domainatedCount++;
                }
            }
        }

/*Original:
        // Bucket data into Sl & Sln
        for (int j = 0; j < dataSize; j++) {
        // Bucket data into Sl & Sln
        for (int j = 0; j < dataSize; j++) {
            if (bucket[i].data[j]->domainatedCount == kValue) bucket[i].Sln.push_back(bucket[i].data[j]);
            else bucket[i].Sl.push_back(bucket[i].data[j]);
        }
        bucket[i].data.clear();
        // [STEP 3] Push Bucket.Sl -> Stwh
        for (int j = 0; j < bucket[i].Sl.size(); j++) Stwh.push_back(bucket[i].Sl[j]);
    }
*/
        // Bucket data into Sl & Sln
        for (j = 0; j < SSize; j++) {
        // Bucket data into Sl & Sln
        for (j = 0; j < SSize; j++) {
            if (bucket[i].data[j]->domainatedCount >= kValue) bucket[i].Sln.push_back(bucket[i].data[j]);
            else bucket[i].Sl.push_back(bucket[i].data[j]);
        }
        //Original: bucket[i].data.clear();
				FreeAllPoints(bucket[i].data,bucket.data.SizePoint(bucket[i].data));
        // [STEP 3] Push Bucket.Sl -> Stwh
        //for (int j = 0; j < bucket[i].Sl.size(); j++) Stwh.push_back(bucket[i].Sl[j]);
    }
/*
    // [STEP 4] Push Swth -> Ses
    std::sort(Stwh.begin(), Stwh.end(), gtSortAlgo);
    vector<gtPoint *>::iterator itHead, itTail;
    for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++) {
        if(!*itHead) continue;
        for (itTail = Stwh.end(); itTail != Stwh.begin(); itTail--) {
            if(!*itTail) continue;
            if (isPoint1DominatePoint2(*itTail, *itHead)) (*itHead)->domainatedCount ++;
            if ((*itHead)->domainatedCount > kValue) {
                Ses.push_back(*itHead);
                Stwh.erase(itHead);
                break;
            }
            if (isPoint1DominatePoint2(*itHead, *itTail)) (*itTail)->domainatedCount ++;
            if ((*itTail)->domainatedCount > kValue) {
                Ses.push_back(*itTail);
                Stwh.erase(itTail);
            }
        }
    }

    // [STEP 5] (Stwh, Ses) -> Sg
    for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++) {
        for (itTail = Ses.begin(); itTail != Ses.end(); itTail++) {
            if (isPoint1DominatePoint2(*itTail, *itHead)) (*itHead)->domainatedCount ++;
            if ((*itHead)->domainatedCount > kValue) {
                Stwh.erase(itHead);
            }
        }
    }
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
