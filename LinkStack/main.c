#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int SElemType;
typedef int Status;

//栈节点定义
typedef struct SNode
{
	SElemType data;
	struct SNode *next;
}SNode, *LinkStack;

//初始化
Status InitStack(LinkStack *S)
{
	*S = (SNode *)malloc(sizeof(SNode));
	if (!(*S)) return ERROR;
	(*S)->next = NULL;
	return OK;
}

//栈空
Status StackEmpty(LinkStack S)
{
	if (!S->next)
		return OK;
	return ERROR;
}

//压栈
Status PushStack(LinkStack *S, SElemType e)
{
	SNode *p;
	p = (SNode *)malloc(sizeof(SNode));
	if (!p) return ERROR;
	p->data = e;
	p->next = (*S)->next;
	(*S)->next = p;
	return OK;
}

//弹栈
Status PopStack(LinkStack *S, SElemType *e)
{
	SNode *p;
	if (StackEmpty(*S))
		return ERROR;
	p = (*S)->next;
	(*S)->next = p->next;
	(*e) = p->data;
	free(p);
	return OK;
}

//遍历栈
Status StackTraverse(LinkStack S)
{
	SNode *p;
	p = S->next;
	while (p)
	{
		printf("%d ", p->data);
		p=p->next;
	}
	printf("\n");
	return OK;
}

//测试函数
void test()
{
	LinkStack S;
	InitStack(&S);
	int n;
	scanf("%d", &n);
	while (n--)
	{
		SElemType e;
		scanf("%d", &e);
		PushStack(&S, e);
	}
	printf("The elem of the stack is following from the top to base:\n");
	StackTraverse(S);
}

int main()
{
	test();
	return 0;
}
