#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 20

// 顶点类型
typedef char VertexType;

// 访问标记
typedef enum {unvisited, visited} VisitIf;

// 边表节点
typedef struct ArcNode
{
    VisitIf mark; // 访问标记
    int ivex, jvex; // 边的两个顶点的位置
    struct ArcNode *ilink, *jlink; // 分别依附两个顶点的下一条边
    int weight; // 权重
}ArcNode;

// 顶点
typedef struct VexNode
{
    VertexType data; // 顶点
    struct ArcNode *firstarc; // 依附顶点的第一条边
}VexNode;

// 无向图邻接多重表
typedef struct
{
    VexNode adjmulist[MAX_VERTEX_NUM];
    int vexnum, arcnum; // 顶点数，边数
}AMLGraph;

/*=================工具函数===================*/

//输出顶点在图中的位置
int LocateVex(AMGraph *G, VertexType v)
{
    int i;
	for (i = 0; i < G->vexnum; ++i)
		if (G->adjmulist[i].data == v)
			return i;
	return -1;
}

/*=================图的操作===================*/

// 创建邻接多重表
void CreateAMLGraph(AMLGraph *G)
{
    int i, m, n, w; // i为循环序号，带权图增加一个w
    VertexType v1, v2; // v1,v2为弧的顶点
    ArcNode *p; // 生成弧边
    printf("Please input the vexnum and arcnum:");
    scanf("%d,%d", &(G->vexnum), &(G->arcnum));
    getchar();
    // 顶点
    printf("Please input the vertex:");
    for (i = 0; i < G->vexnum; ++i)
    {
        G->adjmulist[i].data = getchar();
        G->adjmulist[i].firstarc = NULL;
    }
    getchar();
    // 边
    printf("Please input the arc:(eg.B-C D-E B-C,3)"); // 3为权
    for (i = 0; i < G->arcnum; ++i)
    {
        scanf("%c-%c,%d", &v1, &v2, &w); // 对于带权图增加一个w，记录权值
        m = LocateVex(G, v1);
        n = LocateVex(G, v2);
        p = (ArcNode *)malloc(sizeof(ArcNode));
        p->ivex = m; p->jvex = n;
        // 插入
        p->ilink = G->adjmulist[m].firstarc;
        p->jlink = G->adjmulist[n].firstarc;
        G->adjmulist[m].firstarc = p;
        G->adjmulist[n].firstarc = p;
    }
    return 0;
}

// 给两个顶点，搜索边是否存在
int SearchArc(AMLGraph *G, VertexType v1, VertexType v2)
{
    int m, n;
    ArcNode *p;
    m = LocateVex(G, v1);
    n = LocateVex(G, v2);
    p = G->adjmulist[m].firstarc;
    while (p)
    {
        if (p->ivex == m && p->jvex == n)
            return 1;
        else if (p->ivex == m && p->jvex == n)
            return 1;
        else if (p->ivex == m)
            p = p->ilink;
        else if (p->jvex == m)
            p = p->jlink;
    }
    return 0;
}

// 遍历边
void TraverseArc(AMLGraph *G)
{
    int i;
    ArcNode *p;
    for (i = 0; i < G->vexnum; ++i)
    {
        p = G->adjmulist[i].firstarc;
        while (p)
        {

            if (p->ivex == i)
            {
                if (unvisited == p->mark)
                {
                    printf("%c--%c\n", G->adjmulist[i].data, G->adjmulist[p->jvex].data);
                    p->mark = visited;
                }
                p = p->ivex;
            }
            else
            {
                if (unvisited == p->mark)
                {
                    printf("%c--%c\n", G->adjmulist[p->ivex].data, G->adjmulist[i].data);
                    p->mark = visited;
                }
                p = p->jvex;
            }
        }
    }
}
