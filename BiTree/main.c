#include "BiTree.h"

int main()
{
    char *pre = "ABCDEFGHI", *in = "BCAEDGHFI", *post="CBEHGIFDA", *level = "ABDCEFGIH";
    BiTree T, T1;
    //CreateBiTree_Pre_In(pre, in, 9, &T);
    CreateBiTree_Post_In(post, in, 9, &T1);
    //CreateBiTree_Level_In(level, in, 9, &T);

    T = CopyTree_Recursion(T1);

    ChangeValue(T, 'D', 'K');
    BiTNode *p = FindFather_Recursion(T, 'K');
    printf("K's Father Node:%c\n", p->data);
    FindNode_Recursion(T, 'H');
    //InsertLeftNode(T, 'K', 'Z');
    //Insert_Root_LeftNode(T, 'X');
    BiTNode *q = MaxNode_Recursion(T);
    printf("Max Node:%c\n", q->data);
    // ExchangeChild_Recursion(T);
    ExchangeChild_Non_Recursion(T);
    //DeleteNode(T, 'K');

    printf("Node: %d\n", CountNode_Recursion(T));
    printf("Depth: %d\n", Depth_Recursion(T));
    printf("Depth: %d\n", PreOrderTravel_Depth(T));
    printf("Depth: %d\n", LevelOrderTravel_Depth(T));

    //printf("Leaf: %d\n", CountLeaf_Recursion(T));
    printf("Leaf: %d\n", CountLeaf_Non_Recursion(T));

    printf("\nLeaf Node:\n");
    PreOrderTraverse_Leaf(T); printf("\n");
    LevelOrderTraverse_Leaf(T); printf("\n");

    printf("T is equal with T2?: %d\n", IsEqual_Recursion(T, T1));

    printf("\nPreOrder Travel:\n");
    PreOrderTravel_Recursion(T); printf("\n");
    PreOrderTravel_Non_Recursion(T); printf("\n");
    PreOrderTravel_Non_Recursion_2(T); printf("\n");

    printf("\nInOrder Travel:\n");
    InOrderTraverse_Recursion(T); printf("\n");
    InOrderTravel_Non_Recursion(T); printf("\n");
    InOrderTravel_Non_Recursion_2(T); printf("\n");

    printf("\nPostOrder Travel:\n");
    PostOrderTraverse_Recursion(T); printf("\n");
    PostOrderTravel_Non_Recursion(T); printf("\n");

    printf("\nLevelOrder Travel:\n");
    LevelOrderTravel_Non_Recursion(T); printf("\n");
    return 0;
}
