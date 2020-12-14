#include <stdio.h>
#include <stdlib.h>

typedef char TElemType;
typedef int Status;

// 线索标志位
// Link(0)表示孩子指针；Tread(1)表示前驱后继
typedef enum {Link, Thread} PointerTag;

typedef struct BiThrNode
{
    TElemType data;
    struct BiThrNode *lchild, *rchild;
    PointerTag ltag, rtag;
}BiThrNode, *BiThrTree;

void visit(BiThrNode *e)
{
    printf("%c", e->data);
}

// 根据先序和中序初始化创建二叉链表
Status CreateBiThrTree_Pre_In(TElemType *pre, TElemType *in, int len, BiThrTree *T)
{
    int i;
    if (len <= 0)
    {
        *T = NULL; return 0;
    }
    // 产生一个子树根节点（先序头）
    *T = (BiThrNode *)malloc(sizeof(BiThrNode));
    (*T)->data = *pre; (*T)->ltag = Link; (*T)->rtag = Link;
    // 根据根节点分割中序子树序列
    for (i = 0; i < len; ++i)
        if (*pre == *(in+i))
            break;
    // 递归子树序列
    CreateBiThrTree_Pre_In(pre+1, in, i, &((*T)->lchild));
    CreateBiThrTree_Pre_In(pre+i+1, in+i+1, len-i-1, &((*T)->rchild));
    return 0;
}

/*=========================先序线索化===========================*/

// 构建中序线索二叉树
// pre 指针指向前一个访问的节点，以便于建立线索
Status PreThreading_Recursion(BiThrTree T, BiThrTree *pre)
{
    if (T)
    {
        // 建立当前节点的前驱
        if (T->lchild == NULL)
        {
            T->ltag = Thread;
            T->lchild = (*pre);
        }
        // 建立前驱节点的后继
        if ((*pre)->rchild == NULL)
        {
            (*pre)->rtag = Thread;
            (*pre)->rchild = T;
        }
        // 移动 pre 指针
        (*pre) = T;
        if (T->ltag == Link)
            PreThreading_Recursion(T->lchild, pre);
        if (T->rtag == Link)
            PreThreading_Recursion(T->rchild, pre);
    }
    return 0;
}

// 先序线索二叉树后续处理
// 添加一个头节点 Thrt，便于遍历
Status PreOrderThreading_Thrt(BiThrTree T, BiThrTree *Thrt)
{
    BiThrTree pre;
    // 初始化头节点
    (*Thrt) = (BiThrNode *)malloc(sizeof(BiThrNode));
    // lchild 指向原树的根（作为左孩子）
    (*Thrt)->ltag = Link; (*Thrt)->lchild = (*Thrt);
    // rchild 指向先序序列最后一个节点（作为后继，构成双向线索链表）
    (*Thrt)->rtag = Thread; (*Thrt)->rchild = (*Thrt);
    if (T)
    {
        (*Thrt)->lchild = T; // lchild 指向原树的根

        pre = (*Thrt); // 先序第一个节点前驱指向 *Thrt
        PreThreading_Recursion(T, &pre);
        pre->rtag = Thread;
        pre->rchild = (*Thrt); // 先序最后一个节点后继指向 *Thrt

        (*Thrt)->rchild = pre; // rchild 指向后序最后一个节点
    }
    return 0;
}

/*=========================中序线索化===========================*/

// 构建中序线索二叉树
// pre 指针指向前一个访问的节点，以便于建立线索
Status InThreading_Recursion(BiThrTree T, BiThrTree *pre)
{
    if (T)
    {
        InThreading_Recursion(T->lchild, pre);

        // 建立当前节点的前驱
        if (T->lchild == NULL)
        {
            T->ltag = Thread;
            T->lchild = (*pre);
        }
        // 建立前驱节点的后继
        if ((*pre)->rchild == NULL)
        {
            (*pre)->rtag = Thread;
            (*pre)->rchild = T;
        }
        // 移动 pre 指针
        (*pre) = T;

        InThreading_Recursion(T->rchild, pre);
    }
    return 0;
}

// 中序线索二叉树后续处理
// 不添加头节点
Status InOrderThreading(BiThrTree T)
{
    BiThrTree pre = NULL;
    if (T)
    {
        InThreading_Recursion(T, &pre);
        pre->rtag = Thread;
        pre->rchild = NULL; // 中序最后一个节点后继指 NULL
    }
    return 0;
}

// 中序线索二叉树后续处理
// 添加一个头节点 Thrt，便于遍历
Status InOrderThreading_Thrt(BiThrTree T, BiThrTree *Thrt)
{
    BiThrTree pre;
    // 初始化头节点
    (*Thrt) = (BiThrNode *)malloc(sizeof(BiThrNode));
    // lchild 指向原树的根（作为左孩子）
    (*Thrt)->ltag = Link; (*Thrt)->lchild = (*Thrt);
    // rchild 指向中序序列最后一个节点（作为后继，构成双向线索链表）
    (*Thrt)->rtag = Thread; (*Thrt)->rchild = (*Thrt);
    if (T)
    {
        (*Thrt)->lchild = T; // lchild 指向原树的根

        pre = (*Thrt); // 中序第一个节点前驱指向 *Thrt
        InThreading_Recursion(T, &pre);
        pre->rtag = Thread;
        pre->rchild = (*Thrt); // 中序最后一个节点后继指向 *Thrt

        (*Thrt)->rchild = pre; // rchild 指向中序最后一个节点
    }
    return 0;
}

/*=========================后序线索化===========================*/

// 构建后序线索二叉树
// pre 指针指向前一个访问的节点，以便于建立线索
Status PostThreading_Recursion(BiThrTree T, BiThrTree *pre)
{
    if (T)
    {
        if (T->ltag == Link)
            PostThreading_Recursion(T->lchild, pre);
        if (T->rtag == Link)
            PostThreading_Recursion(T->rchild, pre);
        // 建立当前节点的前驱
        if (T->lchild == NULL)
        {
            T->ltag = Thread;
            T->lchild = (*pre);
        }
        // 建立前驱节点的后继
        if ((*pre)->rchild == NULL)
        {
            (*pre)->rtag = Thread;
            (*pre)->rchild = T;
        }
        // 移动 pre 指针
        (*pre) = T;
    }
    return 0;
}

// 后序线索二叉树后续处理
// 添加一个头节点 Thrt，便于遍历
Status PostOrderThreading_Thrt(BiThrTree T, BiThrTree *Thrt)
{
    BiThrTree pre;
    // 初始化头节点
    (*Thrt) = (BiThrNode *)malloc(sizeof(BiThrNode));
    // lchild 指向原树的根（作为左孩子）
    (*Thrt)->ltag = Link; (*Thrt)->lchild = (*Thrt);
    // rchild 指向后序序列最后一个节点（作为后继，构成双向线索链表）
    (*Thrt)->rtag = Thread; (*Thrt)->rchild = (*Thrt);
    if (T)
    {
        (*Thrt)->lchild = T; // lchild 指向原树的根

        pre = (*Thrt); // 后序第一个节点前驱指向 *Thrt
        PostThreading_Recursion(T, &pre);
        // 后序最后一个节点（根）后继指向 *Thrt，
        // 前提是根节点无右孩子，需要判断一下，否则就破坏了树结构
        // 这一点与先/中序不同
        if (pre->rchild == NULL)
        {
            pre->rtag = Thread;
            pre->rchild = (*Thrt);
        }

        (*Thrt)->rchild = pre; // rchild 指向后序最后一个节点
    }
    return 0;
}

/*=========================先序线索遍历===========================*/

// 先序线索二叉树遍历（后继正序），不需要辅助栈
Status PreOrderTravel_Thr_Non_Recursion(BiThrTree Thrt)
{
    BiThrNode *p = Thrt->lchild; // 根
    // 先序最后一个节点后继指向 Thrt
    // p 节点后继线索回到Thrt时，遍历结束
    while(p != Thrt)
    {
        // 最开始起点为根
        visit(p);
        // 节点后续：
        // rtag == 1，即为 rchild
        while(p->rtag == Thread && p->rchild != Thrt)
        {
            visit(p->rchild);
            p = p->rchild;
        }
        // rtag == 0，有左子树即为左孩子；无左子树即为右孩子
        while(p->ltag == Link)
        {
            p = p->lchild;
            visit(p);
        }
        p = p->rchild;
    }
    return 0;
}

// 先序线索二叉树遍历（前驱逆序），找前驱需要双亲信息，故仍需要辅助栈
Status Re_PreOrderTravel_Thr_Non_Recursion(BiThrTree Thrt)
{
    return 0;
}

/*=========================中序线索遍历===========================*/

// 中序线索二叉树遍历（后继正序），不需要辅助栈
Status InOrderTravel_Thr_Non_Recursion(BiThrTree Thrt)
{
    BiThrNode *p = Thrt->lchild; // 根
    // 中序最后一个节点后继指向 Thrt
    // p 节点后继线索回到Thrt时，遍历结束
    while(p != Thrt)
    {
        // 最开始起点为整个子树最左下
        // 节点后续：
        // rtag == 0，即为右子树最左下节点
        while(p->ltag == Link) p = p->lchild;
        visit(p);
        // rtag == 1，即为 rchild
        while(p->rtag == Thread && p->rchild != Thrt)
        {
            visit(p->rchild);
            p = p->rchild;
        }
        // rtag == 0，即为右子树最左下节点，因此跳转至右子树
        p = p->rchild;
    }
    return 0;
}

// 中序线索二叉树遍历（前驱逆序），不需要辅助栈
Status Re_InOrderTravel_Thr_Non_Recursion(BiThrTree Thrt)
{
    BiThrNode *p = Thrt->rchild; // 最后一个
    // 中序第一个节点前驱指向 Thrt
    // p 节点前驱线索回到Thrt时，遍历结束
    while(p != Thrt)
    {
        // 节点后续：
        visit(p);
        // ltag == 1，即为 lchild
        while(p->ltag == Thread && p->lchild != Thrt)
        {
            visit(p->lchild);
            p = p->lchild;
        }
        // ltag == 0，即为左子树最右下节点，因此跳转至左子树
        p = p->lchild;
        while(p->rtag == Link) p = p->rchild;
    }
    return 0;
}

/*=========================后序线索遍历===========================*/

// 后序线索二叉树遍历（后继正序），找后继需要双亲信息，故仍需要辅助栈
// 不能用 p == Thrt 作为结束条件，因为后序序列最后一个是根节点
// 如果根节点有右孩子，根就没与后继指针指向头节点
Status PostOrderTravel_Thr_Non_Recursion(BiThrTree Thrt)
{
    return 0;
}

// 后序线索二叉树遍历（前驱逆序），不需要辅助栈
Status Re_PostOrderTravel_Thr_Non_Recursion(BiThrTree Thrt)
{
    BiThrNode *p = Thrt->rchild; // 根
    // 后序最后一个节点后继指向 Thrt
    // p 节点后继线索回到Thrt时，遍历结束
    while(p != Thrt)
    {
        // 最开始起点为根
        visit(p);
        // 节点前驱：
        // ltag == 1，即为 lchild
        while(p->ltag == Thread && p->lchild != Thrt)
        {
            visit(p->lchild);
            p = p->lchild;
        }
        // ltag == 0，有右子树即为右孩子；无右子树即为左孩子
        while(p->rtag == Link)
        {
            p = p->rchild;
            visit(p);
        }
        p = p->lchild;
    }
    return 0;
}

int main()
{
    char *pre = "ABCDEFGHI", *in = "BCAEDGHFI", *post="CBEHGIFDA", *level = "ABDCEFGIH";
    BiThrTree T, Thrt;
    CreateBiThrTree_Pre_In(pre, in, 9, &T);

    /*PreOrderThreading_Thrt(T, &Thrt);
    PreOrderTravel_Thr_Non_Recursion(Thrt);printf("\n");

    InOrderThreading_Thrt(T, &Thrt);
    InOrderTravel_Thr_Non_Recursion(Thrt);printf("\n");
    Re_InOrderTravel_Thr_Non_Recursion(Thrt);*/

    PostOrderThreading_Thrt(T, &Thrt);
    Re_PostOrderTravel_Thr_Non_Recursion(Thrt);
    return 0;
}

