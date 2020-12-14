#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_SIZE 20

typedef int Status;

// 顶点类型
typedef char VertexType;

// 弧边（作为节点存储）
typedef struct ArcNode
{
    int tailvex, headvex; // 尾 --> 头
    struct ArcNode *headlink, *taillink; // 同头的下一条弧（入），同尾的下一条弧（出）
    int weight;
} ArcNode;

// 顶点
typedef struct VexNode
{
    VertexType data;
    ArcNode *firstin, *firstout; // 同头第一条弧，同尾第一条弧
} VexNode;

// 有向图十字链表
// 邻接表和逆邻接表结合体
typedef struct
{
    VexNode olist[MAX_VERTEX_SIZE];
    int vexnum, arcnum;
} OLGraph;

/*=================工具函数===================*/

//输出顶点在图中的位置
int LocateVex(OLGraph *G, VertexType v)
{
    int i;
	for (i = 0; i < G->vexnum; ++i)
		if (G->olist[i].data == v)
			return i;
	return -1;
}

// 打印
Status PrintOLGraph(OLGraph *G)
{
    int i;
    ArcNode *p;
    // 同尾，邻接表
    for (i = 0; i < G->vexnum; ++i)
    {
        printf("[%d]%c|", i, G->olist[i].data);
        p = G->olist[i].firstout;
        while(p)
        {
            printf("->%c", G->olist[p->headvex].data);
            p = p->taillink;
        }
        printf("\n");
    }
    printf("\n");
    // 同头，逆邻接表
    for (i = 0; i < G->vexnum; ++i)
    {
        printf("[%d]%c|", i, G->olist[i].data);
        p = G->olist[i].firstin;
        while(p)
        {
            printf("<-%c", G->olist[p->tailvex].data);
            p = p->headlink;
        }
        printf("\n");
    }
    return 0;
}

/*=================图的操作===================*/

// 创建
Status CreateOLGraph(OLGraph *G)
{
    int i, m, n, w; // w 为权重
    VertexType v1, v2; // v1,v2为弧的顶点
    ArcNode *p; // 生成弧边节点
    printf("Please input the vexnum and arcnum:");
    scanf("%d,%d", &(G->vexnum), &(G->arcnum));
    getchar();
    // 顶点
    printf("Please input the vertex:");
    for (i = 0; i < G->vexnum; ++i)
    {
        G->olist[i].data = getchar();
        G->olist[i].firstin = NULL;
        G->olist[i].firstout = NULL;
    }
    getchar();
    // 弧边
    printf("Please input the arc:(eg->B-C C-D,3)");
    for (i = 0; i < G->arcnum; ++i)
    {
        scanf("%c-%c,", &v1, &v2); // v1-->v2
        m = LocateVex(G, v1); // 尾
        n = LocateVex(G, v2); // 头
        p = (ArcNode *)malloc(sizeof(ArcNode));
        p->tailvex = m; p->headvex = n;
        // 插入
        p->headlink = G->olist[n].firstin;
        p->taillink = G->olist[m].firstout;
        G->olist[n].firstin = p; // 逆邻接
        G->olist[m].firstout = p; // 邻接
    }
    return 0;
}
