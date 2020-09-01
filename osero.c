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
#define OSERO_FIELD 10

enum Piece {	//�I�Z���̋�̎��
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
_Bool piece_check(int, int, int*, _Bool);
void piece_flip(int, int, int*);

int main(){
	int player_x = 1;
	int player_y = 1;
	int turn = PLAYER_B;
	int no_black = 0, no_white = 0;

	for(int y=1; y<OSERO_FIELD-1; y++){
		for(int x=1; x<OSERO_FIELD-1; x++){
			piece[y][x] = NOTHING;
		}
	}
	piece[4][4] = PLAYER_W;
	piece[4][5] = PLAYER_B;
	piece[5][4] = PLAYER_B;
	piece[5][5] = PLAYER_W;
	for(int y=1; y<OSERO_FIELD-2; y++){
		for(int x=1; x<OSERO_FIELD-2; x++)
			piece_check(y, x, &turn, FALSE);
	}

	for(int count=0; count<200;){
		piece[player_y][player_x] *= 2;	//player�̈ʒu�̋��CHOSEN��

		show(turn);
		printf("���݂̃^�[���F");
			if(turn == PLAYER_B)
		printf("��\n");
			if(turn == PLAYER_W)
		printf("��\n");

		piece[player_y][player_x] /= 2;	//player�̋�����Ƃ�

		get_cursor(&player_y, &player_x, &turn, &count);
	}
	for(int x=1; x<OSERO_FIELD-2; x++){
		for(int y=1; y<OSERO_FIELD-2; y++){
			if(piece[x][y] == PLAYER_B)
				no_black++;
			if(piece[x][y] == PLAYER_W)
				no_white++;
		}
	}
	show(turn);
	printf( "\n" "���F%2d\t���F%2d��", no_black, no_white);

	if(no_black >  no_white)
		printf("���̏���!!\n");

	if(no_black == no_white)
		printf("���������I�H\n");

	if(no_black <  no_white)
		printf("���̏���!!\n");

	printf("�Ȃɂ��L�[����͂��Ă��������B");
	getch();
}

void show(int turn){
	system("cls");
	printf("The �I�Z���I\n\n");
	printf("�{----------------�{\n");

	for(int y=1; y <= OSERO_FIELD-2; y++){
		printf("�b");
		for(int x=1; x <= OSERO_FIELD-2; x++){
			switch(piece[y][x]){
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

	printf("�{----------------�{\n\n");
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
				_Bool CheckPassFlag = TRUE;
				do{
					piece_flip(*player_y, *player_x, turn);

					for(int i=1; i<=OSERO_FIELD-2; i++){
						for(int r=1; r<=OSERO_FIELD-2; r++)
							CheckPassFlag = piece_check(i, r, turn, CheckPassFlag);
					}
					*count+=1;
					if(CheckPassFlag)	*turn *= -1;
				}while(CheckPassFlag && *count <= 199);
			}
			break;
		case EXIT:
			printf("\n�ً}�I���I�I\n");
			exit(0);
		default: goto CONTINUE;
	}
}

_Bool piece_check(int point_y, int point_x, int *turn, _Bool CheckPassFlag){
	int dir_x[] = {0, 0, -1, 1, -1, 1, -1, 1};	//�T�������i�W�����j
	int dir_y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

	if(piece[point_y][point_x] == CANTURN)
		piece[point_y][point_x] = NOTHING;	//CANTURN�̋���ꎞ�I�ɂ��ׂď���

	if(piece[point_y][point_x] == NOTHING){
		for(int dir_index=0; dir_index<8; dir_index++){	//dir_index...dir_x(y)�̕�������
			int moved_y = dir_y[dir_index];
			int moved_x = dir_x[dir_index];

			while(piece[point_y + moved_y][point_x + moved_x] == -*turn){	//�i�񂾐悪����̋������
				if(piece[point_y + (moved_y + dir_y[dir_index])]
						[point_x + (moved_x + dir_x[dir_index])] == *turn){	//���A���̂P�悪�����̋������
					piece[point_y][point_x] = CANTURN;
					CheckPassFlag = FALSE;
					break;
				}
			moved_y += dir_y[dir_index];
			moved_x += dir_x[dir_index];
			}
		}
	}
	return CheckPassFlag;
}

void piece_flip(int point_y, int point_x, int *turn){
	int tmp;
	int dir_x[] = {0, 0, -1, 1, -1, 1, -1, 1};	//�T�������i�W�����j
	int dir_y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

	for(int dir_num=0; dir_num<8; dir_num++){	//dir_num...dir_x,y�����֐i�ސ�
		int moved_y = dir_y[dir_num];
		int moved_x = dir_x[dir_num];
		tmp = 0;
		while(piece[point_y+moved_y][point_x+moved_x] == -*turn){	//�i�񂾐悪����̋�ł����
			if(piece[point_y + (moved_y + dir_y[dir_num])]			//���̂P�悪�����̋������
					[point_x + (moved_x + dir_x[dir_num])] == *turn){
				if(moved_y != 0)			//�����_�ł̑���̋�̐���tmp�ɑ��(move_x,y�̂ǂ��炩)
					tmp = abs(moved_y);		//abs�֐�...��Βl�����߂�
				else
					tmp = abs(moved_x);
				break;
			}
			moved_y += dir_y[dir_num];
			moved_x += dir_x[dir_num];
		}

		moved_y = dir_y[dir_num];
		moved_x = dir_x[dir_num];
		while(abs(moved_y) <= tmp && abs(moved_x) <= tmp){	//tmp�̐������A����̋�𔽓]
			piece[point_y+moved_y][point_x+moved_x] *= -1;
			moved_y += dir_y[dir_num];
			moved_x += dir_x[dir_num];
		}
	}
	piece[point_y][point_x] = *turn;	//�R�}��u�����ꏊ�̔��]
	*turn *= -1;		//�����̌��
}

