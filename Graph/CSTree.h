// （首）孩子-兄弟表示法
// 用于记录生成树

typedef char VertexType;

typedef struct CSTNode
{
    VertexType data;
    struct CSTNode *firstchild, *nextsibling;
} CSTNode, *CSTree;

// 先序遍历
void TraverseCSTree(CSTree T)
{
    if (T)
    {
        printf("%c", T->data);
        TraverseCSTree(T->firstchild);
        TraverseCSTree(T->nextsibling);
    }
}
