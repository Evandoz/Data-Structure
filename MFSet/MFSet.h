#include "PTree.h"

typedef int Status;

// 用双亲表示法来定义并查集
typedef PTree MFSet;

// 初始化，从0开始
Status InitMFSet(MFSet *S, int num, TElemType *sub)
{
    int i;
    S->num = num; S->root = 0;
    for (i = 0; i < num; ++i)
    {
        S->nodes[i].data = sub[i];
        S->nodes[i].parent = -1;
    }
    return 0;
}

// 查找元素e所属集合（即查找它的根）
int Find_MFSet(MFSet *S, int i/* TElemType e*/ )
{
    /*int i;
    for (i = 0; i < S->num; ++i)
    {
        if (S->nodes[i].data == e)
            break;
    } */
    // 直接通过索引查找
    if (i < 0 || i >= S->num) return -1;
    int j = i; // 不修改参数
    while (S->nodes[j].parent >= 0)
        j = S->nodes[j].parent;
    return j;
}

// Merge或者Union，i，j表示待合并的两个集合（根）
Status Merge_MFSet(MFSet *S, int i, int j)
{
    if (i < 0 || i >= S->num || j < 0 || j >= S->num) return -1;
    S->nodes[i].parent = j; // 将i指向j
    return 0;
}

// 直接合并的问题，可能会导致并查集树的高度太大，会增加查找的时间复杂度
// 如果我们让数量小的集合指向数量大的集合，即可有效减小树的深度，而集合树的深度则用负值存储在parent中
// 对 Merge_MFSet 优化
Status Mix_MFSet(MFSet *S, int i, int j)
{
    if (i < 0 || i >= S->num || j < 0 || j >= S->num) return -1;
    // i数量大于j
    if (S->nodes[i].parent < S->nodes[j].parent)
    {
        S->nodes[i].parent += S->nodes[j].parent;
        S->nodes[j].parent = i;
    }
    else
    {
        S->nodes[j].parent += S->nodes[i].parent;
        S->nodes[i].parent = j;
    }
    return 0;
}

// 尽管在合并时，将小集合指向大集合，减小了集合树的深度，但仍有优化空间
// 既然一个集合只有一个根，所以可以让集合中的元素都直接指向根
// 这个可以在查找的过程中来实现，只需在原查找代码 Find_MFSet 加入调整的代码即可
// 对 Find_MFSet 优化
int Fix_MFSet(MFSet *S, int i)
{
    if (i < 0 || i >= S->num) return -1;
    int j = i, k = i, tmp;
    while (S->nodes[j].parent >= 0)
        j = S->nodes[j].parent; // 找到gen
    while (k != j)
    {
        tmp = S->nodes[k].parent;
        S->nodes[k].parent = j; // 将其指向根
        k = tmp;
    }
    return j;
}
