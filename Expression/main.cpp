#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define MAXSIZE 100

// ��������ȼ�����
//                   45 91 47 93 40 41 42 43
//                   -  [  /  ]  (  )  *  +
//               %9  0  1  2  3  4  5  6  7
int table[]       = {1, 0, 2, 0, 0, 0, 2, 1};

//                   -  [  /  ]  (  )  *  +
int precede[8][8] = {
                    {1, 0, 0, 1, 0, 1, 0, 1}, // -
                    {0, 0, 0, 2, 0, 7, 0, 0}, // [
                    {1, 0, 1, 1, 0, 1, 1, 1}, // /
                    {1, 9, 1, 1, 1, 1, 1, 1}, // ]
                    {0, 0, 0, 7, 0, 2, 0, 0}, // (
                    {1, 1, 1, 1, 9, 1, 1, 1}, // )
                    {1, 0, 1, 1, 0, 1, 1, 1}, // *
                    {1, 0, 0, 1, 0, 1, 0, 1}  // +
                };

int expression(char *in)
{
    // ���ջ optr��������ջ opnd
    // ջ��ָ�� tr��nd
    char optr[MAXSIZE];
    unsigned int i = 0;
    int s = 0, tr = -1, nd = -1, opnd[MAXSIZE];
    do
    {
        if(in[i] >= '0' && in[i] <= '9')
        {
            // �ָ���ɢ������
            s = 0;
            while(in[i] >= '0' && in[i] <= '9')
                s=s * 10 + (in[i++] - '0');
            opnd[++nd] = s;
        }
        else if (tr == -1)
        {
            optr[++tr] = in[i++];
        }
        else
        {
            switch(precede[int(optr[tr])%9][int(in[i])%9])
            {
            case 0:
                optr[++tr] = in[i++]; break;
            case 1:
                switch(optr[tr--])
                {
                case '*':
                    s = opnd[nd-1] * opnd[nd];
                    opnd[--nd] = s;
                    break;
                case '+':
                    s = opnd[nd-1] + opnd[nd];
                    opnd[--nd] = s;
                    break;
                case '-':
                    s = opnd[nd-1] - opnd[nd];
                    opnd[--nd] = s;
                    break;
                case '/':
                    s = opnd[nd-1] / opnd[nd];
                    opnd[--nd] = s;
                    break;
                default:
                    break;
                }
            case 2:
                --tr; ++i; break;
            case 7:
                return -1;
            }
        }
    }while(i < strlen(in));
    while(tr != -1)
    {
        switch(optr[tr--])
        {
        case '*':
            s = opnd[nd-1] * opnd[nd];
            opnd[--nd] = s;
            break;
        case '+':
            s = opnd[nd-1] + opnd[nd];
            opnd[--nd] = s;
            break;
        case '-':
            s = opnd[nd-1] - opnd[nd];
            opnd[--nd] = s;
            break;
        case '/':
            s = opnd[nd-1] / opnd[nd];
            opnd[--nd] = s;
            break;
        default:
            break;
        }
    }
    return opnd[0];
}

int main()
{
    char istr[MAXSIZE];  // �������ǣ���׺���ʽ

    cout<<"Input expression:"<<endl;
    cin>>istr;

    cout<<"Result of expression:"<<endl;
    for(unsigned int i=0; i < strlen(istr); ++i)
        cout<<istr[i];
    cout<<"="<<expression(istr)<<endl;

    return 0;
}
