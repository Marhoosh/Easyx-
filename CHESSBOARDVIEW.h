#ifndef CHESSBOARDVIEW
#define CHESSBOARDVIEW

#include <graphics.h>

//引用Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

const int borad_size = 15;

int borad[borad_size][borad_size]={0};//0表示该处没有棋子 1表示白棋 2表示黑棋

int lastx = -1, lasty = -1;//我方上一回合棋子位置

//统一位置在上方的人先手，执黑；在下方的人后手，执白

//返回当前客户端需要下棋的颜色
int myname_user_number() {
	if (strcmp(myname, user_1.username) == 0) {
		return 2;
	}
	else if(strcmp(myname, user_2.username) == 0) {
		return 1;
	}
	else {
		puts("用户名信息错误");
		exit(0);
	}
}

void chessboard_view() {
	//棋盘 540 * 540
	
	peekmessage(&m, EM_MOUSE, false);
	//绘制棋盘


	putimage(247, 27, &chessboard[init_chessborad+1]);
	int basx = 247, basy = 27;
	
	//游戏开始
	//printf("--%d %d\n", gamestart_statu, myround);
	if (gamestart_statu) {


		//追踪显示工作
		//枚举 15 * 15的格子信息 
		//int cnt = 0;

		//如果当前是我的回合
		if (myround) {
			int flag = 1;
			for (int i = 0; i < 15; ++i) {//枚举行
				if (!flag)break;
				for (int j = 0; j < 15; ++j) {//枚举列
					if (!flag)break;
					int leftup_y = basy + i * 36;
					int leftup_x = basx + j * 36;
					//追踪空格子
					if (myround && m.x >= leftup_x && m.x < leftup_x + 36 && m.y >= leftup_y && m.y < leftup_y + 36 && borad[i][j] == 0) {
						putimage(leftup_x, leftup_y, &chessboard_tracet_view[1], 0x008800C6);
						putimage(leftup_x, leftup_y, &chessboard_tracet_view[2], 0x00EE0086);

						//落子点击事件
						if (m.message == WM_LBUTTONDOWN && borad[i][j] == 0) {
							
							mciSendString(L"close chessmusic", NULL, 0, NULL);
							mciSendString(L"open 02.mp3 alias chessmusic", NULL, 0, NULL);
							mciSendString(L"play chessmusic", NULL, 0, NULL);

							lastx = i, lasty = j;//更新上一步棋子信息
							//PlaySound(L"01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
							borad[i][j] = myname_user_number();
							myround = 0;//落子 , 本回合结束
							sprintf(sendbuffer, "_move _clientsend %d %d ",i,j);//发送_move client x y
							send(fd, sendbuffer, strlen(sendbuffer), 0);
							flag = 0;
						}
						
					}
				}
			}
		}


		//棋盘局势绘画
		for (int i = 0; i < 15; ++i) {//枚举行
			for (int j = 0; j < 15; ++j) {//枚举列
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