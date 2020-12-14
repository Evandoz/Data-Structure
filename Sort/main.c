#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "LinkQueue.h" // 用于基数排序

#define SIZE 20
#define MAXINT 65535

// 用于表插入排序
typedef int RcdType; // 记录项

typedef struct
{
    RcdType rcd; // 记录项
    int next; // 指针项
} SLNode;

typedef struct
{
    SLNode r[SIZE];
    int length;
} SLinkListType;

// 交换函数
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b; *b = tmp;
}

// 1.插入排序

// 1.1直接插入排序（稳定）
// 此次将一个待排序的记录插入到已排好的有序表中
// 过程：选择一个待排序记录，为它在有序表中找到插入位置i（查找过程，可以使用哨兵）
// 将有序表位置i及其之后的记录逐位后移，空出位置i给待插入记录，复杂度O(n^2)
void InsertSort(int L[], int n)
{
    int i, j;
    for (i = 2; i <=n; ++i) // 起始首位置1（0号为哨兵）为有序部分，剩下n-1为待排序部分
    {
        L[0] = L[i]; // L[0]既为哨兵，又起到临时存储作用
        // 因为L[0]位置即为当前待插入元素，因此当判断到L[0]时，循环必将结束，
        // 不必考虑数组越界问题，即为哨兵作用
        for (j = i-1; L[j] > L[0]; --j) // 在有序部分中为L[i]找位置
            L[j+1] = L[j]; // 元素后移
        L[j+1] = L[0]; // 第j位时退出循环，此时应插入到j+1，原来j+1位已空出
    }
}

// 1.2折半插入排序（稳定）
// 由于在有序表中插入，故可以使用折半查找，另外之前的直接插入是一边查找一边移动
// 而这里则是要将二者分开，先查找，然后统一移动，此时比较次数减少，但移动次数不变，时间复杂度不变
// 由于折半查找只适用于顺序存储的线性表，因此折半插入仅针对顺序存储优化
void BInsertSort(int L[], int n)
{
    int i, j, low, high, mid;
    for (i = 2; i <= n; ++i)
    {
        low = 1; high = i-1;
        while(low <= high) // 查找，循环中满足L[low]<=L[0]<=L[high]
        {
            mid = (low + high)/2;
            if (L[i] < L[mid]) high = mid - 1;
            else low = mid +1; // 将相等的情况归结到low，可以保证算法稳定（记录相等时，排序之后相对位置不变）
        } // 退出时low-high=1，应当插在high+1的位置
        L[0] = L[i];
        for (j = i; j > high+1; --j) // 移动
            L[j] = L[j-1];
        L[high+1] = L[0];
    }
}

// 1.3二路插入排序（稳定）
// 需要增设同样大小的数组，旨在减少移动的次数，可以有序表前后两个方向插入
// 代码实现时，设最大和最小指针分别指向最大最小，每次先跟这两个指针比较，根据比较结果插入相应位置
// 最后将这个临时数组元素回填到L中
void TwoInsertSort(int L[], int n)
{
    int i, j, first = 0, last = 0; // 初始化首尾指针
    int *tmpbuffer = (int *)malloc(n * sizeof(int));
    for (i = 1; i < n; ++i) tmpbuffer[i] = 0;
    tmpbuffer[0] = L[1]; // tmpbuffer[0]为中轴
    for (i = 2; i <= n; ++i)
    {
        if (L[i] >= tmpbuffer[0]) // 与中轴tmpbuffer[0]比较，右插，保证稳定大于时取等
        {
            // 首次运行到这时，循环不会执行
            for (j = last; L[i] < tmpbuffer[j]; --j) tmpbuffer[j+1] = tmpbuffer[j]; // 从last开始，比较查找，向后移动
            tmpbuffer[j+1] = L[i]; last = last + 1;
        }
        else // 左插
        {
            // 首次运行到这时，循环不会执行，
            // 之后，first 指针将移动到数组尾部 d[n-1]不需要管j会不会越界
            for (j = first; L[i] >= tmpbuffer[j]; ++j) tmpbuffer[j-1] = tmpbuffer[j]; // 从first开始，比较查找，向前移动
            // 首次运行到这时，first 指针将移动到数组尾部 d[n-1]，此时需要取余，之后不需要
            tmpbuffer[(j-1+n)%n] = L[i]; first = (first - 1 + n) % n;
        }
        /* for (j = 0; j < n; ++j)
            printf("%2d ", tmpbuffer[j]);
        printf("\n\n"); */
    }
    // 回填L
    for (i = 1; i <= n; ++i)
        L[i] = tmpbuffer[(first++)%n]; // first 为有序序列头指针
    free(tmpbuffer);
}

void ThreeInsertSort(int L[], int n) // 这个其实是3路，跟first和last比较，待排序列被first和last分成了三部分
{
    int i, j, first = 0, last = 0; // 初始化首尾指针
    int *tmpbuffer = (int *)malloc(n * sizeof(int));
    for (i = 1; i < n; ++i) tmpbuffer[i] = 0;

    for (i = 2; i <= n; ++i)
    {
        if (L[i] >= tmpbuffer[last]) // 保证稳定大于时取等
        {
            last = last + 1;
            tmpbuffer[last] = L[i];
        }
        else if (L[i] < tmpbuffer[first]) // 小于时不取等
        {
            first = (first-1+n) % n;
            tmpbuffer[first] = L[i];
        }
        else // 中间这部分仍是简单插入排序
        {
            j = last++;
            while(L[i] < tmpbuffer[j])
            {
                tmpbuffer[(j+1)%n] = tmpbuffer[j];
                j = (j-1+n) % n;
            }
            tmpbuffer[(j+1)%n] = L[i];
        }
        /* for (j = 0; j < n; ++j)
            printf("%2d ", tmpbuffer[j]);
        printf("\n\n"); */
    }
    // 回填L
    for (i = 1; i <= n; ++i)
        L[i] = tmpbuffer[(first++)%n]; // first 为有序序列头指针
    free(tmpbuffer);
}

// 1.4表插入排序
void Arrange(SLinkListType *SL) // 重排
{
    int i, q, p = SL->r[0].next; // p指向待排序最小关键字位置，p位置元素应当摆在i位
    SLNode tmp;
    for (i = 1; i <= SL->length; ++i) // 1->i-1为已排好的部分
    {
        // 每次循环，将待排序的最小关键字SL->r[p]交换至i,对于未排序的部分p>=i
        while (p < i) p = SL->r[p].next;
        q = SL->r[p].next; // 保存下一个最小关键字位置，以备后用
        if (p != i) // p位置元素应当摆在i位
        {
            tmp = SL->r[p]; SL->r[p] = SL->r[i]; SL->r[i] = tmp;
            SL->r[i].next = p; // 将i的next置为p，防止断链
        }
        p = q; // p指向待排序最小关键字位置
    }
    for (i = 1; i <= SL->length; ++i)
        printf("%2d ", SL->r[i].rcd);
    printf("\n");
}

void Arrange_2(SLinkListType *SL, int L[]) // 重排，直接赋给L[]
{
    int i = 1, p = SL->r[0].next; // p指向待排序最小关键字位置，p位置元素应当摆在i位
    while (p != 0)
    {
        L[i++] = SL->r[p].rcd;
        p = SL->r[p].next;
    }
}

void InitSList(SLinkListType *SL, int L[], int n)
{
    int i; SL->length = n;
    for (i = 1; i <= n; ++i)
    {
        SL->r[i].rcd = L[i];
        SL->r[i].next = -1;
    }
}

void LInsertSort(SLinkListType *SL)
{
    SL->r[0].rcd = MAXINT;
    SL->r[0].next = 1;
    SL->r[1].next =  0;
    /*
    int j;

    printf("%5d ", 0);
    for (j = 1; j <= SL->length; ++j)
        printf("%2d ", j);
    printf("\n\n");

    printf("%5d ", SL->r[0].rcd);
    for (j = 1; j <= SL->length; ++j)
        printf("%2d ", SL->r[j].rcd);
    printf("\n");
    printf("%5d ", SL->r[0].next);
    for (j = 1; j <= SL->length; ++j)
        printf("%2d ", SL->r[j].next);
    printf("\n\n");
    */
    int i, pre, p;
    for (i = 2; i <= SL->length; ++i)
    {
        pre = 0; p = SL->r[0].next;
        // SL->r[0].rcd=65535，因此不需要判断结尾
        // SL->r[0].rcd设置不当将造成死循环，因为它是个循环链表
        while (SL->r[i].rcd >= SL->r[p].rcd)
        {
            pre = p;
            p = SL->r[p].next;
        }
        SL->r[i].next = SL->r[pre].next;
        SL->r[pre].next = i;
        /*
        printf("%5d ", SL->r[0].rcd);
        for (j = 1; j <= SL->length; ++j)
            printf("%2d ", SL->r[j].rcd);
        printf("\n");
        printf("%5d ", SL->r[0].next);
        for (j = 1; j <= SL->length; ++j)
            printf("%2d ", SL->r[j].next);
        printf("\n\n");
        */
    }
}

void Arrange_3(int L[], int n, int next[]) // 重排
{
    int i, tmp, q, p = next[0]; // p指向待排序最小关键字位置，p位置元素应当摆在i位

    for (i = 1; i <= n; ++i) // 1->i-1为已排好的部分
    {
        // 每次循环，将待排序的最小关键字SL->r[p]交换至i,对于未排序的部分p>=i
        while (p < i) p = next[p];
        q = next[p]; // 保存下一个最小关键字位置，以备后用
        if (p != i) // p位置元素应当摆在i位
        {
            tmp = L[p]; L[p] = L[i]; L[i] = tmp; // 交换
            next[p] = next[i]; next[i] = p; // 因为原本i位置的元素移动到了p，因此将i的next置为p，这样当通过next找到位置i时可以往下找到原来的L[i]（现在L[p]）,防止断链
        }
        p = q; // p指向待排序最小关键字位置
        /*
        int j;
        for (j = 0; j <= n; ++j)
            printf("%2d ", next[j]);
        printf("\n\n");*/
    }
}

// 不再定义表结构，直接用一个辅助数组代替链表排序
void LInsertSort_2(int L[], int n)
{
    int i, *next = (int *)malloc((n+1) * sizeof(int));
    for (i = 2; i <= n; ++i) next[i] = -1;
    next[0] = 1; next[1] = 0; // next[0]做头节点，它与next[1]起始时构成循环链表
    /*
    int j;
    for (j = 0; j <= n; ++j)
        printf("%2d ", j);
    printf("\n\n");

    for (j = 0; j <= n; ++j)
        printf("%2d ", next[j]);
    printf("\n\n"); */

    int pre, p; // 排序开始
    for (i = 2; i <= n; ++i)
    {
        pre = 0; p = next[0];
        // 循环链表，需要判断表尾
        while (L[i] >= L[p] && p != 0)
        {
            pre = p; p = next[p];
        }
        next[i] = next[pre];
        next[pre] = i;
        /*
        for (j = 0; j <= n; ++j)
            printf("%2d ", next[j]);
        printf("\n\n"); */
    }
    Arrange_3(L, n, next);
}

// 1.5希尔排序（不稳定）
// 跨度从1变成dk
void ShellSort(int L[], int n)
{
    int dk, i, j;
    for (dk = n/2; dk >= 1; dk /= 2) // 这里 dk序列取 n/2,n/4,...1
    {
        //ShellInsert(int L[], int n, int dk)
        for (i = 1+dk; i <= n; ++i) // i=i+dk
        {
            L[0] = L[i]; // 暂存
            for (j = i-dk; j >0 && L[0] < L[j]; j -= dk) // 一组元素间隔为dk,原来是1（相邻）
                L[j+dk] = L[j];
            L[j+dk] = L[0];
        }
    }
}

void Print(int L[], int n)
{
    int i;
    for (i = 1; i <= n; ++i)
        printf("%d ", L[i]);
    printf("\n");
}

// 2.交换排序

// 2.1冒泡排序（稳定）
// 每一趟排序都会有一个最大/小的元素沉底/上浮，这也是冒泡排序名称的由来
// 已经沉底的元素下一趟就不必在参入排序，因为它已经到达最终的排序位置
void BubbleSort(int L[], int n)
{
    int i, j, flag; // flag变量记录是否发生交换，如果一趟排序中没有发生交换，那么说明已经有序，不需要再排序
    for (i = 1; i < n; ++i) // n-1趟
    {
        flag = 0;
        // 第 i 躺时，已有 i-1 个元素沉底，不再次参与排序，
        // 剩余 j-(i-1) 个元素进行 j-(i-1)-1 次比较
        for (j = 1; j < n-i+1; ++j)
        {
            if (L[j] > L[j+1]) // 大值沉底
            {
                swap(&L[j], &L[j+1]);
                flag = 1;
            }
        } // 一趟排序
        if (flag == 0) return; // 因为相邻元素两两比较，所以若一趟排序中没有发生交换，那么说明已经有序，不需要再排序
    }
}

// 算法改进一，设置标志性变量 pos，记录一趟排序最后一次发生交换的位置
// 那么下一趟排序时它之后的就不需要再排序了
// 此处代码中数组元素从 L[1] 开始，故用 L[0] 来存储标识位置
void BubbleSort_1(int L[], int n)
{
    int i = n, j; // //初始时，比较的最后位置仍为 n
    while (i > 1) // 仅代表排序趟数，没有其他含义（n-1趟）
    {
        L[0] = 1; // 每趟开始时，无记录交换
        for (j = 1; j < i; ++j) // [1,...,i-1, i]
        {
            if (L[j] > L[j+1])
            {
                swap(&L[j], &L[j+1]);
                L[0] = j; // 保存最后一次发生交换的位置
            }
        } // 一趟排序
        i = L[0]; // 下一次比较到 L[0] 即停止
    }
}

// 算法改进二，传统冒泡排序中每一趟排序操作只能找到一个最大值或最小值，
// 考虑利用在每趟排序中进行正向和反向两遍冒泡的方法一次
// 可以得到两个最终值(最大者和最小者) , 从而使排序趟数几乎减少了一半。
void BubbleSort_2(int L[], int n)
{
    int i, low = 1, high = n; // 设置变量的初始值
    while (low < high)
    {
        for (i = low; i < high; ++i) // 正向冒泡，找到最大者
            if (L[i] > L[i+1])
                swap(&L[i], &L[i+1]);

        --high; // 修改high值，前移一位
        for (i = high; i > low; --i) // 反向冒泡，找到最小者
            if (L[i] < L[i-1])
                swap(&L[i], &L[i-1]);
        ++low; // 修改low值，后移一位
    }
}

// 2.2快速排序（不稳定）
// 单向遍历
// 基准枢纽 pivotkey = L[q]
// L[p...i] 均小于 pivotkey，L[i+1...j-1] 均大于pivotkey
int Partition(int L[], int p, int q)
{
    int pivotkey = L[q]; // 基准枢纽 pivotkey = L[q]
    int i = p-1, j = p; // 初始化 i=p-1, j=p 即L[p...i] = L[i+1...j-1] 均为空
    for (; j < q; ++j) // 遍历 {p,...,q-1}
        if (L[j] <= pivotkey)
            swap(&L[++i], &L[j]); // 将小于 pivotkey 的元素移到前面
    swap(&L[i+1], &L[j]); // pivotkey 归位
    return i+1; // 返回基准值位置
}

// 双向遍历
// 选择中轴，将小于它的放到左边，大于它的放到右边
int Partition_1(int L[], int low, int high)
{
    // 基准点的选择对快排的性能有很大影响，关系到递归栈深度，尤其是在最坏情况下
    // 使用三者选中原则，优化基准点的选择

    int mid = (low + high) / 2;

    // 使 L[low] 取得中间值作为基准，L[ligh]取三者中最大
    if (L[low] > L[high]) swap(&L[low], &L[high]); // 保证L[low]<L[high]
    if (L[mid] > L[high]) swap(&L[mid], &L[high]); // 保证L[mid]<L[high]

    if (L[mid] > L[low]) swap(&L[mid], &L[low]);   // 保证L[mid]<L[low]<L[high]

    int pivotkey = L[low]; // 起始，基准点为L[low]的值
    while (low < high)
    {
        while(low < high && L[high] >= pivotkey) --high; // 高端过滤，比它小的移动到它左侧
        swap(&L[low], &L[high]); // 基准值现在移动到high

        while(low < high && L[low] <= pivotkey) ++low; // 低端过滤，比它大的移动到它右侧
        swap(&L[low], &L[high]);
    } // 基准值移动到最终正确的位置
    return low;
}

// 升级，low=high的位置才是最终枢纽值的位置，不需要不断移动枢纽值位置
int Partition_1_1(int L[], int low, int high)
{
    // 基准点的选择对快排的性能有很大影响，关系到递归栈深度，尤其是在最坏情况下
    // 起始，基准点为L[low]的值
    L[0] = L[low]; // 暂存基准点的值pivotkey，省一个变量
    while (low < high)
    {
        while(low < high && L[high] >= L[0]) --high; // 高端过滤，比它小的移动到它左侧（交换）
        L[low] = L[high]; // 直接将其移动到L[low]

        while(low < high && L[low] <= L[0]) ++low; // 低端过滤，比它大的移动到它右侧
        L[high] = L[low];
    }
    L[low] = L[0]; //基准值归位
    return low;
}

void QSort(int L[], int low, int high)
{
    int pivotloc;
    if (low < high)
    {
        pivotloc = Partition_1(L, low, high);
        QSort(L, low, pivotloc-1);
        QSort(L, pivotloc+1, high);
    }
}

// 性能跟基准点的选择以及划分算法有关系
// 优化点：基准点选择：三者选中原则
void QuickSort(int L[], int n)
{
    QSort(L, 1, n);
}

// 改进算法，只对长度大于 k 的子序列递归调用快速排序。
// 让原序列基本有序，然后再对整个基本有序序列用插入排序算法排序。
// 实践证明，改进后的算法时间复杂度有所降低，且当k取值为 8 左右时，改进算法的性能最佳。
void QSort_2(int L[], int low, int high, int k)
{
    int pivotloc;
    if (high - low > k)  // 仅在大于 k 时使用快排递归
    {
        pivotloc = Partition_1(L, low, high);
        QSort_2(L, low, pivotloc-1, k);
        QSort_2(L, pivotloc+1, high, k);
    }
}

void QuickSort_2(int L[], int n)
{
    QSort_2(L, 1, n, 8);
    InsertSort(L, n);
}

// 3.选择排序

// 3.1简单选择排序（不稳定）
// 每次从待排序的元素中选择最小/大的放到已排序部分中
void SelectSort(int L[], int n)
{
    int i, j, min; // 记录最小值的下标
    for (i = 1; i <= n; ++i)
    {
        min = i;
        for (j = i+1; j <= n; ++j)
            if (L[j] < L[min])
                min = j;
        if (min != i)
            swap(&L[min], &L[i]); // 将当前最小值与头L[i]交换
    }
}

// 升级版，二元原则排序，同时选择最大和最小
void SelectSort_1(int L[], int n)
{
    int i, j, min, max; // 记录最小值的下标

    for (i = 1; i <= n/2; ++i) // 每次选出两个，故需要n/2趟
    {
        min = max = i;
        // 第 i 趟排序开始时，已分别确定了 i-1 个最大值和最小值
        // 剩余范围 [i...n-i+1]
        for (j = i+1; j <= n-i+1; ++j)
        {
            if (L[j] < L[min])
                min = j;
            else if (L[j] > L[max])
                max = j;
        }

        // 存在的问题：如果先移动 min ，那么当 max 刚好位于 i 时，
        // 会将 max 移动到原来 min 的位置，所以要判断 max 与 i 的关系
        // 同理，如果先移动 max ，那么当 max 刚好位于 n-i+1 时，
        // 会将 min 移动到原来 max 的位置，所以要判断 min 与 i 的关系

        if (min != i) swap(&L[min], &L[i]); // 当前最小值归位 L[i]

        // 先交换完 min 后，判断 max 与 i 的关系，来决定是否要更新 max 的位置
        if (max == i) max = min;

        if (max != n-i+1) swap(&L[max], &L[n-i+1]); // 当前最大值归位 L[n-i+1]
    }
}

// 3.2树形选择排序 - 堆排序

// 向上调整，适用于建堆（通常不确定元素个数，元素是动态从末尾增加），增加元素
// 不能用于堆排序中输出堆顶后调整新堆
// 因为向上调整的前提是，除最后一个元素外，前面的元素符合堆的要求
void HeapAdjustUp(int L[], int p) // 从叶子L[p]向上（根）进行调整L[1..p]
{
    int i;
    L[0] = L[p]; // 暂存L[p]，因为要往上筛选
    for (i = p/2; i >= 1; i /= 2) // 与其双亲进行比较
    {
        if (L[0] <= L[i]) // 不大于双亲，不再需要调整，L[p]归位
            break;
        //else // 因为前面的if结果是break，所以else可以不需要
        //{
            L[p] = L[i]; // 比它的双亲大，交换（下移），
            p = i; // L[i]空出，继续往上
        //}
    }
    L[p] = L[0]; // 要么循环比较完退出，要么通过break退出，L[p]归位
}

/*void HeapAdjustUp(int L[], int n)
{
    int i;
    L[0] = L[n]; // 暂存
    for (i = n; i >= 1; i /= 2)
    {
        if (i/2 >= 1 && L[i/2] < L[0])
            L[i] = L[i/2];
        else break;
    }
    L[i] = L[0];
}*/

// 向下调整，适用于建堆（通常已确定元素个数，元素均已到位存入数组），删除元素
void HeapAdjustDown(int L[], int p, int n) // 对以L[p]为根的子树进行调整L[p..n]
{
    int i; // 复用 p 作为双亲指针，而 i 作为孩子指针
    L[0] = L[p]; // 暂存L[p]，因为要往下筛选
    for (i = 2*p; i <= n; i *= 2) // 与其子树进行比较
    {
        // i <= n, i+1 <= n
        if(i < n && L[i] < L[i+1]) ++i; // 找其子树最大值
        if (L[0] >= L[i]) // 不比孩子小，不再需要调整，L[p]归位
            break;

        //else // 因为前面的if结果是break，所以else可以不需要
        //{
            L[p] = L[i]; // 比它的最大孩子小，交换（上移），
            p = i; // L[i]空出，继续往下
        //}
    }
    L[p] = L[0]; // 要么循环比较完退出，要么通过break退出，L[p]归位
}

void HeapSort(int L[], int n)
{
    int i;

    for (i = 1; i <= n; ++i) // 初始，通过向上筛选的方式建堆，从1开始逐渐增加（在后面加入）元素
        HeapAdjustUp(L, i);

    /*for (i = n/2; i >= 1; --i) // 初始，通过向下筛选的方式建堆，从n/2到1往前（上）调整
        HeapAdjustDown(L, i, n);
    */

    for (i = n; i > 1; --i)
    {
        Print(L, n);
        swap(&L[1], &L[i]); // 堆顶（最大）与数组最后一位交换，最大值归位
        HeapAdjustDown(L, 1, i-1); // 从新调整
    }
}

// 4.归并排序

// Merge函数，用于合并两个有序序列
void Merge(int L[], int l, int m, int h)
{
    // 将有序序列L[l..m]和L[m+1..h]合并，
    // 事实上，此处是指将一个序列的两个有序段合并成一个有序段
    // 需要一个相同大小的附属数组 T[]，此处是根据实际长度分配的辅助空间
    int i, j, k, *T = (int *)malloc((h-l+1) * sizeof(int)); // 根据实际
    for (i = 0, k = l; i < h-l+1 && k <= h; ++i, ++k) T[i] = L[k]; // 复制L到T，L用于存储合并后的序列

    // i为T[l..m]段索引，j为T[m+1..h]段索引，k为L的索引，L永于存储合并后的序列
    for (i = 0, j = m-l+1, k = l; i < m-l+1 && j < h-l+1; ++k)
    {
        if (T[i] <= T[j]) L[k] = T[i++];
        else L[k] = T[j++];
    }

    while (i < m-l+1) L[k++] = T[i++]; // 若L[l..m]有剩余，则直接填入
    while (j < h-l+1) L[k++] = T[j++]; // 若L[m+1..h]有剩余，则直接填入

    free(T);
 }

 // 2路归并，递归实现，代码简洁，但是效率不高
void MSort_Recursion(int L[], int l, int h)
{
    int m;
    if (l < h)
    {
        m = (l + h) / 2;
        MSort_Recursion(L, l, m);
        MSort_Recursion(L, m+1, h);
        Merge(L, l, m, h);
    }
}

void MSort_Non_Recursion(int L[], int l, int n) // l为待合并的每段段长
{
    int i;
    // i+2l-1待合并的两段最大下标索引
    for (i=1; i+2*l-1<=n; i+=2*l)
        Merge(L, i, i+l-1, i+2*l-1);

    // i+2*l-1 > n，说明最后一段序列无法分成两个长度均为l的段

    // 要么一段为l，另一段小于l（i+l <= n < i+2*l-1）
    // 那么此时两段分别为L[i..i+l-1]和L[i+l..n]
    if (i+l <= n) Merge(L, i, i+l-1, n);

    // 要么只有一段，并且小于等于l（i+l > n）
    // 那么此时直接把这一段作为下一趟归并的段，将其全部追加到本趟归并序列之后
    // 此处不需要处理
}

void MSort(int L[], int len, int n)
{
    int i;
    // i+len<=n 表示有第二段
    for (i=1; i+len<=n; i+=2*len)
    {
        if (i+2*len-1 <= n) // 第二段长度为 len
            Merge(L, i, i+len-1, i+2*len-1);
        else                // 第二段长度小于 len
            Merge(L, i, i+len-1, n);
    }
}

void MergeSort(int L[], int n)
{
    // if (1 < n)  MSort_Recursion(L, 1, n);

    int l;
    for (l=1; l<n; l*= 2)
        MSort(L, l, n);
        //MSort_Non_Recursion(L, l, n);
}

// 5.基数排序
// 计数排序
void CountSort(int L[], int n)
{

}

// 桶排序
void BucketSort(int L[], int n)
{

}

// 基于分配和收集的排序，当然其操作过程也就是先分配再收集的过程
// 不是基于比较的排序，所以不会有O(nlogn)的限制

// 取关键字k的第p位（从低往高）
int Radix(int k, int p)
{
    return (int)(k/pow(10, p-1))%10; // math.h
}

// 计算关键字位数
int Figure(int k)
{
    int count = 1;
    while (count < 100)
    {
        if (pow(10, count) <= k && pow(10, count+1) > k)
            return count+1;
        ++count;
    }
    return -1; // 位数太大，不便于直接根据位数进行分配
}

void RadixSort(int L[], int n)
{
    // i 用于位数计数，分配时使用，j用 于队列计数，收集时使用
    int i, j, key, figure = 2; //Figure(L[1]);

    LinkQueue P; InitQueue(&P); // P 队列用于收集
    LinkQueue Q[10]; // 10 个 Q 队列用于分配，因为关键字的取值 [0,9]
    for (i = 0; i < 10; ++i) InitQueue(&Q[i]);

    // 因为待排序的元素个数未知，因此使用链队列
    for (i = 1; i <= n; ++i) EnQueue(&P, L[i]); // 起始

    for (i = 1; i <= figure; ++i) // 位数计数
    {
        for (j = 0; j < 10; ++j) ClearQueue(&Q[j]); // 重新收集前清空队列

        while (!QueueEmpty(&P)) // 分配
        {
            DeQueue(&P, &key);
            j = Radix(key, i);
            EnQueue(&Q[j], key);
        }

        for (j = 0; j < 10; ++j) // 收集
        {
            while (!QueueEmpty(&Q[j]))
            {
                DeQueue(&Q[j], &key);
                EnQueue(&P, key);
            }
        }
    }

    for (i = 1; i <= n; ++i) // 还原到L
    {
        DeQueue(&P, &key);
        L[i] = key;
    }
}

int main()
{
    int L[] = {0, 49, 38, 65, 97, 76, 13, 27, 49}; // 0号空出
    int n = sizeof(L)/sizeof(L[0]) - 1;
    Print(L, n);
    //InsertSort(L, n);

    //BInsertSort(L, n);

    //TwoInsertSort(L, n);

    //ThreeInsertSort(L, n);

    //SLinkListType SL;
    //InitSList(&SL, L, n);
    //LInsertSort(&SL);
    //Arrange(&SL);
    //Arrange_2(&SL, L);
    //LInsertSort_2(L, n);
    //ShellSort(L, n);

    //BubbleSort(L, n);
    //BubbleSort_1(L, n);
    //QuickSort(L, n);
    // QuickSort_2(L, n);

    //SelectSort(L, n);
    //SelectSort_1(L, n);

    //HeapSort(L, n);

    MergeSort(L, n);
    //RadixSort(L, n);

    Print(L, n);
    getchar();
    return 0;
}
