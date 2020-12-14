#include <stdio.h>
#include <stdlib.h>

#define MAXQSIZE 20 //队列最大长度

#define OK 1
#define ERROR 0

typedef int QElemType;
typedef int Status;

//顺序（循环）队列定义
typedef struct
{
	QElemType *base;
	int front;
	int rear;
}SqQueue;

//初始化
Status InitQueue(SqQueue *Q)
{
	Q->base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q->base) return ERROR;
	Q->front = Q->rear = 0;
	return OK;
}

int LenQueue(SqQueue *Q)
{
	return (Q->rear - Q->front + MAXQSIZE) % MAXQSIZE;
}

//队满
Status QueueFull(SqQueue *Q)
{
	if ((Q->rear + 1) % MAXQSIZE == Q->front)
		return OK;
	return ERROR;
}

//队空
Status QueueEmpty(SqQueue *Q)
{
	if (Q->front == Q->rear)
		return OK;
	return ERROR;
}

//入队
Status EnQueue(SqQueue *Q, QElemType e)
{
	if (QueueFull(Q))
		return ERROR;
	Q->base[Q->rear] = e;
	Q->rear = (Q->rear + 1) % MAXQSIZE;
	return OK;
}

//出队
Status DeQueue(SqQueue *Q, QElemType *e)
{
	if (QueueEmpty(Q))
		return ERROR;
	(*e) = Q->base[Q->front];
	Q->front = (Q->front + 1) % MAXQSIZE;
	return OK;
}

//遍历
Status Traverse(SqQueue *Q)
{
	int i = 0;
	while (i < LenQueue(Q))
	{
		printf("%d ", Q->base[i]);
		i++;
	}
	printf("\n");
	return OK;
}
