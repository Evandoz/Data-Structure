#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X 8
#define Y 8

int chess[X][Y];

// 找到基于(x, y)的下一个可走的位置
int nextxy(int *x, int *y, int count)
{
    // 共有8个方向，对每个方向进行可行性判断
    switch(count)
    {
    case 0:
        // 右进二，上进一
        if ((*x)+2 <= X-1 && (*y)-1 >=0 && chess[(*x)+2][(*y)-1] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) += 2; (*y) -= 1; return 1;
        }
        break;
    case 1:
        // 右进二，下进一
        if ((*x)+2 <= X-1 && (*y)+1 <=Y-1 && chess[(*x)+2][(*y)+1] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) += 2; (*y) += 1; return 1;
        }
        break;
    case 2:
        // 右进一，下进二
        if ((*x)+1 <= X-1 && (*y)+2 <=Y-1 && chess[(*x)+1][(*y)+2] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) += 1; (*y) += 2; return 1;
        }
        break;
    case 3:
        // 左进一，下进二
        if ((*x)-1 >= 0 && (*y)+2 <= Y-1 && chess[(*x)-1][(*y)+2] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) -= 1; (*y) += 2; return 1;
        }
        break;
    case 4:
        // 左进二，下进一
        if ((*x)-2 >= 0 && (*y)+1 <= Y-1 && chess[(*x)-2][(*y)+1] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) -= 2; (*y) += 1; return 1;
        }
        break;
    case 5:
        // 左进二，上进一
        if ((*x)-2 >= 0 && (*y)-1 >=0 && chess[(*x)-2][(*y)-1] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) -= 2; (*y) -= 1; return 1;
        }
        break;
    case 6:
        // 左进一，上进二
        if ((*x)-1 >= 0 && (*y)-2 >=0 && chess[(*x)-1][(*y)-2] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) -= 1; (*y) -= 2; return 1;
        }
        break;
    case 7:
        // 右进一，上进二
        if ((*x)+1 <= X-1 && (*y)-2 >=0 && chess[(*x)+1][(*y)-2] == 0) // 判断是否在棋盘内，以及是否已访问
        {
            (*x) += 1; (*y) -= 2; return 1;
        }
        break;
    default:
        break; // 无路可走
    }
    return 0;
}

// 深度优先遍历棋盘，(x,y)为其实位置，tag为标记变量，每走一步，tag+1，标识第几部，tag最终值应为 X*Y（棋盘位置总数）
int TravelChessBoard(int x, int y, int tag)
{
    int x1 = x, y1 = y, flag = 0, count = 0;

    // 标记该位置，表示已走过
    chess[x][y] = tag;

    // 走完棋盘，递归出口
    if (X*Y == tag)
    {
        // 遍历结束，打印棋盘
        print();
        return 1;
    }

    // 找到下一个可走的坐标(x1, y1)，即调用 nextxy() 函数，如果找到flag=1,否则为0，flag为标志
    flag = nextxy(&x1, &y1, count);

    while(0 == flag && count < 7) // 如果当前count方向不可走，尝试下一个方向，直到找到一个可走的位置
    {
        count++;
        flag = nextxy(&x1, &y1, count);
    }

    // 如果找到下一步，递归下一步，当这条路径没能到达终点，即中间出现flag = 0，
    while(flag)
    {
        if (TravelChessBoard(x1, y1, tag+1)) // 递归调用，传入下一个位置
        {
            return 1;
        }
        // 出口，当X*Y=tag时，从if中返回，否则该路径失败，需要回退到(x, y)

        // 找到下一个可走的坐标(x1, y1)，即调用 nextxy() 函数，如果找到flag=1,否则为0
        x1 = x; y1 = y; count++; // count++ 尝试下一个方向 nextxy()

        flag = nextxy(&x1, &y1, count);

        while(0 == flag && count < 7)
        {
            count++;
            flag = nextxy(&x1, &y1, tag+1);
        }
    }

    // 到达此点时，八个方向都走不通，于是回退，从上一个点继续找下一步
    if (0 == flag)
    {
        chess[x][y] = 0; // 重置
    }

    return 0;
}

void print()
{
    int i, j;
    for (i = 0; i < X; ++i)
    {
        for (j = 0; j < Y; ++j)
        {
            printf("%2d\t", chess[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int i, j;
    clock_t start, finish;
    start = clock();
    for (i = 0; i < X; ++i)
    {
        for (j = 0; j < Y; ++j)
        {
            chess[i][j] = 0;
        }
    }

    if (TravelChessBoard(2, 0, 1) == 0)
    {
        printf("fail!!");
    }

    finish = clock();

    printf("\nIt costs time(s): %f\n", (double)(finish-start)/CLOCKS_PER_SEC);

    getchar();

    return 0;
}
