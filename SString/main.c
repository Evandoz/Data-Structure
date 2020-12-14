#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 串：定长顺序存储表示
// 地址连续空间存储，按照预定义大小，为串分配固定长度空间（定长数组）
// 串超过预定义长度的串值会被丢弃，称之为截断。

#define MAXSTRLEN 10

typedef int Status;

typedef unsigned char SString[MAXSTRLEN+1]; // 0号单元存放串长度

/*
  对于串长的表示有两种方法：
  1.如此处定义一样，用0号单元存放串实际长度，如PASCAL；
  2.在串值后加一个*不计入串长*的结束标记字符，如C语言中用"\0"表示串值结束。此时串长为隐含值，显然不便于某些串操作
*/

Status StrAssign(SString S, char *str) // 创建值为str的串
{
    int i;
    S[0] = strlen(str);
    for (i = 0; i < S[0]; ++i)
        S[i+1] = str[i];
    return 0;
}

// 串联接
Status Concat(SString S1, SString S2, SString S) // S为联接后的新串
{
    int i;
    if (S1[0] + S2[0] <= MAXSTRLEN) // 未截断，S1和S2串长和小于预定义大小
    {
        for (i = 1; i <= S1[0]; ++i) S[i] = S1[i];
        for (i = 1; i <= S2[0]; ++i) S[i+S1[0]] = S2[i];
        S[0] = S1[0] + S2[0]; return 1; // 返回1，表示未截断
    }
    else if (S1[0] < MAXSTRLEN) // S2被部分截断
    {
        for (i = 1; i <= S1[0]; ++i) S[i] = S1[i];
        for (i = 1; i <= MAXSTRLEN-S1[0]; ++i) S[i+S1[0]] = S2[i];
        S[0] = MAXSTRLEN; return 0; // 返回1，表示未截断
    }
    else // S1长度已达到预定义值，注意此处else并非S1[0]>=MAXSTRLEN，因为定长存储其长度不超过MAXSTRLEN
    {
        // S1[0] == MAXSTRLEN
        for (i = 0; i <= S1[0]; ++i) S[i] = S1[i]; // 直接从0开始复制，长度一样
        /*S[0] = MAXSTRLEN;*/ return 0; // 返回1，表示未截断
    }
}

// 求子串，从主串S的pos位置起截取长度为len的子串
// 1<=pos<=S[0]。1<=pos+len-1
Status SubString(SString S, SString Sub, int pos, int len) // S为联接后的新串
{
    int i;
    if (pos < 1 || pos > S[0] || len < 0 || len > S[0]-pos+1)
        return 0;
    for (i = 1; i <= len; ++i) Sub[i] = S[pos+i-1];
    Sub[0] = len; return 1;
}

// 删子串，从主串S的pos位置起删除长度为len的子串
// 1<=pos<=S[0]。1<=pos+len-1<=S[0]
Status StrDelete(SString S, int pos, int len) //
{
    int i;
    if (pos < 1 || pos > S[0] || len < 0)
        return 0;
    if (pos+len-1 >= S[0]) S[0] = pos - 1; // 待删除的字串位于主串的末尾，直接修改串长即可
    else
    {
        // 待删除的字串位于主串的中间，需要移动S[pos+len..S[0]]到S[pos..S[0]-len]
        for (i = pos; i <= S[0]-len; ++i) S[i] = S[i+len];
        S[0] -= len;
    }
    return 1;
}

// 朴素模式匹配
int Index(SString S, SString Sub)
{
    int i = 1, j = 1, count = 0;
    while (i <= S[0] && j <= Sub[0])
    {
        ++count;
        if (S[i] == Sub[j])
        {
            ++i; ++j;
        }
        else
        {
            i = i-j+2; j = 1; // i回到与Sub起始相匹配的下一个位置，j会到起始
        }
    }
    printf("Index: %d\n", count);
    if (j > Sub[0]) return i-Sub[0]; // 匹配成功
    else return 0;
}

// next数组
void get_next(SString S, int next[])
{
    int j = 0, i = 1; next[1] = 0;
    while (i < S[0])
    {
        /*if (j == 0 || Sub[j] == Sub[i])
        {
            next[i+1] = next[i]+1; // j+1
            ++i; ++j;
        }*/
        if (j == 0 || S[j] == S[i]) next[++i] = ++j; // j+1
        else j = next[j];
    }
}

// next数组优化
// 问题：根据next数组，当S[i]与Sub[j]失配时，j应当滑动到next[j]
// 即，S[i]应该与Sub[next[j]]比较，但如果此时Sub[next[j]]==Sub[j]的话，此次滑动比较完全没有必要
void get_nextval(SString S, int nextval[])
{
    int j = 0, i = 1; nextval[1] = 0;
    while (i < S[0])
    {
        /*if (j == 0 || S[j] == S[i])
        {
            nextval[i+1] = nextval[i]+1; // j+1
            ++i; ++j;
        }*/
        if (j == 0 || S[j] == S[i])
        {
            ++i; ++j;
            if (S[i] == S[j]) nextval[i] = nextval[j];
            else nextval[i] = j;
        }
        else j = nextval[j];
    }
}

// KMP模式匹配算法
int Index_KMP(SString S, SString Sub)
{
    int i = 1, j = 1, count = 0;

    int *next = (int *)malloc((Sub[0]+1) * sizeof(int));
    //get_next(Sub, next);
    get_nextval(Sub, next);

    while (i <= S[0] && j <= Sub[0])
    {
        ++count;
        if (j == 0 || S[i] == Sub[j])
        {
            ++i; ++j;
        }
        else
        {
            j = next[j]; // i不动，j滑到最大公共前缀末端的下一位置
        }
    }
    printf("KMP: %d\n", count);
    if (j > Sub[0]) return i-Sub[0]; // 匹配成功
    else return 0;
}

Status Print(SString S)
{
    int i;
    printf("Length: %2d ", S[0]);
    for (i = 1; i <= S[0]; ++i)
        printf("%c", S[i]);
    printf("\n");
    return 0;
}

int main()
{
    char *str1 = "ABDABCFG", *str2 = "HIJKLMN";
    SString S1, S2, S, Sub;
    StrAssign(S1, str1); StrAssign(S2, str2);
    Print(S1); Print(S2);
    Concat(S1, S2, S); Print(S);
    SubString(S, Sub, 4, 5); Print(Sub);
    printf("Index: Sub index %d of S\n", Index(S, Sub));
    printf("KMP: Sub index %d of S\n", Index_KMP(S, Sub));
    //StrDelete(S, 2, 6); Print(S);
    return 0;
}
