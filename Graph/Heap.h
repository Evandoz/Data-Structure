#ifndef Heap_h
#define Heap_h

#define INF 65535
// 堆排序，用于找到当前最小权值的弧边

// 定义边集数组，建立小根堆
typedef struct
{
    int vex_1, vex_2; // 边的两个顶点
    int weight; // 权值
} ArcHeap;

// 向下调整
void HeapAdjust(ArcHeap *AH, int p, int n)
{
    int i;
    AH[0] = AH[p]; // 暂存，p指向双亲
    for (i = 2*p; i <= n; ++i) // i 指向孩子
    {
        // 小根堆，找到较小的孩子
        if (i < n && AH[i+1].weight < AH[i].weight) ++i;
        if (AH[0].weight <= AH[i].weight) break;
        AH[p] = AH[i]; p = i;
    }
    AH[p] = AH[0];
}

// 初始化边集数组，置小根堆
Status InitArcHeap(ArcHeap *AH, int (*arcs)[20], int arcnum, int vexnum)
{
    int i, j, c = 1; // 从1开始
    for (i = 0; i < vexnum; ++i)
    {
        for (j = 0; j < i; ++j) // 防止重复录入，只查找左下三角矩阵
        {
            if (*(*(arcs+i)+j) != INF)
            {
                AH[c].vex_1 = i;
                AH[c].vex_2 = j;
                AH[c++].weight = arcs[i][j];
            }
        }
    }


    for (c = arcnum/2; c > 0; --c)
        HeapAdjust(AH, c, arcnum);

    return 0;
}

// 取当前最小值，并重新调整
int GetRoot(ArcHeap *AH, int n)
{
    AH[0] = AH[1]; AH[1] = AH[n]; AH[n] = AH[0];

    HeapAdjust(AH, 1, n-1);

    return n;
}
#endif
