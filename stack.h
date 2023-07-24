#pragma once
#include<iostream>
using namespace std;
#define MaxSize 200
typedef struct box
{
    int i;
    int j;
    int di;
}Box;//方块类型
typedef struct st
{
    Box data[MaxSize];
    int top;
}SqStack;
void InitStack(SqStack*& s)
{
    s = (SqStack*)malloc(sizeof(SqStack));
    s->top = -1;
}
void DestoryStack(SqStack*& s)
{
    free(s);
    s = NULL;
}
bool PushStack(SqStack*& s, Box e)//进栈
{
    if (s->top == MaxSize - 1)
        return false;
    s->top++;
    s->data[s->top] = e;
    return true;
}
bool PopStack(SqStack*& s, Box& e)//出栈
{
    if (s->top == -1)
        return false;
    e = s->data[s->top];
    s->top--;
    return true;
}
bool GetTop(SqStack* s, Box& e)
{
    if (s->top == -1)
        return false;
    e = s->data[s->top];
    return true;
}
bool StackEmpty(SqStack* s)
{
    return(s->top == -1);
}

