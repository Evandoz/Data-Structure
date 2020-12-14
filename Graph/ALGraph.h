#ifndef ALGraph_h
#define ALGraph_h

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "SqQueue.h"
#include "SqStack.h"
#include "CSTree.h"

#define MAX_VERTEX_NUM 20

#define INF 65535

typedef int Status;

// 有向图，有向网，无向图，无向网
typedef enum { DG, DN, UDG, UDN } GraphKind;

// 顶点类型
typedef char VertexType;

// 边表节点
typedef struct ArcNode
{
    int adjvex; // 该边所指节点位置
    struct ArcNode *nextarc; //下一个邻接点（下一条弧）
    //InfoType *info;
    // 该边相关信息的指针，通常指带权图的权值，
    // 此处做一些简化，直接定义为权值
    int weight;
} ArcNode, *Arc;

// 顶点表节点
typedef struct VexNode
{
    VertexType data; // 节点
    ArcNode *firstarc; // 边表头节点指针
} VexNode, AdjList[MAX_VERTEX_NUM];

// 邻接表存储图
typedef struct
{
    AdjList vertices; // 顶点数组
    int vexnum, arcnum; // 顶点数、边数
    GraphKind kind; //图的种类标志
} ALGraph;

/*=================工具函数===================*/

//输出顶点在图中的位置
int LocateVex(ALGraph *G, VertexType v)
{
    int i;
	for (i = 0; i<G->vexnum; ++i)
		if (G->vertices[i].data == v)
			return i;
	return -1;
}

// 邻接表头插，(i,j)为弧的两个顶点，带权图增加一个w，记录权值
Status HeadInsertArc(ALGraph *G, int i, int j, int weight)
{
    ArcNode *e;
    e = (ArcNode *)malloc(sizeof(ArcNode));
    e->adjvex = j;
    e->weight = weight; // 对于带权图增加一个w，记录权值
    e->nextarc = G->vertices[i].firstarc;
    G->vertices[i].firstarc = e;
    return 0;
}

//邻接表尾插，(i,j)为弧的两个顶点，带权图增加一个w，记录权值
Status TailInsertArc(ALGraph *G, int i, int j, int weight)
{
	ArcNode *e, *p;
	e = (ArcNode *)malloc(sizeof(ArcNode));
	e->adjvex = j;
    e->weight = weight; // 对于带权图增加一个w，记录权值
	e->nextarc = NULL;
	if (G->vertices[i].firstarc == NULL)
		G->vertices[i].firstarc = e;
	else
	{
		p = G->vertices[i].firstarc;
		while (p->nextarc)
			p = p->nextarc;
		p->nextarc = e;
	}
	return OK;
}

// 打印
Status PrintALGraph(ALGraph *G)
{
    int i;
    for (i = 0; i < G->vexnum; ++i)
    {
        printf("(%d)%c|", i, G->vertices[i].data);
        ArcNode *p = G->vertices[i].firstarc;
        while(p)
        {
            printf("-[%d]->%c", p->weight, G->vertices[p->adjvex].data);
            p = p->nextarc;
        }
        printf("\n");
    }
    return 0;
}

/*=================图的操作===================*/

// 创建
Status CreateGraph(ALGraph *G)
{
    int i, m, n, w; // i为循环序号，带权图增加一个w
    VertexType v1, v2; // v1,v2为弧的顶点
    G->kind = UDN;
    printf("Please input the vexnum and arcnum:");
    scanf("%d,%d", &(G->vexnum), &(G->arcnum));
    getchar();
    // 顶点
    printf("Please input the vertex:");
    for (i = 0; i < G->vexnum; ++i)
    {
        G->vertices[i].data = getchar();
        G->vertices[i].firstarc = NULL;
    }
    getchar();
    // 边
    printf("Please input the arc:(eg.B-C D-E B-C,3)"); // 3为权
    for (i = 0; i < G->arcnum; ++i)
    {
        scanf("%c-%c,%d", &v1, &v2, &w); // 对于带权图增加一个w，记录权值
        m = LocateVex(G, v1);
        n = LocateVex(G, v2);
        HeadInsertArc(G, m, n, w); // 有向图，只需一处插入
        //HeadInsertArc(G, n, m, w); // 无向图，需两处插入
    }
    return 0;
}

// 深度优先搜索DFS--递归
void DFS_Recursion(ALGraph *G, int i, int *visited)
{
    ArcNode *p;
    if (0 == visited[i])
    {
        visited[i] = 1;
        printf("(%d)%c ", i, G->vertices[i].data);
        p = G->vertices[i].firstarc;
        while (p)
        {
            DFS_Recursion(G, p->adjvex, visited);
            p = p->nextarc;
        }
    }
}

// DFS--非递归（需要将之前访问过的邻接点存入栈）
void DFS_Non_Recursion(ALGraph *G, int i, int *visited)
{
    SqStack S; InitStack(&S);
    ArcNode *p;
    if (0 == visited[i])
    {
        visited[i] = 1;
        printf("(%d)%c ", i, G->vertices[i].data);
        PushStack(&S, i);
        while(!StackEmpty(&S))
        {
            // 栈顶为当前层的前驱
            p = G->vertices[GetTop(&S)].firstarc;
            while (p)
            {
                // 当前层该节点未访问，没有走到头则访问
                if (0 == visited[p->adjvex])
                {
                    visited[p->adjvex] = 1;
                    printf("(%d)%c ", p->adjvex, G->vertices[p->adjvex].data);
                    PushStack(&S, p->adjvex);
                    // 继续到下一层
                    p = G->vertices[p->adjvex].firstarc;
                }
                // 当前层该节点已访问，已经走到头，则访问该节点相同前驱的下一个节点
                else
                {
                    p = p->nextarc;
                }
            }
            // 当前层没有节点了或者当前层节点全部已访问
            // 栈顶未当前层的前驱（上一层），弹出栈顶，回到上一层
            PopStack(&S, &i);
        }
    }
}

// 深度遍历
Status DFSTraverse(ALGraph *G)
{
    int i, visited[MAX_VERTEX_NUM] = {0};
    // 对于连通图，从任意一个顶点出发，就可以遍历所有的顶点，即只需要DFS_Recursion()即可
    // 但对于非连通图，DFS_Recursion()只能遍历其连通分量，因此，要遍历所有顶点，需要不断更换遍历起点
    for (i = 0; i < G->vexnum; ++i)
        DFS_Recursion(G, i, visited);
        //DFS_Non_Recursion(G, i, visited);
    return 0;
}

// 广度优先搜索BFS
void BFS(ALGraph *G, int i, int *visited)
{
    SqQueue Q; InitQueue(&Q);
    ArcNode *p;
    if (0 == visited[i])
    {
        visited[i] = 1;
        printf("(%d)%c ", i, G->vertices[i].data);
        EnQueue(&Q, i); // 记录顶点位置即可
        while(!QueueEmpty(&Q))
        {
            DeQueue(&Q, &i); // 复用i
            p = G->vertices[i].firstarc;
            while(p)
            {
                if (0 == visited[p->adjvex])
                {
                    visited[p->adjvex] = 1;
                    printf("(%d)%c ", p->adjvex, G->vertices[p->adjvex].data);
                    EnQueue(&Q, p->adjvex);
                }
                p = p->nextarc;
            }
        }
    }
}

// 广度遍历
Status BFSTraverse(ALGraph *G)
{
    int i, visited[MAX_VERTEX_NUM] = {0};
    // 对于连通图，从任意一个顶点出发，就可以遍历所有的顶点，即只需要DFS_Recursion()即可
    // 但对于非连通图，DFS_Recursion()只能遍历其连通分量，因此，要遍历所有顶点，需要不断更换遍历起点
    // 因此，此处对循环内部的遍历部分，单读提取出来，构成一个新函数
    for (i = 0; i < G->vexnum; ++i)
        BFS(G, i, visited);
    return 0;
}

// 深度优先生成树
// 从第 i 个顶点出发构建以 pre 为根的生成树
// 复用 pre 指向当前子树树根
void DFSTree(ALGraph *G, int i, int *visited, CSTNode *pre)
{
    // 以T为根的生成树，只有左子树，没有右子树
    int first = 1; // 用一个标志来标识第一个孩子节点
    CSTNode *e;
    ArcNode *p = G->vertices[i].firstarc;
    while(p)
    {
        if (0 == visited[p->adjvex])
        {
            visited[p->adjvex] = 1;
            // 分配新节点
            e = (CSTNode *)malloc(sizeof(CSTNode));
            e->data = G->vertices[p->adjvex].data;
            e->firstchild = NULL;
            e->nextsibling = NULL;
            // 添加到树中
            if (first)
            {
                pre->firstchild = e;
                first = 0;
            }
            else
            {
                pre->nextsibling = e;
            }
            // 记录上一个访问的节点，它是后一节点的兄弟或下一层的根
            pre = e;
            DFSTree(G, p->adjvex, visited, pre);
        }
        p = p->nextarc;
    }
}

// 深度优先生成森林
void DFSForest(ALGraph *G, CSTree *T)
{
    int i, visited[MAX_VERTEX_NUM] = {0};
    // pre 记录上一棵生成树的根
    // e 生成树节点
    CSTNode *pre, *e;
    // 遍历
    for (i = 0; i < G->vexnum; ++i)
    {
        if (0 == visited[i])
        {
            visited[i] = 1;
            // 生成树根节点
            e = (CSTNode *)malloc(sizeof(CSTNode));
            e->data = G->vertices[i].data;
            e->firstchild = NULL;
            e->nextsibling = NULL;
            // 添加到树中
            if (*T)
                pre->nextsibling = e; // 当前生成树的根为上一生成树根的兄弟
            else
                (*T) = e; // 第一棵生成树的根
            pre = e; // 记下当前根，用于后续的生成树
            DFSTree(G, i, visited, pre);
        }
    }
}

// 广度优先生成树
// 从第 i 个顶点出发构建以 pre 为根的生成树i
// 复用 pre 指向当前子树树根
void BFSTree(ALGraph *G, int i, int *visited, CSTNode *pre)
{
    // 以T为根的生成树，只有左子树，没有右子树
    SqQueue Q; InitQueue(&Q);
    int first; // 用一个标志来标识第一个孩子节
    CSTNode *e;
    ArcNode *p;
    EnQueue(&Q, i);
    while(!QueueEmpty(&Q))
    {
        DeQueue(&Q, &i);
        p = G->vertices[i].firstarc;
        first = 1; // 用一个标志来标识第一个孩子节
        // 生成节点 i 的子树，第一个邻接点（用first来标识）存入firstchild，之后的邻接点存入 firstchild 的 nextsibling
        while(p)
        {
            if (0 == visited[p->adjvex])
            {
                visited[p->adjvex] = 1;
                // 分配新节点
                e = (CSTNode *)malloc(sizeof(CSTNode));
                e->data = G->vertices[p->adjvex].data;
                e->firstchild = NULL;
                e->nextsibling = NULL;
                // 添加到树中
                if (first)
                {
                    pre->firstchild = e;
                    first = 0;
                }
                else
                {
                    pre->nextsibling = e;
                }
                // 记录上一个访问的节点，它是后一节点的兄弟或下一层的根
                pre = e;
                // 将已生成的树节点入队，以便于之后构建它的子树
                EnQueue(&Q, p->adjvex);
            }
            p = p->nextarc;
        }
    }
}

// 广度优先生成森林
void  BFSForest(ALGraph *G, CSTree *T)
{
    int i, visited[MAX_VERTEX_NUM] = {0};
    // pre 记录上一棵生成树的根
    // e 生成树节点
    CSTNode *pre, *e;
    // 遍历
    for (i = 0; i < G->vexnum; ++i)
    {
        if (0 == visited[i])
        {
            visited[i] = 1;
            e = (CSTNode *)malloc(sizeof(CSTNode)); // 生成树根节点
            e->data = G->vertices[i].data;
            e->firstchild = NULL;
            e->nextsibling = NULL;
            // 添加到树中
            if (*T)
                pre->nextsibling = e; // 当前生成树的根为上一生成树根的兄弟
            else
                (*T) = e; // 第一棵生成树的根
            pre = e; // 记下当前根，用于后续的生成树
            BFSTree(G, i, visited, pre);
        }
    }
}

// 深度优先搜索求关节点
void DFSArticul(ALGraph *G, int i, int *c, int *visited, int *low)
{
    visited[i] = ++(*c);
    int w, minv = visited[i]; // min{visited[i], low[w], visited[k]}
    ArcNode *p = G->vertices[i].firstarc;
    while (p) // 遍历 i 所有邻接点（孩子）计算 low[i]
    {
        w = p->adjvex; // 邻接点 w
        // w 未访问，是 i 的孩子
        if (0 == visited[w])
        {
            // 判断 w 为根的子树是否有回边
            DFSArticul(G, w, c, visited, low); // 计算 low[w]
            if (low[w] < minv) minv = low[w]; // w 子树中有回边
            // 指回双亲也当作回边不做区分，因此取等于
            if (low[w] >= visited[i]) printf("(%d)%c", i, G->vertices[i].data);
        }
        // w 已访问，说明这是条回边 visited[k]
        // 指回双亲也当作回边，但是不影响判断关键点，因此不做区分
        else if (minv > visited[w])
        {
            minv = visited[w];
        }
        p = p->nextarc;
    }
    low[i] = minv; // 计算 low[i]
}

// 求关节点
void FindArticul(ALGraph *G)
{
    int w, c = 1, visited[MAX_VERTEX_NUM] = {0}, low[MAX_VERTEX_NUM];
    // 初始化 visited[0]
    visited[0] = 1; low[0] = 1; // 选取 0 作为根出发
    // 深度优先
    ArcNode *p = G->vertices[0].firstarc;
    w = p->adjvex; // 邻接点
    DFSArticul(G, w, &c, visited, low);
    // 判断根节点是不是关节点（子树个数）
    // 从根 0 的一个邻接点（孩子）出发深度遍历
    // 若返回时仍有节点没有访问
    // 说明根节点至少两棵子树，需要从根的另个邻接点（孩子）出发进行遍历
    // 可判定为关节点

    if (c < G->vexnum)
    {
        printf("(0)%c", G->vertices[0].data);
        p = p->nextarc;
        while(p)
        {
            w = p->adjvex; // 邻接点
            if (0 == visited[w]) DFSArticul(G, w, &c, visited, low);
            p = p->nextarc;
        }
    }
}

// 求各个顶点的入度
void FindInDegree(ALGraph *G, int *InDegree)
{
    int i; ArcNode *p;
    for (i = 0; i < G->vexnum; ++i)
    {
        p = G->vertices[i].firstarc;
        while(p)
        {
            ++InDegree[p->adjvex];
            p = p->nextarc;
        }
    }
}

// 有向无环图 AOV 网--拓扑排序
Status TopologicalSort(ALGraph *G)
{
    // c 用来记录已排序的顶点数，检测图中是否有环，进而判断排序是否成功
    int i, c = 0, InDegree[MAX_VERTEX_NUM] = {0};
    ArcNode *p;
    // 初始化
    FindInDegree(G, InDegree);
    // 定义栈存储顶点，用队列（或数组）也可以，只是用来找入度为 0 的点重新开始排序
    SqStack S; InitStack(&S);
    for (i = 0; i < G->vexnum; ++i)
        if (0 == InDegree[i])
            PushStack(&S, i); // 入度为0入栈
    // 拓扑排序
    while(!StackEmpty(&S))
    {
        PopStack(&S, &i); ++c; // 出栈输出，c++
        printf("(%d)%c ", i, G->vertices[i].data);
        p = G->vertices[i].firstarc;
        while(p)
        {
            // 将它的邻接点入度减一，若减为0，则入栈
            if ((--InDegree[p->adjvex]) == 0) PushStack(&S, p->adjvex);
            p = p->nextarc;
        }
    }

    if (c < G->vexnum) // 有环
        return -1;
    else
        return 0;
}

// 带权有向无环图--AOE网---关键路径--正向拓扑排序计算事件（顶点）的最早发生时间 vet
// 该过程直接基于 AOV 的拓扑排序，只修改/增加部分代码：
// 用 T 记录正向拓扑排序序列，用 vet 记录事件（顶点）的最早发生时间
Status TopologicalOrder(ALGraph *G, SqStack *T, int *vet)
{
    // c 用来记录已排序的顶点数，检测图中是否有环，进而判断排序是否成功
    int i, c = 0, InDegree[MAX_VERTEX_NUM] = {0};
    // int vet[i] = {0}; // 【增加】vet初始化，在CriticalPath函数中初始化
    ArcNode *p;
    // 初始化入度进行拓扑
    FindInDegree(G, InDegree);
    // 定义栈存储顶点，用队列（或数组）也可以，因为只是用来找入度为 0 的点重新开始排序
    SqStack S; InitStack(&S);
    for (i = 0; i < G->vexnum; ++i)
        if (0 == InDegree[i])
            PushStack(&S, i); // 入度为0入栈
    // 正向拓扑排序
    while(!StackEmpty(&S))
    {
        PopStack(&S, &i); ++c; // 出栈输出，c++
        PushStack(T, i); // 【修改】排序序列不再输出，而是存入栈T中
        p = G->vertices[i].firstarc;
        while(p)
        {
            // 将它的邻接点入度减一，若减为0，则入栈
            if ((--InDegree[p->adjvex]) == 0) PushStack(&S, p->adjvex);

            // 【增加】计算邻接点（后续事件）的最早发生时间，vet[p->adjvex] 取最大值，p->weight记录权值
            // 弧 <i, p->adjvex>，由 i 求  p->adjvex
            if (vet[i] + p->weight > vet[p->adjvex]) vet[p->adjvex] = vet[i] + p->weight;

            p = p->nextarc;
        }
    }

    if (c < G->vexnum) // 有环
        return -1;
    else
        return 0;
}

// 6,8 ABCDEF
// A-B,3A-C,2A-D,2A-E,3C-D,4C-F,3D-F,2E-F,1
// 逆向拓扑排序，计算事件最迟发生时间 vlt，进而计算活动的 eet, elt
Status CriticalPath(ALGraph *G)
{
    // 事件最早发生时间 vet = 活动最早发生时间 eet
    // 事件最迟发生时间 vlt
    // vet 初始化
    int i, vet[MAX_VERTEX_NUM] = {0}, vlt[MAX_VERTEX_NUM];
    ArcNode *p;
    // 定义栈存储正向拓扑排序结果
    SqStack T; InitStack(&T);
    // 正向拓扑排序
    if(TopologicalOrder(G, &T, vet) == -1) return -1; // 有环路，正向拓扑失败，无法求关键路径，直接返回

    // 经过正向拓扑得到序列栈以及 vet，接下来逆向拓扑，计算 vlt
    for (i = 0; i < G->vexnum; ++i)
        vlt[i] = vet[G->vexnum-1]; // vlt 初始化 vet[G->vexnum-1]（终点，汇点），因为要逆向求的 vlt（减法）
    // 逆向拓扑
    while(!StackEmpty(&T))
    {
        PopStack(&T, &i);
        p = G->vertices[i].firstarc;
        while(p)
        {
            // 弧 <i, p->adjvex>，由 p->adjvex 求 i
            if (vlt[i] > vlt[p->adjvex] - p->weight) vlt[i] = vlt[p->adjvex] - p->weight;
            p = p->nextarc;
        }
    }
    // 代码运行到这里，所有顶点（事件）的最早发生和最迟发生都以求得：vet，vlt
    for (i = 0; i < G->vexnum; ++i)
        printf("%c et:%d lt:%d\n", G->vertices[i].data, vet[i], vlt[i]);

    // 接下来通过事件的发生时间求边（活动）的发生时间
    int eet, elt; // 活动最早/迟发生时间
    char tag; // 标记关键路径
    for (i = 0; i <G->vexnum; ++i)
    {
        p = G->vertices[i].firstarc;
        while(p)
        {
            // 弧 <i, p->adjvex>
            eet = vet[i]; elt = vlt[p->adjvex] - p->weight;
            tag = (eet == elt) ? '*' : ' '; // 比较 eet, elt
            printf("%c-->%c [et:%d lt:%d] %c\n", G->vertices[i].data, G->vertices[p->adjvex].data, eet, elt, tag);
            p = p->nextarc;
        }
    }
    return 0;
}

// 6,8 ABCDEF
// A-C,10A-E,30A-F,100B-C,5C-D,50D-F,10E-D,20E-F,60
// 迪杰斯特拉求单源最短路径
// 单源 start 到各顶点最短距离存储在 Dist[]中，路径记录在 Path[]，这是个一维数组，它只记录最短路径上的前序节点
// 一个性质：对于 start 到顶点i的最短路径，则 start 到 i 前序节点的最短路径也一定在这条路径上，可用反证法证明。
// 因此只记录下最短路径上的前序节点即可在程序结束时还原最短路径
// 迪杰斯特拉采用由近及远不断累加，路径长度递增的策略，因此不允许有负边
Status ShortestPath_Dijkstra(ALGraph *G, int start, int *Dist, int *Path)
{
    int i, c, min_vex, min;
    // 标记已求得最短距离顶点
    int visited[MAX_VERTEX_NUM] = {0};
    visited[start] = 1;
    ArcNode *p;

    // 初始化 Dist[i]
    // 不相邻初始化为 INF
    for (i = 0; i < G->vexnum; ++i)
    {
        if (i == start) Dist[i] = 0;
        else Dist[i] = INF;
        Path[i] = start; // 记录最短路径上前序节点
    }
    // 相邻初始化为 p->weight
    p = G->vertices[start].firstarc;
    while(p)
    {
        Dist[p->adjvex] = p->weight;
        p = p->nextarc;
    }

    // 剩余 G->vexnum-1 个节点，按路径长度递增生成 Dist[i]
    for (c = 1; c < G->vexnum; ++c)
    {
        // 取剩余节点中当前最小 Dist[i]
        min = INF;
        for (i = 0; i < G->vexnum; ++i)
        {
            if (visited[i] == 0 && Dist[i] < min)
            {
                min = Dist[i]; min_vex = i;
            }
        }
        // 将当前最小距离顶点（min_vex）加入已求得最小距离集合中
        visited[min_vex] = 1;
        // 然后更新 min_vex 邻接点的最小距离
        p = G->vertices[min_vex].firstarc;
        while(p)
        {
            // 迪杰斯特拉采用由近及远不断累加，路径长度递增的策略，因此不允许有负边（&& p->weight > 0）
            if (0 == visited[p->adjvex] && Dist[min_vex] + p->weight < Dist[p->adjvex])
            {
                Dist[p->adjvex] = Dist[min_vex] + p->weight; // 更新 Dist
                Path[p->adjvex] = min_vex; // 更新 Path 前序节点
            }
            p = p->nextarc;
        }
    }
    return 0;
}

void PrintPath_Dist_Dijkstra(ALGraph *G, int start, int *Dist, int *Path)
{
    int i, j, c, path[MAX_VERTEX_NUM];
    for(i = 0; i < G->vexnum; ++i)
    {
        if(i != start)
        {
            j = Path[i]; c = 0;
            while(j != start)
            {
                path[c++] = j;
                j = Path[j];
            }
            printf("%c", G->vertices[start].data);
            for(j = c-1; j >= 0; --j)
                printf("-->%c", G->vertices[path[j]].data);
            printf("-->%c[%d]\n", G->vertices[i].data, Dist[i]);
        }
    }
}

// 弗洛伊德算法
Status ShortestPath_Floyd(ALGraph *G, int (*DistMatrix)[MAX_VERTEX_NUM], int (*PathMatrix)[MAX_VERTEX_NUM])
{
    int u, v, w; ArcNode *p; // w 表示不断插入的顶点
    // 初始化
    // 不相邻初始化为 INF
    for (u = 0; u < G->vexnum; ++u)
    {
        for (v = 0; v < G->vexnum; ++v)
        {
            DistMatrix[u][v] = INF;
            PathMatrix[u][v] = v; // 对于邻接点，初始化为终点，则记录后继，还原时正向找
            //PathMatrix[u][v] = u; // 对于邻接点，初始化为起点，则记录前序，还原时逆向找
        }
    }
    // 相邻初始化为 p->weight
    for (u = 0; u < G->vexnum; ++u)
    {
        DistMatrix[u][u] = 0;
        p = G->vertices[u].firstarc;
        while(p)
        {
            DistMatrix[u][p->adjvex] = p->weight;
            p = p->nextarc;
        }
    }
    // 加点探索
    for (w = 0; w < G->vexnum; ++w)
    {
        for (u = 0; u < G->vexnum; ++u)
        {
            for (v = 0; v < G->vexnum; ++v)
            {
                // 如果新插入的 w 导致 DistMatrix[u][v] 距离减小
                if (DistMatrix[u][w] + DistMatrix[w][v] < DistMatrix[u][v])
                {
                    // 弗洛伊德算法允许有负边，但不允许有负边构成的回路，应加以判断，此处默认都是正边
                    DistMatrix[u][v] = DistMatrix[u][w] + DistMatrix[w][v];
                    PathMatrix[u][v] = PathMatrix[u][w]; // 对应初始化，设置为终点，还原时正向找
                    //PathMatrix[u][v] = PathMatrix[w][v]; // 对应初始化，设置为起点
                }
            }
        }
    }
    return 0;
}

void PrintPath_Dist_Floyd(ALGraph *G, int (*DistMatrix)[MAX_VERTEX_NUM], int (*PathMatrix)[MAX_VERTEX_NUM])
{
    int u, v, j;
    for (u = 0; u < G->vexnum; ++u)
    {
        for (v = 0; v < G->vexnum; ++v)
        {
            printf("%c", G->vertices[u].data);
            j = PathMatrix[u][v];
            while(j != v)
            {
                printf("-->%c", G->vertices[j].data);
                j = PathMatrix[j][v];
            }
            printf("-->%c[%d]\n", G->vertices[v].data, DistMatrix[u][v]);
        }
    }
}
#endif
