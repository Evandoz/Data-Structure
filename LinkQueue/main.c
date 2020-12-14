#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int QElemType;
typedef int Status;

//节点定义
typedef struct QNode
{
	QElemType data;
	struct QNode *next;
}QNode, *QueuePtr;

//链队列定义
typedef struct
{
	QueuePtr front;
	QueuePtr rear;
	int length;
}LinkQueue;

//初始化
Status InitQueue(LinkQueue *Q)
{
	Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q->front) return ERROR;
	Q->front->next = NULL; // 头节点数据域为空
	Q->length = 0;
	return OK;
}

//入队
Status EnQueue(LinkQueue *Q, QElemType e)
{
	QueuePtr p;
	p = (QueuePtr)malloc(sizeof(QNode));
	if (!p) return ERROR;
	p->data = e; p->next = NULL;
	Q->rear->next = p; // 从队尾加入
	Q->rear = p; // 尾指针移动
	Q->length++;
	return OK;
}

//队空
Status QueueEmpty(LinkQueue *Q)
{
	if (Q->front == Q->rear) return OK;
	return ERROR;
}

//出队
Status DeQueue(LinkQueue *Q, QElemType *e)
{
	QueuePtr p;
	if (QueueEmpty(Q)) return ERROR;
	p = Q->front->next; // 从队头离开
	*e = p->data;
	Q->front->next = p->next; // 头节点next指向p->next
	if (p == Q->rear) Q->rear = Q->front; // 只有一个元素，队尾指针指向头节点
	free(p);
	Q->length--;
	return OK;
}

//销毁
Status Destroy(LinkQueue *Q)
{
    // 头指针向后滑动，依次删除节点
	while (Q->front)
	{
		Q->rear = Q->front->next;
		free(Q->front);
		Q->front = Q->rear;
	}
	return OK;
}

//遍历
Status Traverse(LinkQueue *Q)
{
	QueuePtr p;
	p = Q->front->next;
	while (p)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}

//测试函数
void test()
{
	LinkQueue Q;
	InitQueue(&Q);
	int n;
	scanf("%d", &n);
	while (n--)
	{
		QElemType e;
		scanf("%d", &e);
		EnQueue(&Q, e);
	}
	printf("The length of the queue is: %d\n", Q.length);
	printf("The elem of the queue is following from the front to rear:\n");
	Traverse(&Q);
}

int main()
{
	test();
	return 0;
}
