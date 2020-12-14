#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char data;
    int key;
} HeapType;

// 0号单元空出，为了方便根据序号对堆（完全二叉树）进行调整
void HeapAdjust(HeapType *H, int p, int n)
{
    // 将n个元素的数组中以H[p]为根的子堆调整为最大堆
    // 即，将H[p..n]调整为大根堆（小根堆同理）

    // 先把待调整的子树根p保存下来
    H[0] = H[p];
    int i;
    for (i = 2*p; i <= n; i *= 2)
    {
        // 要保证i<=n，i+1<=n，因此i<n
        if (i < n && H[i].key < H[i+1].key) ++i; //由于调整为大根堆，所以找孩子中key较大的进行交换
        if (H[0].key >= H[i].key) break; // 此时H[0]比H[I]大了，于是将H[0]调整至i的双亲，即p所指向的节点
        H[0] = H[i]; p = i; // 用于指向下一层节点的双亲，用于后续的调整
    }
    H[P] = H[0];
}

int GetRoot(HeapType *H, int size)
{
    // 对于一个大小为size的大根堆，根H[1]即为最大值
    H[0] = H[1]; H[1] = H[size]; H[size] = H[0];
    // 交换，将根与最后一个交换，
    // 然后减小堆的大小，重新调整为大根堆H[1..size-1]
    HeapAdjust(H, 1, size-1);

    return size; // 返回最大元素位置
}

int main()
{
    return 0;
}
