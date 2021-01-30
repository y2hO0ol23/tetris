#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <conio.h>

#pragma warning(disable : 4996)

using namespace std;

#define LEFT 'a'
#define RIGHT 'd'
#define HARDDROP 's'
#define HOLD 'w'


#define color(a) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), a)

void gotoxy(int x, int y) {

	COORD pos = { (2 * x),y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gameset();
void choose_block();
void draw_map();
void push_down();
void erase(int line);

void keyget(int key);

int map[11][23];
int block[11][23];
int block_count[23];
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
int fix_delay;

int main() {
	srand(time(NULL));
	CONSOLE_CURSOR_INFO Curinfo;
	Curinfo.dwSize = 1;
	Curinfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Curinfo);
	int key = 0;
	next_block = rand() % 6;
	gettime = clock();
	delay = 1000;
	fix_delay = 0;
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
			keyget(key);
			key = 0;
		}
		draw_map();
	}
}

void keyget(int key) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[block_now][s][i][j] == 1) {
				map[x + i][y + j] = 0;
			}
		}
	}
	if (key == LEFT) {
		x--;
		int flag = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[block_now][s][i][j] == 1) {
					if (block[x + i][y + j] || x + i < 0) flag = 1;
				}
			}
		}
		if (flag == 1) x++;
	}
	if (key == RIGHT) {
		x++;
		int flag = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[block_now][s][i][j] == 1) {
					if (block[x + i][y + j] || x + i >= 10) flag = 1;
				}
			}
		}
		if (flag == 1) x--;
	}
	if (key == HARDDROP) {
		int flag = 0;
		y++;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[block_now][s][i][j] == 1) {
					if (block[x + i][y + j] || y + j >= 20) flag = 1;
				}
			}
		}
		if (flag == 1) y--;
		if (flag == 0) gettime = clock();
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[block_now][s][i][j] == 1) {
				map[x + i][y + j] = block_now + 2;
			}
		}
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
				map[x + i][y + j] = next_block + 2;
			}
		}
	}
	block_now = next_block;
	next_block = rand() % 6;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[next_block][0][i][j] == 1) {
				gotoxy(i + 1, j + 1);
				color(next_block + 2); printf("■");
				color(7);
			}
			else {
				gotoxy(i + 1, j + 1);
				printf("  ");
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
			int val = map[i][j];
			if (!val) val = block[i][j];
			if (val > 0) {
				color(val); printf("■");
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
	int flag = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (shape[block_now][s][i][j] == 1) {
				if (block[x + i][y + j] || y + j >= 20) flag = 1;
				map[x + i][y + j] = block_now + 2;
			}
		}
	}
	if (flag == 0) {
		fix_delay = clock();
		gettime = clock();
	}
	if (flag == 1) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[block_now][s][i][j] == 1) {
					map[x + i][y + j] = 0;
				}
			}
		}
		y--;
		int flag = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[block_now][s][i][j] == 1) {
					if (block[x + i][y + j]) flag = 1;
					map[x + i][y + j] = block_now + 2;        
				}
			}
		}
		if (checktime - fix_delay > delay) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (shape[block_now][s][j][i] == 1) {
						block[x + j][y + i] = block_now + 2;
						block_count[y + i]++;
					}
				}
				if (block_count[y + i] == 10) {
					erase(y + i);
					draw_map();
				}
				gotoxy(30, y + i);
				printf("%2d", block_count[y + i]);
			}
			gettime = clock();
			choose_block();
		}
	}
}

void erase(int line) {
	for (int i = line; i > 0; i--) {
		for (int j = 0; j < 10; j++) {
			block[j][i] = block[j][i - 1];
		}
		block_count[i] = block_count[i - 1];
	}
	for (int j = 0; j < 10; j++) {
		block[j][0] = 0;
	}
}
