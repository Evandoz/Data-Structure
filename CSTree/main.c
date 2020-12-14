#include <stdio.h>
#include <stdlib.h>

typedef char TElemType;

// 孩子兄弟表示法
typedef struct CSTNode
{
    TElemType data;
    struct CSTNode *firstchild, *nextsibling;
} CSTNode, *CSTree;

int main()
{
    printf("Hello world!\n");
    return 0;
}
