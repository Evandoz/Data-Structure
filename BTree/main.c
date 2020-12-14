#include <stdio.h>
#include <stdlib.h>

// 一般 3 阶起步，即 2-3 树
#define m 3

typedef int KeyType;

// B 树 Node
typedef struct BTNode {
    int keynum; // 关键字数量（节点大小）
    struct BTNode *parent; // 双亲指针，插入和删除中的拆分合并操作会用到
    KeyType key[m+1]; // 关键字向量，0 号空闲
    struct *ptr[m+1]; // 子树指针向量（指针数组）
    // Record *recptr[m+1]; // 关键字关联的记录指针
}BTNode, *BTree;

// 查找结果
typedef struct {
    BTNode *pt; // 指向找到的节点
    int i; // 节点中关键字序号[1...m]
    int tag; // 查抄成功或失败标记
}Result;

Result SerachBTree(BTree T, KeyType k);

int main()
{
    printf("Hello world!\n");
    return 0;
}

// 查找
Result SerachBTree(BTree T, KeyType k)
{
    // f 指向双亲，用于查找失败时新关键字的插入位置
    BTNode *p = T, *f = NULL;
    int i = 0, found = 0;
    while(p && found == 0)
    {
        i = Search(p, k); // p->key 中搜索 k
        if (i > 0 && p->key[i] == k)
            found = 1; // 找到
        else
        {
            f = p; p = p->ptr[i];
        }
    }
    if (found) return (p, i, 1);
    else return (f, i, 0);
    // 查找失败，返回插入位置
}

// 插入
// 在 B 树 T的 p 节点的 key[i] 和 key[i+1] 之前插入关键字 k
// p 和 i 均有之前的 SerachBTree 得到
// 插入后导致节点过大时，需要沿着双亲链进行分裂调整
void InsertBTree(BTree *T, KeyType k, BTNode *p, int i)
{
    int s, flag = 0;
    KeyType x; // 分裂后的 K(⌈m/2⌉)
    BTNode *q = NULL; // 和插入关键字关联的孩子指针
    while (p && flag == 0)
    {
        Insert(T, p, i, q);
        // 未超标则结束
        if (p->keynum < m)
            flag = 1;
        // 否则，分裂
        else
        {
            split(p, s, q); // 分裂成 p，q
            x = p->key[s];
            p = p->parent;
            if (p)
                i = Search(p, x); // 双亲中寻找插入位置
        }
    }
    // 1.树 T 为空则生成新节点，此时 p==NULL，q==NULL
    // 2.树 T 根节点也发生了分裂，得到 x 和 p，q
    if (flag == 0)
        NewRoot(T, p, x, q);
}
