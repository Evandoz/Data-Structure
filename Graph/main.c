// #include "MGraph.h"
// #include "ALGraph.h"
#include "OLGraph.h"

int main()
{
    // 6,10 ABCDEF
    // 0-1,6 0-2,1 0-3,5 1-2,5 1-4,3 2-3,5 2-4,6 2-5,4 3-5,2 4-5,6
    // A-B,6A-C,1A-D,5B-C,5B-E,3C-D,5C-E,6C-F,4D-F,2E-F,6
    // 13,17 ABCDEFGHIJKLM
    // A-B,1A-C,1A-F,1A-L,1B-C,1B-D,1B-G,1B-H,1B-M,1D-E,1G-H,1G-K,1G-I,1H-K,1J-M,1J-L,1M-L,1
    /*
    // 邻接矩阵
    MGraph G;
    CreateUDN(&G);
    printf("\nMatrix Graph:\n");
    PrintMGraph(&G);

    // 邻接表
    ALGraph G;
    CreateGraph(&G);
    printf("Adjacency Graph:\n");
    PrintALGraph(&G);*/

    // 有向图十字链表
    // 4,7 ABCD
    // A-B,A-C,C-A,C-D,D-A,D-B,D-C
    OLGraph OLG;
    CreateOLGraph(&OLG);
    printf("Orthogonal Graph:\n");
    PrintOLGraph(&OLG);

    /*printf("\nDFS Traverse:\n");
    DFSTraverse(&G);
    printf("\nBFS Traverse:\n");
    BFSTraverse(&G);

    printf("\nDFS Forest:\n");
    CSTree DT = NULL;
    DFSForest(&G, &DT);
    TraverseCSTree(DT);

    printf("\nBFS Forest:\n");
    CSTree BT = NULL;
    BFSForest(&G, &BT);
    TraverseCSTree(BT);

    // 5,7 ABCDE
    // A-B,1A-D,1B-C,1B-D,1C-E,1D-C,1D-E,1
    printf("\nTopological Sort:\n");
    TopologicalSort(&G);

    // 6,8 ABCDEF
    // A-B,3A-C,2A-D,2A-E,3C-D,4C-F,3D-F,2E-F,1
    printf("\nCritical Path:\n");
    CriticalPath(&G);

    printf("\nArticulation Point:\n");
    FindArticul(&G);*/

    /*printf("\nDijkstra Shorset Path:\n");
    int dist[MAX_VERTEX_NUM], path[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    ShortestPath_Dijkstra(&G, 0, dist, path);
    PrintPath_Dist_Dijkstra(&G, 0, dist, path);*/

    // 6,8 ABCDEF
    // A-C,10A-E,30A-F,100B-C,5C-D,50D-F,10E-D,20E-F,60
    /*printf("\nDijkstra Shorset Path:\n");
    int dist[MAX_VERTEX_NUM], path[MAX_VERTEX_NUM];
    ShortestPath_Dijkstra(&G, 0, dist, path);
    PrintPath_Dist_Dijkstra(&G, 0, dist, path);

    printf("\nFloyd Shorset Path:\n");
    int distmatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM], pathmatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    ShortestPath_Floyd(&G, distmatrix, pathmatrix);
    PrintPath_Dist_Floyd(&G, distmatrix, pathmatrix);

    printf("\nPrim MiniSpanTree:\n");
    MiniSpanTree_Prim(&G, 'A');
    printf("\nKruskal MiniSpanTree:\n");
    MiniSpanTree_Kruskal(&G);*/

    return 0;
}
