#ifndef MGraph_h
#define MGraph_h

#include <stdio.h>
#include <stdlib.h>
#include "SqQueue.h"
#include "MFSet.h"
#include "Heap.h"

#define INF 65535         // 最大值，用于网（带权图）

#define MAX_VERTEX_NUM 20 // 顶点个数

typedef int Status;

// 有向图，有向网，无向图，无向网
typedef enum { DG, DN, UDG, UDN } GraphKind;

// 顶点的数据类型
typedef char VertexType;
// 边的数据类型
typedef int ArcType;

typedef struct {
    VertexType vexs[MAX_VERTEX_NUM]; // 一维数组表顶点
    ArcType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 二维数组表示边
    int vexnum, arcnum; // 当前顶点数和边数
    GraphKind kind; // 图的种类标识
}MGraph;

/*=================工具函数===================*/
//输出顶点在图中的位置
int LocateVex(MGraph *G, VertexType v)
{
    int i;
	for (i = 0; i<G->vexnum; ++i)
		if (G->vexs[i] == v)
			return i;
	return -1;
}

//打印
Status PrintMGraph(MGraph *G)
{
    int i, j;
    printf("\t");
    for (i = 0; i < G->vexnum; ++i)
    {
        printf("%c\t", G->vexs[i]);
    }
    for (i = 0; i < G->vexnum; ++i)
    {
        printf("\n%c\t", G->vexs[i]);
        for (j = 0; j < G->vexnum; ++j)
        {
            if (G->arcs[i][j] == INF)
                printf("INF\t");
            else
                printf("%d\t", G->arcs[i][j]);
        }
    }
    return 0;
}
/*=================图的操作===================*/

// 创建无向网
Status CreateUDN(MGraph *G)
{
    int i, j, k, w; // w 为权值
    VertexType v1, v2;
    G->kind = UDN;
    printf("Please input the vexnum and arcnum:");
    scanf("%d,%d", &(G->vexnum), &(G->arcnum));
    getchar();

    // 输入顶点
    printf("Please input the vertex:");
    for (i = 0; i < G->vexnum; ++i)
        G->vexs[i] = getchar();
    getchar();

    // 初始化边
    for (i = 0; i < G->vexnum; ++i)
        for (j = 0; j < G->vexnum; ++j)
            if (i == j) G->arcs[i][j] = 0;
            else G->arcs[i][j] = INF; // 带权图，设置为 INF

    // 输入边
    printf("Please input the arc:(eg.A-C,4 C-D,5)");
    for (k = 0; k < G->arcnum; ++k)
    {
        scanf("%c-%c,%d", &v1, &v2, &w);
        i = LocateVex(G, v1);
        j = LocateVex(G, v2);
        G->arcs[i][j] = w; // 有向图，只需一处插入
        // G->arcs[j][i] = w; // 无向图，需两处插入
    }
    return 0;
}

// BFS
Status BFS(MGraph *G, int i, int *visited)
{
    int j; SqQueue Q; InitQueue(&Q);
    // 必须在进栈之前访问，否则会重复访问
    visited[i] = 1;
    printf("%c\t", G->vexs[i]);
    EnQueue(&Q, i);
    while(!QueueEmpty(&Q))
    {
        DeQueue(&Q, &i);
        for (j = 0; j < G->vexnum; ++j)
        {
            if(G->arcs[i][j] != INF && 0 == visited[j])
            {
                visited[j] = 1;
                printf("%c\t", G->vexs[j]);
                EnQueue(&Q, j);
            }
        }
    }
    return 0;
}

// 广度优先遍历
Status BFSTraverse(MGraph *G)
{
    int i, visited[MAX_VERTEX_NUM];
    // 标示是否已访问
    for (i = 0; i < G->vexnum; ++i)
        visited[i] = 0;

    for (i = 0; i < G->vexnum; ++i)
        if (0 == visited[i])
            BFS(G, i, visited);
    return 0;
}

// DFS
Status DFS(MGraph *G, int i, int *visited)
{
    int j;
    visited[i] = 1;
    printf("%c\t", G->vexs[i]);
    for (j = 0; j < G->vexnum; ++j)
        if(G->arcs[i][j] != INF && 0 == visited[j])
            DFS(G, j, visited);
    return 0;
}

// 深度优先遍历
Status DFSTraverse(MGraph *G)
{
    int i, visited[MAX_VERTEX_NUM];
    // 标示是否已访问
    for (i = 0; i < G->vexnum; ++i)
        visited[i] = 0;

    for (i = 0; i < G->vexnum; ++i)
        if (0 == visited[i])
            DFS(G, i, visited);
    return 0;
}

/*===================== 小生成树 =======================*/

// Prim
void MiniSpanTree_Prim(MGraph *G, VertexType v)
{
    int i, j, k;
    ArcType minicost;
    // 定义辅助数组，记录当前最小生成树与树外节点的最小距离及关联的树内顶点
    struct {
        VertexType adjvex; // 用于复原
        ArcType lowcost;
    }closedge[MAX_VERTEX_NUM];

    // 初始化 closedge
    k = LocateVex(G, v);
    for (i = 0; i < G->vexnum; ++i)
    {
        if (G->vexs[i] == v)
            closedge[i].lowcost = 0;
        else
        {
            closedge[i].adjvex = v;
            closedge[i].lowcost = G->arcs[k][i];
        }
    }

    // Prim 算法运行 G->vexnum -1 次
    for (i = 1; i < G->vexnum; ++i)
    {
        // 计算最小，得到k，k为最顶距离对应的节点（位置下标）
        minicost = INF;
        for (j = 0; j < G->vexnum; ++j)
        {
            if (closedge[j].lowcost != 0 && closedge[j].lowcost < minicost)
            {
                minicost = closedge[j].lowcost;
                k = j;
            }
        }
        printf("%c-[%d]-%c\t", closedge[k].adjvex, G->arcs[LocateVex(G, closedge[k].adjvex)][k], G->vexs[k]);
        // 将节点 G->vexs[k] 纳入生成树
        closedge[k].lowcost = 0;
        // 更新 closedge
        for (j = 0; j < G->vexnum; ++j)
        {
            if (G->arcs[k][j] < closedge[j].lowcost)
            {
                closedge[j].adjvex = G->vexs[k];
                closedge[j].lowcost = G->arcs[k][j];
            }
        }
    }
}

// Kruskal
void MiniSpanTree_Kruskal(MGraph *G)
{
    // k 用于取当前权值最小边
    // m,n用于取顶点所在的连通分量
    int i, k = G->arcnum, m, n;
    int AS[MAX_VERTEX_NUM]; // 连通分量
    ArcHeap AH[MAX_VERTEX_NUM]; // 边集堆
    InitMFSet(AS, G->vexnum);
    InitArcHeap(AH, G->arcs, G->arcnum, G->vexnum);

    for (i = 0; i < G->arcnum; ++i)
    {
        k = GetRoot(AH, k);
        m = Fix_MFSet(AS, AH[k].vex_1);
        n = Fix_MFSet(AS, AH[k].vex_2);
        // 属于不同的连通分量，将其加入
        if (m != n)
        {
            // 合并连通分量
            Mix_MFSet(AS, m, n);
            printf("%c-[%d]-%c\t", G->vexs[AH[k].vex_1], G->arcs[AH[k].vex_1][AH[k].vex_2], G->vexs[AH[k].vex_2]);
        }
        --k;
    }
}

// 迪杰斯特拉最短路径
// 换个方式存储路径，用二维数组存储路径，这样可以只通过这个二维数组即可还原路径
Status ShortestPath_Dijkstra(MGraph *G, int start, int *Dist, int (*PathMatrix)[MAX_VERTEX_NUM])
{
    int v, w, c, min_dist, min_vex, visited[MAX_VERTEX_NUM] = {0};
    // 初始化
    visited[start] = 1;
    // 创建时设置了 G->arcs[v][v] = 0
    for (v = 0; v < G->vexnum; ++v)
    {
        // 距离
        Dist[v] = G->arcs[start][v];
        // 第 2 维存储路径
        for (w = 0; w < G->vexnum; ++w) PathMatrix[v][w] = 0;
        // 直接相邻，起点 start 到终点 v
        if (Dist[v] < INF) { PathMatrix[v][start] = 1; PathMatrix[v][v] = 1; }
    }
    // 路径递增搜索
    for (c = 1; c < G->vexnum; ++c)
    {
        // 搜索到没访问的 min{Dist[v]}
        min_dist = INF;
        for (v = 0; v < G->vexnum; ++v)
        {
            if (0 == visited[v] && Dist[v] < INF)
            {
                min_dist = Dist[v]; min_vex = v;
            }
        }
        visited[min_vex] = 1;
        // 更新与 min_vex 相邻的 Dist[v]
        for (v = 0; v < G->vexnum; ++v)
        {
            if (0 == visited[v] && Dist[min_vex] + G->arcs[min_vex][v] < Dist[v])
            {
                Dist[v] = Dist[min_vex] + G->arcs[min_vex][v];
                for (w = 0; w < G->vexnum; ++w)
                    PathMatrix[v][w] = PathMatrix[min_vex][w]; // 继承start到min_vex的路径
                PathMatrix[v][v] = 1; // 加入自身到路径
            }
        }
    }
    return 0;
}

void PrintPath_Dist_Dijkstra(MGraph *G, int start, int *Dist, int (*PathMatrix)[MAX_VERTEX_NUM])
{
    int v, w;
    for (v = 0; v < G->vexnum; ++v)
    {
        for (w = 0; w < G->vexnum; ++w)
        {
            if (w == start) printf("%c", G->vexs[w]);
            else if (PathMatrix[v][w]) printf("-->%c", G->vexs[w]);
        }
        if (Dist[v] == INF) printf("-->%c[%d]\n", G->vexs[v], Dist[v]);
        else printf("[%d]\n", Dist[v]);
    }
}

// 弗洛伊德最短路径
Status ShortestPath_Floyd(MGraph *G, int (*DistMatrix)[MAX_VERTEX_NUM], int (*PathMatrix)[MAX_VERTEX_NUM])
{
    return 0;
}
#endif
