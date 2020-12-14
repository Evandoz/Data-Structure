// 树节点类型
typedef char TElemType;

// 树节点
typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;
