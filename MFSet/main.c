#include "MFSet.h"

int main()
{
    char *c = "ADVFESZCBS";
    struct {
        int i, j;
    }EC[8] = { {1,2}, {3,4}, {1,3}, {1,6}, {4,9}, {4,7}, {7,8}, {5,6} };

    int k;
    MFSet S;
    InitMFSet(&S, 10, c);
    for (k = 0; k < 8; ++k)
    {
        Mix_MFSet(&S, EC[k].i, EC[k].j);
    }

    printf("|");
    for (k = 0; k < 10; ++k)
    {
        printf(" %d |", k);
    }
    printf("\n");
    for (k = 0; k < 10; ++k)
    {
        printf("---");
    }
    printf("\n|");
    for (k = 0; k < 10; ++k)
    {
        printf(" %c |", S.nodes[k].data);
    }
    printf("\n");
    for (k = 0; k < 10; ++k)
    {
        printf("---");
    }
    printf("\n|");
    for (k = 0; k < 10; ++k)
    {
        printf(" %d |", S.nodes[k].parent);
    }
    printf("\n");
    for (k = 0; k < 10; ++k)
    {
        printf("---");
    }
    printf("\n");

    return 0;
}
