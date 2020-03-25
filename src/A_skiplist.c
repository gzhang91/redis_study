#include "A_skiplist.h"
#include <stdlib.h>
#include "zmalloc.h"

#define AZSKIPLIST_MAXLEVEL 64 /* Should be enough for 2^64 elements */
#define AZSKIPLIST_P 0.25      /* Skiplist P = 1/4 */

AzskiplistNode *AzslCreateNode(int level, double score, sds ele) {
    AzskiplistNode *zn =
        zmalloc(sizeof(*zn)+level*sizeof(struct AzskiplistLevel));
    zn->score = score;
    zn->ele = ele;
    return zn;
}

/* Create a new skiplist. */
/*
创建一个skiplist
1. 初始化有个头结点,本节点的索引指针具有最高的层数
2. 层数初始化为1 ?
*/
Azskiplist *AzslCreate(void) {
    int j;
    Azskiplist *zsl;

    zsl = zmalloc(sizeof(*zsl));
    zsl->level = 1;
    zsl->length = 0;
    zsl->header = AzslCreateNode(AZSKIPLIST_MAXLEVEL,0,NULL);
    for (j = 0; j < AZSKIPLIST_MAXLEVEL; j++) {
        zsl->header->level[j].forward = NULL;
        zsl->header->level[j].span = 0;
    }
    zsl->header->backward = NULL;
    zsl->tail = NULL;
    return zsl;
}

void AzslFreeNode(AzskiplistNode *node) {
    sdsfree(node->ele);
    zfree(node);
}

/* Free a whole skiplist. */
void AzslFree(Azskiplist *zsl) {
    AzskiplistNode *node = zsl->header->level[0].forward, *next;

    zfree(zsl->header);
    while(node) {
        next = node->level[0].forward;
        AzslFreeNode(node);
        node = next;
    }
    zfree(zsl);
}

/*
每个新添加的节点初始化的层数,采用随机方案
*/
int AzslRandomLevel(void) {
    int level = 1;
    while ((random()&0xFFFF) < (AZSKIPLIST_P * 0xFFFF))
        level += 1;
    return (level<AZSKIPLIST_MAXLEVEL) ? level : AZSKIPLIST_MAXLEVEL;
}

/*
插入的思想:
1. 根据score和ele查找到最适合插入的地方
2. 随机得到新节点的层数
3. 创建新节点并为之赋值
update和rank分别来预装新节点位置的信息
*/
AzskiplistNode *AzslInsert(Azskiplist *zsl, double score, sds ele) {
    AzskiplistNode *update[AZSKIPLIST_MAXLEVEL], *x;
    unsigned int rank[AZSKIPLIST_MAXLEVEL];
    int i, level;

    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {
        /* store rank that is crossed to reach the insert position */
        rank[i] = i == (zsl->level-1) ? 0 : rank[i+1];
        while (x->level[i].forward &&
                (x->level[i].forward->score < score ||
                    (x->level[i].forward->score == score &&
                    sdscmp(x->level[i].forward->ele,ele) < 0)))
        {
            rank[i] += x->level[i].span;
            x = x->level[i].forward;
        }
        update[i] = x;
    }
   
    level = AzslRandomLevel();
    if (level > zsl->level) {
        for (i = zsl->level; i < level; i++) {
            rank[i] = 0;
            update[i] = zsl->header;
            update[i]->level[i].span = zsl->length;
        }
        zsl->level = level;
    }
    x = AzslCreateNode(level,score,ele);
    for (i = 0; i < level; i++) {
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;

        /* update span covered by update[i] as x is inserted here */
        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    /* increment span for untouched levels */
    for (i = level; i < zsl->level; i++) {
        update[i]->level[i].span++;
    }

    x->backward = (update[0] == zsl->header) ? NULL : update[0];
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        zsl->tail = x;
    zsl->length++;
    return x;
}

void AzslDeleteNode(Azskiplist *zsl, AzskiplistNode *x, AzskiplistNode **update) {
    int i;
    for (i = 0; i < zsl->level; i++) {
        if (update[i]->level[i].forward == x) {
            update[i]->level[i].span += x->level[i].span - 1;
            update[i]->level[i].forward = x->level[i].forward;
        } else {
            update[i]->level[i].span -= 1;
        }
    }
    if (x->level[0].forward) {
        x->level[0].forward->backward = x->backward;
    } else {
        zsl->tail = x->backward;
    }
    while(zsl->level > 1 && zsl->header->level[zsl->level-1].forward == NULL)
        zsl->level--;
    zsl->length--;
}

/*
1. 找到要删除节点的前一个节点
2. 保存前一个节点的指针信息
3. 删除节点
*/
int AzslDelete(Azskiplist *zsl, double score, sds ele, AzskiplistNode **node) {
    AzskiplistNode *update[AZSKIPLIST_MAXLEVEL], *x;
    int i;

    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {
        while (x->level[i].forward &&
                (x->level[i].forward->score < score ||
                    (x->level[i].forward->score == score &&
                     sdscmp(x->level[i].forward->ele,ele) < 0)))
        {
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    /* We may have multiple elements with the same score, what we need
     * is to find the element with both the right score and object. */
    x = x->level[0].forward;
    if (x && score == x->score && sdscmp(x->ele,ele) == 0) {
        AzslDeleteNode(zsl, x, update);
        if (!node)
            AzslFreeNode(x);
        else
            *node = x;
        return 1;
    }
    return 0; /* not found */
}

int AzslValueGteMin(double value, Azrangespec *spec) {
    return spec->minex ? (value > spec->min) : (value >= spec->min);
}

int AzslValueLteMax(double value, Azrangespec *spec) {
    return spec->maxex ? (value < spec->max) : (value <= spec->max);
}

int AzslIsInRange(Azskiplist *zsl, Azrangespec *range) {
    AzskiplistNode *x;

    /* Test for ranges that will always be empty. */
    if (range->min > range->max ||
            (range->min == range->max && (range->minex || range->maxex)))
        return 0;
    x = zsl->tail;
    if (x == NULL || !AzslValueGteMin(x->score,range))
        return 0;
    x = zsl->header->level[0].forward;
    if (x == NULL || !AzslValueLteMax(x->score,range))
        return 0;
    return 1;
}


AzskiplistNode *AzslFirstInRange(Azskiplist *zsl, Azrangespec *range) {
    AzskiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    if (!AzslIsInRange(zsl,range)) return NULL;

    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {
        /* Go forward while *OUT* of range. */
        while (x->level[i].forward &&
            !AzslValueGteMin(x->level[i].forward->score,range))
                x = x->level[i].forward;
    }

    /* This is an inner range, so the next node cannot be NULL. */
    x = x->level[0].forward;

    /* Check if score <= max. */
    if (!AzslValueLteMax(x->score,range)) return NULL;
    return x;
}

/* Find the last node that is contained in the specified range.
 * Returns NULL when no element is contained in the range. */
AzskiplistNode *AzslLastInRange(Azskiplist *zsl, Azrangespec *range) {
    AzskiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    if (!AzslIsInRange(zsl,range)) return NULL;

    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {
        /* Go forward while *IN* range. */
        while (x->level[i].forward &&
            AzslValueLteMax(x->level[i].forward->score,range))
                x = x->level[i].forward;
    }

    /* Check if score >= min. */
    if (!AzslValueGteMin(x->score,range)) return NULL;
    return x;
}

unsigned long AzslGetRank(Azskiplist *zsl, double score, sds ele) {
    AzskiplistNode *x;
    unsigned long rank = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score &&
                sdscmp(x->level[i].forward->ele,ele) <= 0))) {
            rank += x->level[i].span;
            x = x->level[i].forward;
        }

        /* x might be equal to zsl->header, so test if obj is non-NULL */
        if (x->ele && sdscmp(x->ele,ele) == 0) {
            return rank;
        }
    }
    return 0;
}

