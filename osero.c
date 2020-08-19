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

enum Piece {	PLAYER_W = -1,
				PLAYER_B = 1,
				CHOSEN_W = -2,
				CHOSEN_B = 2,
				NOTHING = 5,
				CANTURN = 3,
				CHOSEN_CANT = 6,
				CHOSEN_NO = 10
			};

void show(const int[8][8], int);
void get_cursor(int[8][8], int*, int*, int*, int*);
void piece_flip(int[8][8], int ,int, int*, int*);
_Bool* piece_check(int[8][8], int ,int, int*, int*);

int main(){
	enum Piece piece[8][8];
	int player_x = 0;
	int player_y = 0;
	int turn = PLAYER_B;
	int no_black = 0, no_white = 0;
	_Bool* IsThereCanTurn;

	for(int i=0; i<8; i++){
		for(int r=0; r<8; r++){
			piece[i][r] = NOTHING;
		}
	}
	piece[3][3] = PLAYER_W;
	piece[3][4] = PLAYER_B;
	piece[4][3] = PLAYER_B;
	piece[4][4] = PLAYER_W;

	for(int count=0; count<75;){
		for(int i=0; i<8; i++){
			for(int r=0; r<8; r++)
				IsThereCanTurn = piece_check(piece, i, r, &turn, &count);
		}
		piece[player_y][player_x] *= 2;
		show(piece, turn);
		piece[player_y][player_x] /= 2;
		if(*IsThereCanTurn == FALSE){
			count++;
			turn*=-1;
			continue;
		}
		printf("現在のターン：");
			if(turn == PLAYER_B)
		printf("黒\n");
			if(turn == PLAYER_W)
		printf("白\n");

		get_cursor(piece, &player_y, &player_x, &turn, &count);
		*IsThereCanTurn = FALSE;
	}
	for(int i=0; i<8; i++){
		for(int r=0; r<8; r++){
			if(piece[i][r] == PLAYER_B)
				no_black++;
			else if(piece[i][r] == PLAYER_W)
				no_white++;
		}
	}
	show(piece, turn);
	printf( "\n"
			"黒：%d\t白：%d\n"
			"で", no_black, no_white);
	if(no_black > no_white)
		printf("黒の勝ち!!\n");
	if(no_black == no_white)
		printf("引き分け！？\n");
	if(no_black < no_white)
		printf("白の勝ち!!\n");
	getch();
}

void show(const int piece_array[8][8], int turn){
	system("cls");
	printf("The オセロ！\n\n");
	printf("＋");
	for(int i=0; i<8; i++){
		printf("--");
	}
	printf("＋\n");

	for(int i=0; i<8; i++){
		printf("｜");
		for(int r=0; r<8; r++){
			switch(piece_array[i][r]){
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
					printf("　");
					break;
				case CANTURN:
					printf("・");
					break;
				case CHOSEN_CANT:
					printf("◎");
					break;
			}
		}
		printf("｜\n");
	}

	printf("＋");
	for(int i=0; i<8; i++){
		printf("--");
	}
	printf("＋\n\n");
}

void get_cursor(int piece_array[8][8], int *player_y, int *player_x, int *turn, int *count){
	CONTINUE:
	switch(getch()){
		case KB_UP:
			if(0<*player_y)	*player_y-=1;
			break;
		case KB_LEFT:
			if(0<*player_x)	*player_x-=1;
			break;
		case KB_RIGHT:
			if(*player_x<7)	*player_x+=1;
			break;
		case KB_DOWN:
			if(*player_y<7)	*player_y+=1;
			break;
		case KB_ENTER:
			piece_flip(piece_array, *player_y, *player_x, turn, count);
			return;
		case EXIT:
			printf("\n緊急終了！！\n");
			exit(0);
		default: goto CONTINUE;
	}
}

_Bool* piece_check(int array[8][8], int point_y, int point_x, int *turn, int *count){
	//WANT もっと短く！
	static _Bool IsThereCanTurn = FALSE;

	if(array[point_y][point_x] == CANTURN)
		array[point_y][point_x] = NOTHING;

	if(1 < point_y && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y-i][point_x] == -*turn; i++){	//上
			if(array[point_y-i-1][point_x] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(point_y < 6 && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y+i][point_x] == -*turn; i++){		//下
			if(array[point_y+i+1][point_x] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(1 < point_x && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y][point_x-i] == -*turn; i++){	//左
			if(array[point_y][point_x-i-1] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(point_x < 6 && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y][point_x+i] == -*turn; i++){		//右
			if(array[point_y][point_x+i+1] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(1 < point_y && 1 < point_x && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y-i][point_x-i] == -*turn; i++){		//左上
			if(array[point_y-i-1][point_x-i-1] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(1 < point_y && point_x < 6 && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y-i][point_x+i] == -*turn; i++){		//右上
			if(array[point_y-i-1][point_x+i+1] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(point_y < 6 && 1 < point_x && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y+i][point_x-i] == -*turn; i++){		//左下
			if(array[point_y+i+1][point_x-i-1] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}

	if(point_y < 6 && point_x < 6 && array[point_y][point_x] == NOTHING){
		for(int i=1; array[point_y+i][point_x+i] == -*turn; i++){		//右下
			if(array[point_y+i+1][point_x+i+1] == *turn){
				array[point_y][point_x] = CANTURN;
				IsThereCanTurn = TRUE;
				break;
			}
		}
	}
	return &IsThereCanTurn;
}

void piece_flip(int array[8][8], int point_y, int point_x, int *turn, int *count){
	int tmp;
	_Bool WasTurn = FALSE;
	//WANT もっと短く！

	tmp = 0;
	if(1 < point_y && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y-i][point_x] == -*turn; i++){	//上
			if(array[point_y-i-1][point_x] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y-i][point_x] *= -1;
		}
	}

	tmp = 0;
	if(point_y < 6 && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y+i][point_x] == -*turn; i++){		//下
			if(array[point_y+i+1][point_x] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y+i][point_x] *= -1;
		}
	}

	tmp = 0;
	if(1 < point_x && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y][point_x-i] == -*turn && 0<point_x-i; i++){	//左
			if(array[point_y][point_x-i-1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y][point_x-i] *= -1;
		}
	}

	tmp = 0;
	if(point_x < 6 && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y][point_x+i] == -*turn && point_x+i<7; i++){	//右
			if(array[point_y][point_x+i+1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y][point_x+i] *= -1;
		}
	}

	tmp = 0;
	if(1 < point_y && 1 < point_x && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y-i][point_x-i] == -*turn && 0<point_x-i; i++){//左上
			if(array[point_y-i-1][point_x-i-1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y-i][point_x-i] *= -1;
		}
	}

	tmp = 0;
	if(1 < point_y && point_x < 6 && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y-i][point_x+i] == -*turn && point_x+i<7; i++){//右上
			if(array[point_y-i-1][point_x+i+1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y-i][point_x+i] *= -1;
		}
	}

	tmp = 0;
	if(point_y < 6 && 1 < point_x && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y+i][point_x-i] == -*turn && 0<point_x-i; i++){//左下
			if(array[point_y+i+1][point_x-i-1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y+i][point_x-i] *= -1;
		}
	}

	tmp = 0;
	if(point_y < 6 && point_x < 6 && array[point_y][point_x] == CANTURN){
		for(int i=1; array[point_y+i][point_x+i] == -*turn && point_x+i<7; i++){//右下
			if(array[point_y+i+1][point_x+i+1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++){
			if(WasTurn == FALSE)
				WasTurn = TRUE;
			array[point_y+i][point_x+i] *= -1;
		}
	}

	if(WasTurn){
		array[point_y][point_x] = *turn;
		*turn *= -1;
		*count+=1;
	}
}

