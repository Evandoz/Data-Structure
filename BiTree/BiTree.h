#include <stdio.h>
#include <stdlib.h>

// 树节点类型
typedef char TElemType;

// 树节点
typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

#include "SqStack.h"
#include "SqQueue.h"

typedef int Status;

/*===================工具函数===================*/

// depth 从 0 开始，先++再输出void
void visit(BiTNode *e)
{
    printf("%c", e->data);
}

/*==================创建二叉树==================*/

// 递归创建二叉树
Status CreateBiTree(BiTree *T)
{
	TElemType e;
	scanf("%c", &e);
	if (e == '#') *T = NULL;
	else
	{
		*T = (BiTNode *)malloc(sizeof(BiTNode));
		if (!(*T)) return -1;
		(*T)->data = e;
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
	return 0;
}

// 根据先序和中序创建二叉树
Status CreateBiTree_Pre_In(TElemType *pre, TElemType *in, int len, BiTree *T)
{
    int i;
    if (len <= 0)
    {
        *T = NULL;
        return 0;
    }
    // 先序头为根
    *T = (BiTNode *)malloc(sizeof(BiTNode));
    (*T)->data = *pre;
    // 根据先序头（根） *pre 将中序 in 分割成两部分（子树中序序列）
    for (i = 0; i < len; ++i)
        if (*pre == *(in+i))
            break;
    // 递归子序列
    CreateBiTree_Pre_In(pre+1, in, i, &((*T)->lchild));
    CreateBiTree_Pre_In(pre+i+1, in+i+1, len-i-1, &((*T)->rchild));
    return 0;
}

// 根据后序和中序创建二叉树
Status CreateBiTree_Post_In(TElemType *post, TElemType *in, int len, BiTree *T)
{
    int i;
    if (len <= 0)
    {
        *T = NULL;
        return 0;
    }
    // 后序尾为根
    *T = (BiTNode *)malloc(sizeof(BiTNode));
    (*T)->data = *(post+len-1);
    // 根据后序尾（根）*post 将中序 in 分割成两部分（子树中序序列）
    for (i = 0; i < len; ++i)
        if (*(post+len-1) == *(in+i))
            break;
    // 递归子序列
    CreateBiTree_Post_In(post, in, i, &((*T)->lchild));
    CreateBiTree_Post_In(post+i, in+i+1, len-i-1, &((*T)->rchild));
    return 0;
}

// 根据层序和中序创建二叉树
Status CreateBiTree_Level_In(TElemType *level, TElemType *in, int len, BiTree *T)
{
    int i;
    int j, l;
    // 子树层序序列
    int le = 0, ri = 0, isLeft = 0;
    TElemType left[len], right[len];
    if (len <= 0)
    {
        *T = NULL;
        return 0;
    }
    // 层序头为根
    *T = (BiTNode *)malloc(sizeof(BiTNode));
    (*T)->data = *level;
    // 根据层序头（根）*level 将中序 in 划分为两部分（子树中序序列）
    for (i = 0; i < len; ++i)
        if (*level == *(in+i))
            break;
    // 根据中序子树序列（i为界）将剩余层序划分成两部分（子树层序序列）
    for (j = 1; j < len; ++j)
    {
        // 判断 j 属于哪部分
        isLeft = 0;
        for (l = 0; l < i; ++l)
        {
            // 找到属于左子树
            if (level[j] == in[l])
            {
                isLeft = 1; break;
            }
            // 否则属于右子树
        }
        // 得到左右子树层序序列
        if (isLeft)
            left[le++] = level[j];
        else
            right[ri++] = level[j];
    }
    CreateBiTree_Level_In(left, in, i, &((*T)->lchild));
    CreateBiTree_Level_In(right, in+i+1, len-i-1, &((*T)->rchild));
    return 0;
}

/*==================遍历二叉树==================*/

// 先序递归
Status PreOrderTravel_Recursion(BiTree T)
{
    if (T)
    {
        visit(T);
        PreOrderTravel_Recursion(T->lchild);
        PreOrderTravel_Recursion(T->rchild);
    }
    return 0;
}

// 先序非递归
Status PreOrderTravel_Non_Recursion(BiTree T)
{
    SqStack S; InitStack(&S);
    BiTNode *p = T;
    while(p || !StackEmpty(&S))
    {
        // 左子树进入，栈顶为 p 的双亲
        // 右子树进入，栈顶为 p 的双亲的双亲
        if(p)
        {
            visit(p); PushStack(&S, p); p = p->lchild;
        }
        // 左子树返回（左侧空，已到尽头），退栈（当前子树根），转入右子树
        // 右子树返回（右侧空）当前子树结束，退栈（上一层子树根），转入右子树
        else
        {
            PopStack(&S, &p); p = p->rchild;
        }
    }
    return 0;
}

// 先序非递归2
Status PreOrderTravel_Non_Recursion_2(BiTree T)
{
    SqStack S; InitStack(&S);
    BiTNode *p; PushStack(&S, T);
    while(!StackEmpty(&S))
    {
        while(GetTop(&S, &p) && p)
        {
            visit(p); PushStack(&S, p->lchild);
        }
        // 左子树为空返回，转入右子树
        PopStack(&S, &p);
        if (!StackEmpty(&S))
        {
            PopStack(&S, &p); PushStack(&S, p->rchild);
        }
    }
    return 0;
}

//中序递归
Status InOrderTraverse_Recursion(BiTree T)
{
	if (T)
	{
		InOrderTraverse_Recursion(T->lchild);
		visit(T);
		InOrderTraverse_Recursion(T->rchild);
	}
	return 0;
}

// 中序非递归
Status InOrderTravel_Non_Recursion(BiTree T)
{
    SqStack S; InitStack(&S);
    BiTNode *p = T;
    while(p || !StackEmpty(&S))
    {
        // 左子树进入，栈顶为 p 的双亲
        // 右子树进入，栈顶为 p 的双亲的双亲
        if(p)
        {
            PushStack(&S, p); p = p->lchild;
        }
        // 左子树返回（左侧空，已到尽头），退栈访问当前子树根，转入右子树
        // 右子树返回（右侧空）当前子树结束，退栈访问上一层子树根，转入右子树
        else
        {
            PopStack(&S, &p); visit(p); p = p->rchild;
        }
    }
    return 0;
}

// 中序非递归2
Status InOrderTravel_Non_Recursion_2(BiTree T)
{
    SqStack S; InitStack(&S);
    BiTNode *p; PushStack(&S, T);
    while(!StackEmpty(&S))
    {
        while(GetTop(&S, &p) && p)
        {
            PushStack(&S, p->lchild);
        }
        // 左子树为空返回，访问根，转入右子树
        PopStack(&S, &p);
        if (!StackEmpty(&S))
        {
            PopStack(&S, &p); visit(p); PushStack(&S, p->rchild);
        }
    }
    return 0;
}

//后序递归
Status PostOrderTraverse_Recursion(BiTree T)
{
	if (T)
	{
		PostOrderTraverse_Recursion(T->lchild);
		PostOrderTraverse_Recursion(T->rchild);
		visit(T);
	}
	return 0;
}

// 后序非递归
Status PostOrderTravel_Non_Recursion(BiTree T)
{
    SqStack S; InitStack(&S);
    BiTNode *p = T;
    BiTNode *flag;
    while(p || !StackEmpty(&S))
    {
        // 左子树进入，栈顶为 p 的双亲
        // 右子树进入，栈顶为 p 的双亲的双亲
        if(p)
        {
            PushStack(&S, p); p = p->lchild;
        }
        // 左子树返回（左侧空，已到尽头），取栈顶当前子树根（不退栈），转入右子树
        // 右子树返回(空或已访问)，访问栈顶当前子树根并退栈，取栈顶上一层子树根（不退栈），转入右子树
        else
        {
            GetTop(&S, &p);
            // 右子树非空且未访问
            if (p->rchild && p->rchild != flag)
            {
                p = p->rchild;
            }
            // 右子树空或已访问
            else
            {
                visit(p);
                flag = p;
                PopStack(&S, &p);
                p = NULL; // p 置为 NULL，防止 p 指向的子树返回时再次进栈，造成死循环
            }
        }
    }
    return 0;
}

// 后序非递归2
// 破坏了树的结构，可改用三叉链表来实现（增加一个标记位）
/*Status PostOrderTravel_Non_Recursion_2(BiTree T)
{
    SqStack S; InitStack(&S);
    BiTNode *p, *flag; PushStack(&S, T);
    while(!StackEmpty(&S))
    {
        while(GetTop(&S, &p) && p)
        {
            // 左孩子已访问，防止重复访问，破坏了树的结构
            if (p->lchild == flag) p->lchild = NULL;
            PushStack(&S, p->lchild);
        }
        // 左子树为空返回，访问根，转入右子树
        PopStack(&S, &p);
        if (!StackEmpty(&S))
        {
            GetTop(&S, &p);
            if (p->rchild && p->rchild != flag)
            {
                PushStack(&S, p->rchild);
            }
            else // 右孩子为空，或者右孩子已访问
            {
                visit(p);
                PopStack(&S, &p);
                flag = p; // flag记录上一次访问的节点
            }
        }
    }
    return 0;
}*/

// 层序遍历
Status LevelOrderTravel_Non_Recursion(BiTree T)
{
    SqQueue Q; InitQueue(&Q);
    BiTNode *p; EnQueue(&Q, T);
    while(!QueueEmpty(&Q))
    {
        DeQueue(&Q, &p); visit(p);
        if (p->lchild) EnQueue(&Q, p->lchild);
        if (p->rchild) EnQueue(&Q, p->rchild);
    }
    return 0;
}

/*==================二叉树属性==================*/

//深度
int Depth_Recursion(BiTree T)
{
	int h, hl, hr;
	if (T == NULL)
		h = 0;
	else
	{
		hl = Depth_Recursion(T->lchild);
		hr = Depth_Recursion(T->rchild);
		if (hl >= hr) h = hl + 1;
		else h = hr + 1;
	}
	return h;
}

// 利用先序求深度
int PreOrderTravel_Depth(BiTree T)
{
    int tmp = 0, depth = 0;
    SqStack S; InitStack(&S);
    BiTNode *p = T;
    while(p || !StackEmpty(&S))
    {
        // 左子树进入，栈顶为 p 的双亲
        // 右子树进入，栈顶为 p 的双亲的双亲
        if(p)
        {
            ++tmp; PushStack(&S, p); p = p->lchild;
        }
        // 左子树返回（左侧空，已到尽头），退栈（当前子树根），转入右子树
        // 右子树返回（右侧空）当前子树结束，退栈（上一层子树根），转入右子树
        else
        {
            PopStack(&S, &p); --tmp; p = p->rchild;
        }
        if (depth < tmp) depth = tmp;
    }
    return depth+1;
}

// 利用层序遍历求深度
int LevelOrderTravel_Depth(BiTree T)
{
    int depth = 0;
    SqQueue Q; InitQueue(&Q);
    BiTNode *p, *last; EnQueue(&Q, T);
    // last 标记每一层最后一个节点,当前队列尾
    last = T; // GetRear(&Q, &last);
    while(!QueueEmpty(&Q))
    {
        // 更新last指向下一层最后一个节点
        DeQueue(&Q, &p);
        // 为当前层最后一个节点时 ++depth
        if (p->lchild) EnQueue(&Q, p->lchild);
        if (p->rchild) EnQueue(&Q, p->rchild);
        if (last == p)
        {
            ++depth;
            GetRear(&Q, &last);
        }
    }
    return depth;
}

//节点总个数
int CountNode_Recursion(BiTree T)
{
	int node;
	if (T == NULL)
		node = 0;
	else
		node = CountNode_Recursion(T->lchild) + CountNode_Recursion(T->rchild) + 1;
	return node;
}

//递归计算叶子结点数
int CountLeaf_Recursion(BiTree T)
{
	int leaf;
	if (T == NULL)
		leaf = 0;
	else if (T->lchild == NULL && T->rchild == NULL)
		leaf = 1;
	else
		leaf = CountLeaf_Recursion(T->lchild) + CountLeaf_Recursion(T->rchild);
	return leaf;
}

//非递归计算叶子结点数
int CountLeaf_Non_Recursion(BiTree T)
{
	int leaf = 0;
	SqStack S; InitStack(&S);
	BiTNode *p; p = T;
	while (p || !StackEmpty(&S))
	{
		if (p)
		{
			PushStack(&S, p);
			if (p->lchild == NULL && p->rchild == NULL)
				leaf++;
			p = p->lchild;
		}
		else
		{
			PopStack(&S, &p);
			p = p->rchild;
		}
	}
	return leaf;
}

//先序输出叶子结点
Status PreOrderTraverse_Leaf(BiTree T)
{
	if (T)
	{
		if (T->lchild == NULL && T->rchild == NULL)
			visit(T);
		PreOrderTraverse_Leaf(T->lchild);
		PreOrderTraverse_Leaf(T->rchild);
	}
	return 0;
}

//层序输出叶子结点
Status LevelOrderTraverse_Leaf(BiTree T)
{
	SqQueue Q; InitQueue(&Q);
	BiTNode *p; p = T;
	if (p) //访问根节点
	{
		EnQueue(&Q, p); //根节点入队
		while (!QueueEmpty(&Q))
		{
			DeQueue(&Q, &p); //出队
			if (p->lchild == NULL && p->rchild == NULL)
				visit(p);
			else
			{
				if (p->lchild)
					EnQueue(&Q, p->lchild);
				if (p->rchild)
					EnQueue(&Q, p->rchild);
			}
		}
	}
	return 0;
}

//判断两株二叉树是否相等
Status IsEqual_Recursion(BiTree T1, BiTree T2)
{
	int flag = 0;
	if (T1 == NULL && T2 == NULL)
		flag = 1;
	else if (T1 && T2)
	{
		if (T1->data == T2->data)
		{
			if (IsEqual_Recursion(T1->lchild, T2->lchild))
				flag = IsEqual_Recursion(T1->rchild, T2->rchild);
		}
	}
	return flag;
}

// 复制二叉树
BiTree CopyTree_Recursion(BiTree T)
{
	BiTNode *p;
	if (T == NULL) return T;
	p = (BiTNode*)malloc(sizeof(BiTNode));
	p->data = T->data;
	p->lchild = CopyTree_Recursion(T->lchild);
	p->rchild = CopyTree_Recursion(T->rchild);
	return p;
}

//递归交换二叉树左右孩子
Status ExchangeChild_Recursion(BiTree T)
{
	BiTNode *tmp = NULL;
	if (T)
	{
		ExchangeChild_Recursion(T->lchild);
		ExchangeChild_Recursion(T->rchild);
		tmp = T->lchild;
		T->lchild = T->rchild;
		T->rchild = tmp;
		return 0;
	}
	return -1;
}

//非递归交换左右孩子
Status ExchangeChild_Non_Recursion(BiTree T)
{
	SqStack S; InitStack(&S);
	BiTNode *p; p = T;
	BiTNode *tmp = NULL;
	while (p || !StackEmpty(&S))
	{
		if (p)
		{
			PushStack(&S, p);

			tmp = p->lchild;
			p->lchild = p->rchild;
			p->rchild = tmp;

			p = p->lchild;
		}
		else
		{
			PopStack(&S, &p);
			p = p->rchild;
		}
	}
	return 0;
}

//查找某个信息是否在树中
BiTNode* FindNode_Recursion(BiTree T, TElemType e)
{
	BiTNode *p; p = T;
	if (T == NULL)
		return NULL;
	else
	{
		if (T->data == e)
			return T;
		else
		{
			p = FindNode_Recursion(T->lchild, e);
			if (p)
			{
			    printf("%c", T->lchild->data); // 输出祖先
			}
			else
            {
			    p = FindNode_Recursion(T->rchild, e);
			    if (p) printf("%c", T->rchild->data); // 输出祖先
			}
			return p;
		}
	}
}

// 查找父亲节点
BiTNode* FindFather_Recursion(BiTree T, TElemType e)
{
    BiTNode *p;
	if (T == NULL || T->data == e)
		return NULL;
	if ((T->lchild && T->lchild->data == e) ||
		(T->rchild && T->rchild->data == e))
		return T;
	p = FindFather_Recursion(T->lchild, e);
	if (p == NULL)
		p = FindFather_Recursion(T->rchild, e);
	return p;
}

//任意节点左插入，原来 s 的左孩子作为新节点 e 的左孩子
Status InsertLeftNode(BiTree T, TElemType s, TElemType e)
{
	BiTNode *p;
	p = FindNode_Recursion(T, s);
	if (p == NULL) return -1;
	BiTNode *q = (BiTNode *)malloc(sizeof(BiTNode));
	q->data = e;
	q->lchild = p->lchild;
	q->rchild = NULL;
	p->lchild = q;
	return 0;
}

//根节点左插入
Status Insert_Root_LeftNode(BiTree T, TElemType e)
{
	BiTNode *p;
	if (T == NULL) return -1;
	p = (BiTNode *)malloc(sizeof(BiTNode));
	p->data = e;
	p->lchild = T->lchild;
	p->rchild = NULL;
	T->lchild = p;
	return 0;
}

//根节点右插入
Status Insert_Root_RightNode(BiTree T, TElemType e)
{
	BiTNode *p;
	if (T == NULL)
		return -1;
	p = (BiTNode *)malloc(sizeof(BiTNode));
	p->data = e;
	p->lchild = NULL;
	p->rchild = T->rchild;
	T->rchild = p;
	return 0;
}

Status ChangeValue(BiTree T, TElemType s, TElemType e)
{
	BiTNode *p = NULL;
	p = FindNode_Recursion(T, s);
	if (p == NULL)
		return -1;
	p->data = e;
	return 0;
}

//删除子树
Status DeleteNode(BiTree T, TElemType e)
{
    BiTNode *p;
    p = FindFather_Recursion(T, e);
    if(p == NULL) return -1;
    if(p->lchild->data == e)
        p->lchild = NULL;
    else
        p->rchild = NULL;
    return 0;
}

// 销毁二叉树
Status Destroy_Recursion(BiTree T)
{
	if (T == NULL)
		return -1;
	Destroy_Recursion(T->lchild);
	Destroy_Recursion(T->rchild);
	free(T);
	return 0;
}

//删除左子树
Status DestroyLeftTree_Recursion(BiTree T)
{
	if (T == NULL)
		return -1;
	DestroyLeftTree_Recursion(T->lchild);
	T->lchild = NULL;
	return 0;
}

//删除右子树
Status DestroyRightTree_Recursion(BiTree T)
{
	if (T == NULL)
		return -1;
	DestroyLeftTree_Recursion(T->rchild);
	T->rchild = NULL;
	return 0;
}

// 最大值
BiTNode* MaxNode_Recursion(BiTree T)
{
	if (T == NULL) return NULL;
	BiTNode *tmp, *pMax = T;
	tmp = MaxNode_Recursion(T->lchild);
	if (tmp)
	{
		if (tmp->data > pMax->data)
			pMax = tmp;
	}
	tmp = MaxNode_Recursion(T->rchild);
	if (tmp)
	{
		if (tmp->data > pMax->data)
			pMax = tmp;
	}
	return pMax;
}
