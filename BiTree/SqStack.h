#define STACK_INIT_SIZE 20
#define STACKINCREMENT 10

typedef int Status;

typedef BiTree SElemType;

// 定义栈（顺序栈）
typedef struct
{
	SElemType *top;     // 栈顶指针
	SElemType *base;    // 在栈构造之前和销毁之后为NULL
	int stacksize;      // 当前已分配大小
}SqStack;

// 初始化
Status InitStack(SqStack *S)
{
    //初始化一段连续空间，并让栈底指针指向它
	S->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S->base) return 0;
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return 1;
}

//栈满
Status StackFull(SqStack *S)
{
	if (S->top - S->base >= S->stacksize)
		return 1;
	return 0;
}

//判空
Status StackEmpty(SqStack *S)
{
	if (S->top == S->base)
		return 1;
	return 0;
}

//压栈
Status PushStack(SqStack *S, SElemType e)
{
	if (StackFull(S))
	{
		S->base = (SElemType *)realloc(S->base,
			(S->stacksize + STACKINCREMENT)*sizeof(SElemType));
		if (!S->base) return 0;
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCREMENT;
	}
	*(S->top++) = e;
	return 1;
}

//弹栈
Status PopStack(SqStack *S, SElemType *e)
{
	if (StackEmpty(S)) return 0;
	(*e) = *(--S->top);
	return 1;
}

//栈顶
Status GetTop(SqStack *S, SElemType* e)
{
	if (StackEmpty(S))
		return 0;
	(*e) = *(S->top - 1);
	return 1;
}
