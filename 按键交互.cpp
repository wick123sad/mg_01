#include<iostream>
#include<stdlib.h>
#include<graphics.h>	//插件easyx
#include"assert.h"		//断言处理
#include<string>
#include<time.h>
#include"stack.h"
#include"queue.h"

int mg[25][25] = {0};
int L = 10, W = 10;//初始迷宫的大小

bool mgpath(int xi, int yi, int xe, int ye);
bool mgpath2(int xi, int yi, int xe, int ye);
void CrtMg(int mg[][25],int L,int W);

typedef struct Button
{
	int x, y, w, h;
	COLORREF color;
	char* text;
}BUTTON,*LPBUTTON;

LPBUTTON S_Button;//起点，终点
LPBUTTON E_Button;

LPBUTTON Crt(int x, int y, int w, int h,const char* str)//初始化按钮
{
	LPBUTTON button = new BUTTON;
	assert(button);
	button->x = x;
	button->y = y;
	button->w = w;
	button->h = h;
	int length = strlen(str)+1;
	button->text = new char(length);
	assert(button->text);
	strcpy_s(button->text,length, str);		
	button->color = RGB(255,255,255);
	return button;
}

void DrawButton(LPBUTTON button)//画按钮函数
{
	setfillcolor(button->color);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3); //画线样式为宽度 3 像素的实线，连接处为斜面
	setlinecolor(BLACK);
	fillrectangle(button->x, button->y, button->x + button->w, button->y + button->h);
	setbkmode(TRANSPARENT);//考虑到文字背景可能也有默认颜色，可能发生遮挡，所以将文字背景改为透明
	settextstyle(25, 0, "楷体");	
	settextcolor(BLACK);
	int xx, yy;//使文本居中
	xx = (button->w - textwidth(button->text) )/ 2 + button->x;
	yy = (button->h - textheight(button->text) )/ 2 + button->y;
	outtextxy(xx, yy, button->text);
}


void DrawMg(int mg[][25],int L,int W)
{
	int x = 300, y = 10;
	for (int i = 0; i < L; i++)
	{
		x = 300;
		for (int j = 0; j < L; j++)
		{
			x += 40;
			LPBUTTON Mg_Button = Crt(x, y, 40, 40, "");
			
			if (mg[i][j] == 1)			//墙
				Mg_Button->color = RGB(181, 181, 181);
			if (mg[i][j] == 0)			//可走方块
				Mg_Button->color = WHITE;
			if (mg[i][j] == -2)			//退栈元素，颜色和可走方块一样
				Mg_Button->color = YELLOW;
			if (mg[i][j] == -1)			//进栈元素、最短路径元素
				Mg_Button->color = RGB(0, 255, 0);
			DrawButton(Mg_Button);
		}
		y += 40;
	}
	BeginBatchDraw();
	DrawButton(S_Button);
	DrawButton(E_Button);
	FlushBatchDraw();
	EndBatchDraw();
}

int main()
{
	initgraph(1300, 800);
	setbkcolor(RGB(204,255, 204));
	cleardevice();
	LPBUTTON DFS_Button = Crt(30, 30, 100, 40, "深度遍历");
	LPBUTTON BFS_Button = Crt(30, 80, 100, 40, "广度遍历");
	LPBUTTON Continue_Button = Crt(30, 130, 100, 40, "继续游戏");
	LPBUTTON Quit_Button = Crt(30, 230, 100, 40, "退出游戏");
	LPBUTTON LW_Button = Crt(30, 180, 100, 40, "迷宫大小");
	
	while (1)
	{
		BeginBatchDraw();//双缓冲
		DrawButton(DFS_Button);
		DrawButton(BFS_Button);
		DrawButton(Continue_Button);
		DrawButton(Quit_Button);
		DrawButton(LW_Button);
		FlushBatchDraw();
		EndBatchDraw();
		CrtMg(mg,L,W);
		srand((unsigned)time(NULL));
		int xi = rand() % (L-2) + 1;
		int yi = rand() % (L-2) + 1;
		int xe = rand() % (L-2) + 1;
		int ye = rand() % (L-2) + 1;
		mg[xi][yi] = 0;//起点和终点置为可走
		mg[xe][ye] = 0;
		S_Button = Crt(yi * 40 + 340, xi * 40 + 10, 40, 40, "始");
		S_Button->color = GREEN;
		E_Button = Crt(ye * 40 + 340, xe * 40 + 10, 40, 40, "终");
		E_Button->color = GREEN;
		DrawMg(mg,L,W);
		while (1)
		{
			ExMessage m;//鼠标消息
			peekmessage(&m, EM_MOUSE);//监听鼠标消息
			for (int j = 50; j <= 50+40*(L-3); j+=40)
			{
				for (int i = 380; i <= 380+40*(L-3); i+=40)
				{
					if (m.x > i && m.x < i + 40 && m.y > j && m.y < j + 40 && m.message == WM_LBUTTONDOWN)
					{
						if (mg[(j - 10) / 40][(i - 340) / 40] == 0)
						{
							mg[(j - 10) / 40][(i - 340) / 40] = 1;
							DrawMg(mg,L,W);
							Sleep(150);
						}
						else if(mg[(j - 10) / 40][(i - 340) / 40] == 1)
						{
							mg[(j - 10) / 40][(i - 340) / 40] = 0;
							DrawMg(mg,L,W);
							Sleep(150);
						}
					}
						
				}
			}
			//深度遍历
			if (m.x > DFS_Button->x && m.x<DFS_Button->x + DFS_Button->w &&
				m.y > DFS_Button->y && m.y < DFS_Button->y + DFS_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				bool res = mgpath(xi, yi, xe, ye);	
				if (res == true)
				{
					const char* text1 = "成功";
					settextstyle(35, 0, "楷体");
					settextcolor(BLACK);
					outtextxy(570, 200, text1);
				}
				else
				{
					const char* text2 = "失败";
					settextstyle(35, 0, "楷体");
					settextcolor(BLACK);
					outtextxy(570, 200, text2);
				}
			}
			//广度遍历
			if (m.x > BFS_Button->x && m.x<BFS_Button->x + BFS_Button->w &&
				m.y > BFS_Button->y && m.y < BFS_Button->y + BFS_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				bool res2 = mgpath2(xi, yi, xe, ye);
				if (res2 == true)
				{
					const char* text1 = "成功";
					settextstyle(35, 0, "楷体");
					settextcolor(BLACK);
					outtextxy(570, 200, text1);
					Sleep(150);
				}
				else
				{
					const char* text2 = "失败";
					settextstyle(35, 0, "楷体");
					settextcolor(BLACK);
					outtextxy(570, 200, text2);
					Sleep(150);
				}
			}
			//继续游戏
			if (m.x > Continue_Button->x && m.x<Continue_Button->x + Continue_Button->w &&
				m.y > Continue_Button->y && m.y < Continue_Button->y + Continue_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				break;
			}
			//退出游戏
			if (m.x > Quit_Button->x && m.x<Quit_Button->x + Quit_Button->w &&
				m.y > Quit_Button->y && m.y < Quit_Button->y + Quit_Button->h &&
				m.message == WM_LBUTTONDOWN)
				return 1;
			//迷宫大小
			if (m.x > LW_Button->x && m.x<LW_Button->x + LW_Button->w &&
				m.y > LW_Button->y && m.y < LW_Button->y + LW_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				char s[5];
				char s1[5];
				InputBox(s, 10, "输入迷宫的长");
				InputBox(s1, 10, "输入迷宫的宽");
				L = atoi(s);
				W = atoi(s1);
				cleardevice();
				break;
			}
		}
	}
	/*closegraph();*/
	return 0;
}

void CrtMg(int mg[][25],int L,int W)
{
	for (int i = 0; i < W; i++)
	{
		mg[i][0] = 1;
		mg[i][W-1] = 1;
		mg[0][i] = 1;
		mg[W-1][i] = 1;
	}
	int num[] = { 2,4,6,8,10,12,6,1,1};//随机生成障碍物
	srand((unsigned)time(0));
	for (int i = 1; i < W-1; i++)
	{
		for (int j = 1; j < W-1; j++)
		{
			int index = num[rand() % 10];
			mg[i][j] = index % 2;
		}
	}
}

bool mgpath(int xi, int yi, int xe, int ye)//深度遍历
{
	Box path[MaxSize], e;
	int i, j, di, i1, j1, k;
	bool find;
	SqStack* st;//定义栈并初始化
	InitStack(st);
	e.i = xi;//设置e为入口
	e.j = yi;
	e.di = -1;//表示还未探测任何方位
	PushStack(st, e);//起点方块e进栈
	mg[xi][yi] = -1;//将入口的迷宫值置为-1，避免重复走到该路径
	while (!StackEmpty(st))
	{
		GetTop(st, e);//取栈顶方块e
		i = e.i;
		j = e.j;
		di = e.di;
		if (i == xe && j == ye)//当方块e是出口时
		{
			DestoryStack(st);
			return true;//表示找到一条可走路进
		}
		find = false;
		while (di < 4 && !find)//当方块e不是终点时，找方块e的下一个相邻方块
		{
			di++;
			switch (di)
			{
			case 0:
				i1 = i - 1;
				j1 = j;
				break;
			case 1:
				i1 = i;
				j1 = j + 1;
				break;
			case 2:
				i1 = i + 1;
				j1 = j;
				break;
			case 3:
				i1 = i;
				j1 = j - 1;
				break;
			}
			if (mg[i1][j1] == 0)//若相邻方块可走
			{
				find = true;
			}
		}
		if (find)
		{
			st->data[st->top].di = di;//修改原栈顶元素的di值
			e.i = i1;//将e设置为相邻可走方块
			e.j = j1;
			e.di = -1;
			PushStack(st, e);//e进栈
			mg[i1][j1] = -1;//将（i1，j1）迷宫值置为-1，避免重复走到该方块
			DrawMg(mg, L, W);
			Sleep(150);//延时程序，达到动画效果
		}
		else//没有相邻方向可走，退栈
		{
			PopStack(st, e);
			mg[e.i][e.j] = -2;//让退栈方块位置不可走
			DrawMg(mg,L,W);
			Sleep(150);
		}
	}
	DestoryStack(st);//栈已经空了，也没有找到终点
	return false;//表示没有可走路径
}

bool mgpath2(int xi, int yi, int xe, int ye)//广度优先遍历
{
	Box1 e;
	int i, j, di, i1, j1;
	QuType* qu;//定义顺序队指针并初始化
	InitQueue(qu);
	e.i = xi, e.j = yi, e.pre = -1;//将起点的pre置为-1并进队
	enQueue(qu, e);
	mg[xi][yi] = -1;
	while (!QueueEmpty(qu))
	{
		deQueue(qu, e);//出队一个方块e，他在队列中的位置是front
		DrawMg(mg, L, W);
		Sleep(150);
		i = e.i;
		j = e.j;
		if (i == xe && j == ye)//当出队方块e是终点时
		{
			for (int i = 0; i < L; i++)//先把所有走过的迷宫方块的值置为0
			{
				for (int j = 0; j < L; j++)
				{
					if (mg[i][j] == -1)
						mg[i][j] = 0;
				}
			}
			int k = qu->front;//根据当前方块的pre值回推找到迷宫路径
			do
			{
				j = k;
				k = qu->data[k].pre;
				qu->data[j].pre = -1;//并将迷宫路径的pre值置为-1
			} while (k != 0);
			while (k < MaxSize)//遍历顺序队的所有元素
			{
				if (qu->data[k].pre == -1)
				{
					mg[qu->data[k].i][qu->data[k].j] = -1;//将对应的迷宫的值置为-1
				}
				k++;
			}
			DrawMg(mg,L,W);
			DestroyQueue(qu);
			return true;//找到迷宫路径
		}
		for (di = 0; di < 4; di++)//当出队方块e不是终点时
		{
			switch (di)//对于方块e的所有相邻方块，上下左右方向
			{
			case 0:i1 = i - 1; j1 = j;
				break;
			case 1:i1 = i; j1 = j + 1;
				break;
			case 2:i1 = i + 1; j1 = j;
				break;
			case 3:i1 = i; j1 = j - 1;
				break;
			}
			if (mg[i1][j1] == 0)//如果相邻方块（i1，j1）可走
			{
				e.i = i1;
				e.j = j1;
				e.pre = qu->front;//将方块的pre指向路径中上一个方块的下标
				enQueue(qu, e);//（i1，j1）方块进队
				mg[i1][j1] = -1;//将方块e1的迷宫数组值设置为-1
				DrawMg(mg, L, W);
				Sleep(150);
			}
		}
	}
	DestroyQueue(qu);
	return false;
}

