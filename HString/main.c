#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 串：堆分配存储（存储空间在执行时动态分配）

typedef int Status;

typedef struct
{
    char *str; // 若是非空串，则按串长分配存储区，否则str为NULL
    int length; // 串长度
} HString;

// 创建串
Status StrAssign(HString *S, char *str)
{
    int i, len = strlen(str);
    if (S->str) free(S->str); // 释放S原有空间
    if (len)
    {
        S->str = (char *)malloc(len * sizeof(char));
        for (i = 0; i < len; ++i) S->str[i] = str[i];
        S->length = len;
    }
    else // 串长为0
    {
        S->str = NULL; S->length = 0;
    }
    return 0;
}

// 指定位置插入子串
Status StrInsert(HString *S, int pos, HString T)
{
    int i;
    if (pos < 1 || pos > S->length+1) return -1; // 插入位置不合法
    if (T.length)
    {
        S->str = (char *)realloc(S->str, (S->length+T.length) * sizeof(char)); //重新分配空间
        for (i = S->length-1; i >= pos-1; --i) S->str[i+T.length] = S->str[i]; // 此时S->length尚未更新
        for (i = pos-1; i < pos+T.length-1; ++i) S->str[i] = T.str[i-pos+1]; // 插入
        S->length += T.length; //free(&T); // 释放T
    }
    return 0;
}

 // 串联接
Status StrConcat(HString S1, HString S2, HString *S)
{
    int i;
    if (S->str) free(S->str); // 释放S原有空间
    if (S1.length + S2.length)
    {
        S->str = (char *)malloc((S1.length+S2.length) * sizeof(char)); //重新分配空间
        for (i = 0; i < S1.length; ++i) S->str[i] = S1.str[i]; // 复制S1
        for (i = 0; i < S2.length; ++i) S->str[i+S1.length] = S2.str[i]; // 复制S2
        S->length = S1.length + S2.length;
    }
    return 0;
}

// 求子串
Status SubString(HString S, HString *Sub, int pos, int len)
{
    int i;
    // 1<=pos<=S.length 1<=pos+len-1<=S.length
    if (pos < 1 || pos > S.length || len < 0 || len > S.length-pos+1) return -1; // 位置或长度不合法
    if (Sub->str) free(Sub->str); // 释放Sub原有空间
    if (len)
    {
        Sub->str = (char *)malloc(len * sizeof(char)); //重新分配空间
        for (i = 0; i < len; ++i) Sub->str[i] = S.str[i+pos-1];
        Sub->length = len;
    }
    else
    {
        Sub->str = NULL; Sub->length = 0;
    }
    return 0;
}

// 删子串
Status StrDelete(HString *S, int pos, int len)
{
    int i;
    // 1<=pos<=S.length 1<=pos+len-1
    if (pos < 1 || pos > S->length || len < 0) return -1; // 位置或长度不合法

    if (pos+len-1 >= S->length) S->length = pos-1; // 待删串位于末尾
    else
    {
        for (i = pos-1; i < S->length-len; ++i) S->str[i] = S->str[i+len];
        S->length -= len;
    }
    return 0;
}

// 清空串
Status ClearString(HString *S)
{
    if (S->str)
    {
        free(S->str); S->str = NULL;
    }
    S->length = 0;
    return 0;
}

// 比较串
Status StrCompare(HString S1, HString S2)
{
    int i;
    for (i = 0; i < S1.length && i < S2.length; ++i)
        if (S1.str[i] != S2.str[i]) return S1.str[i] - S2.str[i];
    return S1.length - S2.length;
}

// 输出
Status Print(HString S)
{
    int i;
    printf("Length: %2d ", S.length);
    for (i = 0; i < S.length; ++i)
        printf("%c", S.str[i]);
    printf("\n");
    return 0;
}

int main()
{
    char *str1 = "qwerdcn", *str2 = "CLWEVWE";
    HString S1, S2, S, Sub;
    StrAssign(&S1, str1); StrAssign(&S2, str2);
    Print(S1); Print(S2);
    StrConcat(S1, S2, &S); Print(S);
    SubString(S, &Sub, 2, 7); Print(Sub);
    StrDelete(&S, 2, 6); Print(S);
    return 0;
}
