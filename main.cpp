#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <conio.h>

#pragma warning(disable : 4996)

using namespace std;

#define LEFT 120
#define RIGHT 122
#define HARDDROP 32
#define HOLD 99


#define color(a) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), a)

void gotoxy(int x, int y) {

	COORD pos = { (2 * x),y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gameset();
void choose_block();
void draw_map();
void push_down();

int map[11][23];
int block[11][23];
int x,y,s; //x = 좌우 y는 높이
int next_block;
int block_now;


int shape[7][4][4][4] = {
	{
		{
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		}
	},
	{
		{
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},
		{
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,1,1},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,0,1,1},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,1},
			{0,0,0,1},
			{0,0,0,0}
		}
	},
	{
		{
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{0,0,0,0}
		}
	}
};

int gettime;
int checktime;
int delay;

int main() {
	srand(time(NULL));
	CONSOLE_CURSOR_INFO Curinfo;
	Curinfo.dwSize = 1;
	Curinfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Curinfo);
	int key = 0;
	next_block = rand() % 6;
	gettime = clock();
	delay = 2000;
	gameset();
	while (1) {
		checktime = clock();
		if (checktime - gettime > delay) push_down();
		if (kbhit()) key = getch();
		switch (key) { // 움직임
		case LEFT:
		case RIGHT:
		case HARDDROP:
		case HOLD:

			break;
		default: //움직이는 것이 아닐경우 키 초기화
			key = 0;
		}
		draw_map();
	}
}

void gameset() {
	gotoxy(0, 0);
	for (int i = 0; i <= 16; i++) printf("□");
	for (int i = 1; i < 5; i++) {
		gotoxy(0, i);
		printf("□");
		gotoxy(5, i);
		printf("□");
		gotoxy(16, i);
		printf("□");
	}
	gotoxy(0, 5);
	printf("□ⓖⓡⓐⓟ");
	for (int i = 5; i <= 20; i++) {
		gotoxy(5, i);
		printf("□");
		gotoxy(16, i);
		printf("□");
	}
	for (int i = 5; i <= 16; i++) {
		gotoxy(i, 21);
		printf("□");
	}
	choose_block();
}

void choose_block() {
	x = 3;
	y = 0;
	s = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[next_block][0][i][j] == 1) {
				map[x + i][y + j] = next_block + 1;
			}
		}
	}
	block_now = next_block;
	next_block = rand() % 6;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[next_block][0][i][j] == 1) {
				gotoxy(i + 1, j + 1);
				color(next_block + 1); printf("■");
				color(7);
			}
		}
	}
	gotoxy(30, 0);
	printf("%d", block_now);
	gotoxy(30, 1);
	printf("%d", next_block);
}

void draw_map() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			gotoxy(i + 6, j + 1);
			if (map[i][j] > 1) {
				color(map[i][j]); printf("■");
				color(7);
			}
			else printf("  ");
		}
	}
}

void push_down() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[block_now][s][i][j] == 1) {
				map[x + i][y + j] = 0;
			}
		}
	}
	y++;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[block_now][s][i][j] == 1) {
				map[x + i][y + j] = block_now;
			}
		}
	}
	gettime = clock();
}
