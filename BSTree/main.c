#include <stdio.h>
#include <stdlib.h>

typedef int Status;

// 二叉排序树，直接将树节点值定义为数值型，简化处理
typedef int TElemType;

typedef struct BSTNode
{
    TElemType data;
    struct BSTNode *lchild, *rchild;
} BSTNode, *BSTree;

/*=========================查找===========================*/

/*BSTree SearchBST_Recursion(BSTree T, TElemType key)
{
    if (T == NULL || key == T->data) return T;
    else if (key < T->data) return SearchBST(T->lchild, key);
    else return SearchBST(T->rchild, key);
}

BSTree SearchBST_Non_Recursion(BSTree T, TElemType key)
{
    BSTNode *p = T;
    while (p && p->data != key)
    {
        if (key < p->data) p = p->lchild;
        else p = p->rchild;
    }
    return p;
}*/

/*=========================插入===========================*/
// 增删过程中也需要查找节点
// 找到待查值时，返回所得节点，没有找到时，返回最后一个访问的非空节点，以便进行后续插入
// 为了能在没有找到时，返回最后一个访问的节点，在查找过程中需要记录一下上一个访问的节点
// 即当前访问节点的双亲，于是引入指针 f 标识双亲，起始 f = NULL，运行结束不需要返回

// 对原来的查找算法修改，增加指向上一个访问的节点（双亲）
// 返回值修改为布尔量，标识是否找到待查值
Status SearchBST_Recursion(BSTree T, TElemType key, BSTree f, BSTree *p)
{
    // 没有找到，返回最后（上一个）访问到的节点
    if (T == NULL) { (*p) = f; return 0; }
    else if (key == T->data) { (*p) = T; return 1; }
    else if (key < T->data) return SearchBST_Recursion(T->lchild, key, T, p);
    else return SearchBST_Recursion(T->rchild, key, T, p);
}

// 对原来的查找算法修改，增加指向上一个访问的节点（双亲）
// 返回值修改为布尔量，标识是否找到待查值
Status SearchBST_Non_Recursion(BSTree T, TElemType key, BSTree *p)
{
    /*
    BSTNode *f = NULL, *t = T;
    while (t && key != t->data)
    {
        f = t;
        if (key < t->data)  t = t->lchild;
        else t = t->rchild;
    }
    if (t) { (*p) = t; return 1; }
    else { (*p) = f; return 0; }
    */
    BSTNode *f = NULL, *t = T;
    while (t)
    {
        f = t;
        if (key == t->data) { (*p) = t; return 1; }
        else if (key < t->data) t = t->lchild;
        else t = t->rchild;
    }
    (*p) = f;
    return 0;
}

Status InsertBST(BSTree *T, TElemType key)
{
    BSTNode *p, *s;
    if (0 == SearchBST_Non_Recursion(*T, key, &p))
    {
        // 生成新结点
        s = (BSTNode *)malloc(sizeof(BSTNode));
        s->data = key; s->lchild = s->rchild = NULL;
        // 插入
        if (p == NULL) (*T) = s; // 根
        else if (key < p->data) p->lchild = s;
        else p->rchild = s;
        return 1;
    }
    else
        return 0;
}

/*=========================新建===========================*/

// 通过节点插入新建一棵二叉树
Status InitBST(BSTree *T, TElemType *key, int n)
{
    int i;
    (*T) = NULL;
    for (i = 0; i < n; ++i)
    {
        InsertBST(T, key[i]);
    }
    return 1;
}

/*=========================删除===========================*/

// 用前驱顶替
Status Delete(BSTree *p)
{
    BSTNode *q, *s;
    if ((*p)->rchild == NULL)
    {
        q = (*p); (*p) = (*p)->lchild; free(q);
    }
    else if ((*p)->lchild == NULL)
    {
        q = (*p); (*p) = (*p)->rchild; free(q);
    }
    else
    {
        // 左树右下找 p 的前驱 s
        q = (*p); s = (*p)->lchild;
        while(s->rchild)
        {
            q = s;
            s = s->rchild;
        }
        // 前驱 s 的值赋给 (*p)
        (*p)->data = s->data;
        // (*p) 的左孩子即为它的前驱（左孩子没有右子树）
        if (q == *p)
            q->lchild = s->lchild;
        else
            q->rchild = s->lchild;
        free(s);
    }
    return 0;
}

// 将待删除节点 p 的左子树链接到它的双亲 f 顶替自己，
// 将 p 的右子树链接到 p 直接前驱 s，作为 s 的右子树。
Status Delete_2(BSTree *p)
{
    BSTNode *q, *s;
    if ((*p)->rchild == NULL)
    {
        q = (*p); (*p) = (*p)->lchild; free(q);
    }
    else
    {
        // 左树右下找 p 的前驱 s
        q = (*p); s = (*p)->lchild;
        while(s->rchild)
            s = s->rchild;
        // (*p) 的右子树作为 s 的右子树
        s->rchild = (*p)->rchild;
        // (*p) 的左孩子顶替自己
        (*p) = (*p)->lchild;
        free(q);
    }
    return 0;
}

// 递归
Status DeleteBST_Recrsion(BSTree *T, TElemType key)
{
    if ((*T) == NULL) return 0;
    else
    {
        if (key == (*T)->data) return Delete_2(T);
        else if (key < (*T)->data) return DeleteBST_Recrsion(&((*T)->lchild), key);
        else return DeleteBST_Recrsion(&((*T)->rchild), key);
    }
}

// 非递归
Status DeleteBST_Non_Recrsion(BSTree *T, TElemType key)
{
    BSTree p;
    if (SearchBST_Non_Recursion(*T, key, &p))
    {
        Delete(&p);
        return 1;
    }
    else
        return 0;
}

Status InderTraverse(BSTree T)
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
    BSTree T;
    int key[] = {3, 10, 2, 4, 8, 7, 9, 11};
    InitBST(&T, key, 8);
    InderTraverse(T); printf("\n");
    DeleteBST_Recrsion(&T, 8);
    InderTraverse(T); printf("\n");
    InsertBST(&T, 90);
    InderTraverse(T); printf("\n");
    DeleteBST_Non_Recrsion(&T, 10);
    InderTraverse(T);
    return 0;
}
