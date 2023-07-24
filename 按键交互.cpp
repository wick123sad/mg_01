#include<iostream>
#include<stdlib.h>
#include<graphics.h>	//���easyx
#include"assert.h"		//���Դ���
#include<string>
#include<time.h>
#include"stack.h"
#include"queue.h"

int mg[25][25] = {0};
int L = 10, W = 10;//��ʼ�Թ��Ĵ�С

bool mgpath(int xi, int yi, int xe, int ye);
bool mgpath2(int xi, int yi, int xe, int ye);
void CrtMg(int mg[][25],int L,int W);

typedef struct Button
{
	int x, y, w, h;
	COLORREF color;
	char* text;
}BUTTON,*LPBUTTON;

LPBUTTON S_Button;//��㣬�յ�
LPBUTTON E_Button;

LPBUTTON Crt(int x, int y, int w, int h,const char* str)//��ʼ����ť
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

void DrawButton(LPBUTTON button)//����ť����
{
	setfillcolor(button->color);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3); //������ʽΪ��� 3 ���ص�ʵ�ߣ����Ӵ�Ϊб��
	setlinecolor(BLACK);
	fillrectangle(button->x, button->y, button->x + button->w, button->y + button->h);
	setbkmode(TRANSPARENT);//���ǵ����ֱ�������Ҳ��Ĭ����ɫ�����ܷ����ڵ������Խ����ֱ�����Ϊ͸��
	settextstyle(25, 0, "����");	
	settextcolor(BLACK);
	int xx, yy;//ʹ�ı�����
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
			
			if (mg[i][j] == 1)			//ǽ
				Mg_Button->color = RGB(181, 181, 181);
			if (mg[i][j] == 0)			//���߷���
				Mg_Button->color = WHITE;
			if (mg[i][j] == -2)			//��ջԪ�أ���ɫ�Ϳ��߷���һ��
				Mg_Button->color = YELLOW;
			if (mg[i][j] == -1)			//��ջԪ�ء����·��Ԫ��
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
	LPBUTTON DFS_Button = Crt(30, 30, 100, 40, "��ȱ���");
	LPBUTTON BFS_Button = Crt(30, 80, 100, 40, "��ȱ���");
	LPBUTTON Continue_Button = Crt(30, 130, 100, 40, "������Ϸ");
	LPBUTTON Quit_Button = Crt(30, 230, 100, 40, "�˳���Ϸ");
	LPBUTTON LW_Button = Crt(30, 180, 100, 40, "�Թ���С");
	
	while (1)
	{
		BeginBatchDraw();//˫����
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
		mg[xi][yi] = 0;//�����յ���Ϊ����
		mg[xe][ye] = 0;
		S_Button = Crt(yi * 40 + 340, xi * 40 + 10, 40, 40, "ʼ");
		S_Button->color = GREEN;
		E_Button = Crt(ye * 40 + 340, xe * 40 + 10, 40, 40, "��");
		E_Button->color = GREEN;
		DrawMg(mg,L,W);
		while (1)
		{
			ExMessage m;//�����Ϣ
			peekmessage(&m, EM_MOUSE);//���������Ϣ
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
			//��ȱ���
			if (m.x > DFS_Button->x && m.x<DFS_Button->x + DFS_Button->w &&
				m.y > DFS_Button->y && m.y < DFS_Button->y + DFS_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				bool res = mgpath(xi, yi, xe, ye);	
				if (res == true)
				{
					const char* text1 = "�ɹ�";
					settextstyle(35, 0, "����");
					settextcolor(BLACK);
					outtextxy(570, 200, text1);
				}
				else
				{
					const char* text2 = "ʧ��";
					settextstyle(35, 0, "����");
					settextcolor(BLACK);
					outtextxy(570, 200, text2);
				}
			}
			//��ȱ���
			if (m.x > BFS_Button->x && m.x<BFS_Button->x + BFS_Button->w &&
				m.y > BFS_Button->y && m.y < BFS_Button->y + BFS_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				bool res2 = mgpath2(xi, yi, xe, ye);
				if (res2 == true)
				{
					const char* text1 = "�ɹ�";
					settextstyle(35, 0, "����");
					settextcolor(BLACK);
					outtextxy(570, 200, text1);
					Sleep(150);
				}
				else
				{
					const char* text2 = "ʧ��";
					settextstyle(35, 0, "����");
					settextcolor(BLACK);
					outtextxy(570, 200, text2);
					Sleep(150);
				}
			}
			//������Ϸ
			if (m.x > Continue_Button->x && m.x<Continue_Button->x + Continue_Button->w &&
				m.y > Continue_Button->y && m.y < Continue_Button->y + Continue_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				break;
			}
			//�˳���Ϸ
			if (m.x > Quit_Button->x && m.x<Quit_Button->x + Quit_Button->w &&
				m.y > Quit_Button->y && m.y < Quit_Button->y + Quit_Button->h &&
				m.message == WM_LBUTTONDOWN)
				return 1;
			//�Թ���С
			if (m.x > LW_Button->x && m.x<LW_Button->x + LW_Button->w &&
				m.y > LW_Button->y && m.y < LW_Button->y + LW_Button->h &&
				m.message == WM_LBUTTONDOWN)
			{
				char s[5];
				char s1[5];
				InputBox(s, 10, "�����Թ��ĳ�");
				InputBox(s1, 10, "�����Թ��Ŀ�");
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
	int num[] = { 2,4,6,8,10,12,6,1,1};//��������ϰ���
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

bool mgpath(int xi, int yi, int xe, int ye)//��ȱ���
{
	Box path[MaxSize], e;
	int i, j, di, i1, j1, k;
	bool find;
	SqStack* st;//����ջ����ʼ��
	InitStack(st);
	e.i = xi;//����eΪ���
	e.j = yi;
	e.di = -1;//��ʾ��δ̽���κη�λ
	PushStack(st, e);//��㷽��e��ջ
	mg[xi][yi] = -1;//����ڵ��Թ�ֵ��Ϊ-1�������ظ��ߵ���·��
	while (!StackEmpty(st))
	{
		GetTop(st, e);//ȡջ������e
		i = e.i;
		j = e.j;
		di = e.di;
		if (i == xe && j == ye)//������e�ǳ���ʱ
		{
			DestoryStack(st);
			return true;//��ʾ�ҵ�һ������·��
		}
		find = false;
		while (di < 4 && !find)//������e�����յ�ʱ���ҷ���e����һ�����ڷ���
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
			if (mg[i1][j1] == 0)//�����ڷ������
			{
				find = true;
			}
		}
		if (find)
		{
			st->data[st->top].di = di;//�޸�ԭջ��Ԫ�ص�diֵ
			e.i = i1;//��e����Ϊ���ڿ��߷���
			e.j = j1;
			e.di = -1;
			PushStack(st, e);//e��ջ
			mg[i1][j1] = -1;//����i1��j1���Թ�ֵ��Ϊ-1�������ظ��ߵ��÷���
			DrawMg(mg, L, W);
			Sleep(150);//��ʱ���򣬴ﵽ����Ч��
		}
		else//û�����ڷ�����ߣ���ջ
		{
			PopStack(st, e);
			mg[e.i][e.j] = -2;//����ջ����λ�ò�����
			DrawMg(mg,L,W);
			Sleep(150);
		}
	}
	DestoryStack(st);//ջ�Ѿ����ˣ�Ҳû���ҵ��յ�
	return false;//��ʾû�п���·��
}

bool mgpath2(int xi, int yi, int xe, int ye)//������ȱ���
{
	Box1 e;
	int i, j, di, i1, j1;
	QuType* qu;//����˳���ָ�벢��ʼ��
	InitQueue(qu);
	e.i = xi, e.j = yi, e.pre = -1;//������pre��Ϊ-1������
	enQueue(qu, e);
	mg[xi][yi] = -1;
	while (!QueueEmpty(qu))
	{
		deQueue(qu, e);//����һ������e�����ڶ����е�λ����front
		DrawMg(mg, L, W);
		Sleep(150);
		i = e.i;
		j = e.j;
		if (i == xe && j == ye)//�����ӷ���e���յ�ʱ
		{
			for (int i = 0; i < L; i++)//�Ȱ������߹����Թ������ֵ��Ϊ0
			{
				for (int j = 0; j < L; j++)
				{
					if (mg[i][j] == -1)
						mg[i][j] = 0;
				}
			}
			int k = qu->front;//���ݵ�ǰ�����preֵ�����ҵ��Թ�·��
			do
			{
				j = k;
				k = qu->data[k].pre;
				qu->data[j].pre = -1;//�����Թ�·����preֵ��Ϊ-1
			} while (k != 0);
			while (k < MaxSize)//����˳��ӵ�����Ԫ��
			{
				if (qu->data[k].pre == -1)
				{
					mg[qu->data[k].i][qu->data[k].j] = -1;//����Ӧ���Թ���ֵ��Ϊ-1
				}
				k++;
			}
			DrawMg(mg,L,W);
			DestroyQueue(qu);
			return true;//�ҵ��Թ�·��
		}
		for (di = 0; di < 4; di++)//�����ӷ���e�����յ�ʱ
		{
			switch (di)//���ڷ���e���������ڷ��飬�������ҷ���
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
			if (mg[i1][j1] == 0)//������ڷ��飨i1��j1������
			{
				e.i = i1;
				e.j = j1;
				e.pre = qu->front;//�������preָ��·������һ��������±�
				enQueue(qu, e);//��i1��j1���������
				mg[i1][j1] = -1;//������e1���Թ�����ֵ����Ϊ-1
				DrawMg(mg, L, W);
				Sleep(150);
			}
		}
	}
	DestroyQueue(qu);
	return false;
}

