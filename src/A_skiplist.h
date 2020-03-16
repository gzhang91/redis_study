#ifndef _A_SKIPLIST_H_
#define _A_SKIPLIST_H_

#include "sds.h"

typedef struct {
    double min, max;
    int minex, maxex; /* are min or max exclusive? */
} Azrangespec;

typedef struct AzskiplistNode {
    sds ele;
    double score;
    struct AzskiplistNode *backward;
    struct AzskiplistLevel {
        struct AzskiplistNode *forward;
        unsigned long span;
    } level[];
} AzskiplistNode;

typedef struct Azskiplist {
    struct AzskiplistNode *header, *tail;
    unsigned long length;
    int level;
} Azskiplist;

Azskiplist *AzslCreate(void);
void AzslFree(Azskiplist *zsl);
AzskiplistNode *AzslInsert(Azskiplist *zsl, double score, sds ele);
int AzslDelete(Azskiplist *zsl, double score, sds ele, AzskiplistNode **node);
AzskiplistNode *AzslFirstInRange(Azskiplist *zsl, Azrangespec *range);
AzskiplistNode *AzslLastInRange(Azskiplist *zsl, Azrangespec *range);
unsigned long AzslGetRank(Azskiplist *zsl, double score, sds o);

#endif // _A_SKIPLIST_H_