#include <stdio.h>
#include <stdlib.h>

#define HASHSIZE 12
#define NULLKEY -32768

typedef int Status;

typedef int ElemType;

typedef struct
{
    ElemType *elem; // 数据元素基址，由于不知道元素个数，所以像之前一样动态分配空间
    int count; // 当前数据元素个数
    int sizeindex; // 当前容量
} HashTable;

Status InitHash(HashTable *H)
{
    int i;

    H->elem = (ElemType *)malloc(HASHSIZE * sizeof(ElemType));
    H->count = 0; H->sizeindex = HASHSIZE;

    if (H->elem == NULL) return -1;

    for(i = 0; i < H->sizeindex; ++i)
        H->elem[i] = NULLKEY; // 初始化成一个比可能的值
    return 0;
}

// Hash函数
int Hash(ElemType key)
{
    return key % HASHSIZE;
}

Status SearchHash(HashTable H, ElemType key, int *addr) // addr用于存放地址（下标）
{
    (*addr) = Hash(key);
    while (H.elem[*addr] != NULLKEY && H.elem[*addr] != key) // 如果为NULLKEY就不用继续探测了
        (*addr) = ((*addr) + 1) % HASHSIZE;
    if (H.elem[*addr] == key) return 1;
    else return 0;
}

Status InsertHash(HashTable *H, ElemType e)
{
    int addr;
    if (SearchHash(*H, e, &addr))
        return -2;
    else
    {
        H->elem[addr] = e; ++H->count; return 0;
    }
}

int main()
{
    printf("Hello world!\n");
    return 0;
}
