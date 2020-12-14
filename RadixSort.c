#define MaxDigit 4
#define Radix 10

/* 假设元素最多有MaxDigit个关键字，基数全是同样的Radix */

/* 桶元素结点 */
struct Node {
    int key;
    PtrToNode next;
};
typedef struct Node *PtrToNode;

/* 桶头结点 */
struct HeadNode {
    PtrToNode head, tail;
};
typedef struct HeadNode Bucket[Radix];

// 获取某一位上的数字（关键字）
int GetDigit (int X, int D)
{
    // 默认次位 D=1, 主位 D<=MaxDigit
    int i, d;

    for (i = 1; i <= D; ++i) {
        d = X % Radix;
        X /= Radix;
    }
    return d;
}

/* 基数排序 - 次位优先 */
void LSDRadixSort(ElementType A[], int N)
{
     int D, Di, i;
     Bucket B;
     PtrToNode tmp, p, List = NULL;
    // 初始化每个桶为空链表
     for (i = 0; i < Radix; ++i)
         B[i].head = B[i].tail = NULL;
     // 将原始序列逆序存入初始链表 List
     for (i = 0; i < N; ++i)
     {
         tmp = (PtrToNode)malloc(sizeof(struct Node));
         tmp->key = A[i];
         tmp->next = List;
         List = tmp;
     }
     // 开始排序，对数据的每一位循环处理
     for (D = 1; D <= MaxDigit; ++D)
     {
         // 分配的过程
         p = List;
         while (p)
         {
             // 获得当前元素的当前位数字
             Di = GetDigit(p->key, D);
             // 指针后移，将原 p 从 List 中摘除
             tmp = p; p = p->next;
             // 插入B[Di]号桶尾
             tmp->next = NULL;
             if (B[Di].head == NULL)
                 B[Di].head = B[Di].tail = tmp;
             else {
                 B[Di].tail->next = tmp;
                 B[Di].tail = tmp;
             }
         }
         // 收集的过程，将每个桶的元素顺序收集入 List
         List = NULL;
         for (Di = Radix-1; Di >= 0; --Di)
         {
             if (B[Di].head) // 如果桶不为空
             {
                 // 整桶插入List表头
                 B[Di].tail->next = List;
                 List = B[Di].head;
                 B[Di].head = B[Di].tail = NULL; // 清空桶
             }
         }
     }
     /* 将List倒入A[]并释放空间 */
     for (i = 0; i < N; ++i) {
        tmp = List;
        List = List->next;
        A[i] = tmp->key;
        free(tmp);
     }
}



/* 基数排序 - 主位优先 */
void MSD(ElementType A[], int L, int R, int D)
{
    // 核心递归函数: 对A[L]...A[R]的第D位数进行排序
    int Di, i, j;
    Bucket B;
    PtrToNode tmp, p, List = NULL;
    //递归终止条件
    if (D == 0) return;
    // 初始化每个桶为空链表
    for (i = 0; i < Radix; ++i)
        B[i].head = B[i].tail = NULL;
    // 将原始序列逆序存入初始链表 List
    for (i=L; i<=R; ++i)
    {
        tmp = (PtrToNode)malloc(sizeof(struct Node));
        tmp->key = A[i];
        tmp->next = List;
        List = tmp;
     }
     // 分配的过程
     p = List;
     while (p)
     {
         // 获得当前元素的当前位数字
         Di = GetDigit(p->key, D);
         // 指针后移，将原 p 从 List 中摘除
         tmp = p; p = p->next;
         // 插入B[Di]号桶尾
         if (B[Di].head == NULL) B[Di].tail = tmp;
         tmp->next = B[Di].head;
         B[Di].head = tmp;
     }
     // 收集的过程
     i = j = L; // i, j记录当前要处理的A[]的左右端下标
     for (Di = 0; Di < Radix; ++Di)
     {
         // 对于每个桶
         if (B[Di].head)
         {
             // 将非空的桶整桶倒入A[], 递归排序
             p = B[Di].head;
             while (p)
             {
                 tmp = p;
                 p = p->next;
                 A[j++] = tmp->key;
                 free(tmp);
             }
             // 递归对该桶数据排序, 位数减 1
             MSD(A, i, j-1, D-1);
             i = j; // 为下一个桶对应的A[]左端
         }
     }
}

void MSDRadixSort(ElementType A[], int N)
{
    MSD(A, 0, N-1, MaxDigit);
}
