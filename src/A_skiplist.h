#ifndef _A_SKIPLIST_H_
#define _A_SKIPLIST_H_

#include "sds.h"

/*
区间操作
*/
typedef struct {
    double min, max;
    int minex, maxex; /* are min or max exclusive? */
} Azrangespec;

typedef struct AzskiplistNode {
    sds ele;  // 节点数据
    double score;  // 每个节点的随机评分,在元素比较的时候可以用,也是代表一个优先级
    struct AzskiplistNode *backward;  // 往前移动指针
    struct AzskiplistLevel {
        struct AzskiplistNode *forward; // 指向下一个节点
        unsigned long span;  // 本节点到下一个节点的间隔
    } level[];  // 每个节点层数数组
} AzskiplistNode;

// skiplist
typedef struct Azskiplist {
    struct AzskiplistNode *header, *tail; 
    unsigned long length; // 节点个数
    int level;   // 有效最高层数
} Azskiplist;

Azskiplist *AzslCreate(void);
void AzslFree(Azskiplist *zsl);
AzskiplistNode *AzslInsert(Azskiplist *zsl, double score, sds ele);
int AzslDelete(Azskiplist *zsl, double score, sds ele, AzskiplistNode **node);
AzskiplistNode *AzslFirstInRange(Azskiplist *zsl, Azrangespec *range);
AzskiplistNode *AzslLastInRange(Azskiplist *zsl, Azrangespec *range);
unsigned long AzslGetRank(Azskiplist *zsl, double score, sds o);

#endif // _A_SKIPLIST_H_