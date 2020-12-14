#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_SIZE 20

typedef char TElemType;

typedef struct
{
    TElemType data;
    int parent;
} PTNode;

typedef struct
{
    PTNode nodes[MAX_TREE_SIZE];
    int root, num; // 根节点位置，节点总数
} PTree;
