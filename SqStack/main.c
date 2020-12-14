#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <windows.h>

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

#define OK 1
#define ERROR 0

#define MAZE_WIDTH 10
#define MAZE_HEIGHT 10

typedef int Status;

typedef int SElemType;
//typedef char SElemType;

/*
// 坐标
typedef struct
{
    int x, y; // 坐标
} PosType;

// 迷宫求解
typedef struct
{
    int order; // 通道块在路径上的序号
    PosType seat; // 通道块在迷宫中的坐标
    int direction; // 从该通道块走向下一通道块的方向
} SElemType;

typedef int MazeType;
*/

// 由于栈在使用中最大空间难以估计，因此采用堆动态分配的方式
// 起始分配一个初始空间，当空间不足时逐渐增加

// 若是固定大小，则可如下面这样定义

/*
typedef struct
{
    SElemType data[MAXSIZE];
    int top;
} SqStack;
*/

//顺序栈定义
typedef struct
{
	SElemType *top; //栈顶指针
	SElemType *base; //在栈构造之前和销毁之后为NULL
	int stacksize; //当前已分配大小
}SqStack;

//初始化
Status InitStack(SqStack *S)
{
	S->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S->base) return ERROR;
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}

//判空
Status StackEmpty(SqStack S)
{
	if (S.top == S.base)
		return OK;
	return ERROR;
}

//栈满
Status StackFull(SqStack S)
{
	if (S.top - S.base >= S.stacksize)
		return OK;
	return ERROR;
}

//栈顶
Status GetTop(SqStack S, SElemType* e)
{
	if (StackEmpty(S))
		return ERROR;
	(*e) = *(S.top - 1);
	return OK;
}

//压栈
Status PushStack(SqStack *S, SElemType e)
{
	if (StackFull(*S))
	{
		S->base = (SElemType *)realloc(S->base,
			(S->stacksize + STACKINCREMENT)*sizeof(SElemType));
		if (!S->base) return ERROR;
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCREMENT;
	}
	*(S->top++) = e;
	return OK;
}

//弹栈
Status PopStack(SqStack *S, SElemType* e)
{
	if (StackEmpty(*S)) return ERROR;
	(*e) = *(--S->top);
	return OK;
}

//清空
Status ClearStack(SqStack *S)
{
	S->top = S->base;
	S->stacksize = 0;
	return OK;
}

//销毁
Status DestroyStack(SqStack *S)
{
	int i;
	for (i = 0; i < S->stacksize; ++i)
	{
		free(S->base);
		++S->base;
	}

	S->top = S->base = NULL;
	S->stacksize = 0;
	return OK;
}

//长度
int StackLength(SqStack S)
{
	return S.top - S.base;
}

//遍历栈
/*Status TraverseUp(SqStack S)
{
    SElemType *p;
    p = S.base;
    while(p < S.top)
    {
        printf("%c",*p);
        p++;
    }
    return OK;
}

Status TraverseDown(SqStack S)
{
	SElemType *p;
	p = S.top - 1;
	while (p >= S.base)
	{
		printf("%c", *p);
		p--;
	}
	return OK;
}

//测试函数
void test()
{
	SqStack S;
	InitStack(&S);
	int n;
	scanf("%d", &n);
	while (n--)
	{
		SElemType e;
		scanf("%c", &e);
		PushStack(&S, e);
	}
	printf("The length of the stack is: %d\n", StackLength(S));
	printf("The elem of the stack is following from the top to base:\n");
	TraverseDown(S);
}

// 数制转换：将X进制数N变成Y进制
// Nx = An x Y^n + An-1 x Y^(n-1) + ... + A1 x Y^1 + A0 x Y^0
// 因此需要用Y对N不断取余，然后逆序输出（后进先出）
void conversion()
{
    int n, t;
    SqStack S; InitStack(&S);
    scanf("%d", &n); t = n;
    while (t)
    {
        PushStack(&S, 48+t%8);
        t /= 8;
    }
    printf("%d(10) = ",  n);
    TraverseDown(S);
    printf("(8)\n");
}

// 二进制转十进制
void Bin2Dec()
{
    SqStack S; InitStack(&S); char c; int i, len, sum = 0;
    printf("Please input integer in the form of bin(end with #):\n");
    c = getchar();
    while (c != '#')
    {
        PushStack(&S, c); c = getchar();
    }
    getchar();// 过滤回车

    len = StackLength(S);
    for (i = 0; i < len; ++i)
    {
        PopStack(&S, &c);
        sum += (c-48) * pow(2, i);
    }
    printf("->%d(10)\n", sum);
}

// 二进制转八进制
void Bin2Oct()
{

}

// 二进制转十六进制
void Bin2Hex()
{

}

Status CheckBracket()
{
    SqStack S; InitStack(&S); int count = 0; char c, s;
    printf("Please input(end with #):\n");
    c = getchar();
    while (c != '#')
    {
        ++count;
        switch (c)
        {
        case '(':
            PushStack(&S, c); break;
        case '[':
            PushStack(&S, c); break;
        case '{':
            PushStack(&S, c); break;
        case ')':
            if (StackEmpty(S)) return 0;
            else
            {
                PopStack(&S, &s);
                if ('(' != s)
                {
                    printf("[%d].%c", count, c);
                    return 0;
                }
            }
            break;
        case ']':
            if (StackEmpty(S)) return 0;
            else
            {
                PopStack(&S, &s);
                if ('[' != s)
                {
                    printf("[%d].%c", count, c);
                    return 0;
                }
            }
            break;
        case '}':
            if (StackEmpty(S)) return 0;
            else
            {
                PopStack(&S, &s);
                if ('{' != s)
                {

                    printf("[%d].%c", count, c);
                    return 0;
                }
            }
            break;
        default:
            break;
        }
        c = getchar();
    }
    if (StackEmpty(S)) return 1;
    else { GetTop(S, &s); printf("%c", c); return 0; } // 栈中有剩余
}

void LineEdit()
{
    SqStack S; InitStack(&S);
    char s, c = getchar();
    while (c != EOF)
    {
        while (c != EOF && c != '\n')
        {
            switch (c)
            {
            case '#': // 退格
                PopStack(&S, &s); break;
            case '@': // 退行
                ClearStack(&S);   break;
            default:
                PushStack(&S, c); break;
            }
            c = getchar();
        }
        // 一行结束或者一页结束，送入存储区
        TraverseUp(S);
        ClearStack(&S);
        if (c != EOF) c = getchar(); // 如果是行结束，继续接受字符
    }
    DestroyStack(&S);
}*/

/*
// 判断当前位置可通（未曾走过的通道块）
Status Pass(MazeType (*maze)[MAZE_WIDTH], PosType curpos)
{
    return !maze[curpos.x][curpos.y];
}

// 留下足迹
Status FootPrint(MazeType (*maze)[MAZE_WIDTH], PosType curpos, int di)
{
    maze[curpos.x][curpos.y] = di;
    return 0;
}

// 标记不可通
Status MarkPrint(MazeType (*maze)[MAZE_WIDTH], PosType curpos)
{
    maze[curpos.x][curpos.y] = -1;
    return 0;
}

Status IsEqual(PosType pos1, PosType pos2)
{
    if (pos1.x == pos2.x && pos1.y == pos2.y)
        return 1;
    return 0;
}

PosType NextPos(PosType curpos, int di)
{
    struct {
        int x, y;
    } move[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    PosType nextpos;
    nextpos.x = curpos.x + move[di-1].x;
    nextpos.y = curpos.y + move[di-1].y;
    return nextpos;
}

Status MazePath(MazeType (*maze)[10], PosType start, PosType end)
{
    SqStack S; InitStack(&S);
    SElemType e; int curstep = 1;
    PosType curpos = start; // 当前位置
    do
    {
        if (Pass(maze, curpos)) // 当前可通
        {
            FootPrint(maze, curpos, 1);
            e.order = curstep; e.seat = curpos; e.direction = 1;
            PushStack(&S, e);
            if (IsEqual(curpos, end)) return 1;
            curpos = NextPos(curpos, 1);
            ++curstep;
        }
        else // 当前不可通
        {
            if (!StackEmpty(S))
            {
                PopStack(&S, &e);
                while (e.direction == 4 && !StackEmpty(S)) // 栈顶通路四方不通
                {
                    MarkPrint(maze, e.seat); //标记不可通
                    PopStack(&S, &e);
                }

                if (e.direction < 4) // 栈顶通路尚有方向可走
                {
                    e.direction += 1;
                    FootPrint(maze, e.seat, e.direction);
                    PushStack(&S, e);
                    curpos = NextPos(e.seat, e.direction); // 切换到该方向
                }
            }
        }
    } while(!StackEmpty(S));
    return 0;
}

void maze()
{
    int maze[MAZE_HEIGHT][MAZE_WIDTH] = {
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5, 0, 0, 5, 0, 0, 0, 5, 0, 5},
	    {5, 0, 0, 5, 0, 0, 0, 5, 0, 5},
	    {5, 0, 0, 0, 0, 5, 5, 0, 0, 5},
	    {5, 0, 5, 5, 5, 0, 0, 0, 0, 5},
	    {5, 0, 0, 0, 5, 0, 0, 0, 0, 5},
	    {5, 0, 5, 0, 0, 0, 5, 0, 0, 5},
	    {5, 0, 5, 5, 5, 0, 5, 5, 0, 5},
	    {5, 5, 0, 0, 0, 0, 0, 0, 0, 5},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}
    };
    int i, j;
    PosType start = {1, 1}, end = {7, 8};
    MazePath(maze, start, end);
    printf("  ");
    for (i = 0; i < MAZE_HEIGHT; ++i)
        printf(" %d", i);
    printf("\n");
    for (i = 0; i < MAZE_HEIGHT; ++i)
    {
        printf("%d ", i);
        for (j = 0; j < MAZE_WIDTH; ++j)
        {
            switch (maze[i][j])
            {
            case -1:
                printf("  ");
                break;
            case 0:
                printf("  ");
                break;
            case 1:
                printf("->");
                break;
            case 2:
                printf("||");
                break;
            case 3:
                printf("<-");
                break;
            case 4:
                printf("|1");
                break;
            case 5:
                SetConsoleOutputCP(437);
                printf("%c", 219);
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
}
*/

// 比较运算符优先级

// ASCII: [40]( [41])  [42]* [43]+ [45]- [47]/  [91][ [93]] [123]{ [125]}
// MOD 9    4     5      6     7     0     2      1     3      6      8
// MOD 10   0     1      2     3     5     7      1     3      3      5
// MOD 11   7     8      9    10     1     3      3     5      2      4
// MOD 12   4     5      6     7     9    11      7     9      3      5
// MOD 13   1     2      3     4     6     8      0     2      6      8
// MOD 17   6     7      8     9     11   13      6     8      6      8
// MOD 40   0     1      2     3     5     7     11    13      3      5
Status Precede(char optr1, char optr2) // optr1>=optr2
{
    //运算符优先级数组（取巧--对9取余，不计算大括号）：
    int optr[]={1,0,2,0,0,0,2,1};
             // - [ / ] ( ) * +
    if (optr[optr1%9] >= optr[optr2%9])
        return 1;
    return 0;
}

int Operate(int opnd1, int opnd2, char optr) // 运算：opnd1 optr opnd2
{
    switch(optr)
    {
    case '*':
        return opnd1 * opnd2; break;
    case '+':
        return opnd1 + opnd2; break;
    case '-':
        return opnd1 - opnd2; break;
    case '/':
        return opnd1 / opnd2; break; // 除数不能为0注意判断
    }
    return 0;
}

// 表达式运算（普通版本，均为整型）

// 如果所有运算符优先级一致，从左往右依次运算即可
// 然而事与愿违，运算符有等级，于是就不能从左往右的计算
// 需要先运算级别高的，但计算机不知道哪个位置的运算符级别更高
// 所以我们需要用栈来存储之前的运算符，每次运算符进栈之前需要跟上一次进栈的进行比较
// 如果栈顶运算符优先级高，那么说明该运算符是目前为止优先级最高的（极点），需要立即计算该部分结果
// 事实上，这跟我们手动计算的过程是一致的，我们需要把之前优先级低的运算符存入我们的大脑（留在纸上，暂时忽略）
// 而计算机也需要把它们存在自己的大脑里（堆栈[内存]）

// 定义两个栈，运算数栈和运算符栈
Status EvaluateExpression()
{
    SqStack OPND, OPTR; // 运算数，运算符
    InitStack(&OPND); InitStack(&OPTR);
    char c = getchar();
    int opnd, opnd1, opnd2; // 顶部数字opnd1, opnd2
    char optr; // 顶部运算符
    while (1)
    {
        if(c >= '0' && c <= '9') //数字，也可以直接调用isdigit()函数，头文件为ctype.h
        {
            // 因为按照字符的形式接收，因此数字入栈时，
            // 需要将打散的数字重新组合

            opnd = 0;//每次重新赋值为零

            do { // 整型，如果为浮点型，还需要考虑小数点
                opnd = opnd * 10 + (c - '0');
                c = getchar();
            } while(c >= '0' && c <= '9'); //恢复打散的数码
            //printf("%d\n", opnd);
            PushStack(&OPND, opnd);
            //continue; // 不是数字，还需要判断是哪个字符
        }
        else if (c >= '*' && c <= '/') // 运算符
        {
            // 栈顶优先级高
            if (GetTop(OPTR, &optr) && Precede(optr, c) && optr != '(' && optr != '[')
            {
                PopStack(&OPTR, &optr); PopStack(&OPND, &opnd1); PopStack(&OPND, &opnd2);
                printf("%d%c%d=%d\n", opnd2, optr, opnd1, Operate(opnd2, opnd1, optr));
                PushStack(&OPND, Operate(opnd2, opnd1, optr));
            }
            else
            {
                PushStack(&OPTR, c); c = getchar();
            }
        }
        else if (c == '(' || c == '[') // 左（小/中）括号直接入栈
        {
            PushStack(&OPTR, c); c = getchar();
        }
        else if (c == ')') // 右小括号
        {
            while (GetTop(OPTR, &optr) && optr != '(' ) // 左括号以上，自顶向下运算出栈（上层运算符优先级一定高于下层）
            {
                PopStack(&OPTR, &optr); PopStack(&OPND, &opnd1); PopStack(&OPND, &opnd2);
                printf("%d%c%d=%d\n", opnd2, optr, opnd1, Operate(opnd2, opnd1, optr));
                PushStack(&OPND, Operate(opnd2, opnd1, optr));
            }
            if (PopStack(&OPTR, &optr)) // 左括号(出栈丢弃
                c = getchar();
            else
                return -1; // 栈空出错
        }
        else if (c == ']') // 右中括号
        {
            while (GetTop(OPTR, &optr) && optr != '[')
            {
                PopStack(&OPTR, &optr); PopStack(&OPND, &opnd1); PopStack(&OPND, &opnd2);
                printf("%d%c%d=%d\n", opnd2, optr, opnd1, Operate(opnd2, opnd1, optr));
                PushStack(&OPND, Operate(opnd2, opnd1, optr));
            }
            if (PopStack(&OPTR, &optr)) // 左括号[出栈丢弃
                c = getchar();
            else
                return -1; // 栈空出错
        }
        else if (c == '\n') // 回车结束
        {
            while (GetTop(OPTR, &optr) && optr != '(' && optr != '[')
            {
                PopStack(&OPTR, &optr); PopStack(&OPND, &opnd1); PopStack(&OPND, &opnd2);
                printf("%d%c%d=%d\n", opnd2, optr, opnd1, Operate(opnd2, opnd1, optr));
                PushStack(&OPND, Operate(opnd2, opnd1, optr));
            }
            break;
        }
    }
    GetTop(OPND, &opnd1);
    printf("%d\n", opnd1);
    return 0;
}

int main()
{
	//test();
	//conversion();
	//Bin2Dec();
	//printf("  %d\n", CheckBracket());
	//LineEdit();
    //maze();
    EvaluateExpression();
	return 0;
}

