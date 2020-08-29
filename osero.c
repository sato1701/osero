#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

#define KB_UP 0x0048	//キーボード入力判定用
#define KB_DOWN 0x0050
#define KB_LEFT 0x004b
#define KB_RIGHT 0x004d
#define KB_ENTER 0x000D
#define EXIT 0x001b
#define OSERO_FIELD 10

enum Piece {	//オセロの駒の種類
	PLAYER_W = -1,
	PLAYER_B = 1,
	CHOSEN_W = -2,
	CHOSEN_B = 2,
	NOTHING = 5,
	CANTURN = 3,
	CHOSEN_CAN = 6,
	CHOSEN_NO = 10
} piece[OSERO_FIELD][OSERO_FIELD];

void show(int);
void get_cursor(int*, int*, int*, int*);
void piece_flip(int ,int, int*);
void piece_check(int ,int, int*);
void piece_check_2(int, int, int*, _Bool);

int main(){
	int player_x = 1;
	int player_y = 1;
	int turn = PLAYER_B;
	int no_black = 0, no_white = 0;

	for(int i=1; i<OSERO_FIELD-1; i++){
		for(int r=1; r<OSERO_FIELD-1; r++){
			piece[i][r] = NOTHING;
		}
	}
	piece[4][4] = PLAYER_W;
	piece[4][5] = PLAYER_B;
	piece[5][4] = PLAYER_B;
	piece[5][5] = PLAYER_W;
	for(int i=1; i<OSERO_FIELD-2; i++){
		for(int r=1; r<OSERO_FIELD-2; r++)
			piece_check_2(i, r, &turn, FALSE);
	}

	for(int count=0; count<200;){
		piece[player_y][player_x] *= 2;	//playerの位置の駒をCHOSENに

		show(turn);
		printf("現在のターン：");
			if(turn == PLAYER_B)
		printf("黒\n");
			if(turn == PLAYER_W)
		printf("白\n");

		printf("%d", count);
		piece[player_y][player_x] /= 2;	//playerの駒をもとに

		get_cursor(&player_y, &player_x, &turn, &count);
	}
	for(int i=1; i<OSERO_FIELD-2; i++){
		for(int r=1; r<-1; r++){
			if(piece[i][r] == PLAYER_B)
				no_black++;
			if(piece[i][r] == PLAYER_W)
				no_white++;
		}
	}
	show(turn);
	printf( "\n" "黒：%2d\t白：%2dで", no_black, no_white);
	if(no_black > no_white)
		printf("黒の勝ち!!\n");
	if(no_black == no_white)
		printf("引き分け！？\n");
	if(no_black < no_white)
		printf("白の勝ち!!\n");
	getch();
}

void show(int turn){
	system("cls");
	printf("The オセロ！\n\n");
	printf("＋");
	for(int i=0; i<8; i++)
		printf("--");
	printf("＋\n");

	for(int i=1; i<=OSERO_FIELD-2; i++){
		printf("｜");
		for(int r=1; r<=OSERO_FIELD-2; r++){
			switch(piece[i][r]){
				case CHOSEN_W:
					printf("★");
					break;
				case CHOSEN_B:
					printf("☆");
					break;
				case CHOSEN_NO:
					printf("＊");
					break;
				case PLAYER_W:
					printf("●");
					break;
				case PLAYER_B:
					printf("○");
					break;
				case NOTHING:
					printf("  ");
					break;
				case CANTURN:
					printf("・");
					break;
				case CHOSEN_CAN:
					printf("◎");
					break;
			}
		}
		printf("｜\n");
	}

	printf("＋");
	for(int i=0; i<8; i++)
		printf("--");
	printf("＋\n\n");
}

void get_cursor(int *player_y, int *player_x, int *turn, int *count){
	CONTINUE:
	switch(getch()){
		case KB_UP:
			if(1<*player_y)	*player_y-=1;
			break;
		case KB_LEFT:
			if(1<*player_x)	*player_x-=1;
			break;
		case KB_RIGHT:
			if(*player_x<OSERO_FIELD-2)	*player_x+=1;
			break;
		case KB_DOWN:
			if(*player_y<OSERO_FIELD-2)	*player_y+=1;
			break;
		case KB_ENTER:
			if(piece[*player_y][*player_x] == CANTURN){
				piece_check_2(*player_y, *player_x, turn, TRUE);
				for(int i=1; i<=OSERO_FIELD-2; i++){
					for(int r=1; r<=OSERO_FIELD-2; r++)
						piece_check_2(i, r, turn, FALSE);
				}
				*count+=1;
			}
			break;
		case EXIT:
			printf("\n緊急終了！！\n");
			exit(0);
		default: goto CONTINUE;
	}
}

void piece_check_2(int point_y, int point_x, int *turn, _Bool IsFlip){
	int tmp;
	int dir_x[] = {0, 0, -1, 1, -1, 1, -1, 1};
	int dir_y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

	if(IsFlip == FALSE){
		if(piece[point_y][point_x] == CANTURN)
			piece[point_y][point_x] = NOTHING;
	}
	if(piece[point_y][point_x] == NOTHING || IsFlip == TRUE){
		for(int k=0; k<8; k++){
			int i=dir_y[k];
			int r=dir_x[k];
			tmp = 0;
			while(piece[point_y+i][point_x+r] == -*turn){	//上
				if(piece[point_y+(i+dir_y[k])][point_x+(r+dir_x[k])] == *turn){
					if(IsFlip == TRUE){
						if(i != 0)
							tmp = abs(i);
						else
							tmp = abs(r);
					}else
						piece[point_y][point_x] = CANTURN;
					break;
				}
				i+=dir_y[k];
				r+=dir_x[k];
			}

			if(IsFlip == TRUE){
				i=dir_y[k];
				r=dir_x[k];
				while(abs(i)<=tmp && abs(r)<=tmp){
					piece[point_y+i][point_x+r] *= -1;
					i+=dir_y[k];
					r+=dir_x[k];
				}
			}
		}
	}
	if(IsFlip == TRUE){
		piece[point_y][point_x] = *turn;
		*turn *= -1;
	}
}

