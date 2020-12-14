#include <stdio.h>
#include <stdlib.h>

#define LH 1  // 左高
#define EH 0  // 同高
#define RH -1 // 右高

#define TRUE 1
#define FALSE 0

typedef int Status;

typedef int TElemType;

// 平衡二叉树 AVL
typedef struct BBSTNode
{
    TElemType data;
    struct BBSTNode *lchild, *rchild;
    int bf; //平衡因子
} BBSTNode, *BBSTree;

// 左旋
void L_Rotate(BBSTree *p)
{
    // 对以(*p)为根的树右旋处理
    BBSTree rc = (*p)->rchild;
    (*p)->rchild = rc->lchild; // 把rc的左子树挂在到 (*p) 的右子树
    rc->lchild = (*p);
    (*p) = rc; // rc 为右旋后的新根，(*p) 指向 rc
}

// 右旋
void R_Rotate(BBSTree *p)
{
    // 对以(*p)为根的树右旋处理
    BBSTree L = (*p)->lchild;
    (*p)->lchild = L->rchild; // 把L的右子树挂在到(*p)的左子树
    L->rchild = (*p);
    (*p) = L; // L 为右旋后的新根，让(*p)指向L
}

// 左平衡，大于+1需要右旋转，都大于0时只需单向右旋转，否则先要将小于0的左旋转调整为负，即，要先左后右双向旋转
void LeftBalance(BBSTree *T) // 对以 指针T所指节点 为根的二叉树进行做平衡旋转处理
{
    BBSTree L = (*T)->lchild, Lr; // Lr指向(*T)左孩子的右孩子（T左孩子右子树的根）

    switch (L->bf)
    {
    case LH: // 插入后左高，新插入节点在(*T)左孩子的左子树上，只需要单项右旋
        (*T)->bf = L->bf = EH; // +2旋转变为0
        R_Rotate(T);
        break;
    case RH: // 插入后右高，新插入节点在(*T)左孩子的右子树上，需要先左旋转
        Lr = L->rchild;
        switch (Lr->bf) // 修改(*T)左、右子树的平衡因子
        {
        case LH:
            (*T)->bf = RH; L->bf = EH; break;
        case EH:
            (*T)->bf = EH; L->bf = EH; break;
        case RH:
            (*T)->bf = EH; L->bf = LH; break;
        }
        修改(*T)的平衡因子
        Lr->bf = EH;
        L_Rotate(&((*T)->lchild)); //左旋
        R_Rotate(T); // 右旋
    }
}

// 右平衡，小于-1需要右旋转，都小于0时只需单向左旋转，否则先要将小于0的右旋转调整为负，即，要先右后左双向旋转
void RightBalance(BBSTree *T)
{
    BBSTree R = (*T)->rchild, Rl; // Lr指向(*T)右孩子的左孩子（T右孩子左子树的根）

    switch (R->bf)
    {
    case LH: // 插入后左高，新插入节点在(*T)左孩子的右子树上，需要先左旋转
        Rl = R->lchild;
        switch (Rl->bf) // 修改(*T)及其右子树的平衡因子
        {
        case LH:
            (*T)->bf = EH; R->bf = RH; break;
        case EH:
            (*T)->bf = EH; R->bf = EH; break;
        case RH:
            (*T)->bf = LH; R->bf = EH; break;
        }

        Rl->bf = EH;
        R_Rotate(&((*T)->rchild)); //右旋
        L_Rotate(T); // 左旋
    case RH: // 插入后右高，新插入节点在(*T)右孩子的右子树上，只需要单项左旋
        (*T)->bf = R->bf = EH; // -2旋转变为0
        L_Rotate(T);
        break;
    }
}

// 插入
// taller变量用于标识树*T是否长高
Status InsertAVL(BBSTree *T, TElemType key, int *taller)
{
    if ((*T) == NULL)
    {
        (*T) = (BBSTNode *)malloc(sizeof(BBSTNode));
        (*T)->data = key;
        (*T)->lchild = (*T)->rchild = NULL;
        (*T)->bf = EH; // 只有根
        (*taller) = TRUE;
        return 0;
    }
    else
    {
        if (key == (*T)->data)
        {
            (*taller) = FALSE; // 已存在不需要插入
            return -1;
        }
        else if (key < (*T)->data)
        {
            if (InsertAVL(&((*T)->lchild), key, taller) == -1) // 已存在没插入
            {
                return -1;
            }

            if (*taller) // 插入新元素后，检查树是否长高，以便于计算bf，进而进行平衡调整
            {
                switch ((*T)->bf)
                {
                case LH: // 原本是左高，现在左边又插入新节点，不再平衡,需要调整
                    LeftBalance(T); // 左平衡
                    (*taller) = FALSE;
                    break;
                case EH: // 原本是同高，现在左边又插入新节点，因此变成左高
                    (*T)->bf = LH;
                    (*taller) = TRUE;
                    break;
                case RH: // 原本是右高，现在左边又插入新节点，因此变成同高
                    (*T)->bf = EH;
                    (*taller) = FALSE;
                    break;
                }
            }
        }
        else
        {
            if (InsertAVL(&((*T)->rchild), key, taller) == -1) // 已存在没插入
            {
                return -1;
            }

            if (*taller) // 检查树是否长高，以便于计算bf，进而进行平衡调整
            {
                // 与左插情形相反
                switch ((*T)->bf)
                {
                case LH: // // 原本是左高，现在右边又插入新节点，因此变成同高
                    (*T)->bf = EH;
                    (*taller) = FALSE;
                    break;
                case EH:
                    (*T)->bf = RH;
                    (*taller) = TRUE; // 长高
                    break;
                case RH:
                    RightBalance(T); // 右平衡
                    (*taller) = FALSE;
                    break;
                }
            }
        }
        return 0;
    }
}

Status InderTraverse(BBSTree T)
{
    if (T)
    {
        InderTraverse(T->lchild);
        printf("%d ", T->data);
        InderTraverse(T->rchild);
    }
    return 0;
}

int main()
{
    BBSTree T = NULL; int key[] = {4, 1, 9, 6, 5, 3};
    int i, taller;
    for (i = 0; i < 6; ++i)
        InsertAVL(&T, key[i], &taller);
    InderTraverse(T);
    return 0;
}
