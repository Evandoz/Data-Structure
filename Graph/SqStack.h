#include <stdio.h>
#include <stdlib.h>

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

#define OK 1
#define ERROR 0

typedef int SElemType;
typedef int Status;

//顺序栈定义
typedef struct
{
	SElemType *top; //栈顶指针
	SElemType *base; //在栈构造之前和销毁之后为NULL
	int stacksize; //当前已分配大小
}SqStack;

//初始化
Status InitStack(SqStack *S)
{
	S->base = (SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if (!S->base) return ERROR;
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}

//判空
Status StackEmpty(SqStack *S)
{
	if (S->top == S->base)
		return OK;
	return ERROR;
}

//栈满
Status StackFull(SqStack *S)
{
	if (S->top - S->base >= S->stacksize)
		return OK;
	return ERROR;
}

//栈顶
SElemType GetTop(SqStack *S)
{
	if (StackEmpty(S))
		return -1;
	return *(S->top - 1);
}

//压栈
Status PushStack(SqStack *S, SElemType e)
{
	if (StackFull(S))
	{
		S->base = (SElemType *)realloc(S->base,
			(S->stacksize + STACKINCREMENT)*sizeof(SElemType));
		if (!S->base) return ERROR;
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCREMENT;
	}
	*(S->top++) = e;
	return OK;
}

//弹栈
Status PopStack(SqStack *S, SElemType* e)
{
	if (StackEmpty(S)) return ERROR;
	(*e) = *(--S->top);
	return OK;
}

//清空
Status ClearStack(SqStack *S)
{
	S->top = S->base;
	S->stacksize = 0;
	return OK;
}

//销毁
Status DestroyStack(SqStack *S)
{
	free(S->base);
	S->top = NULL;
	S->base = NULL;
	S->stacksize = 0;
	return OK;
}

//长度
int LenStack(SqStack *S)
{
	return S->top - S->base;
}

//遍历栈
Status TraverseUp(SqStack *S)
{
    SElemType *p;
    p = S->base;
    while(p < S->top)
    {
        printf("%d ",*p);
        p++;
    }
    printf("\n");
    return OK;
}

Status TraverseDown(SqStack *S)
{
	SElemType *p;
	p = S->top - 1;
	while (p >= S->base)
	{
		printf("%d ", *p);
		p--;
	}
	printf("\n");
	return OK;
}
