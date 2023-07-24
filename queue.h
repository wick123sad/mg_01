#pragma once
#include<iostream>
using namespace std;
#include<malloc.h>
#define MaxSize 1000
typedef struct
{
	int i, j;
	int pre;//��·����һ�������ڶ��е��±�
}Box1;
typedef struct
{
	Box1 data[MaxSize];
	int front, rear;
}QuType;

void InitQueue(QuType*& qu)
{
	qu = (QuType*)malloc(sizeof(QuType));
	qu->front = qu->rear = -1;
}

void DestroyQueue(QuType*& qu)
{
	free(qu);
}

bool QueueEmpty(QuType* qu)
{
	return (qu->front == qu->rear);
}

bool enQueue(QuType*& qu, Box1 e)
{
	if (qu->rear == MaxSize - 1)
		return false;
	qu->rear++;
	qu->data[qu->rear] = e;
	return true;
}

bool deQueue(QuType*& qu, Box1& e)
{
	if (qu->front == qu->rear)
		return false;
	qu->front++;
	e = qu->data[qu->front];
	return true;
}


