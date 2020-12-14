#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int count = 0; // 统计数量

int IsDanger(int (*chess)[8], int row, int col)
{
    int i, j;

    // 判断列方向是否有危险
    for (i = 0; i < 8; ++i)
    {
        if (chess[i][col])
            return 1;
    }

    // 判断左上方是否有危险
    for (i = row, j = col; i >= 0 && j >= 0; --i, --j)
    {
        if (chess[i][j])
            return 1;
    }
    // 判断右下方是否有危险
    for (i = row, j = col; i < 8 && j < 8; ++i, ++j)
    {
        if (chess[i][j])
            return 1;
    }

    // 判断右上方是否有危险
    for (i = row, j = col; i >= 0 && j < 8; --i, ++j)
    {
        if (chess[i][j])
            return 1;
    }
    // 判断左下方是否有危险
    for (i = row, j = col; i < 8 && j >= 0; ++i, --j)
    {
        if (chess[i][j])
            return 1;
    }

    return 0;
}

// (*chess)[8]为棋盘行指针，row为起始行，rol表示列数
void EightQueen(int (*chess)[8], int row, int col)
{
    int i, j, chess2[8][8];

    for (i = 0; i < 8; ++i)
        for (j = 0; j < 8; ++j)
            chess2[i][j] = chess[i][j];

    if (8 == row)
    {
        printf("NO.%d:\n", count+1);
        for (i = 0; i < 8; ++i)
        {
           for (j = 0; j < 8; ++j)
           {
               if (chess2[i][j] == 0)
               {
                    printf("\1 ");
               }
               else
               {
                    SetConsoleOutputCP(437);
                    printf("%c", 219);
               }
           }
                //printf("%d ", chess2[i][j]);
            printf("\n");
        }
        printf("\n");
        ++count;
    }
    else
    {
        // 判断这个位置是否有危险
        // 如果没有危险，继续往下
        for (i = 0; i < col; ++i)
        {
           if (!IsDanger(chess, row, i)) // 不危险
           {
               for (j = 0; j < 8; ++j)
                   chess2[row][j]  = 0; // 接下来，这一行其他列不可放置
                chess2[row][i] = 1; // 放置

                EightQueen(chess2, row+1, col);
           }
        }
    }
}

int main()
{
    int i, j, chess[8][8];

    for (i = 0; i < 8; ++i)
        for (j = 0; j < 8; ++j)
            chess[i][j] = 0;
    EightQueen(chess, 0, 8);

    return 0;
}
