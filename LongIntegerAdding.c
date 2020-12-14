#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    char data;
    struct node *next;
}LNode;
// 创建链表，带头结点
LNode *create()
{
    LNode *head;
    head = (LNode *)malloc(sizeof(LNode));
    head->next = NULL;
    return head;
}
// 头插法，先插的元素排在后面
LNode *insert(LNode *head, char value)
{
    LNode *p, *t;
    t = head;
    p = (LNode *)malloc(sizeof(LNode));
    p->data = value;
    p->next = t->next;
    t->next = p;
    return head;
}
// 打印链表
LNode *print(LNode *head)
{
    LNode *p;
    p = head->next;
    if(!p)  return 0;
    while(p)
    {
      printf("%c",p->data);
      p = p->next;
    }
    return head;
}
// 相加，低位到高位
LNode* add(LNode *head1, LNode *head2)
{
    LNode *p1, *p2, *head;
    head = create();
    p1 = head1->next;
    p2 = head2->next;
    int tmp, carry = 0; // 两个数对应位相加产生的进位，初始化为 0
    char data;
    // 对应位没加完
    while(p1 && p2)
    {   // 从低位开始运算，字符'0'~'9'的ASCII值为 48~57
        tmp = (int)(p1->data)-48 + (int)(p2->data)-48 + carry;
        data = (char)(tmp % 10 + 48);
        carry = tmp / 10; // 进位
        insert(head, data);
        p1 = p1->next;
        p2 = p2->next;
    }
    // 对应位加完了，且第一个数大
    while(p1)
    {
        tmp = (int)(p1->data)-48 + carry;
        data = (char)(tmp % 10 + 48);
        carry = tmp / 10;
        insert(head, data);
        p1 = p1->next;
    }
    // 对应位加完了，且第二个数大
    while(p2)
    {
        tmp = (int)(p2->data)-48 + carry;
        data = (char)(tmp % 10 + 48);
        carry = tmp / 10;
        insert(head, data);
        p2 = p2->next;
    }
    // 最后的进位
    if(carry!=0) insert(head, (char)(carry+48));
    return head;
}

int main()
{
    LNode *head1, *head2, *head3;
    char c;
    head1 = create();
    while ((scanf("%c",&c)) != EOF)
    {
        if(c==' ' || c=='\n')  break;
        insert(head1, c);
    }
    head2 = create();
    while ((scanf("%c",&c)) != EOF)
    {
        if (c==' ' || c=='\n')  break;
        insert(head2, c);
    }
    head3 = create();
    head3 = add(head1, head2);
    print(head3);
    return 0;
}
