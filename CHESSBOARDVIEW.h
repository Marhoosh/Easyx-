#ifndef CHESSBOARDVIEW
#define CHESSBOARDVIEW

#include <graphics.h>

//����Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

const int borad_size = 15;

int borad[borad_size][borad_size]={0};//0��ʾ�ô�û������ 1��ʾ���� 2��ʾ����

int lastx = -1, lasty = -1;//�ҷ���һ�غ�����λ��

//ͳһλ�����Ϸ��������֣�ִ�ڣ����·����˺��֣�ִ��

//���ص�ǰ�ͻ�����Ҫ�������ɫ
int myname_user_number() {
	if (strcmp(myname, user_1.username) == 0) {
		return 2;
	}
	else if(strcmp(myname, user_2.username) == 0) {
		return 1;
	}
	else {
		puts("�û�����Ϣ����");
		exit(0);
	}
}

void chessboard_view() {
	//���� 540 * 540
	
	peekmessage(&m, EM_MOUSE, false);
	//��������


	putimage(247, 27, &chessboard[init_chessborad+1]);
	int basx = 247, basy = 27;
	
	//��Ϸ��ʼ
	//printf("--%d %d\n", gamestart_statu, myround);
	if (gamestart_statu) {


		//׷����ʾ����
		//ö�� 15 * 15�ĸ�����Ϣ 
		//int cnt = 0;

		//�����ǰ���ҵĻغ�
		if (myround) {
			int flag = 1;
			for (int i = 0; i < 15; ++i) {//ö����
				if (!flag)break;
				for (int j = 0; j < 15; ++j) {//ö����
					if (!flag)break;
					int leftup_y = basy + i * 36;
					int leftup_x = basx + j * 36;
					//׷�ٿո���
					if (myround && m.x >= leftup_x && m.x < leftup_x + 36 && m.y >= leftup_y && m.y < leftup_y + 36 && borad[i][j] == 0) {
						putimage(leftup_x, leftup_y, &chessboard_tracet_view[1], 0x008800C6);
						putimage(leftup_x, leftup_y, &chessboard_tracet_view[2], 0x00EE0086);

						//���ӵ���¼�
						if (m.message == WM_LBUTTONDOWN && borad[i][j] == 0) {
							
							mciSendString(L"close chessmusic", NULL, 0, NULL);
							mciSendString(L"open 02.mp3 alias chessmusic", NULL, 0, NULL);
							mciSendString(L"play chessmusic", NULL, 0, NULL);

							lastx = i, lasty = j;//������һ��������Ϣ
							//PlaySound(L"01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
							borad[i][j] = myname_user_number();
							myround = 0;//���� , ���غϽ���
							sprintf(sendbuffer, "_move _clientsend %d %d ",i,j);//����_move client x y
							send(fd, sendbuffer, strlen(sendbuffer), 0);
							flag = 0;
						}
						
					}
				}
			}
		}


		//���̾��ƻ滭
		for (int i = 0; i < 15; ++i) {//ö����
			for (int j = 0; j < 15; ++j) {//ö����
				int leftup_y = basy + i * 36;
				int leftup_x = basx + j * 36;

				if (borad[i][j] == 1) {
					putimage(leftup_x, leftup_y, &chessboard_piece_white[1], 0x008800C6);
					putimage(leftup_x, leftup_y, &chessboard_piece_white[2], 0x00EE0086);
				}
				if (borad[i][j] == 2) {
					putimage(leftup_x, leftup_y, &chessboard_piece_black[1], 0x008800C6);
					putimage(leftup_x, leftup_y, &chessboard_piece_black[2], 0x00EE0086);
				}
			}
		}
	}
	

}


#endif