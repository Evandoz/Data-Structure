// 简版并查集用于 Kruskal 最小生成树
// 判断所选择的边是否落在两个连通分量中

void InitMFSet(int *S, int num)
{
    int i;
    for (i = 0; i < num; ++i)
        S[i] = -1;
}

// 合并集合（升级版）
void Mix_MFSet(int *S, int i, int j)
{
    // i数量大于j
    if (S[i] < S[j])
    {
        S[i] += S[j];
        S[j] = i;
    }
    else
    {
        S[j] += S[i];
        S[i] = j;
    }
}

// 查找元素所属集合（根）（升级版）
int Fix_MFSet(int *S, int i)
{
    int j = i, k = i, tmp;
    // 找到根（所属集合）
    while (S[j] >= 0)
        j = S[j];
    // 顺道优化集合的存储，加快检索
    while(k != j)
    {
        tmp = S[k]; // 暂存k的parent
        S[k] = j; // 直接指向根
        k = tmp;
    }
    return j;
}
