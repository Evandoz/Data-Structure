#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define MAXSIZE 100

//栈的数据结构
typedef struct
{
    int data[MAXSIZE];
    int top;
}Stack;

// 运算符优先级数组
//              45 91 47 93 40 41 42 43
//              -  [  /  ]  (  )  *  +
//          %9  0  1  2  3  4  5  6  7
int table[] = { 1, 0, 2, 0, 0, 0, 2, 1 };

//初始化栈空间
void init(Stack *sta)
{
    for(int i = 0; i < MAXSIZE; ++i)
        sta->data[i] = 0;
    sta->top = -1;
}

// 中缀表达式(infix)转后缀表达式(suffix)，返回值为后缀表达式数组长度
// 因为数字和运算符都当作 int 存储，为了区分，op 用于标识哪些元素是运算符(1)，哪些是运算量(0)
int infix_to_suffix(Stack *sta, char *infix, int *suffix, int *op)
{
    unsigned int i = 0; // 循环变量
    int b = 0; // 当数字是十位或以上的时候进行记录
    int j = 0; // suffix数组的下标
    int priority = 0; // 记录栈顶元素的优先级

    // for循环的第三表达式不进行i++，将其放在每一次的压栈后或直接输出到suffix进行i++
    while(i < strlen(infix))
    {
        // 如果是数字的话，直接放在suffix中,然后continue
        if(infix[i] >= '0' && infix[i] <= '9')
        {
            // 恢复打散的数码
            b=0; // 每次重新赋值为零
            while(infix[i] >= '0' && infix[i] <= '9')
                b=b * 10 + (infix[i++] - '0');
            suffix[j++] = b;
            continue;
        }

        // 如果是右中括号的话，栈中在左中括号以上的所有运算符出栈，然后continue
        if(infix[i] == 93) // ]
        {
            while(sta->data[sta->top] != 91)
            {
                suffix[j++] = sta->data[sta->top];
                sta->data[sta->top--] = 0;
            }
            sta->data[sta->top--] = 0;
            priority = table[sta->data[sta->top]%9];
            ++i;
            continue;
        }

        // 如果是右括号的话，栈中在左括号以上的所有运算符出栈，然后continue
        if(infix[i] == 41) // )
        {
            while(sta->data[sta->top] != 40)
            {
                suffix[j++] = sta->data[sta->top];
                sta->data[sta->top--] = 0;
            }
            sta->data[sta->top--] = 0;
            priority = table[sta->data[sta->top]%9];
            ++i;
            continue;
        }

        // 如果是左(中)括号的话，直接压栈
        if(infix[i] == 40 || infix[i] == 91)
        {
            sta->data[++sta->top] = infix[i++];
            priority = table[sta->data[sta->top]%9];
            continue;
        }

        // 如果只是普通的运算符，则压栈
        if(infix[i] >= 42 && infix[i] <= 47)
        {
            // 当栈顶运算符优先级大于待入栈运算符优先级时，栈顶元素依次出栈
            if(sta->top != -1 && priority >= table[infix[i]%9])
            {
                while(sta->top != -1 && priority >= table[infix[i]%9] && sta->data[sta->top] != 40 && sta->data[sta->top] != 91)
                {
                    op[j] = 1; // 【重要】标记为运算符
                    suffix[j++] = sta->data[sta->top];
                    sta->data[sta->top--] = 0;
                    priority = table[sta->data[sta->top]%9];
                }
                sta->data[++sta->top] = infix[i++];
                priority = table[sta->data[sta->top]%9];
            }
            else
            {
                // 处理负数的提取 (-x)
                if (infix[i] == 45 && infix[i-1] == 40 && sta->data[sta->top] == 40)
                {
                    b = 0;
                    while (infix[i+1] >= '0' && infix[i+1] <= '9')
                    {
                        b = b * 10 + (infix[i+1] - '0');
                        ++i;
                    }
                    suffix[j++] = b * (-1);
                    sta->data[sta->top--] = 0;
                    i += 2; // )
                    priority = table[sta->data[sta->top]%9];
                    continue;
                }
                sta->data[++sta->top] = infix[i++];
                priority = table[sta->data[sta->top]%9];
            }

        }
    }
    // 栈中剩余的元素出栈
    while (sta->top != -1)
    {
        op[j] = 1; // 【重要】标记为运算符
        suffix[j++] = sta->data[sta->top--];
    }
    return j;
}

// 后缀表达式(suffix)转为结果(result)，返回值即为计算结果
// 循环遍历后缀表达式，数字就直接压栈，运算符就取栈顶两个元素出来计算，并将结果重新压栈，栈中最后就是结果
int suffix_to_result(Stack *sta, int *suffix, int *op, int length)
{
    int i, result = 0;

    for(i = 0; i < length; ++i)
    {
        if (op[i])
        {
            switch(suffix[i])
            {
            case 42: // *
                result = sta->data[sta->top-1] * sta->data[sta->top];
                sta->data[--sta->top] = result;
                break;
            case 43: // +
                result = sta->data[sta->top-1] + sta->data[sta->top];
                sta->data[--sta->top] = result;
                break;
            case 45: // -
                result = sta->data[sta->top-1] - sta->data[sta->top];
                sta->data[--sta->top] = result;
                break;
            case 47: // /
                result = sta->data[sta->top -1] / sta->data[sta->top];
                sta->data[--sta->top] = result;
                break;
            default:
                break;
            }
        }
        else
        {
            sta->data[++sta->top] = suffix[i];
        }

    }
    return result;
}

int main()
{
    Stack sta;
    int length, result; // 后缀表达式长度，运算结果
    char istr[MAXSIZE]; // 中缀表达式
    int op[MAXSIZE] = {0}, sstr[MAXSIZE];  // 运算符标记，后缀表达式

    cout<<"Input expression:"<<endl;
    cin>>istr;

    init(&sta);
    length = infix_to_suffix(&sta, istr, sstr, op);

    cout<<"Suffix expression:"<<endl;
    for(int i=0; i < length; ++i)
    {
        if (op[i]) cout<<char(sstr[i])<<"  ";
        else cout<<sstr[i]<<"  ";
    }
    cout<<endl;

    init(&sta);
    result = suffix_to_result(&sta, sstr, op, length);

    cout<<"Result of expression:"<<endl;
    for(unsigned int i=0; i < strlen(istr); ++i)
        cout<<istr[i];
    cout<<"="<<result<<endl;

    return 0;
}
