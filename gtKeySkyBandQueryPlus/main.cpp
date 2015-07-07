//
//  main.cpp
//  gtKeySkyBandQueryPlus
//
//  Created by GreysTone on 6/25/15.
//  Copyright (c) 2015 GreysTone. All rights reserved.
//
//  Applying Algorithm: Thickness Warehouse
//
//

#include <iostream>
#include <vector>

#include "gtPoint.h"
#include "gtBucket.h"

using namespace std;



int32_t kValue;
int32_t dataDimension;
int32_t dataCount;

gtPoint *raw = NULL;
gtBucket *bucket = NULL;
vector<gtPoint *> S, Stwh, Ses, Sg;

void inputData(int32_t dataDimension, int32_t dataCount) {      // [!!!] Not catching failed
    raw = new gtPoint[dataCount];
    
    for (int32_t i = 0; i < dataCount; i++) {
        raw[i].pDs = dataDimension;
        
        raw[i].data = new int32_t *[dataDimension];
        for(int32_t j = 0; j < dataDimension; j++) {
            raw[i].data[j] = new int32_t;
            cin >> *(raw[i].data[j]);                           // Input Actual Data
        }
        
        for(int32_t j = 0; j < dataDimension; j++) {            // Set Bit Map
            int32_t bitValid;   cin >> bitValid;
            if (bitValid != 1)  {
                raw[i].data[j] = NULL;
                raw[i].bitmap <<= 1;
            } else {
                raw[i].bitmap <<= 1;
                raw[i].bitmap |= 1;
            }
        }
        
        S.push_back(raw + i);
    }
}

void printAllPoint() {
    for (int32_t i=0; i < S.size(); i++) {
        S[i] -> printPoint();
    }
}
bool inline isPoint1DominatePoint2(gtPoint *p1, gtPoint *p2) {
    if (!p1 || !p2) return false;
    int32_t dimension = p1->pDs;
    int32_t smallCount = 0;
    bool atLeastOneSmall = false;
    for (int32_t i = 0; i < dimension; i++) {
        int32_t x1= (p1->data[i])==NULL?0:*(p1->data[i]);
        int32_t x2= (p2->data[i])==NULL?0:*(p2->data[i]);
        if (x1 <= x2) smallCount++;
        if (x1 < x2) atLeastOneSmall = true;
    }
    
    if (smallCount == dimension && atLeastOneSmall) return true;
    else return false;
}

bool gtSortAlgo(const gtPoint *v1, const gtPoint *v2) {
    return v1->domainatedCount > v2->domainatedCount;
}

void thicknessWarehouse(int32_t dataDimension, int32_t kValue) {
    Stwh.clear(); Ses.clear(); Sg.clear();
    
    // [STEP 1]
    int32_t bucketCount = 1;
    for (int32_t i = 0; i < dataDimension; i++) bucketCount *= 2;

    bucket = new gtBucket[bucketCount];
    for (int i = 0; i < S.size(); i++) bucket[S[i]->bitmap].data.push_back(S[i]);
    
    // [STEP 2]
    for (int32_t i = 0; i < bucketCount; i++) {
        bucket[i].bitmap = i;                       // Set Bitmap

        size_t dataSize = bucket[i].data.size();
        bucket[i].dominanceTable = new bool *[dataSize];
        for (int j = 0; j < dataSize; j++) bucket[i].dominanceTable[j] = new bool [dataSize];
        for (int j = 0; j < dataSize; j++) {
            for (int k = j+1; k < dataSize; k++) {
                bucket[i].dominanceTable[j][k] = isPoint1DominatePoint2(bucket[i].data[j], bucket[i].data[k]);
                bucket[i].dominanceTable[k][j] = !bucket[i].dominanceTable[j][k];
            }
            bucket[i].dominanceTable[j][j] = 0;
        }

        //Calculate Dominance Count
        for (int j = 0; j < dataSize; j++) {
            for (int k = j+1; k < dataSize; k++) {
                if (bucket[i].dominanceTable[j][k]) {
                    bucket[i].data[k]->domainatedCount++;
                }
            }
        }
        // Bucket data into Sl & Sln
        for (int j = 0; j < dataSize; j++) {
            if (bucket[i].data[j]->domainatedCount == kValue) bucket[i].Sln.push_back(bucket[i].data[j]);
            else bucket[i].Sl.push_back(bucket[i].data[j]);
        }
        bucket[i].data.clear();
        
        // [STEP 3] Push Bucket.Sl -> Stwh
        for (int j = 0; j < bucket[i].Sl.size(); j++) Stwh.push_back(bucket[i].Sl[j]);
        
        
    }
    
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
        for (int32_t i = 0; i < bucketCount; i++) {
            if (i == (*itHead)->bitmap) continue;
            else {
                for (int j = 0; j < Stwh_b[i].StwhSes.size(); j++) cmpS.push_back(Stwh_b[i].StwhSes[j]);
                for (int j = 0; j < Ses_b[i].StwhSes.size(); j++) cmpS.push_back(Ses_b[i].StwhSes[j]);
            }
        }
        for (int32_t i = 0; i < cmpS.size(); i++) {
            bool isIteratorErased = false;
            if (isPoint1DominatePoint2(*itHead, cmpS[i])) continue;
            else {
                for (int32_t j = 0; j < bucket[cmpS[i]->bitmap].Sln.size(); j++) {
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
}


int main(int argc, const char * argv[]) {
    
    cin >> dataCount >> kValue >> dataDimension;
    cout << "Count: [" << dataCount << "]data in " << dataDimension << "-Dimensions\n";
    cout << "K-Value: " << kValue << endl;
    
    inputData(dataDimension, dataCount);
    
    thicknessWarehouse(dataDimension, kValue);
    
    for (int i = 0; i < Sg.size(); i++) {
        Sg[i]->printPoint();
    }
    
    return 0;
}
