#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_SIZE

typedef char TElemType;

// 双亲孩子表示法
// 结合双亲表示法和孩子链表表示法得到
// 孩子节点
typedef struct CTNode
{
    int child; // 孩子的位置
    struct CTNode *next;
} *ChildPtr;

typedef struct
{
    TElemType data;
    int parent; // 双亲节点位置，升级版，双亲孩子表示法，结合双亲表示法和孩子表示法的优点
    ChildPtr firstchild;
} CTBox;

typedef struct
{
    CTBox nodes[MAX_TREE_SIZE];
    int root, num; // 根节点，节点总数
} CTree;

int main()
{
    printf("Hello world!\n");
    return 0;
}
