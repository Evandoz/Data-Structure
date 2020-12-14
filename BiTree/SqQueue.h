#define MAXQSIZE 20

typedef BiTree QElemType;

//队列定义（顺序循环队列）
typedef struct
{
	QElemType *base;
	int fron;
	int rear;
}SqQueue;

//初始化空队列
Status InitQueue(SqQueue *Q)
{
	Q->base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q->base) return 0;
	Q->fron = Q->rear = 0;
	return 1;
}

//队满
Status QueueFull(SqQueue *Q)
{
	if ((Q->rear + 1) % MAXQSIZE == Q->fron)
		return 1;
	return 0;
}

//队空
Status QueueEmpty(SqQueue *Q)
{
	if (Q->fron == Q->rear)
		return 1;
	return 0;
}

//入队
Status EnQueue(SqQueue *Q, QElemType e)
{
	if (QueueFull(Q))
		return 0;
	Q->base[Q->rear] = e;
	Q->rear = (Q->rear + 1) % MAXQSIZE;
	return 1;
}

//出队
Status DeQueue(SqQueue *Q, QElemType *e)
{
	if (QueueEmpty(Q))
		return 0;
	(*e) = Q->base[Q->fron];
	Q->fron = (Q->fron + 1) % MAXQSIZE;
	return 1;
}

//队尾元素
Status GetRear(SqQueue *Q, QElemType *e)
{
	if (QueueEmpty(Q))
		return 0;
	(*e) = Q->base[Q->rear-1];
	return 1;
}
