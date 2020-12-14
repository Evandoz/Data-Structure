#include <stdio.h>
#include <stdlib.h>

// 静态链表

#define MAXSIZE 100

typedef int ElemType;

typedef struct
{
    ElemType data; // 元素
    int cur; // 游标，模拟指针，指向下一个元素下标
} component, SLinkList[MAXSIZE];

int main()
{
    printf("Hello world!\n");
    return 0;
}
