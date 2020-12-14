#include <stdio.h>
#include <stdlib.h>

#define MAXQSIZE 100 //队列最大长度

#define OK 1
#define ERROR 0

typedef int QElemType;
typedef int Status;

//顺序队列定义
typedef struct
{
	QElemType *base;
	int front;
	int rear;
}SqQueue;

// 使用顺序存储时，由于只能在队头出队，队尾入队，因此很容易出现“假溢出”现象
// 即，队尾无法继续入队，但此时队头仍有空闲区域未使用，会造成浪费
// 假溢出并非真的溢出，而是逻辑结构的问题，
// 解决办法：将顺序存储队列将其臆测成循环队列，这样就可以解决假溢出问题，称之为循环队列

//初始化
Status InitQueue(SqQueue *Q)
{
	Q->base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q->base) return ERROR;
	Q->front = Q->rear = 0;
	return OK;
}

int Length(SqQueue Q)
{
	return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}

//队满
Status QueueFull(SqQueue Q)
{
    // Q.rear == Q.front 不能作为队满的条件
    // 牺牲一个空间，队头指针在队尾指针下一位置时表示队满
	if ((Q.rear + 1) % MAXQSIZE == Q.front)
		return OK;
	return ERROR;
}

//队空
Status QueueEmpty(SqQueue Q)
{
	if (Q.front == Q.rear)
		return OK;
	return ERROR;
}

//入队
Status EnQueue(SqQueue *Q, QElemType e)
{
	if (QueueFull(*Q))
		return ERROR;
	Q->base[Q->rear] = e;
	Q->rear = (Q->rear + 1) % MAXQSIZE;
	return OK;
}

//出队
Status DeQueue(SqQueue *Q, QElemType *e)
{
	if (QueueEmpty(*Q))
		return ERROR;
	(*e) = Q->base[Q->front];
	Q->front = (Q->front + 1) % MAXQSIZE;
	return OK;
}

//遍历
Status Traverse(SqQueue Q)
{
	int i = 0;
	while (i < Length(Q))
	{
		printf("%d ", Q.base[i]);
		i++;
	}
	printf("\n");
	return OK;
}

/*
//测试函数
void test()
{
	SqQueue Q;
	InitQueue(&Q);
	int n;
	scanf("%d", &n);
	while (n--)
	{
		QElemType e;
		scanf("%d", &e);
		EnQueue(&Q, e);
	}
	printf("The length of the queue is: %d\n", Length(Q));
	printf("The elem of the queue is following from the front to rear:\n");
	Traverse(Q);
}
*/

// 离散事件模拟--银行业务模拟，统计一天内客户在银行逗留的平均时间
void Bank_Stimulation(int CloseTime)
{

}

int main()
{
	//test();
	return 0;
}
