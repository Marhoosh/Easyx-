#pragma warning(disable:6031)
#pragma warning(disable:6054)
#pragma warning(disable:6385)
#pragma warning( disable : 4996)

#include<string.h>
#include<stdio.h>
#include<conio.h>
#include "NETWORK.h"
#include<graphics.h>
#include<math.h>
#include "INIT_IMAGE.h"
#include "EASYTEXTBOX.h"
#include "EASYBUTTON.h"
#include "USERMESSAGE.h"
#include<easyx.h>
#include<random>
#include<time.h>
#include<mmsystem.h>

//#define _stprintf
//#define _CRT_SECURE_NO_WARNINGS

//using namespace std;

int cnt = 0;
#define High 1080
#define Width 600
#define MAX_SIZE 10
#define INF 0x3f3f3f3f

//引用Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

ExMessage  m;  //定义鼠标
ExMessage tmpMsg;

EasyTextBox login_name, login_password;//登录文本框
EasyTextBox register_name, register_password;//注册文本框
EasyButton btnOK, btnOK_register;
EasyButton btn_login_to_regis, btn_regis_to_login;//登录转注册 / 注册转登录
int menu_statu;
bool game_option_statu[10];
bool setting_statu[10] ;//菜单 设置 说明 按钮状态
int recvbufferidx;
int gamestart_statu = 0;//游戏开始状态
int log_sign_statu = -1;//本地 登录 注册状态

#include "CHATBOX.h"
#include "CHESSBOARDVIEW.h"


void startmenu() {//开始菜单

	m = getmessage(EM_MOUSE);

	putimage(0, 0, &menu_bakeground);
	//356 * 125 双人对局
	if (m.x >= 160 && m.x <= 160 + 356 && m.y >= 380 && m.y <= 380 + 125) {
		putimage(160, 380, &menu_option[2], 0x008800C6);
		putimage(160, 380, &menu_option[5], 0x00EE0086);

		//按下 左键
		if (m.message == WM_LBUTTONDOWN) {
			menu_statu = 1;// 登录界面
		}
	}
	else {
		putimage(160, 380, &menu_option[2], 0x008800C6);
		putimage(160, 380, &menu_option[1], 0x00EE0086);
	}

	//341 * 113  退出游戏
	if (m.x >= 560 && m.x <= 550 + 341 && m.y >= 380 && m.y <= 380 + 113) {
		putimage(560, 380, &menu_option[3], 0x008800C6);
		putimage(560, 380, &menu_option[6], 0x00EE0086);
		if (m.message == WM_LBUTTONDOWN) {
			closesocket(fd);
			WSACleanup();//每次结束程序时，都要记得调用这两行代码，用来释放网络连接相关资源------------------------
			exit(0);
		}
	}
	else {
		putimage(560, 380, &menu_option[3], 0x008800C6);
		putimage(560, 380, &menu_option[4], 0x00EE0086);
	}
}

int domessage() //clinum是向服务器发送请求的客户端；flag标志：是否向对方服务器发送请求
{
	recvbufferidx = 0;
	//使用sscanf处理recvbuffer
	char waybuffer[2048];
	char kepname[2048];
	char optionpos[2048];
	
	TCHAR chatmsg_buffer[100];
	printf("debug domessage %s\n", recvbuffer);
	while (recvbufferidx < strlen(recvbuffer)) {
		
		printf("recvbufferidx : %d,  recvbufferlen : %d\n", recvbufferidx, strlen(recvbuffer));

		sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
		recvbufferidx += strlen(waybuffer) + 1;

		if (strcmp(waybuffer, "_sign") == 0) {//注册
			printf("注册\n");
			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;

			if (strcmp(waybuffer, "username_exist") == 0)
			{
				log_sign_statu = 4;
				//return 4;
			}
			if (strcmp(waybuffer, "sign_success") == 0) {
				log_sign_statu = 5;
				//return 5;
			}
		}
		if (strcmp(waybuffer, "_log") == 0) {//登录
			printf("登录\n");

			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			//用户名不存在
			if (strcmp(waybuffer, "username_wrong") == 0) {
				log_sign_statu = 1;
				//return 1;
			}
			//密码错误
			if (strcmp(waybuffer, "password_wrong") == 0) {
				log_sign_statu = 2;
				//return 2;
			}
			//密码正确 登陆成功
			if (strcmp(waybuffer, "password_right") == 0) {
				log_sign_statu = 3;
				//return 3;
			}
			//用户已经被登录
			if (strcmp(waybuffer, "user_loged") == 0) {
				log_sign_statu = 7;
				//return 7;
			}


		}
		
		if (strcmp(waybuffer, "_userinfor") == 0) {//用户信息
			printf("用户信息读取\n");
			
			//username标识
			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			
			//username实际
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			//fitpos secondpos
			sscanf(recvbuffer + recvbufferidx, "%s", optionpos);
			recvbufferidx += strlen(optionpos) + 1;
				
			if (strcmp(optionpos, "firstpos") == 0) {
				play_head_statu[1] = 0;//玩家1已经进入 -！！！！！未设置退出

				strcpy(user_1.username, kepname);
				//score标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//score实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;

				user_1.score = atoi(waybuffer);


				//max_score标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//max_score 实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.max_score = atoi(waybuffer);


				//win_rate标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_rate 实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.win_rate = atof(waybuffer);


				//total_match标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//total_match实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.total_match = atoi(waybuffer);

				//win_count标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_count实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.win_count = atoi(waybuffer);

				//lose_count 标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//lose_count 实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.lose_count = atoi(waybuffer);


				//draw_count  标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//draw_count  实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.draw_count = atoi(waybuffer);
				

				//head_id  标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//head_id  实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.head_id = atoi(waybuffer);
			}
			else {
				play_head_statu[2] = 0;//玩家2已经进入 -！！！！！未设置退出

				strcpy(user_2.username, kepname);
				//score标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//score实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;

				user_2.score = atoi(waybuffer);


				//max_score标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//max_score 实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.max_score = atoi(waybuffer);


				//win_rate标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_rate 实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.win_rate = atof(waybuffer);


				//total_match标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//total_match实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.total_match = atoi(waybuffer);

				//win_count标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_count实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.win_count = atoi(waybuffer);

				//lose_count 标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//lose_count 实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.lose_count = atoi(waybuffer);


				//draw_count  标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//draw_count  实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.draw_count = atoi(waybuffer);


				//head_id  标识
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//head_id  实际
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.head_id = atoi(waybuffer);
			}
			std::cout << "user1: " << "\n";
			user_1.debug();
			std::cout << "user2: " << "\n";
			user_2.debug();
		}
		if (strcmp(waybuffer, "_readyinfor") == 0) {//玩家准备信息
			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			//玩家1/2准备状态更新
			if (strcmp(waybuffer, "firstpos") == 0) {
				user_1.ready_ok = 1;
			}
			else if(strcmp(waybuffer, "secondpos") == 0) {
				user_2.ready_ok = 1;
			}
		}
		if (strcmp(waybuffer, "_chat") == 0) {//聊天信息
			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;

			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;

			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;

			_stprintf(chatmsg_buffer, _T("%S: %S") , kepname , waybuffer);
			printf("gggg---- %S\n", chatmsg_buffer);
			//维护队列
			chat_text tmp;
			_tcscpy(tmp.inputtext, chatmsg_buffer);
if (chat_msg.size() < chat_size) {
	chat_msg.push(tmp);
}
else {
	chat_msg.pop();
	chat_msg.push(tmp);
}
//chatbox();
//FlushBatchDraw();
		}

		if (strcmp(waybuffer, "_gamestart") == 0) //游戏开始
		{
			gamestart_statu = 1;
			game_option_statu[1] = 0;//禁用
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 1;//启用

			lastx = -1, lasty = -1;

		}

		if (strcmp(waybuffer, "_round") == 0)//谁的回合
		{
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			//玩家1/2准备状态更新
			if (strcmp(kepname, myname) == 0) {
				myround = 1;//如果到了我的回合 悔棋禁用
				game_option_statu[2] = 0;
			}
			else {
				myround = 0;
				int full = 1;//检测棋盘上是否全为0
				for (int f = 0; f < 15; ++f) {
					for (int o = 0; o < 15; ++o) {
						if (borad[f][o] != 0)full = 0;
					}
				}
				if(full == 0)
					game_option_statu[2] = 1;//如果当前不是我的回合且棋盘上有棋子 悔棋启用
				else
					game_option_statu[2] = 0;//如果当前不是我的回合且棋盘上没有棋子 悔棋启用

			}
		}

		if (strcmp(waybuffer, "_win") == 0)//赢家
		{
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			//玩家1/2准备状态更新
			if (strcmp(kepname, myname) == 0) {
				MessageBox(GetHWnd(), L"恭喜你，胜利了!", L"游戏结果", MB_OK);
			}
			else {
				MessageBox(GetHWnd(), L"很遗憾，失败了!", L"游戏结果", MB_OK);
			}


			//游戏结束后状态更新
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//游戏结束
			game_option_statu[1] = 1;//启用
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;

			//准备状态要更新 
		}

		if (strcmp(waybuffer, "_move") == 0)//
		{
			//serversend
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			if (strcmp(kepname, "serversend") == 0) {
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//玩家1/2准备状态更新
				if (strcmp(waybuffer, "white") == 0) {

					//x
					sscanf(recvbuffer + recvbufferidx, "%s", kepname);
					recvbufferidx += strlen(kepname) + 1;
					int x = atoi(kepname);

					//y
					sscanf(recvbuffer + recvbufferidx, "%s", kepname);
					recvbufferidx += strlen(kepname) + 1;
					int y = atoi(kepname);
					printf("x =%d y = %d\n", x, y);

					borad[x][y] = 1;
				}
				else {
					//x
					sscanf(recvbuffer + recvbufferidx, "%s", kepname);
					recvbufferidx += strlen(kepname) + 1;
					int x = atoi(kepname);
					//y
					sscanf(recvbuffer + recvbufferidx, "%s", kepname);
					recvbufferidx += strlen(kepname) + 1;
					int y = atoi(kepname);
					printf("x =%d y = %d\n", x, y);
					borad[x][y] = 2;
				}
			}
			else {

			}
			//

		}

		if (strcmp(waybuffer, "_other_want_to_draw") == 0)//接受对方发送和棋请求 并进行响应
		{
			//弹窗 响应事件
			int id = MessageBox(GetHWnd(), L"对方请求和棋，是否同意?", L"和棋", MB_ICONQUESTION| MB_YESNO);
			printf("draw id = %d\n", id);
			//IDYES(6)
			//IDNO(7)
			sprintf(sendbuffer, "_answer_draw_result %d ",((id==6)?1:0));
			send(fd, sendbuffer, strlen(sendbuffer), 0);
			printf("Ask draw  result %s\n", sendbuffer);
			
		}

		if (strcmp(waybuffer, "_draw_is_ok") == 0)//同意和棋
		{
			//弹窗 响应事件
			int id = MessageBox(GetHWnd(), L"和棋成功，状态已重置", L"和棋", MB_OK);

			game_option_statu[3] = 1;//还原

			//游戏结束后状态更新
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//游戏结束
			game_option_statu[1] = 1;//启用
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
			//准备状态要更新 
		}

		if (strcmp(waybuffer, "_draw_is_fail") == 0)//不同意和棋
		{
			//弹窗 响应事件
			int id = MessageBox(GetHWnd(), L"对方拒绝了你的和棋请求!", L"和棋失败", MB_OK);

			game_option_statu[3] = 1;//还原
		}	

		if (strcmp(waybuffer, "_capitulate_is_ok") == 0)//响应自己已经认输
		{
			//弹窗 响应事件
			MessageBox(GetHWnd(), L"很遗憾，失败了!", L"游戏结果", MB_OK);
			//游戏结束后状态更新
			memset(borad, 0, sizeof borad);

			game_option_statu[4] = 1;//还原

			gamestart_statu = 0;//游戏结束
			game_option_statu[1] = 1;//启用
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
			//准备状态要更新 
		}
		if (strcmp(waybuffer, "_other_is_capitulate") == 0)//响应对方已经认输
		{
			//弹窗 响应事件
			MessageBox(GetHWnd(), L"对方已认输！恭喜你，胜利了!", L"游戏结果", MB_OK);
			//游戏结束后状态更新
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//游戏结束
			game_option_statu[1] = 1;//启用
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
			//准备状态要更新 
		}
		if (strcmp(waybuffer, "_other_return_to_menu") == 0) {//接受对方菜单事件消息
			
			//游戏结束后状态更新

			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			if (strcmp(kepname, "_no_game_start") == 0) {

				memset(borad, 0, sizeof borad);
				gamestart_statu = 0;//游戏结束
				lastx = -1, lasty = -1;
				//本地事件不变 对方事件改变
				//game_option_statu[1] = 1;//启用
				//game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

				if (strcmp(user_1.username, myname) == 0) {//如果本地是用户1
					user_2.ready_ok = 0;//清空对方准备状态
					play_head_statu[2] = -1;//对方未进入游戏
				}
				else {//如果本地是用户2
					user_1.ready_ok = 0;//清空对方准备状态
					play_head_statu[1] = -1;//对方未进入游戏
				}
			}
			else if (strcmp(kepname, "_game_start") == 0) {//游戏已经开始

				MessageBox(GetHWnd(), L"对方已退出！恭喜你，胜利了!", L"游戏结果", MB_OK);

				//本地事件返回到游戏未准备状态
				memset(borad, 0, sizeof borad);//清空棋盘
				gamestart_statu = 0;//游戏结束
				lastx = -1, lasty = -1;
				game_option_statu[1] = 1;//启用
				game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用init_chessborad

				if (strcmp(user_1.username, myname) == 0) {//如果本地是用户1
					user_2.ready_ok = 0;//清空对方准备状态
					user_1.ready_ok = 0;//清空自己准备状态
					play_head_statu[2] = -1;//对方未进入游戏
				}
				else {//如果本地是用户2
					user_1.ready_ok = 0;//清空对方准备状态
					user_2.ready_ok = 0;//清空自己准备状态
					play_head_statu[1] = -1;//对方未进入游戏
				}
			}

			else {
				puts("_other_return_to_menu 错误");
				exit(0);
			}

			
		}

		if (strcmp(waybuffer, "_exit") == 0) {//接受对方与服务器断开连接消息
			//firstpos/secondpos
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			if (strcmp(waybuffer, "_no_game_start") == 0) {

				memset(borad, 0, sizeof borad);
				gamestart_statu = 0;//游戏结束
				lastx = -1, lasty = -1;
				//本地事件不变 对方事件改变
				//game_option_statu[1] = 1;//启用
				//game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

				if (strcmp(kepname, "firstpos") == 0) {//如果对方是用户1
					user_1.ready_ok = 0;//清空对方准备状态
					play_head_statu[1] = -1;//对方未进入游戏
				}
				else {//如果对方是用户2
					play_head_statu[2] = -1;//对方未进入游戏
					user_2.ready_ok = 0;//清空对方准备状态
				}
			}
			else if (strcmp(waybuffer, "_game_start") == 0) {
				
				MessageBox(GetHWnd(), L"对方已退出！恭喜你，胜利了!", L"游戏结果", MB_OK);

				//本地事件返回到游戏未准备状态
				memset(borad, 0, sizeof borad);//清空棋盘
				gamestart_statu = 0;//游戏结束
				lastx = -1, lasty = -1;
				game_option_statu[1] = 1;//启用
				game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

				user_2.ready_ok = 0;//清空对方准备状态
				user_1.ready_ok = 0;//清空对方准备状态

				if (strcmp(kepname, "firstpos") == 0) {//如果对方是用户1
					user_1.ready_ok = 0;//清空对方准备状态
					play_head_statu[1] = -1;//对方未进入游戏
				}
				else {//如果对方是用户2
					play_head_statu[2] = -1;//对方未进入游戏
					user_2.ready_ok = 0;//清空对方准备状态
				}

			}
			else {
				puts("接受与服务器断开连接消息出现异常.");
				exit(0);
			}

		}

		if (strcmp(waybuffer, "_other_want_to_regret") == 0) {//接受对方的悔棋请求并回应
			//弹窗 响应事件
			int id = MessageBox(GetHWnd(), L"对方请求悔棋，是否同意?", L"悔棋", MB_ICONQUESTION | MB_YESNO);
			printf("draw id = %d\n", id);
			//IDYES(6)
			//IDNO(7)
			sprintf(sendbuffer, "_answer_regret_result %d ", ((id == 6) ? 1 : 0));
			send(fd, sendbuffer, strlen(sendbuffer), 0);
			printf("Ask regret result %s\n", sendbuffer);
		}
		//---
		if (strcmp(waybuffer, "_regret_is_ok") == 0) {//接受服务器的悔棋请求结果
			MessageBox(GetHWnd(), L"对方同意了你的悔棋请求!", L"悔棋", MB_OK);
			//x
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			int x = atoi(kepname);
			//y
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			int y = atoi(kepname);
			printf("x =%d y = %d\n", x, y);
			borad[x][y] = 0;//悔棋
		}
		if (strcmp(waybuffer, "_regret_is_fail") == 0) {//接受服务器的悔棋请求结果
			MessageBox(GetHWnd(), L"对方拒绝了你的悔棋请求!", L"悔棋", MB_OK);
		}
		if (strcmp(waybuffer, "_regret_chess") == 0) {//接受服务器的悔棋请求结果

			//x
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			int x = atoi(kepname);
			//y
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			int y = atoi(kepname);
			printf("x =%d y = %d\n", x, y);
			borad[x][y] = 0;
		}
		if (strcmp(waybuffer, "_dogfall") == 0) {//接受棋盘已满的和棋结果
			MessageBox(GetHWnd(), L"对局已结束，结果为平局!", L"平局", MB_OK);
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//游戏结束
			game_option_statu[1] = 1;//启用
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
		}
	}
	return -1;
}

// 按钮 btnOK login登录页面 的点击事件
void On_btnOk_Click()
{

	//需要检查用户名是否存在 
	/*
	在recvbuffer中收到3种字符串
	username wrong  表示用户名不存在
	password wrong	表示密码错误
	password right	表示密码正确，登录成功
	*/


	//先检查用户名或者密码是否为空
	bool flag = false;
	bool check = true;//判定用户名格式是否合法

	int wlen = wcslen(login_name.Text());
	for (int i = 0; i < wlen; ++i) {
		if (!(isalpha(login_name.Text()[i]) || isdigit(login_name.Text()[i]))) {
			check = false;
		}
	}



	if (wcslen(login_name.Text()) == 0) {
		MessageBox(GetHWnd(), L"用户名不能为空", L"错误", MB_OK); flag = true;
	}
	if (!check) {
		//用户名称只允许数字和字符
		MessageBox(GetHWnd(), L"用户名称只允许数字和字符", L"错误", MB_OK); 
		return ;
	}
	if (wcslen(login_password.Text()) == 0) {
		MessageBox(GetHWnd(), L"密码不能为空", L"错误", MB_OK); flag = true;
	}
	

	printf("flag : %d check: %d\n", flag,check);
	if (flag)return;

	//发送消息并接受消息
	memset(sendbuffer, 0, sizeof(sendbuffer));
	sprintf(sendbuffer, "_log %S %S ", login_name.Text(), login_password.Text());//发送log标志，表示登陆信息
	send(fd, sendbuffer, strlen(sendbuffer), 0);

	memset(recvbuffer, 0, sizeof(recvbuffer));
	recv(fd, recvbuffer, RECVBUFFERSIZE, 0);
	int ret_option = domessage();

	//用户名不存在
	if (log_sign_statu == 1) {

		MessageBox(GetHWnd(), L"用户名不存在", L"错误", MB_OK);
	}

	//密码错误
	if (log_sign_statu == 2) {
		MessageBox(GetHWnd(), L"密码错误", L"错误", MB_OK);
	}

	//密码正确 登陆成功
	if (log_sign_statu == 3)
	{
		sprintf(myname,"%S", login_name.Text());//记录自己用户名
		menu_statu = 3;//跳转游戏界面
		wchar_t s[100] = L"Hello, ";
		wcscat_s(s, 100, login_name.Text());
		MessageBox(GetHWnd(), s, L"Hello", MB_OK);
	}

	if (log_sign_statu == 7) {
		MessageBox(GetHWnd(), L"该用户目前已被登录", L"错误", MB_OK);
	}
}


// btnOK regis注册页面 的点击事件
void On_btnOK_register_Click()
{

	bool flag = false;
	bool check = true;//判定用户名格式是否合法

	int wlen = wcslen(register_name.Text());
	for (int i = 0; i < wlen; ++i) {
		if (!(isalpha(register_name.Text()[i]) || isdigit(register_name.Text()[i]))) {
			check = false;
		}
	}


	if (wcslen(register_name.Text()) == 0) {
		MessageBox(GetHWnd(), L"用户名不能为空", L"错误", MB_OK);
		flag = true;
	}
	if (!check) {
		//用户名称只允许数字和字符
		MessageBox(GetHWnd(), L"用户名称只允许数字和字符", L"错误", MB_OK);
		return;
	}
	if (wcslen(register_password.Text()) == 0) {
		MessageBox(GetHWnd(), L"密码不能为空", L"错误", MB_OK);
		flag = true;
	}
	if (flag)return;
	memset(sendbuffer, 0, sizeof(sendbuffer));
	sprintf(sendbuffer, "_sign %S %S ", register_name.Text(), register_password.Text());//发送sign标志，表示注册信息
	send(fd, sendbuffer, strlen(sendbuffer), 0);
	
	memset(recvbuffer, 0, sizeof(recvbuffer));
	recv(fd, recvbuffer, RECVBUFFERSIZE, 0);
	int ret_option = domessage();


	/*
	recvbuffer中收到两种信息
	username exist  表示用户名已存在
	sign success  表示注册成功
	*/
	printf("regis : %s\n", recvbuffer);

	if (log_sign_statu == 4)
	{
		wchar_t s[100] = L"用户名已存在, ";
		wcscat_s(s, 100, register_name.Text());
		MessageBox(GetHWnd(), s, L"错误", MB_OK);
	}


	if (log_sign_statu == 5)
	{
		wchar_t s[100] = L"注册成功, ";
		wcscat_s(s, 100, register_name.Text());
		MessageBox(GetHWnd(), s, L"Hello", MB_OK);
	}
}

//登录转注册 点击事件
void On_btn_login_to_regis_Click()
{
	menu_statu = 2;
}

//注册转登录 点击事件
void On_btn_regis_to_login_Click() {
	menu_statu = 1;
}

void startup() {
	
	initgraph(High, Width, SHOWCONSOLE);
	login_name.Create(440, 290, 620, 330, 10); // 创建用户名文本框控件
	login_password.Create(440, 390, 620, 430, 10);	//创建用户名 密码文本框控件
	btnOK.Create(635, 500, 675, 535, L"登录", On_btnOk_Click);
	btn_login_to_regis.Create(490, 500, 610, 535, L"没有账号?注册", On_btn_login_to_regis_Click);//切换到注册


	register_name.Create(440, 290, 620, 330, 10);
	register_password.Create(440, 390, 620, 430, 10);
	btnOK_register.Create(635, 500, 675, 535, L"注册", On_btnOK_register_Click);

	btn_regis_to_login.Create(490, 500, 610, 535, L"已有账号?登录", On_btn_regis_to_login_Click);//切换回登录
	
	//输入框
	chat_input_box.Create(1080 - 265, 533, 1080 - 67, 568, 13);

}

//登录界面
void login() {

	putimage(0, 0, &login_bakeground);

	m = getmessage(EM_MOUSE);
	//填充白色圆角矩形
	setfillcolor(WHITE);
	fillroundrect(300, 250, 750, 550, 20, 20);

	setbkcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(16, 0, _T("微软黑体"));
	outtextxy(350, 300, L"用户名: ");

	//440-620 长度   290-330 宽度
	login_name.Show();

	setbkcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(16, 0, _T("微软黑体"));
	outtextxy(350, 400, L"密　码：");
	login_password.Show();

	btnOK.Show();
	btn_login_to_regis.Show();

	if (m.message == WM_LBUTTONDOWN) {
		// 判断 用户名控件
		if (login_name.Check(m.x, m.y))
			login_name.OnMessage();

		//判断 密码控件
		if (login_password.Check(m.x, m.y))
			login_password.OnMessage();

		// 判断控件
		if (btnOK.Check(m.x, m.y))
			btnOK.OnMessage();

		//判断控件  登录转注册 按钮
		if (btn_login_to_regis.Check(m.x, m.y))
			btn_login_to_regis.OnMessage();
	}
}

void regis() {

	putimage(0, 0, &login_bakeground);

	m = getmessage(EM_MOUSE);
	//填充白色圆角矩形
	setfillcolor(WHITE);
	fillroundrect(310, 260, 740, 540, 20, 20);

	setbkcolor(WHITE);
	settextcolor(BLACK);
	outtextxy(350, 300, L"注册用户名: ");

	//440-620 长度   290-330 宽度
	register_name.Show();

	setbkcolor(WHITE);
	settextcolor(BLACK);
	outtextxy(350, 400, L"注册密码：");
	register_password.Show();
	btnOK_register.Show();
	btn_regis_to_login.Show();

	if (m.message == WM_LBUTTONDOWN) {
		// 判断 用户名控件
		if (register_name.Check(m.x, m.y))
			register_name.OnMessage();

		//判断 密码控件
		if (register_password.Check(m.x, m.y))
			register_password.OnMessage();

		// 判断控件
		if (btnOK_register.Check(m.x, m.y))
			btnOK_register.OnMessage();
		//判断 注册转登录 按钮
		if (btn_regis_to_login.Check(m.x, m.y))
			btn_regis_to_login.OnMessage();

	}
}

void Play1_view() {
	TCHAR kep[200];
	peekmessage(&m, EM_MOUSE, false);

	setcolor(BLACK);
	fillrectangle(5, 5, 241, 151);
	fillrectangle(115, 10, 235, 145);
	
	putimage(6, 6, &player_option_background);
	//头像位置
	//circle(60, 60, 50);
	// 
	

	peekmessage(&m, EM_MOUSE, false);

	if (play_head_statu[1] == -1) {//未进入

	}
	else {

		//进入


		//检测是否在圆内
		if (play_head_statu[1] == 0 && (60 - m.x) * (60 - m.x) + (60 - m.y) * (60 - m.y) <= 50 * 50) {

			if (m.message == WM_LBUTTONDOWN) {
				play_head_statu[1] = 1;//切换到个人信息页面
			}

		}
		if (play_head_statu[1] == 0) {
			putimage(10, 10, &game_player_head[6], 0x008800C6);
			putimage(10, 10, &game_player_head[user_1.head_id], 0x00EE0086);

			//昵称
			roundrect(10, 120, 110, 140, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("微软黑体"));
			outtextxy(13, 122, _T("昵称: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"), user_1.username);
			//printf("%S\n", kep);
			outtextxy(50, 122, kep);// 7个长度显示上限

			//时间框
			//setcolor(RED);
			//fillrectangle(115, 10, 235, 145);
			// 
			//状态栏
			//circle(95, 95, 12);
			if (user_1.ready_ok) {//玩家1已准备好
				putimage(95 - 12, 95 - 12, &play_msg_ok[1], 0x008800C6);
				putimage(95 - 12, 95 - 12, &play_msg_ok[2], 0x00EE0086);
			}

		}
		else {

			//昵称
			roundrect(10, 20, 110, 40, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("微软黑体"));
			outtextxy(13, 24, _T("昵称: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"),user_1.username);
			//printf("%S\n", kep);
			outtextxy(50, 24, kep);// 7个长度显示上限
			
			//积分框
			roundrect(10, 50, 110, 140, 10, 10);
			
			//积分
			outtextxy(13, 63, _T("积分: "));
			_stprintf(kep, _T("%d"), user_1.score);
			outtextxy(13 + 40, 63, kep);

			//最高积分
			settextstyle(14, 0, _T("微软黑体"));
			outtextxy(13, 93, _T("最高积分: "));
			_stprintf(kep, _T("%d"), user_1.max_score);
			outtextxy(13 + 65 , 93, kep);

			//胜率
			//roundrect(125, 70, 230, 135, 10, 10);
			settextstyle(19, 0, _T("楷体"));
			outtextxy(120, 15, _T("胜率: "));
			_stprintf(kep, _T("%.1f%%"), user_1.win_rate * 100);
			outtextxy(120 + 55, 15,kep);

			//总局数
			_stprintf(kep, _T("%d"), user_1.total_match);
			outtextxy(120, 45, _T("总局数: "));
			outtextxy(120 + 70, 45, kep);

			roundrect(120, 70, 225, 135, 10, 10);
			settextstyle(18, 0, _T("楷体"));
			
			//胜场
			_stprintf(kep, _T("%d"), user_1.win_count);
			outtextxy(125, 75, _T("胜场："));
			outtextxy(125 + 45, 75, kep);

			//负场
			_stprintf(kep, _T("%d"), user_1.lose_count);
			outtextxy(125, 95, _T("负场："));
			outtextxy(125 + 45, 95, kep);

			//平局
			_stprintf(kep, _T("%d"), user_1.draw_count);
			outtextxy(125, 115, _T("平局："));
			outtextxy(125 + 45, 115, kep);


			//退出按钮
			putimage(218, 10, &play_msg_exit[1], 0x008800C6);
			putimage(218, 10, &play_msg_exit[2], 0x00EE0086);

			if (m.x >= 218 && m.x <= 218 + 20 && m.y >= 10 && m.y <= 10 + 20) {
				//在区域内点击则退出
				if (m.message == WM_LBUTTONDOWN) {
					play_head_statu[1] = 0;//更改状态
				}
			}
			

		}
	}

	
}
void Play2_view() {
	TCHAR kep[200];
	peekmessage(&m, EM_MOUSE, false);
	setcolor(BLACK);
	fillrectangle(5, 160, 241, 306);
	fillrectangle(115, 165, 235, 300);

	putimage(6, 161, &player_option_background);
	
	peekmessage(&m, EM_MOUSE, false);
	

	if (play_head_statu[2] == -1) {//未进入

	}
	else {
		//已经入

		//检测是否在圆内
		//printf("circle check %d\n", (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50);
		if (play_head_statu[2] == 0 && (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50) {

			if (m.message == WM_LBUTTONDOWN) {
				play_head_statu[2] = 1;//切换到个人信息页面
			}

		}
		//printf("play_head_statu check %d\n", play_head_statu[2]);

		//头像位置
		//circle(60, 215, 50);
		if (play_head_statu[2] == 0) {
			//状态栏
			//circle(95, 250, 12);
			putimage(10, 165, &game_player_head[6], 0x008800C6);
			putimage(10, 165, &game_player_head[user_2.head_id], 0x00EE0086);
			
			//昵称
			roundrect(10, 280, 110, 300, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("微软黑体"));

			outtextxy(13, 184 + 98, _T("昵称: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"), user_2.username);
			//printf("%S\n", kep);
			outtextxy(50, 184 + 98, kep);// 7个长度显示上限
			
			//时间框
			//setcolor(RED);
			//fillrectangle(115, 165, 235, 300);
			if (user_2.ready_ok) {//玩家2已准备好
				putimage(60 + 35 - 12, 215 + 35 - 12, &play_msg_ok[1], 0x008800C6);
				putimage(60 + 35 - 12, 215 + 35 - 12, &play_msg_ok[2], 0x00EE0086);
			}
		}
		else {
			//昵称
			roundrect(10, 180, 110, 200, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("微软黑体"));

			outtextxy(13, 184, _T("昵称: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"), user_2.username);
			//printf("%S\n", kep);
			outtextxy(50, 184, kep);// 7个长度显示上限

			//积分框
			roundrect(10, 210, 110, 295, 10, 10);
			
			//积分
			outtextxy(13, 223, _T("积分: "));
			_stprintf(kep, _T("%d"), user_2.score);
			outtextxy(13 + 40, 223, kep);

			//最高积分
			settextstyle(14, 0, _T("微软黑体"));
			outtextxy(13, 253, _T("最高积分: "));
			_stprintf(kep, _T("%d"), user_2.max_score);
			outtextxy(13 + 65, 253, kep);
			
			//胜率
			//roundrect(125, 225, 230, 290, 10, 10);
			settextstyle(19, 0, _T("楷体"));
			outtextxy(120, 170, _T("胜率: "));
			_stprintf(kep, _T("%.1f%%"), user_2.win_rate * 100);
			outtextxy(120 + 55, 170, kep);

			//总局数
			_stprintf(kep, _T("%d"), user_2.total_match);
			outtextxy(120, 200, _T("总局数: "));
			outtextxy(120 + 70, 200, kep);

			
			roundrect(120, 225, 225, 290, 10, 10);
			settextstyle(18, 0, _T("楷体"));
			
			//胜场
			_stprintf(kep, _T("%d"), user_2.win_count);
			outtextxy(125, 230, _T("胜场："));
			outtextxy(125 + 45, 230, kep);

			
			//负场
			_stprintf(kep, _T("%d"), user_2.lose_count);
			outtextxy(125, 250, _T("负场："));
			outtextxy(125 + 45, 250, kep);

			//平局
			_stprintf(kep, _T("%d"), user_2.draw_count);
			outtextxy(125, 270, _T("平局："));
			outtextxy(125 + 45, 270, kep);


			//退出按钮
			
			putimage(218, 165, &play_msg_exit[1], 0x008800C6);
			putimage(218, 165, &play_msg_exit[2], 0x00EE0086);

			if (m.x >= 218 && m.x <= 218 + 20 && m.y >= 165 && m.y <= 165 + 20) {
				//在区域内点击则退出
				if (m.message == WM_LBUTTONDOWN) {
					play_head_statu[2] = 0;//更改状态
				}
			}
			
		}
	}
}


void ready() //准备
{
	//准备 235 * 71
	
	peekmessage(&m, EM_MOUSE, false);
	//m = getmessage(EM_MOUSE);
	if (game_option_statu[1]) {
		//可用
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 313 && m.y <= 313 + 71) {
			//在区域内
			//左键按下
			if (m.message == WM_LBUTTONDOWN ) {
				//准备 按下 缩小
				//发送 我自己的准备信息
				sprintf(sendbuffer, "_myreadyinfor ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);

				game_option_statu[1] = 0;//准备按下后准备被禁用

				putimage(5, 313, &game_option[18], 0x008800C6);//准备_按下遮罩
				putimage(5, 313, &game_option[17], 0x00EE0086);//准备_按下
			}
			else {
				putimage(5, 313, &game_option[1], 0x008800C6);//准备01
				putimage(5, 313, &game_option[2], 0x00EE0086);//准备02
			}
		}
		else {
			//可用 不在区域内
			putimage(5, 313, &game_option[1], 0x008800C6);//准备01
			putimage(5, 313, &game_option[2], 0x00EE0086);//准备02
		}
	}
	else {
		//被禁用 灰色
		putimage(5, 313, &game_option[1], 0x008800C6);//准备01
		putimage(5, 313, &game_option[19], 0x00EE0086);//准备_禁用
	}
}

void repentance() //悔棋
{
	//BeginBatchDraw();
	//悔棋 235 * 71
	//m = getmessage(EM_MOUSE);
	peekmessage(&m, EM_MOUSE, false);

	if (game_option_statu[2] && lastx != -1) {
		//可用
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 388 && m.y <= 388 + 71) {
			//在区域内
			//左键按下
			if (m.message == WM_LBUTTONDOWN) {
				//悔棋 按下 缩小

				//发送 我自己的悔棋信息
				sprintf(sendbuffer, "_I_want_to_regret ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);
				game_option_statu[2] = 0;//按下后悔棋被禁用

				putimage(5, 388, &game_option[9], 0x008800C6);
				putimage(5, 388, &game_option[11], 0x00EE0086);

			}
			else {
				putimage(5, 388, &game_option[10], 0x008800C6);
				putimage(5, 388, &game_option[12], 0x00EE0086);
			}
		}
		else {
			//可用 不在区域内
			putimage(5, 388, &game_option[10], 0x008800C6);
			putimage(5, 388, &game_option[12], 0x00EE0086);
		}
	}
	else {
		//被禁用 灰色
		putimage(5, 388, &game_option[3], 0x008800C6);
		putimage(5, 388, &game_option[4], 0x00EE0086);
	}
	//FlushBatchDraw();
}
void positional_draw() //和棋
{
	//和棋 235 * 71
	//m = getmessage(EM_MOUSE);
	peekmessage(&m, EM_MOUSE, false);
	
	if (game_option_statu[3]) {
		//可用
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 463 && m.y <= 463 + 71) {
			//在区域内
			//左键按下
			if (m.message == WM_LBUTTONDOWN) {
				//和棋 按下 缩小
				//发送和棋请求
				sprintf(sendbuffer, "_I_want_to_draw ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);

				game_option_statu[3] = 0;//按下后和棋被禁用

				putimage(5, 463, &game_option[9], 0x008800C6);
				putimage(5, 463, &game_option[13], 0x00EE0086);
			}
			else {
				putimage(5, 463, &game_option[10], 0x008800C6);
				putimage(5, 463, &game_option[14], 0x00EE0086);
			}
		}
		else {
			//可用 不在区域内
			putimage(5, 463, &game_option[10], 0x008800C6);
			putimage(5, 463, &game_option[14], 0x00EE0086);
		}
	}
	else {
		//被禁用 灰色
		putimage(5, 463, &game_option[5], 0x008800C6);
		putimage(5, 463, &game_option[6], 0x00EE0086);
	}
}

void concede()//认输
{
	//认输 235 * 71
	//m = getmessage(EM_MOUSE);
	peekmessage(&m, EM_MOUSE, false);
	if (game_option_statu[4]) {
		//可用
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 538 && m.y <= 538 + 71) {
			//在区域内
			//左键按下
			if (m.message == WM_LBUTTONDOWN) {
				//认输 按下 缩小
				// 
				//发送认输请求
				sprintf(sendbuffer, "_I_capitulate ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);

				game_option_statu[4] = 0;//按下后认输被禁用

				putimage(5, 538, &game_option[9], 0x008800C6);
				putimage(5, 538, &game_option[15], 0x00EE0086);
			}
			else {
				putimage(5, 538, &game_option[10], 0x008800C6);
				putimage(5, 538, &game_option[16], 0x00EE0086);
			}
		}
		else {
			//可用 不在区域内
			putimage(5, 538, &game_option[10], 0x008800C6);
			putimage(5, 538, &game_option[16], 0x00EE0086);
		}
	}
	else {
		//被禁用 灰色
		putimage(5, 538, &game_option[7], 0x008800C6);
		putimage(5, 538, &game_option[8], 0x00EE0086);
	}
}

void setting() {
	//右1 280 * 80
	setcolor(BLACK);
	//fillrectangle(1080 - 5 - 280, 5, 1080 - 5, 5 + 80);
	rectangle(1080 - 5 - 280, 5, 1080 - 5, 5 + 80); 
	putimage(1080 - 5 - 279, 6, &sys_option[3]);//背景

	
	//返回菜单
	//peekmessage(&m, EM_MOUSE, true);
	
	ExMessage msg;
	

	//tmpMsg = m;
	//ExMessage msg2 = {0};
	peekmessage(&msg, EM_MOUSE, true);
	{
		//tmpMsg = msg;
		//返回菜单
		putimage(1080 - 265, 10, &sys_option[6], 0x008800C6);
		putimage(1080 - 265, 10, &sys_option[7], 0x00EE0086);

		//游戏说明
		putimage(1080 - 180, 10, &sys_option[10], 0x008800C6);
		putimage(1080 - 180, 10, &sys_option[11], 0x00EE0086);

		//设置
		putimage(1080 - 95, 10, &sys_option[14], 0x008800C6);
		putimage(1080 - 95, 10, &sys_option[15], 0x00EE0086);

		if (msg.message == WM_LBUTTONDOWN){
			if (msg.x >= 1080 - 265 && msg.x <= 1080 - 195 && msg.y >= 10 && msg.y <= 80) {
				//左键按下	菜单
				menu_statu = 0;

				//发送之前就已经本地响应了
				//游戏结束后状态更新
				memset(borad, 0, sizeof borad);

				menu_statu = 0;//返回游戏菜单
				
				gamestart_statu = 0;//无论开没开始游戏，游戏都结束

				game_option_statu[1] = 1;//启用
				game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//禁用

				user_1.ready_ok = 0;
				user_2.ready_ok = 0;
				//准备状态要更新 
				
				init_chessborad = 0;//初始棋盘状态重置

				//send一条我已退出的消息给服务器
				sprintf(sendbuffer, "_I_return_to_menu ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);
				//本地 登录 注册状态重置
				log_sign_statu = -1;
			}
			if (msg.x >= 1080 - 180 && msg.x <= 1080 - 110 && msg.y >= 10 && msg.y <= 80) {
				//左键按下	说明
				MessageBox(GetHWnd(), L"胜利条件：白棋与黑棋双方相互对垒，直至一方任意方向满五个即胜利。\n对局使用无禁手规则，悔棋最多连续一次。\n在双方皆准备后开始对局，请在规定时间内完成落子与对局\n", L"游戏说明", MB_OK);

			}
			if (msg.x >= 1080 - 95 && msg.x <= 1080 - 25 && msg.y >= 10 && msg.y <= 80) {
				//设置
				int id  = MessageBox(GetHWnd(), L"是否更换当前棋盘图片?\n", L"游戏设置",MB_ICONQUESTION | MB_YESNO);
				if (id == 6)init_chessborad = (1 - init_chessborad);//翻转图片
			}
		}
		
	}
	
	
	
	{
		//返回菜单
		putimage(1080 - 265, 10, &sys_option[6], 0x008800C6);
		putimage(1080 - 265, 10, &sys_option[7], 0x00EE0086);

		//游戏说明
		putimage(1080 - 180, 10, &sys_option[10], 0x008800C6);
		putimage(1080 - 180, 10, &sys_option[11], 0x00EE0086);

		//设置
		putimage(1080 - 95, 10, &sys_option[14], 0x008800C6);
		putimage(1080 - 95, 10, &sys_option[15], 0x00EE0086);
	}
	
}



void gameview() {


	//235 * 150
	putimage(0, 0, &game_background);
	//棋盘
	chessboard_view();
	
	//左上区域
	//玩家1 信息
	Play1_view();
	//玩家2 信息
	Play2_view();
	
	ready();//准备
	repentance();//悔棋
	positional_draw();//和棋
	concede();//认输

	//右1 280 * 275
	chatbox();
	setting();

	FlushBatchDraw();
	flushmessage(EM_MOUSE);//清空消息缓冲区
	
}



void process_user_message() //处理用户信息
{
	//发送消息并接受消息
	//设置套接字非阻塞
	u_long iMode = 1;
	ioctlsocket(fd, FIONBIO, &iMode);
	memset(recvbuffer, 0, sizeof(recvbuffer));
	int ret = recv(fd, recvbuffer, sizeof(recvbuffer), 0);
	//domessage(); 
	if (ret > 0) {
		iMode = 0;
		domessage();
		//puts("23323 inter gameview");
		//gameview();//游戏界面
		ioctlsocket(fd, FIONBIO, &iMode);
	}
	else if (ret == -1 && errno==0) {
		//未收到消息 画出图形
		//服务器没有发送消息
		//gameview();//游戏界面
		//puts("inter gameview");
		gameview();//游戏界面
		//用户在此可以点击准备按钮、在聊天框输入消息。
			//（准备消息，聊天消息可以不发送给服务器，因为对方此时还没有进入房间，对方看不到。）

	}
	else {
		//printf("errno %d\n", errno);s
		printf("非阻塞recv错误\n");
		//释放相关资源
	}
}


void showmain() {

	if (menu_statu == 0)startmenu();//游戏菜单
	else if (menu_statu == 1)login();//登陆游戏
	else if (menu_statu == 2)regis();//注册游戏
	else if (menu_statu == 3) {
		process_user_message();
		FlushBatchDraw();
		//gameview();//游戏界面	
	}
	FlushBatchDraw();

}

int main() {
	network_connect();
	Insert_IMAGE();
	startup();
	BeginBatchDraw();


	menu_statu = 0 ;
	for (int i = 1; i <= 4; ++i)game_option_statu[i] = 0;
	game_option_statu[1] = 1;

	//初始设置状态
	setting_statu[1] = setting_statu[2] = setting_statu[3] = 1;

	while (true) {
		showmain();
		FlushBatchDraw();
	}
	EndBatchDraw();
	closegraph();
	closesocket(fd);
	WSACleanup();//每次结束程序时，都要记得调用这两行代码，用来释放网络连接相关资源------------------------
	
	/*
	closesocket(fd);
	WSACleanup();//每次结束程序时，都要记得调用这两行代码
	*/
	return 0;
}