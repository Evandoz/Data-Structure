#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 10
#define INCREMENT 10

#define OK 1
#define ERROR 0

 typedef int Status;
 typedef int ElemType;

 // 动态堆分配空间
 typedef struct
 {
 	ElemType *data; //节点数据
 	int length; //顺序表长度
 	int size; //顺序表大小
 } SqList;

//初始化
Status InitList(SqList *L)
{
	L->data = (ElemType *)malloc(MAXSIZE*sizeof(ElemType));
	if(!L->data) return ERROR;
	L->length = 0;
	L->size = MAXSIZE;
	return OK;
}

//清空
Status ClearList(SqList *L)
{
	if(!L->data) return ERROR;
	L->length = 0;
	return OK;
}

//销毁
 Status DestroyList(SqList *L)
 {
 	if(L->data)
 	{
 		free(L->data);
 		L = NULL;
 		return OK;
 	}
 	return ERROR;
 }

// 获取元素，根据位置返回元素
Status GetElem(SqList L, int pos, ElemType *e)
{
	if(pos < 1 || pos > L.length) return ERROR;
	(*e) = L.data[pos-1];
	return OK;
}

// 定位元素，根据元素返回位置
Status LocateElem(SqList L, ElemType e, int* pos)
{
	int i = 0;
	if(!L.data) return ERROR;
	while(i < L.length)
	{
		if(e == L.data[i])
		{
			(*pos) = i+1;
			return OK;
		}
		i++;
	}
	(*pos) = -1;
	return ERROR;
}

//表空
Status ListEmpty(SqList L)
{
	return L.length == 0 ? 1 : 0;
}

// 根据位置插入元素
// 1<=i<=L->length
Status ListInsert(SqList *L, int pos, ElemType e)
{
	ElemType* newBase;
	int i = 0;
	if(pos < 1||pos > L->length+1)
		return ERROR;
	if(L->length >= L->size)
	{
		newBase = (ElemType *)realloc(L->data,(L->size+INCREMENT)*sizeof(ElemType));
		L->data = newBase;
		L->size += INCREMENT;
	}
	// 线性表从1开始，但数组是从0开始的（C语言）
	for(i = L->length-1; i >= pos-1; --i)
		L->data[i+1] = L->data[i]; //后移
	L->data[pos-1] = e;
	++L->length;
	return OK;
}

//创建
Status Create(SqList *L, int n)
{
    ElemType e;
	int i = 1;
	while(i <= n)
	{
	    scanf("%d", &e);
		ListInsert(L, i, e);
		i++;
	}
	return OK;
}

//根据位置删除元素
Status ListDeleteAccordToPos(SqList *L, int pos, ElemType* e)
{
	int i = 0;
	if(!L->data) return ERROR;
	if(pos < 1 || pos > L->length)
		return ERROR;
	(*e) = L->data[pos-1];
	for(i = pos-1; i < L->length-1; ++i)
		L->data[i] = L->data[i+1];
	--L->length;
	return OK;
}

//根据值删除元素
Status ListDeleteAccordToElem(SqList *L, ElemType e, int* pos)
{
    int i = 0;
	if(!L->data) return ERROR;
	while(i < L->length)
	{
		if(e == L->data[i])
		{
			(*pos) = i+1;
			while(i < L->length)
            {
                L->data[i] = L->data[i+1];
                i++;
            }
            L->length--;
			return OK;
		}
		i++;
	}
	(*pos) = -1;
	return ERROR;
}

// 有序非递减L1和L2合并为L
/*Status ListMerge(SqList L1, SqList L2, SqList *L)
{

}*/

//遍历
Status ListTraverse(SqList L)
{
	int i = 0;
	while(i < L.length)
	{
		printf("%d ",L.data[i]);
		++i;
	}
	printf("\n");
	return OK;
}

//测试
void test()
{
    // 定义，初始化
	SqList L; InitList(&L);

	// 创建
	printf("=============================================\n");
	int n;
	printf("Please input the number of elem:");
	scanf("%d",&n);
	printf("Please input the elems:");
	Create(&L,n);
	ListTraverse(L);

	// 判空
	printf("=============================================\n");
	printf("Is it empty ? %d\n",ListEmpty(L));

	// 指定位置检查元素
	printf("=============================================\n");
	ElemType e2;
	int pos2;
	printf("Please input the pos of elem to be checked:");
	scanf("%d",&pos2);
	GetElem(L,pos2,&e2);
	printf("NO.%d is: %d\n",pos2,e2);

	// 指定元素检查位置
	printf("=============================================\n");
	ElemType e3;
	int pos3;
	printf("Please input the elem to be checked:");
	scanf("%d",&e3);
	LocateElem(L,e3,&pos3);
	printf("%d is: NO.%d\n",e3,pos3);

	// 指定位置删除
	printf("=============================================\n");
	ElemType e4;
	int pos4;
	printf("Please input the pos of elem to be deleted:");
	scanf("%d",&pos4);
	ListDeleteAccordToPos(&L,pos4,&e4);
	printf("The deleted number is: %d\n",e4);
	ListTraverse(L);

	// 指定元素删除
	printf("=============================================\n");
	ElemType e5;
	int pos5;
	printf("Please input the elem to be deleted:");
	scanf("%d",&e5);
	ListDeleteAccordToElem(&L,e5,&pos5);
	printf("The pos of the deleted number is: %d\n",pos5);
	ListTraverse(L);

	//ָ 指定位置插入
	printf("=============================================\n");
	ElemType e6;
	int pos6;
	printf("Please input the pos of elem to be inserted:");
	scanf("%d",&pos6);
	printf("Please input the elem to be inserted:");
	scanf("%d",&e6);
	ListInsert(&L,pos6,e6);
	ListTraverse(L);
}

int main()
{
	test();
	return 0;
}
