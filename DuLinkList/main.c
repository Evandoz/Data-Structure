#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;

// 节点定义
typedef struct DuLNode
{
	ElemType data;
	struct DuLNode *prior, *next; // 双向指针
}DuLNode, *DuLinkList;

int main()
{
    printf("Hello world!\n");
    return 0;
}
