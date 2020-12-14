#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义 Huffman 树节点
typedef struct
{
    unsigned int weight;
    unsigned int parent, lchild, rchild;
}HTNode, *HuffmanTree; // 动态分配数组存储 Huffman 树

// 动态分配数组存储 Huffman 编码，不定长编码，指针数组
typedef char * *HuffmanCode;

// 统计字符集及字符频度
void CountChar(char *str, char **c, int **w, int *n)
{
    int *weight = (int *)malloc(256 * sizeof(int));
    int i = 0;
    for (i = 0; i < 256; ++i) weight[i] = 0;
    for (i = 0; str[i] != '\0'; ++i) weight[(unsigned char) str[i]]++;
    int count = 0;
    char *tmp_c = (char *)malloc(256 * sizeof(char)); // 字符集
    int *tmp_w = (int *)malloc(256 * sizeof(int)); // 字符权值
    for (i = 0; i < 256; ++i)
    {
        if (weight[i] > 0)
        {
            tmp_c[count] = (char) i;
            tmp_w[count] = weight[i];
            count++;
        }
    }
    (*n) = count; // 字符数量
    (*c) = (char *) malloc(count * sizeof(char)); // 字符集
    (*w) = (int *)malloc(count * sizeof(int)); // 字符权值
    for(i = 0; i < count; ++i)
    {
        (*c)[i] = tmp_c[i];
        (*w)[i] = tmp_w[i];
    }
    free(weight); free(tmp_c); free(tmp_w);
}

// 选择两个最小权值节点
void Select(HuffmanTree HT, int *min, int *sec, int m)
{
    int i; int mi, se, mflag = 0, sflag = 0;
    for (i = 1; i <= m; ++i)
    {
        if (sflag == 0 && mflag == 1 && HT[i].parent == 0)
        {
            se = i; sflag = 1; break;
        }
        if (mflag == 0 && HT[i].parent == 0)
        {
            mi = i; mflag = 1;
        }
    }
    if (HT[mi].weight > HT[se].weight)
    {
        int tmp = mi;
        mi = se;
        se = tmp;
    }
    for (i = i+1; i <= m; ++i)
    {
        if (HT[i].parent == 0)
        {
            if (HT[i].weight < HT[mi].weight)
            {
                se = mi; mi = i;
            }
            else if (HT[i].weight < HT[se].weight)
            {
                se = i;
            }
        }
    }

    (*min) = mi;
    (*sec) = se;
}

// 从叶子到根，求解 Huffman 编码
// w 存放 n 个字符的权值(>0)，从叶子到根构建 Huffman 树并求得 Huffman 编码
// 也可以从根到叶子向下搜索来求编码
void HuffmanCoding(HuffmanTree *HT, HuffmanCode *HC, int *w, int n)
{
    if (n < 1) return;
    // 根据字符数量 n 分配 2n 个树节点空间（一维数组，0号未使用）
    int i, m = 2 * n -1;
    (*HT) = (HTNode *)malloc((m+1) * sizeof(HTNode));
    // 定义一个 (*HT) 树的游标指针
    HTNode *p;
    // 初始化 n 个叶子节点，0 号空间未使用，p = (*HT)+1 开始，[1~n]
    for (i = 1, p = (*HT)+1; i <= n; ++i, ++w, ++p)
    {
        (*p).weight = *w;
        (*p).parent = 0;
        (*p).lchild = 0;
        (*p).rchild = 0;
    }
    // 初始化剩下 n-1 个非终端节点，[n+1~2n-1]
    for (; i <= m; ++i, ++p) // i = n+1
    {
        (*p).weight = 0;
        (*p).parent = 0;
        (*p).lchild = 0;
        (*p).rchild = 0;
    }
    // 每次选取两个最小节点，从叶子到根建立 Huffman 树
    int min, sec;
    for (i = n+1; i <= m; ++i)
    {
        Select((*HT), &min, &sec, i-1);
        // 两个当前最小
        (*HT)[min].parent = i; (*HT)[sec].parent = i;
        // 更新根节点
        (*HT)[i].lchild = min; (*HT)[i].rchild = sec;
        (*HT)[i].weight = (*HT)[min].weight + (*HT)[sec].weight;
    }
    // 从叶子到根，逆向求每个字符的 Huffman 编码（也可以正向遍历求解）
    // n 个指针向量，不定长编码（一维数组）
    (*HC) = (HuffmanCode)malloc((n+1) * sizeof(char *));
    // 每个字符的编码的工作空间，不是编码最终存储位置
    char *cd = (char *)malloc(n * sizeof(char));
    cd[n-1] = '\0'; // 编码（字符串）结束符
    // n 个字符逆向求编码
    int start = n-1;
    int j, pa; // 当前和 parent
    for (i = 1; i <= n; ++i)
    {
        start = n-1;
        // 从叶子向根逆向求编码，j 指向当前节点，p 指向双亲；逆向搜索，所以逆着存
        for (j = i, pa = (*HT)[j].parent; pa != 0; j = pa, pa = (*HT)[pa].parent)
            if ((*HT)[pa].lchild == j) cd[--start] = '0'; // 左 0
            else cd[--start] = '1'; // 右 1
        // 不定长编码，动态分配空间
        (*HC)[i] = (char *)malloc((n-start) * sizeof(char));
        strcpy((*HC)[i], &(cd[start]));
    }
}

// 编码
// code 为编码后的序列，l 为编码后长度
void EnCode(char *str, HuffmanCode HC, char *c, int n, char **code, int *l)
{
    int i, j, len = 1;
    for (j = 0; j < n; ++j)
    {

    }
    for (i = 0; str[i] != '\0'; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            if (str[i] == c[j])
            {
                len += strlen(HC[j+1]);
                if (0 == i)
                {
                    (*code) = (char *)malloc(len * sizeof(char));
                    strcpy((*code), HC[j+1]);
                }
                else
                {
                    // 这里最好不要直接赋值，当内存分配失败时，会出bug
                    (*code) = (char *)realloc((*code), len * sizeof(char));
                    strcat((*code), HC[j+1]);
                }
                break;

            }
        }
    }
    (*code)[len-1] = '\0';
    (*l) = len-1;
}

// 根到叶子正向搜索解码
void DeCode(char *code, HuffmanTree HT, char *c, int n)
{
    int i, p = 2 * n -1; // 从根开始向下搜索
    for (i = 0; code[i] != '\0'; ++i)
    {
        if (code[i] == '0') p = HT[p].lchild; // 往左
        else if (code[i] == '1') p = HT[p].rchild; // 往右
        else return;
        if (HT[p].lchild == 0 && HT[p].rchild == 0)
        {
            printf("%c", c[p-1]);
            p = 2 * n - 1; // 回到根重新开始
        }
    }
}

int main()
{
    // str 原始字符串，c 统计字符集，code 加密之后
    char *str = "I love Vivian", *c, *code;
    int *w; // w 字符权值（频度）
    int n, l; // w 字符数量，l 加密长度
    CountChar(str, &c, &w, &n);
    HuffmanTree HT;
    HuffmanCode HC;
    HuffmanCoding(&HT, &HC, w, n);

    EnCode(str, HC, c, n, &code, &l);
    printf("%d, %s\n", l, code);

    DeCode(code, HT, c, n);
    return 0;
}
