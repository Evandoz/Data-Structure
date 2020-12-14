#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;

// 循环链表的定义与普通链表没有什么不同
// 只是它最后一个数据元素的指针域不再为NULL，而是指向头节点

// 节点定义
typedef struct LNode
{
	ElemType data;
	struct LNode *next;
} LNode, *LinkList;

// 初始化
Status InitList(LinkList *L)
{
	*L = (LNode *)malloc(sizeof(LNode));
	if(!(*L)) return ERROR;
	(*L)->next = (*L); // 起始，指向自己，头节点不存放数据
	return OK;
}

// 头插法
Status HeadInsertCreate(LinkList L, int n)
{
	ElemType e;
	while(n--)
	{
	    scanf("%d",&e);
		LNode *p;
		p = (LNode *)malloc(sizeof(LNode));
		p->data = e;
		p->next = L->next;
		L->next = p;
	}
	return OK;
}

// 尾插法
Status TailInsertCreate(LinkList L, int n)
{
	LinkList tail;
	tail = L;
	ElemType e;
	while(n--)
	{
	    scanf("%d",&e);
		LNode *p;
		p = (LNode *)malloc(sizeof(LNode));
		p->data = e;
		p->next = tail->next; // 不再是NULL
		tail->next = p;
		tail = p;
	}
	return OK;
}

// 根据位置返回元素
Status GetElem(LinkList L, int pos, ElemType *e)
{
	LNode *p;
	p = L->next;
	int i = 1;
	while(p != L && i < pos)
	{
		p = p->next;
		++i;
	}
	if(p == L || i > pos) return ERROR;
	(*e) = p->data;
	return OK;
}

// 根据元素返回位置
Status LocateElem(LinkList L, ElemType e, int *pos)
{
	LNode *p;
	int i = 1;
	p = L->next;
	while(p != L && p->data != e)
	{
		p = p->next;
		++i;
	}
	if (p != L) { (*pos) = i; return OK; }
	return ERROR;
}

// 根据位置插入元素
Status ListInsert(LinkList L, int pos, ElemType e)
{
	LNode *pre = L, *p;
	// 获取pos 位置前驱
	int i = 0;
	while(pre && i < pos-1)
	{
		pre = pre->next;
		++i;
	}
	if(!pre || i > pos-1) return ERROR;

	p = (LNode *)malloc(sizeof(LNode));
	p->data = e;
	p->next = pre->next;
	pre->next = p;
	return OK;
}

/*
// 根据位置删除元素
Status ListDeleteAccordToPos(LinkList L, int pos, ElemType *e)
{
	LNode *pre;
	pre = L;
	int i = 0;
	while(pre->next && i < pos-1)
	{
		pre = pre->next;
		i++;
	}
	if(!pre->next || i > pos-1) return ERROR;
	LNode *p;
	p = pre->next; pre->next = p->next;
	(*e) = p->data;
	free(p);
	return OK;
}

// 根据值删除元素，返回位置
Status ListDeleteAccordToElem(LinkList L, ElemType e, int *pos)
{
	LNode *pre,*p;
	int i = 1;
	pre = L;
	p = L->next;
	while(p)
	{
		if(p->data == e)
		{
			pre->next = p->next;
			(*pos) = i;
			free(p);
			return OK;
		}
		pre = p; //记录它的前驱节点
		p = p->next;
		i++;
	}
	return ERROR;
}

//长度
int Length(LinkList L)
{
	LNode *p;
	int length = 0;
	p = L;
	while(p->next)
	{
		length++;
		p = p->next;
	}
	if(!p) length = 0;
	return length;
}

//销毁
Status DestroyList(LinkList L)
{
	LNode *p, *tmp;
	p = L;
	while(p)
	{
		tmp = p;
		p = p->next;
		free(tmp);
	}
	return OK;
}

//逆置
Status InverseList(LinkList L)
{
	LNode *p, *q, *tmp;
	p = L->next;
	q = p->next;
	tmp = NULL;
	while(q)
	{
		tmp = q->next;
		q->next = p;
		p = q;
		q = tmp;
	}
	L->next->next = NULL;
	L->next = p;
	return OK;
}
*/

//遍历
Status ListTraverse(LinkList L)
{
	LNode *p;
	p = L->next;
	while(p != L)
	{
		printf("%d ",p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}

// 魔术师发牌问题
LinkList CreateLinkList(int n)
{
    // 不要头节点创建
    LinkList head = NULL;
    LinkList s, r;
    int i;
    r = head;
    for (i = 1; i <= n; ++i)
    {
        s = (LinkList)malloc(sizeof(LNode));
        // s->data = 0; // 13张牌
        s->data = i; // 拉丁方阵

        if (head == NULL)
            head = s;
        else
            r->next = s;
        r = s; // 尾指针移动
    }
    r->next = head; // 构成环，循环链表
    return head;
}

// 发牌
void Magician(LinkList head)
{
    LinkList p;
    int i, countNumber = 2;

    p = head;
    p->data = 1; // 第一张牌

    while (1)
    {
        // 发下一张牌
        for (i = 0; i < countNumber; ++i)
        {
            p = p->next;
            if (p->data != 0) --i; // 该位置已发牌，牌放到到了桌面，故跳过该位置
        }

        // 找到下一张牌的位置
        if (p->data == 0)
        {
            p->data = countNumber;

            ++countNumber;

            if (countNumber == 14)
                break; // 牌已发完
        }
    }

    p = head;

    for (i = 0; i < 13; ++i)
	{
		printf("黑桃%d ",p->data);
		p = p->next;
	}
	printf("\n");
}

// 拉丁方阵
void latin()
{
    int i, j, n;
    printf("Please input the size of matrix:");
    scanf("%d", &n);
    LinkList head = CreateLinkList(n);

    LinkList p = head;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            printf("%2d ",p->data);
            p = p->next;
        }
        printf("\n");
        p = head->next; // 下一行从下一个位置开始
        head = p;
    }
    printf("\n");
}

int main()
{
    //LinkList L = CreateLinkList(13);
    //Magician(L);
    latin();
	return 0;
}

