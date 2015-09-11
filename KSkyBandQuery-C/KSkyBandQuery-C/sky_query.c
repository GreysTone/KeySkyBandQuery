/*-------------------------------------------------
 *
 *  sky_query.h
 *  KeySkyBandQuery
 *
 *  Created by Armour on 7/10/15.
 *  Copyright (c) 2015 Armour. All rights reserved.
 *
 *-------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sky_point_list.h"
#include "sky_bucket_list.h"
#include "sky_hashtable.h"

int sky_k;                  /* The k value that in skyband query */
int sky_dim;                /* The dimension of point in skyband query */
int sky_cnt;                /* The number of the points in skyband query */

double **tmp_pointer;

int s_size;                     /* Point number in S */
int stwh_size;                  /* Point number in Stwh */
int ses_size;                   /* Point number in Ses */
int sg_size;                    /* Point number in Sg */
int tmp_size;

SkyPoint *s_head, *s_tail;             /* Head/tail point for S */
SkyPoint *stwh_head, *stwh_tail;       /* Head/tail point for Stwh */
SkyPoint *ses_head, *ses_tail;         /* Head/tail point for Ses */
SkyPoint *sg_head, *sg_tail;           /* Head/tail point for Sg */
SkyPoint *tmp_head, *tmp_tail;

SkyBucket *first_bucket, *last_bucket;         /* First and last bucket for all bitmap that already knows */
SkyBucket *tmp_bucket;

HashTable *h;                           /* Hashtable for skyband query */
ListNode *tmp_listnode;

FILE *fin, *fout;                       /* Used for testing */

/*
 * Function: InputPoints
 * -------------------
 *   Input all points (include data and bitmap)
 *
 *   sky_cnt: The k value that in skyband query
 *   sky_dim: The dimension of point in skyband query
 *
 *   returns: void
 */

void InputPoints() {
    int i, j;
    int bitmap;

    s_head = StartPoint(&s_size, &s_head, &s_tail, sky_dim);            /* Create head point of S */

    tmp_pointer = (double **)malloc(sizeof(double*) * sky_cnt);

    for (i = 0; i < sky_cnt; i++) {
        tmp_head = StartPoint(&tmp_size, &tmp_head, &tmp_tail, sky_dim);        /* Create temp input point */
        tmp_pointer[i] = (double *)malloc(sizeof(double) * sky_dim);            /* Malloc memory to input point's data */
        tmp_head->data = &(tmp_pointer[i]);
        tmp_head->bitmap = (char *)malloc(sizeof(char) * sky_dim);              /* Malloc memory to input point's bimap */

        for(j = 0; j < sky_dim; j++)
            fscanf(fin, "%lf", (*(tmp_head->data) + j));                /* Input point's data from file */

        for(j = 0; j < sky_dim; j++) {                                  /* Input point's bitmap from file */
            fscanf(fin, "%d", &bitmap);
            if (bitmap != 1)
                *(tmp_head->bitmap + j) = '0';
            else
                *(tmp_head->bitmap + j) = '1';
        }

        PushPoint(tmp_head, &s_size, &s_tail);                          /* Push point to S */
    }
}

/*
 * Function: IsP1DominateP2
 * -------------------
 *   To judge weather point p2 is dominated by point p1
 *
 *   p1: The point used to compare with p2
 *   p2: The point used to compare with p1
 *
 *   returns:
 *      if p1 dominate p2 return 1
 *                   else return 0
 */

int IsP1DominateP2(SkyPoint *p1, SkyPoint *p2) {
    double x1, x2;
    int i;
    int dim = p1->dim;
    int is_small = 0;                       /* Used to decide if there exist one dimension that p1 is smaller than p2 */
    int cnt_small_or_equal = 0;             /* Count of dimension number that p1 is smaller than or equal to p2 in such dimension */
    int x1IsNull, x2IsNull;
    if (!p1 || !p2) return 0;               /* If p1 or p2 is not exist */
    for (i = 0; i < dim; i++) {             /* Look at each dimension */
        x1 = *(*(p1->data)+i);
        x2 = *(*(p2->data)+i);
        x1IsNull = (*(p1->bitmap + i)) == '0';
        x2IsNull = (*(p2->bitmap + i)) == '0';
        if (x1IsNull || x2IsNull) {         /* If p1 or p2's bitmap is '0', which means incomplete data */
            cnt_small_or_equal++;
        } else {                            /* Else just do comparation */
            if (x1 <= x2) cnt_small_or_equal++;
            if (x1 < x2) is_small = 1;
        }
    }
    if ((cnt_small_or_equal == dim) && is_small)        /* All dimension not larger than p2 and exist one dimension is smaller than p2 */
        return 1;
    else
        return 0;
}

/*
 * Function: CmpFunc
 * -------------------
 *   Compare function that used in QsortStwh function
 *
 *   p1: The point used to compare with p2
 *   p2: The point used to compare with p1
 *
 *   returns:
 *      if p1's cnt_domi is smaller than p2's, return > 0
 *      if p2's cnt_domi is smaller than p1's, return < 0
 *                                        else return = 0
 */

int CmpFunc(const void *p1, const void *p2) {
    const SkyPoint **t1 = (const SkyPoint **)p1;
    const SkyPoint **t2 = (const SkyPoint **)p2;
    return (**t2).cnt_domi - (**t1).cnt_domi;
}

/*
 * Function: QsortStwh
 * -------------------
 *   Qsort on Stwh list, first we make a copy of list as an array,
 *   after we finished the qsort on array, we'll put it back to list
 *
 *   n: The size of Stwh list (array)
 *
 *   returns: void
 */

void QsortStwh(int n) {
    int i;
    SkyPoint *point_array[n];                       /* Dynamically alloc memory for an array with size n */
    SkyPoint *tmp_point;
    if (sky_k > 1) {                                /* Only need qsort when sky_k is larger than 1 */
        tmp_point = stwh_head;
        for (i = 0; i < n; i++) {                   /* Build the array */
            point_array[i] = tmp_point->next;
            tmp_point = tmp_point->next;
        }
        qsort(point_array, n, sizeof(point_array[0]), CmpFunc);         /* Qsort this array */
        stwh_head->next = point_array[0];           /* Put first point in array back to list */
        point_array[0]->prev = stwh_head;
        point_array[0]->next = NULL;
        stwh_tail = point_array[0];
        for (i = 1; i < n; i++) {                   /* Put rest points in array back to list */
            stwh_tail->next = point_array[i];
            point_array[i]->prev = stwh_tail;
            point_array[i]->next = NULL;
            stwh_tail = point_array[i];
        }
    }
}

/*
 * Function: ThicknessWarehouse
 * -------------------
 *
 *
 *   returns: void
 */

void ThicknessWarehouse() {
    int i, j;
    int iterCount, iterCountB;

    SkyPoint *iterA;
    SkyPoint *iterB;
    SkyPoint *tmp_pointoint = NULL;
    SkyPoint *tmp_pointoint2 = NULL;
    SkyPoint *tmp_pointointNext;
    SkyPoint **tmp_pointointArray;

    stwh_head = StartPoint(&stwh_size, &stwh_head, &stwh_tail, sky_dim);
    ses_head = StartPoint(&ses_size, &ses_head, &ses_tail, sky_dim);
    sg_head = StartPoint(&sg_size, &sg_head, &sg_tail, sky_dim);

    // [STEP 1] Push all points in S to every bucket according to bitmap

    ////////////////////////////////////////////////////
    // Origin: bucket = new SkyBucket[bucketCount];
    h = InitTable(sky_cnt);
    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // Origin: bucket[S[i]->bitmap].data.push_back(S[i]);
    first_bucket = NULL;
    tmp_pointoint = s_head;
    tmp_pointointNext = tmp_pointoint->next;
    while (tmp_pointointNext != NULL) {
        tmp_pointoint = tmp_pointointNext;
        tmp_pointointNext = tmp_pointoint->next;
        tmp_listnode = Find(tmp_pointoint->bitmap, h, sky_dim);
        if (tmp_listnode == NULL) {
            tmp_bucket = (SkyBucket *)malloc(sizeof(SkyBucket));  // 1 bucket + 3 point => 232 bytes
            InitBucket(tmp_bucket, sky_dim);
            Insert(tmp_pointoint->bitmap, h, sky_dim, tmp_bucket, &first_bucket, &last_bucket);
        } else {
            tmp_bucket = tmp_listnode->bucket;
        }
        PushPoint(tmp_pointoint, &tmp_bucket->data_size, &tmp_bucket->data_tail);
    }
    ////////////////////////////////////////////////////

    // [STEP 2] Divide points in every bucket into Sl and Sln
    tmp_bucket = first_bucket;
    tmp_pointointArray = (SkyPoint **)malloc(sizeof(SkyPoint*) * tmp_bucket->data_size);
    while (tmp_bucket != NULL) {
        tmp_pointoint = tmp_bucket->data_head;
        tmp_pointointArray[0] = tmp_pointoint;
        for (i = 0; i < tmp_bucket->data_size; i++) {
            tmp_pointoint = tmp_pointoint->next;
            tmp_pointointArray[i] = tmp_pointoint;
        }
        for (i = 0; i < tmp_bucket->data_size; i++) {
            tmp_pointoint = tmp_pointointArray[i];
            for (j = 0; j < tmp_bucket->data_size; j++) {
                tmp_pointoint2 = tmp_pointointArray[j];
                if (i != j ) {
                    if (IsP1DominateP2(tmp_pointoint2, tmp_pointoint)) {
                        tmp_pointoint->cnt_domi++;
                        if (tmp_pointoint->cnt_domi>= sky_k) {
                            PushPoint(tmp_pointoint, &tmp_bucket->sln_size, &tmp_bucket->sln_tail);
                            break;
                        }
                    }
                }
            }
            if (j == tmp_bucket->data_size) //which means data[j] is not dominted more than k times, then put it into Sl.
                PushPoint(tmp_pointoint, &stwh_size, &stwh_tail);
        }
        tmp_bucket = tmp_bucket->next;
    }

    free(tmp_pointointArray);

    // [STEP 4] Push Swth -> Ses
    QsortStwh(stwh_size);

    /////////////////////////////////////////////////////////////////////////////////////
    // Origin:
    // vector<SkyPoint *>::iterator itHead, itTail;
    // for (itHead = Stwh.begin(); itHead != Stwh.end(); itHead++) {
    //    if(!*itHead) continue;
    //    for (itTail = Stwh.end(); itTail != Stwh.begin(); itTail--) {
    //        if(!*itTail) continue;
    //        if (IsP1DominateP2(*itTail, *itHead)) (*itHead)->domainatedCount ++;
    //        if ((*itHead)->domainatedCount > sky_k) {
    //            Ses.push_back(*itHead);
    //            Stwh.erase(itHead);
    //            break;
    //        }
    //        if (IsP1DominateP2(*itHead, *itTail)) (*itTail)->domainatedCount ++;
    //        if ((*itTail)->domainatedCount > sky_k) {
    //            Ses.push_back(*itTail);
    //            Stwh.erase(itTail);
    //        }
    //    }
    // }

    iterCount = 0;
    iterA = stwh_head->next;
    while (iterA != NULL) {
        iterCount++;
        tmp_pointointNext = iterA->next;
        iterB = stwh_tail;
        iterCountB = 0;
        while (iterB != stwh_head) {
            iterCountB++;
            tmp_pointoint = iterB->prev;
            if (SameBitmap(iterA->bitmap, iterB->bitmap, sky_dim))
                break;
            if (IsP1DominateP2(iterB, iterA)) {
                iterA->cnt_domi++;
                if (iterA->cnt_domi >= sky_k) {
                    DeletePoint(iterCount, &stwh_size, &stwh_head, &stwh_tail);
                    PushPoint(iterA, &ses_size, &ses_tail);
                    iterCount--;
                    break;
                }
            }
            if (IsP1DominateP2(iterA, iterB)) {
                iterB->cnt_domi++;
                if (iterB->cnt_domi >= sky_k) {
                    if (tmp_pointointNext == iterB) // if two nearby nodes, we delete the second, then update first node's next.
                        tmp_pointointNext = iterB->next;
                    DeletePoint(stwh_size - iterCountB + 1, &stwh_size, &stwh_head, &stwh_tail);
                    PushPoint(iterB, &ses_size, &ses_tail);
                    iterCountB--;
                }
            }
            iterB = tmp_pointoint;
        }
        iterA = tmp_pointointNext;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Test:
    //
    // int test = 0;
    // tmp_bucket = first_bucket;
    // while (tmp_bucket != NULL) {
    //     test += tmp_bucket->SlnSize - 1;
    //     tmp_bucket = tmp_bucket->next;
    // }
    // printf("%d %d %d", test, stwh_size - 1, ses_size - 1);    // sum should be total number of point  // success!
    //
    /////////////////////////////////////////////////////////////////////////////

    //[STEP 5] (Stwh, Ses) -> Sg
    /////////////////////////////////////////////////////////////////////////////////////

    // Stwh VS Ses
    iterCount = 0;
    iterA = stwh_head->next;
    while (iterA != NULL) {
        iterCount++;
        tmp_pointointNext = iterA->next;
        iterB = ses_head->next;
        while (iterB != NULL) {
            if (IsP1DominateP2(iterB, iterA)) {
                iterA->cnt_domi++;
                if (iterA->cnt_domi >= sky_k) {
                    DeletePoint(iterCount, &stwh_size, &stwh_head, &stwh_tail);
                    iterCount--;
                    break;
                }
            }
            iterB = iterB->next;
        }
        iterA = tmp_pointointNext;
    }

    //////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Test:
    //
    // int test = 0;
    // tmp_bucket = bucket;
    // for (i = 0; i < bucketCount; i++) {
    //     test += tmp_bucket->SlnSize - 1;
    //     tmp_bucket = tmp_bucket->next;
    // }
    // printf("%d %d %d %d", test, deleted, stwh_size - 1, ses_size - 1);    // sum should be total number of point  // success!
    //
    /////////////////////////////////////////////////////////////////////////////

    // Stwh VS Sln
    iterCount = 0;
    iterA = stwh_head->next;
    while (iterA != NULL) {
        iterCount++;
        tmp_pointointNext = iterA->next;
        tmp_bucket = first_bucket;
        while (tmp_bucket != NULL) {
            iterB = tmp_bucket->sln_head->next;
            while (iterB != NULL) {
                if (IsP1DominateP2(iterB, iterA)) {
                    iterA->cnt_domi++;
                    if (iterA->cnt_domi >= sky_k) {
                        DeletePoint(iterCount, &stwh_size, &stwh_head, &stwh_tail);
                        iterCount--;
                        break;
                    }
                }
                iterB = iterB->next;
            }
            if (iterB != NULL) break;
            tmp_bucket = tmp_bucket->next;
        }
        iterA = tmp_pointointNext;
    }

    sg_size = stwh_size;
    sg_head = stwh_head;
    sg_tail = stwh_tail;
    sg_head = stwh_head;
}

/*
 * Function: Init
 * -------------------
 *   Initialize varibles & input test data
 *
 *   sky_cnt: The k value that in skyband query
 *   sky_dim: The dimension of point in skyband query
 *   sky_k: The number of the points in skyband query
 *
 *   returns: void
 */

void Init() {
    tmp_size = 0;
    s_size = 0;
    stwh_size = 0;
    ses_size = 0;
    sg_size = 0;

    first_bucket = NULL;
    last_bucket = NULL;

    fin = fopen("/Users/armour/Desktop/KSkyBandQuery/KSkyBandQuery-C/KSkyBandQuery-C/Test/stdin.txt", "r+");
    fscanf(fin, "%d %d %d", &sky_cnt, &sky_dim, &sky_k);

    InputPoints();                      /* Input all points */

    fclose(fin);
}

/*
 * Function: Query
 * -------------------
 *   Initialize clock & Run the thickness-warehouse algorithm & Calculate time consumption
 *
 *   start: Start time of thickness-warehouse algorithm
 *   diff: Time duration of thickness-warehouse algorithm
 *
 *   returns: void
 */

void Query() {
    clock_t start = clock(), diff;

    ThicknessWarehouse();

    diff = clock() - start;
    unsigned long msec = diff * 1000 / CLOCKS_PER_SEC;          /* Convert to second and  millisecond */
    printf("Skyline: %lus %lums\n", msec/1000, msec%1000);
}

/*
 * Function: Output
 * -------------------
 *   Output the query result
 *
 *   sg_head: Start point of Sg, which is the final result list
 *
 *   returns: void
 */

void Output() {
    int i, j;
    fout = fopen("/Users/armour/Desktop/KSkyBandQuery/KSkyBandQuery-C/KSkyBandQuery-C/Test/skylineout.txt", "w+");
    tmp_head = sg_head;
    for (i = 0; i < sg_size; i++) {
        tmp_head = tmp_head->next;
        for (j = 0; j < sky_dim; j++)
            fprintf(fout, "%.6lf ", *(*(tmp_head->data) + j));          /* default printing in double type */
        fprintf(fout, "\n");
    }
    fclose(fout);
}

/* Main function */

int main() {
    Init();
    Query();
    Output();
}
