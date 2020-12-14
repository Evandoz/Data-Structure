#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;

// 节点定义
typedef struct LNode
{
	ElemType data;
	struct LNode *next;
}LNode, *LinkList;

// 初始化
Status InitList(LinkList *L)
{
	*L = (LNode *)malloc(sizeof(LNode));
	if(!(*L)) return ERROR;
	(*L)->next = NULL; // 头节点不存放数据
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
		p->next = NULL;
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
	while(p && i < pos)
	{
		p = p->next;
		++i;
	}
	if(!p || i > pos) return ERROR;
	(*e) = p->data;
	return OK;
}

// 根据元素返回位置
Status LocateElem(LinkList L, ElemType e, int *pos)
{
	LNode *p;
	int i = 1;
	p = L->next;
	while(p && p->data != e)
	{
		p = p->next;
		++i;
	}
	if (p) { (*pos) = i; return OK; }
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

// 合并顺序表,并将其返回
/*Status ListMerge(LinkList L1, LinkList L2, LinkList L)
{

}*/

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

//遍历
Status ListTraverse(LinkList L)
{
	LNode *p;
	p = L->next;
	while(p)
	{
		printf("%d ",p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}

//测试函数
void test()
{
    // 定义，初始化
	LinkList L; InitList(&L);

    // 创建
    printf("=====================创建========================\n");
	printf("Please input the number of elem:");
	int n;
	scanf("%d", &n);
	TailInsertCreate(L, n);
    //HeadInsertCreate(L,n);
	printf("The length of the linklist is: %d\n", Length(L));
	printf("The list is following:\n");
	ListTraverse(L);

    // 删除指定元素，返回位置
    printf("============删除指定元素，返回位置===============\n");
	int pos1;
	ElemType e1;
	printf("Please input the elem to be deleted:");
	scanf("%d", &e1);
	if(ListDeleteAccordToElem(L, e1, &pos1))
		printf("The pos of the elem deleted is:%d\n", pos1);
	else
		printf("The elem to be deleted donnot exist!");
	printf("The current length of the linklist is: %d\n", Length(L));
	printf("The current list is following:\n");
	ListTraverse(L);

    // 删除指定位置元素，返回元素
    printf("==========删除指定位置元素，返回元素============\n");
	ElemType e2;
	int pos2;
	printf("Please input the pos of elem to be deleted:");
	scanf("%d", &pos2);
	if(ListDeleteAccordToPos(L, pos2, &e2))
		printf("The the elem deleted is:%d\n", e2);
	else
		printf("The elem to be deleted donnot exist!");
	printf("The current length of the linklist is: %d\n", Length(L));
	printf("The current list is following:\n");
	ListTraverse(L);

    // 指定位置插入元素
    printf("==============指定位置插入元素=================\n");
	ElemType e3;
	int pos3;
	printf("Please input the pos of elem to be inserted:");
	scanf("%d", &pos3);
	printf("Please input elem to be inserted:");
	scanf("%d",&e3);
	if(!ListInsert(L, pos3, e3))
        printf("Unsuccessfully insert!\n");
	printf("The current length of the linklist is: %d\n", Length(L));
	printf("The current list is following:\n");
	ListTraverse(L);

    // 获取指定位置元素
    printf("=============获取指定位置元素=================\n");
	ElemType e4;
	int pos4;
	printf("Please input the pos of elem to be checked:");
	scanf("%d", &pos4);
	if(GetElem(L, pos4, &e4))
		printf("The the elem checked is:%d\n", e4);
	else
		printf("The elem to be deleted donnot exist!\n");

    // 获取指定元素位置
    printf("=============获取指定元素位置=================\n");
	ElemType e5;
	int pos5;
	printf("Please input of elem to be checked:");
	scanf("%d", &e5);
	if(LocateElem(L, e5, &pos5))
		printf("The the pos of elem checked is:%d\n", pos5);
	else
		printf("The elem to be deleted donnot exist!\n");

	// 逆序
	printf("====================逆序======================\n");
	InverseList(L);
	printf("The current list is following:\n");
	ListTraverse(L);

	// 销毁
	printf("====================销毁======================\n");
	if(DestroyList(L))
		printf("Successfully Destroy!\n");
	else
		printf("Unsuccessfully Destroy!\n");
}

int main()
{
	test();
	return 0;
}
