#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

#define KB_UP 0x0048	//�L�[�{�[�h���͔���p
#define KB_DOWN 0x0050
#define KB_LEFT 0x004b
#define KB_RIGHT 0x004d
#define KB_ENTER 0x000D
#define EXIT 0x001b

enum Piece {	//�I�Z���̋�̎��
	PLAYER_W = -1,
	PLAYER_B = 1,
	CHOSEN_W = -2,
	CHOSEN_B = 2,
	NOTHING = 5,
	CANTURN = 3,
	CHOSEN_CAN = 6,
	CHOSEN_NO = 10
} piece[8][8];

void show(int);
void get_cursor(int*, int*, int*, int*);
void piece_flip(int ,int, int*);
void piece_check(int ,int, int*);

int main(){
	int player_x = 0;
	int player_y = 0;
	int turn = PLAYER_B;
	int no_black = 0, no_white = 0;

	for(int i=0; i<8; i++){
		for(int r=0; r<8; r++){
			piece[i][r] = NOTHING;
		}
	}
	piece[3][3] = PLAYER_W;
	piece[3][4] = PLAYER_B;
	piece[4][3] = PLAYER_B;
	piece[4][4] = PLAYER_W;
	for(int i=0; i<8; i++){
		for(int r=0; r<8; r++)
			piece_check(i, r, &turn);
	}

	for(int count=0; count<200;){
		piece[player_y][player_x] *= 2;	//player�̈ʒu�̋��CHOSEN��
		show(turn);
		printf("%d", count);
		piece[player_y][player_x] /= 2;	//player�̋�����Ƃ�

		get_cursor(&player_y, &player_x, &turn, &count);
		/*if(*IsThereCanTurn == FALSE && IsFinalTurn == TRUE)
			count = 199;
		if(*IsThereCanTurn == FALSE){
			count++;
			turn*=-1;
			IsFinalTurn = TRUE;
			continue;
		}
		*IsThereCanTurn = FALSE;*/
	}
	for(int i=0; i<8; i++){
		for(int r=0; r<8; r++){
			if(piece[i][r] == PLAYER_B)
				no_black++;
			else if(piece[i][r] == PLAYER_W)
				no_white++;
		}
	}
	show(turn);
	printf( "\n" "���F%d\t���F%d\n" "��", no_black, no_white);
	if(no_black > no_white)
		printf("���̏���!!\n");
	if(no_black == no_white)
		printf("���������I�H\n");
	if(no_black < no_white)
		printf("���̏���!!\n");
	getch();
}

void show(int turn){
	system("cls");
	printf("The �I�Z���I\n\n");
	printf("�{");
	for(int i=0; i<8; i++)
		printf("--");
	printf("�{\n");

	for(int i=0; i<8; i++){
		printf("�b");
		for(int r=0; r<8; r++){
			switch(piece[i][r]){
				case CHOSEN_W:
					printf("��");
					break;
				case CHOSEN_B:
					printf("��");
					break;
				case CHOSEN_NO:
					printf("��");
					break;
				case PLAYER_W:
					printf("��");
					break;
				case PLAYER_B:
					printf("��");
					break;
				case NOTHING:
					printf("  ");
					break;
				case CANTURN:
					printf("�E");
					break;
				case CHOSEN_CAN:
					printf("��");
					break;
			}
		}
		printf("�b\n");
	}

	printf("�{");
	for(int i=0; i<8; i++)
		printf("--");
	printf("�{\n\n");

	printf("���݂̃^�[���F");
			if(turn == PLAYER_B)
		printf("��\n");
			if(turn == PLAYER_W)
		printf("��\n");
}

void get_cursor(int *player_y, int *player_x, int *turn, int *count){
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
			if(piece[*player_y][*player_x] == CANTURN)
				piece_flip(*player_y, *player_x, turn);
			for(int i=0; i<8; i++){
				for(int r=0; r<8; r++)
					piece_check(i, r, turn);
			}
			*count+=1;
			break;
		case EXIT:
			printf("\n�ً}�I���I�I\n");
			exit(0);
		default: goto CONTINUE;
	}
}

void piece_check(int point_y, int point_x, int *turn){
	//WANT �����ƒZ���I

	if(piece[point_y][point_x] == CANTURN)
		piece[point_y][point_x] = NOTHING;

	if(piece[point_y][point_x] == NOTHING){
		if(1 < point_y){
			for(int i=1; piece[point_y-i][point_x] == -*turn; i++){	//��
				if(piece[point_y-i-1][point_x] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(point_y < 6){
			for(int i=1; piece[point_y+i][point_x] == -*turn; i++){	//��
				if(piece[point_y+i+1][point_x] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(1 < point_x){
			for(int i=1; piece[point_y][point_x-i] == -*turn; i++){	//��
				if(piece[point_y][point_x-i-1] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(point_x < 6){
			for(int i=1; piece[point_y][point_x+i] == -*turn; i++){		//�E
				if(piece[point_y][point_x+i+1] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(1 < point_y && 1 < point_x){
			for(int i=1; piece[point_y-i][point_x-i] == -*turn; i++){		//����
				if(piece[point_y-i-1][point_x-i-1] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(1 < point_y && point_x < 6){
			for(int i=1; piece[point_y-i][point_x+i] == -*turn; i++){		//�E��
				if(piece[point_y-i-1][point_x+i+1] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(point_y < 6 && 1 < point_x){
			for(int i=1; piece[point_y+i][point_x-i] == -*turn; i++){		//����
				if(piece[point_y+i+1][point_x-i-1] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}

		if(point_y < 6 && point_x < 6){
			for(int i=1; piece[point_y+i][point_x+i] == -*turn; i++){		//�E��
				if(piece[point_y+i+1][point_x+i+1] == *turn){
					piece[point_y][point_x] = CANTURN;
					break;
				}
			}
		}
	}
}

void piece_flip(int point_y, int point_x, int *turn){
	int tmp;
	//WANT �����ƒZ���I

	tmp = 0;
	if(1 < point_y){
		for(int i=1; piece[point_y-i][point_x] == -*turn; i++){	//��
			if(piece[point_y-i-1][point_x] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y-i][point_x] *= -1;
	}

	tmp = 0;
	if(point_y < 6){
		for(int i=1; piece[point_y+i][point_x] == -*turn; i++){		//��
			if(piece[point_y+i+1][point_x] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y+i][point_x] *= -1;
	}

	tmp = 0;
	if(1 < point_x){
		for(int i=1; piece[point_y][point_x-i] == -*turn && 0<point_x-i; i++){	//��
			if(piece[point_y][point_x-i-1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y][point_x-i] *= -1;
	}

	tmp = 0;
	if(point_x < 6){
		for(int i=1; piece[point_y][point_x+i] == -*turn && point_x+i<7; i++){	//�E
			if(piece[point_y][point_x+i+1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y][point_x+i] *= -1;
	}

	tmp = 0;
	if(1 < point_y && 1 < point_x){
		for(int i=1; piece[point_y-i][point_x-i] == -*turn && 0<point_x-i; i++){//����
			if(piece[point_y-i-1][point_x-i-1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y-i][point_x-i] *= -1;
	}

	tmp = 0;
	if(1 < point_y && point_x < 6){
		for(int i=1; piece[point_y-i][point_x+i] == -*turn && point_x+i<7; i++){//�E��
			if(piece[point_y-i-1][point_x+i+1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y-i][point_x+i] *= -1;
	}

	tmp = 0;
	if(point_y < 6 && 1 < point_x){
		for(int i=1; piece[point_y+i][point_x-i] == -*turn && 0<point_x-i; i++){//����
			if(piece[point_y+i+1][point_x-i-1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y+i][point_x-i] *= -1;
	}

	tmp = 0;
	if(point_y < 6 && point_x < 6){
		for(int i=1; piece[point_y+i][point_x+i] == -*turn && point_x+i<7; i++){//�E��
			if(piece[point_y+i+1][point_x+i+1] == *turn){
				tmp = i;
				break;
			}
		}
		for(int i=1; i<=tmp; i++)
			piece[point_y+i][point_x+i] *= -1;
	}
	piece[point_y][point_x] = *turn;
	*turn *= -1;
}

