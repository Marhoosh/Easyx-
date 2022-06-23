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

//����Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

ExMessage  m;  //�������
ExMessage tmpMsg;

EasyTextBox login_name, login_password;//��¼�ı���
EasyTextBox register_name, register_password;//ע���ı���
EasyButton btnOK, btnOK_register;
EasyButton btn_login_to_regis, btn_regis_to_login;//��¼תע�� / ע��ת��¼
int menu_statu;
bool game_option_statu[10];
bool setting_statu[10] ;//�˵� ���� ˵�� ��ť״̬
int recvbufferidx;
int gamestart_statu = 0;//��Ϸ��ʼ״̬
int log_sign_statu = -1;//���� ��¼ ע��״̬

#include "CHATBOX.h"
#include "CHESSBOARDVIEW.h"


void startmenu() {//��ʼ�˵�

	m = getmessage(EM_MOUSE);

	putimage(0, 0, &menu_bakeground);
	//356 * 125 ˫�˶Ծ�
	if (m.x >= 160 && m.x <= 160 + 356 && m.y >= 380 && m.y <= 380 + 125) {
		putimage(160, 380, &menu_option[2], 0x008800C6);
		putimage(160, 380, &menu_option[5], 0x00EE0086);

		//���� ���
		if (m.message == WM_LBUTTONDOWN) {
			menu_statu = 1;// ��¼����
		}
	}
	else {
		putimage(160, 380, &menu_option[2], 0x008800C6);
		putimage(160, 380, &menu_option[1], 0x00EE0086);
	}

	//341 * 113  �˳���Ϸ
	if (m.x >= 560 && m.x <= 550 + 341 && m.y >= 380 && m.y <= 380 + 113) {
		putimage(560, 380, &menu_option[3], 0x008800C6);
		putimage(560, 380, &menu_option[6], 0x00EE0086);
		if (m.message == WM_LBUTTONDOWN) {
			closesocket(fd);
			WSACleanup();//ÿ�ν�������ʱ����Ҫ�ǵõ��������д��룬�����ͷ��������������Դ------------------------
			exit(0);
		}
	}
	else {
		putimage(560, 380, &menu_option[3], 0x008800C6);
		putimage(560, 380, &menu_option[4], 0x00EE0086);
	}
}

int domessage() //clinum�����������������Ŀͻ��ˣ�flag��־���Ƿ���Է���������������
{
	recvbufferidx = 0;
	//ʹ��sscanf����recvbuffer
	char waybuffer[2048];
	char kepname[2048];
	char optionpos[2048];
	
	TCHAR chatmsg_buffer[100];
	printf("debug domessage %s\n", recvbuffer);
	while (recvbufferidx < strlen(recvbuffer)) {
		
		printf("recvbufferidx : %d,  recvbufferlen : %d\n", recvbufferidx, strlen(recvbuffer));

		sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
		recvbufferidx += strlen(waybuffer) + 1;

		if (strcmp(waybuffer, "_sign") == 0) {//ע��
			printf("ע��\n");
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
		if (strcmp(waybuffer, "_log") == 0) {//��¼
			printf("��¼\n");

			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			//�û���������
			if (strcmp(waybuffer, "username_wrong") == 0) {
				log_sign_statu = 1;
				//return 1;
			}
			//�������
			if (strcmp(waybuffer, "password_wrong") == 0) {
				log_sign_statu = 2;
				//return 2;
			}
			//������ȷ ��½�ɹ�
			if (strcmp(waybuffer, "password_right") == 0) {
				log_sign_statu = 3;
				//return 3;
			}
			//�û��Ѿ�����¼
			if (strcmp(waybuffer, "user_loged") == 0) {
				log_sign_statu = 7;
				//return 7;
			}


		}
		
		if (strcmp(waybuffer, "_userinfor") == 0) {//�û���Ϣ
			printf("�û���Ϣ��ȡ\n");
			
			//username��ʶ
			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			
			//usernameʵ��
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			//fitpos secondpos
			sscanf(recvbuffer + recvbufferidx, "%s", optionpos);
			recvbufferidx += strlen(optionpos) + 1;
				
			if (strcmp(optionpos, "firstpos") == 0) {
				play_head_statu[1] = 0;//���1�Ѿ����� -����������δ�����˳�

				strcpy(user_1.username, kepname);
				//score��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//scoreʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;

				user_1.score = atoi(waybuffer);


				//max_score��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//max_score ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.max_score = atoi(waybuffer);


				//win_rate��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_rate ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.win_rate = atof(waybuffer);


				//total_match��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//total_matchʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.total_match = atoi(waybuffer);

				//win_count��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_countʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.win_count = atoi(waybuffer);

				//lose_count ��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//lose_count ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.lose_count = atoi(waybuffer);


				//draw_count  ��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//draw_count  ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.draw_count = atoi(waybuffer);
				

				//head_id  ��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//head_id  ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_1.head_id = atoi(waybuffer);
			}
			else {
				play_head_statu[2] = 0;//���2�Ѿ����� -����������δ�����˳�

				strcpy(user_2.username, kepname);
				//score��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//scoreʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;

				user_2.score = atoi(waybuffer);


				//max_score��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//max_score ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.max_score = atoi(waybuffer);


				//win_rate��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_rate ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.win_rate = atof(waybuffer);


				//total_match��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//total_matchʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.total_match = atoi(waybuffer);

				//win_count��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//win_countʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.win_count = atoi(waybuffer);

				//lose_count ��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//lose_count ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.lose_count = atoi(waybuffer);


				//draw_count  ��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//draw_count  ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.draw_count = atoi(waybuffer);


				//head_id  ��ʶ
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//head_id  ʵ��
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				user_2.head_id = atoi(waybuffer);
			}
			std::cout << "user1: " << "\n";
			user_1.debug();
			std::cout << "user2: " << "\n";
			user_2.debug();
		}
		if (strcmp(waybuffer, "_readyinfor") == 0) {//���׼����Ϣ
			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			//���1/2׼��״̬����
			if (strcmp(waybuffer, "firstpos") == 0) {
				user_1.ready_ok = 1;
			}
			else if(strcmp(waybuffer, "secondpos") == 0) {
				user_2.ready_ok = 1;
			}
		}
		if (strcmp(waybuffer, "_chat") == 0) {//������Ϣ
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
			//ά������
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

		if (strcmp(waybuffer, "_gamestart") == 0) //��Ϸ��ʼ
		{
			gamestart_statu = 1;
			game_option_statu[1] = 0;//����
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 1;//����

			lastx = -1, lasty = -1;

		}

		if (strcmp(waybuffer, "_round") == 0)//˭�Ļغ�
		{
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			//���1/2׼��״̬����
			if (strcmp(kepname, myname) == 0) {
				myround = 1;//��������ҵĻغ� �������
				game_option_statu[2] = 0;
			}
			else {
				myround = 0;
				int full = 1;//����������Ƿ�ȫΪ0
				for (int f = 0; f < 15; ++f) {
					for (int o = 0; o < 15; ++o) {
						if (borad[f][o] != 0)full = 0;
					}
				}
				if(full == 0)
					game_option_statu[2] = 1;//�����ǰ�����ҵĻغ��������������� ��������
				else
					game_option_statu[2] = 0;//�����ǰ�����ҵĻغ���������û������ ��������

			}
		}

		if (strcmp(waybuffer, "_win") == 0)//Ӯ��
		{
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			//���1/2׼��״̬����
			if (strcmp(kepname, myname) == 0) {
				MessageBox(GetHWnd(), L"��ϲ�㣬ʤ����!", L"��Ϸ���", MB_OK);
			}
			else {
				MessageBox(GetHWnd(), L"���ź���ʧ����!", L"��Ϸ���", MB_OK);
			}


			//��Ϸ������״̬����
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//��Ϸ����
			game_option_statu[1] = 1;//����
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;

			//׼��״̬Ҫ���� 
		}

		if (strcmp(waybuffer, "_move") == 0)//
		{
			//serversend
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			if (strcmp(kepname, "serversend") == 0) {
				sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
				recvbufferidx += strlen(waybuffer) + 1;
				//���1/2׼��״̬����
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

		if (strcmp(waybuffer, "_other_want_to_draw") == 0)//���ܶԷ����ͺ������� ��������Ӧ
		{
			//���� ��Ӧ�¼�
			int id = MessageBox(GetHWnd(), L"�Է�������壬�Ƿ�ͬ��?", L"����", MB_ICONQUESTION| MB_YESNO);
			printf("draw id = %d\n", id);
			//IDYES(6)
			//IDNO(7)
			sprintf(sendbuffer, "_answer_draw_result %d ",((id==6)?1:0));
			send(fd, sendbuffer, strlen(sendbuffer), 0);
			printf("Ask draw  result %s\n", sendbuffer);
			
		}

		if (strcmp(waybuffer, "_draw_is_ok") == 0)//ͬ�����
		{
			//���� ��Ӧ�¼�
			int id = MessageBox(GetHWnd(), L"����ɹ���״̬������", L"����", MB_OK);

			game_option_statu[3] = 1;//��ԭ

			//��Ϸ������״̬����
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//��Ϸ����
			game_option_statu[1] = 1;//����
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
			//׼��״̬Ҫ���� 
		}

		if (strcmp(waybuffer, "_draw_is_fail") == 0)//��ͬ�����
		{
			//���� ��Ӧ�¼�
			int id = MessageBox(GetHWnd(), L"�Է��ܾ�����ĺ�������!", L"����ʧ��", MB_OK);

			game_option_statu[3] = 1;//��ԭ
		}	

		if (strcmp(waybuffer, "_capitulate_is_ok") == 0)//��Ӧ�Լ��Ѿ�����
		{
			//���� ��Ӧ�¼�
			MessageBox(GetHWnd(), L"���ź���ʧ����!", L"��Ϸ���", MB_OK);
			//��Ϸ������״̬����
			memset(borad, 0, sizeof borad);

			game_option_statu[4] = 1;//��ԭ

			gamestart_statu = 0;//��Ϸ����
			game_option_statu[1] = 1;//����
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
			//׼��״̬Ҫ���� 
		}
		if (strcmp(waybuffer, "_other_is_capitulate") == 0)//��Ӧ�Է��Ѿ�����
		{
			//���� ��Ӧ�¼�
			MessageBox(GetHWnd(), L"�Է������䣡��ϲ�㣬ʤ����!", L"��Ϸ���", MB_OK);
			//��Ϸ������״̬����
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//��Ϸ����
			game_option_statu[1] = 1;//����
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
			//׼��״̬Ҫ���� 
		}
		if (strcmp(waybuffer, "_other_return_to_menu") == 0) {//���ܶԷ��˵��¼���Ϣ
			
			//��Ϸ������״̬����

			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			if (strcmp(kepname, "_no_game_start") == 0) {

				memset(borad, 0, sizeof borad);
				gamestart_statu = 0;//��Ϸ����
				lastx = -1, lasty = -1;
				//�����¼����� �Է��¼��ı�
				//game_option_statu[1] = 1;//����
				//game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

				if (strcmp(user_1.username, myname) == 0) {//����������û�1
					user_2.ready_ok = 0;//��նԷ�׼��״̬
					play_head_statu[2] = -1;//�Է�δ������Ϸ
				}
				else {//����������û�2
					user_1.ready_ok = 0;//��նԷ�׼��״̬
					play_head_statu[1] = -1;//�Է�δ������Ϸ
				}
			}
			else if (strcmp(kepname, "_game_start") == 0) {//��Ϸ�Ѿ���ʼ

				MessageBox(GetHWnd(), L"�Է����˳�����ϲ�㣬ʤ����!", L"��Ϸ���", MB_OK);

				//�����¼����ص���Ϸδ׼��״̬
				memset(borad, 0, sizeof borad);//�������
				gamestart_statu = 0;//��Ϸ����
				lastx = -1, lasty = -1;
				game_option_statu[1] = 1;//����
				game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����init_chessborad

				if (strcmp(user_1.username, myname) == 0) {//����������û�1
					user_2.ready_ok = 0;//��նԷ�׼��״̬
					user_1.ready_ok = 0;//����Լ�׼��״̬
					play_head_statu[2] = -1;//�Է�δ������Ϸ
				}
				else {//����������û�2
					user_1.ready_ok = 0;//��նԷ�׼��״̬
					user_2.ready_ok = 0;//����Լ�׼��״̬
					play_head_statu[1] = -1;//�Է�δ������Ϸ
				}
			}

			else {
				puts("_other_return_to_menu ����");
				exit(0);
			}

			
		}

		if (strcmp(waybuffer, "_exit") == 0) {//���ܶԷ���������Ͽ�������Ϣ
			//firstpos/secondpos
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;

			sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
			recvbufferidx += strlen(waybuffer) + 1;
			if (strcmp(waybuffer, "_no_game_start") == 0) {

				memset(borad, 0, sizeof borad);
				gamestart_statu = 0;//��Ϸ����
				lastx = -1, lasty = -1;
				//�����¼����� �Է��¼��ı�
				//game_option_statu[1] = 1;//����
				//game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

				if (strcmp(kepname, "firstpos") == 0) {//����Է����û�1
					user_1.ready_ok = 0;//��նԷ�׼��״̬
					play_head_statu[1] = -1;//�Է�δ������Ϸ
				}
				else {//����Է����û�2
					play_head_statu[2] = -1;//�Է�δ������Ϸ
					user_2.ready_ok = 0;//��նԷ�׼��״̬
				}
			}
			else if (strcmp(waybuffer, "_game_start") == 0) {
				
				MessageBox(GetHWnd(), L"�Է����˳�����ϲ�㣬ʤ����!", L"��Ϸ���", MB_OK);

				//�����¼����ص���Ϸδ׼��״̬
				memset(borad, 0, sizeof borad);//�������
				gamestart_statu = 0;//��Ϸ����
				lastx = -1, lasty = -1;
				game_option_statu[1] = 1;//����
				game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

				user_2.ready_ok = 0;//��նԷ�׼��״̬
				user_1.ready_ok = 0;//��նԷ�׼��״̬

				if (strcmp(kepname, "firstpos") == 0) {//����Է����û�1
					user_1.ready_ok = 0;//��նԷ�׼��״̬
					play_head_statu[1] = -1;//�Է�δ������Ϸ
				}
				else {//����Է����û�2
					play_head_statu[2] = -1;//�Է�δ������Ϸ
					user_2.ready_ok = 0;//��նԷ�׼��״̬
				}

			}
			else {
				puts("������������Ͽ�������Ϣ�����쳣.");
				exit(0);
			}

		}

		if (strcmp(waybuffer, "_other_want_to_regret") == 0) {//���ܶԷ��Ļ������󲢻�Ӧ
			//���� ��Ӧ�¼�
			int id = MessageBox(GetHWnd(), L"�Է�������壬�Ƿ�ͬ��?", L"����", MB_ICONQUESTION | MB_YESNO);
			printf("draw id = %d\n", id);
			//IDYES(6)
			//IDNO(7)
			sprintf(sendbuffer, "_answer_regret_result %d ", ((id == 6) ? 1 : 0));
			send(fd, sendbuffer, strlen(sendbuffer), 0);
			printf("Ask regret result %s\n", sendbuffer);
		}
		//---
		if (strcmp(waybuffer, "_regret_is_ok") == 0) {//���ܷ������Ļ���������
			MessageBox(GetHWnd(), L"�Է�ͬ������Ļ�������!", L"����", MB_OK);
			//x
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			int x = atoi(kepname);
			//y
			sscanf(recvbuffer + recvbufferidx, "%s", kepname);
			recvbufferidx += strlen(kepname) + 1;
			int y = atoi(kepname);
			printf("x =%d y = %d\n", x, y);
			borad[x][y] = 0;//����
		}
		if (strcmp(waybuffer, "_regret_is_fail") == 0) {//���ܷ������Ļ���������
			MessageBox(GetHWnd(), L"�Է��ܾ�����Ļ�������!", L"����", MB_OK);
		}
		if (strcmp(waybuffer, "_regret_chess") == 0) {//���ܷ������Ļ���������

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
		if (strcmp(waybuffer, "_dogfall") == 0) {//�������������ĺ�����
			MessageBox(GetHWnd(), L"�Ծ��ѽ��������Ϊƽ��!", L"ƽ��", MB_OK);
			memset(borad, 0, sizeof borad);

			gamestart_statu = 0;//��Ϸ����
			game_option_statu[1] = 1;//����
			game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

			user_1.ready_ok = 0;
			user_2.ready_ok = 0;
			lastx = -1, lasty = -1;
		}
	}
	return -1;
}

// ��ť btnOK login��¼ҳ�� �ĵ���¼�
void On_btnOk_Click()
{

	//��Ҫ����û����Ƿ���� 
	/*
	��recvbuffer���յ�3���ַ���
	username wrong  ��ʾ�û���������
	password wrong	��ʾ�������
	password right	��ʾ������ȷ����¼�ɹ�
	*/


	//�ȼ���û������������Ƿ�Ϊ��
	bool flag = false;
	bool check = true;//�ж��û�����ʽ�Ƿ�Ϸ�

	int wlen = wcslen(login_name.Text());
	for (int i = 0; i < wlen; ++i) {
		if (!(isalpha(login_name.Text()[i]) || isdigit(login_name.Text()[i]))) {
			check = false;
		}
	}



	if (wcslen(login_name.Text()) == 0) {
		MessageBox(GetHWnd(), L"�û�������Ϊ��", L"����", MB_OK); flag = true;
	}
	if (!check) {
		//�û�����ֻ�������ֺ��ַ�
		MessageBox(GetHWnd(), L"�û�����ֻ�������ֺ��ַ�", L"����", MB_OK); 
		return ;
	}
	if (wcslen(login_password.Text()) == 0) {
		MessageBox(GetHWnd(), L"���벻��Ϊ��", L"����", MB_OK); flag = true;
	}
	

	printf("flag : %d check: %d\n", flag,check);
	if (flag)return;

	//������Ϣ��������Ϣ
	memset(sendbuffer, 0, sizeof(sendbuffer));
	sprintf(sendbuffer, "_log %S %S ", login_name.Text(), login_password.Text());//����log��־����ʾ��½��Ϣ
	send(fd, sendbuffer, strlen(sendbuffer), 0);

	memset(recvbuffer, 0, sizeof(recvbuffer));
	recv(fd, recvbuffer, RECVBUFFERSIZE, 0);
	int ret_option = domessage();

	//�û���������
	if (log_sign_statu == 1) {

		MessageBox(GetHWnd(), L"�û���������", L"����", MB_OK);
	}

	//�������
	if (log_sign_statu == 2) {
		MessageBox(GetHWnd(), L"�������", L"����", MB_OK);
	}

	//������ȷ ��½�ɹ�
	if (log_sign_statu == 3)
	{
		sprintf(myname,"%S", login_name.Text());//��¼�Լ��û���
		menu_statu = 3;//��ת��Ϸ����
		wchar_t s[100] = L"Hello, ";
		wcscat_s(s, 100, login_name.Text());
		MessageBox(GetHWnd(), s, L"Hello", MB_OK);
	}

	if (log_sign_statu == 7) {
		MessageBox(GetHWnd(), L"���û�Ŀǰ�ѱ���¼", L"����", MB_OK);
	}
}


// btnOK regisע��ҳ�� �ĵ���¼�
void On_btnOK_register_Click()
{

	bool flag = false;
	bool check = true;//�ж��û�����ʽ�Ƿ�Ϸ�

	int wlen = wcslen(register_name.Text());
	for (int i = 0; i < wlen; ++i) {
		if (!(isalpha(register_name.Text()[i]) || isdigit(register_name.Text()[i]))) {
			check = false;
		}
	}


	if (wcslen(register_name.Text()) == 0) {
		MessageBox(GetHWnd(), L"�û�������Ϊ��", L"����", MB_OK);
		flag = true;
	}
	if (!check) {
		//�û�����ֻ�������ֺ��ַ�
		MessageBox(GetHWnd(), L"�û�����ֻ�������ֺ��ַ�", L"����", MB_OK);
		return;
	}
	if (wcslen(register_password.Text()) == 0) {
		MessageBox(GetHWnd(), L"���벻��Ϊ��", L"����", MB_OK);
		flag = true;
	}
	if (flag)return;
	memset(sendbuffer, 0, sizeof(sendbuffer));
	sprintf(sendbuffer, "_sign %S %S ", register_name.Text(), register_password.Text());//����sign��־����ʾע����Ϣ
	send(fd, sendbuffer, strlen(sendbuffer), 0);
	
	memset(recvbuffer, 0, sizeof(recvbuffer));
	recv(fd, recvbuffer, RECVBUFFERSIZE, 0);
	int ret_option = domessage();


	/*
	recvbuffer���յ�������Ϣ
	username exist  ��ʾ�û����Ѵ���
	sign success  ��ʾע��ɹ�
	*/
	printf("regis : %s\n", recvbuffer);

	if (log_sign_statu == 4)
	{
		wchar_t s[100] = L"�û����Ѵ���, ";
		wcscat_s(s, 100, register_name.Text());
		MessageBox(GetHWnd(), s, L"����", MB_OK);
	}


	if (log_sign_statu == 5)
	{
		wchar_t s[100] = L"ע��ɹ�, ";
		wcscat_s(s, 100, register_name.Text());
		MessageBox(GetHWnd(), s, L"Hello", MB_OK);
	}
}

//��¼תע�� ����¼�
void On_btn_login_to_regis_Click()
{
	menu_statu = 2;
}

//ע��ת��¼ ����¼�
void On_btn_regis_to_login_Click() {
	menu_statu = 1;
}

void startup() {
	
	initgraph(High, Width, SHOWCONSOLE);
	login_name.Create(440, 290, 620, 330, 10); // �����û����ı���ؼ�
	login_password.Create(440, 390, 620, 430, 10);	//�����û��� �����ı���ؼ�
	btnOK.Create(635, 500, 675, 535, L"��¼", On_btnOk_Click);
	btn_login_to_regis.Create(490, 500, 610, 535, L"û���˺�?ע��", On_btn_login_to_regis_Click);//�л���ע��


	register_name.Create(440, 290, 620, 330, 10);
	register_password.Create(440, 390, 620, 430, 10);
	btnOK_register.Create(635, 500, 675, 535, L"ע��", On_btnOK_register_Click);

	btn_regis_to_login.Create(490, 500, 610, 535, L"�����˺�?��¼", On_btn_regis_to_login_Click);//�л��ص�¼
	
	//�����
	chat_input_box.Create(1080 - 265, 533, 1080 - 67, 568, 13);

}

//��¼����
void login() {

	putimage(0, 0, &login_bakeground);

	m = getmessage(EM_MOUSE);
	//����ɫԲ�Ǿ���
	setfillcolor(WHITE);
	fillroundrect(300, 250, 750, 550, 20, 20);

	setbkcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(16, 0, _T("΢�����"));
	outtextxy(350, 300, L"�û���: ");

	//440-620 ����   290-330 ���
	login_name.Show();

	setbkcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(16, 0, _T("΢�����"));
	outtextxy(350, 400, L"�ܡ��룺");
	login_password.Show();

	btnOK.Show();
	btn_login_to_regis.Show();

	if (m.message == WM_LBUTTONDOWN) {
		// �ж� �û����ؼ�
		if (login_name.Check(m.x, m.y))
			login_name.OnMessage();

		//�ж� ����ؼ�
		if (login_password.Check(m.x, m.y))
			login_password.OnMessage();

		// �жϿؼ�
		if (btnOK.Check(m.x, m.y))
			btnOK.OnMessage();

		//�жϿؼ�  ��¼תע�� ��ť
		if (btn_login_to_regis.Check(m.x, m.y))
			btn_login_to_regis.OnMessage();
	}
}

void regis() {

	putimage(0, 0, &login_bakeground);

	m = getmessage(EM_MOUSE);
	//����ɫԲ�Ǿ���
	setfillcolor(WHITE);
	fillroundrect(310, 260, 740, 540, 20, 20);

	setbkcolor(WHITE);
	settextcolor(BLACK);
	outtextxy(350, 300, L"ע���û���: ");

	//440-620 ����   290-330 ���
	register_name.Show();

	setbkcolor(WHITE);
	settextcolor(BLACK);
	outtextxy(350, 400, L"ע�����룺");
	register_password.Show();
	btnOK_register.Show();
	btn_regis_to_login.Show();

	if (m.message == WM_LBUTTONDOWN) {
		// �ж� �û����ؼ�
		if (register_name.Check(m.x, m.y))
			register_name.OnMessage();

		//�ж� ����ؼ�
		if (register_password.Check(m.x, m.y))
			register_password.OnMessage();

		// �жϿؼ�
		if (btnOK_register.Check(m.x, m.y))
			btnOK_register.OnMessage();
		//�ж� ע��ת��¼ ��ť
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
	//ͷ��λ��
	//circle(60, 60, 50);
	// 
	

	peekmessage(&m, EM_MOUSE, false);

	if (play_head_statu[1] == -1) {//δ����

	}
	else {

		//����


		//����Ƿ���Բ��
		if (play_head_statu[1] == 0 && (60 - m.x) * (60 - m.x) + (60 - m.y) * (60 - m.y) <= 50 * 50) {

			if (m.message == WM_LBUTTONDOWN) {
				play_head_statu[1] = 1;//�л���������Ϣҳ��
			}

		}
		if (play_head_statu[1] == 0) {
			putimage(10, 10, &game_player_head[6], 0x008800C6);
			putimage(10, 10, &game_player_head[user_1.head_id], 0x00EE0086);

			//�ǳ�
			roundrect(10, 120, 110, 140, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("΢�����"));
			outtextxy(13, 122, _T("�ǳ�: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"), user_1.username);
			//printf("%S\n", kep);
			outtextxy(50, 122, kep);// 7��������ʾ����

			//ʱ���
			//setcolor(RED);
			//fillrectangle(115, 10, 235, 145);
			// 
			//״̬��
			//circle(95, 95, 12);
			if (user_1.ready_ok) {//���1��׼����
				putimage(95 - 12, 95 - 12, &play_msg_ok[1], 0x008800C6);
				putimage(95 - 12, 95 - 12, &play_msg_ok[2], 0x00EE0086);
			}

		}
		else {

			//�ǳ�
			roundrect(10, 20, 110, 40, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("΢�����"));
			outtextxy(13, 24, _T("�ǳ�: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"),user_1.username);
			//printf("%S\n", kep);
			outtextxy(50, 24, kep);// 7��������ʾ����
			
			//���ֿ�
			roundrect(10, 50, 110, 140, 10, 10);
			
			//����
			outtextxy(13, 63, _T("����: "));
			_stprintf(kep, _T("%d"), user_1.score);
			outtextxy(13 + 40, 63, kep);

			//��߻���
			settextstyle(14, 0, _T("΢�����"));
			outtextxy(13, 93, _T("��߻���: "));
			_stprintf(kep, _T("%d"), user_1.max_score);
			outtextxy(13 + 65 , 93, kep);

			//ʤ��
			//roundrect(125, 70, 230, 135, 10, 10);
			settextstyle(19, 0, _T("����"));
			outtextxy(120, 15, _T("ʤ��: "));
			_stprintf(kep, _T("%.1f%%"), user_1.win_rate * 100);
			outtextxy(120 + 55, 15,kep);

			//�ܾ���
			_stprintf(kep, _T("%d"), user_1.total_match);
			outtextxy(120, 45, _T("�ܾ���: "));
			outtextxy(120 + 70, 45, kep);

			roundrect(120, 70, 225, 135, 10, 10);
			settextstyle(18, 0, _T("����"));
			
			//ʤ��
			_stprintf(kep, _T("%d"), user_1.win_count);
			outtextxy(125, 75, _T("ʤ����"));
			outtextxy(125 + 45, 75, kep);

			//����
			_stprintf(kep, _T("%d"), user_1.lose_count);
			outtextxy(125, 95, _T("������"));
			outtextxy(125 + 45, 95, kep);

			//ƽ��
			_stprintf(kep, _T("%d"), user_1.draw_count);
			outtextxy(125, 115, _T("ƽ�֣�"));
			outtextxy(125 + 45, 115, kep);


			//�˳���ť
			putimage(218, 10, &play_msg_exit[1], 0x008800C6);
			putimage(218, 10, &play_msg_exit[2], 0x00EE0086);

			if (m.x >= 218 && m.x <= 218 + 20 && m.y >= 10 && m.y <= 10 + 20) {
				//�������ڵ�����˳�
				if (m.message == WM_LBUTTONDOWN) {
					play_head_statu[1] = 0;//����״̬
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
	

	if (play_head_statu[2] == -1) {//δ����

	}
	else {
		//�Ѿ���

		//����Ƿ���Բ��
		//printf("circle check %d\n", (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50);
		if (play_head_statu[2] == 0 && (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50) {

			if (m.message == WM_LBUTTONDOWN) {
				play_head_statu[2] = 1;//�л���������Ϣҳ��
			}

		}
		//printf("play_head_statu check %d\n", play_head_statu[2]);

		//ͷ��λ��
		//circle(60, 215, 50);
		if (play_head_statu[2] == 0) {
			//״̬��
			//circle(95, 250, 12);
			putimage(10, 165, &game_player_head[6], 0x008800C6);
			putimage(10, 165, &game_player_head[user_2.head_id], 0x00EE0086);
			
			//�ǳ�
			roundrect(10, 280, 110, 300, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("΢�����"));

			outtextxy(13, 184 + 98, _T("�ǳ�: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"), user_2.username);
			//printf("%S\n", kep);
			outtextxy(50, 184 + 98, kep);// 7��������ʾ����
			
			//ʱ���
			//setcolor(RED);
			//fillrectangle(115, 165, 235, 300);
			if (user_2.ready_ok) {//���2��׼����
				putimage(60 + 35 - 12, 215 + 35 - 12, &play_msg_ok[1], 0x008800C6);
				putimage(60 + 35 - 12, 215 + 35 - 12, &play_msg_ok[2], 0x00EE0086);
			}
		}
		else {
			//�ǳ�
			roundrect(10, 180, 110, 200, 10, 10);
			settextcolor(BLACK);
			setbkmode(TRANSPARENT);
			settextstyle(15, 0, _T("΢�����"));

			outtextxy(13, 184, _T("�ǳ�: "));
			//printf("%s\n", user_1.username);
			_stprintf(kep, _T("%S"), user_2.username);
			//printf("%S\n", kep);
			outtextxy(50, 184, kep);// 7��������ʾ����

			//���ֿ�
			roundrect(10, 210, 110, 295, 10, 10);
			
			//����
			outtextxy(13, 223, _T("����: "));
			_stprintf(kep, _T("%d"), user_2.score);
			outtextxy(13 + 40, 223, kep);

			//��߻���
			settextstyle(14, 0, _T("΢�����"));
			outtextxy(13, 253, _T("��߻���: "));
			_stprintf(kep, _T("%d"), user_2.max_score);
			outtextxy(13 + 65, 253, kep);
			
			//ʤ��
			//roundrect(125, 225, 230, 290, 10, 10);
			settextstyle(19, 0, _T("����"));
			outtextxy(120, 170, _T("ʤ��: "));
			_stprintf(kep, _T("%.1f%%"), user_2.win_rate * 100);
			outtextxy(120 + 55, 170, kep);

			//�ܾ���
			_stprintf(kep, _T("%d"), user_2.total_match);
			outtextxy(120, 200, _T("�ܾ���: "));
			outtextxy(120 + 70, 200, kep);

			
			roundrect(120, 225, 225, 290, 10, 10);
			settextstyle(18, 0, _T("����"));
			
			//ʤ��
			_stprintf(kep, _T("%d"), user_2.win_count);
			outtextxy(125, 230, _T("ʤ����"));
			outtextxy(125 + 45, 230, kep);

			
			//����
			_stprintf(kep, _T("%d"), user_2.lose_count);
			outtextxy(125, 250, _T("������"));
			outtextxy(125 + 45, 250, kep);

			//ƽ��
			_stprintf(kep, _T("%d"), user_2.draw_count);
			outtextxy(125, 270, _T("ƽ�֣�"));
			outtextxy(125 + 45, 270, kep);


			//�˳���ť
			
			putimage(218, 165, &play_msg_exit[1], 0x008800C6);
			putimage(218, 165, &play_msg_exit[2], 0x00EE0086);

			if (m.x >= 218 && m.x <= 218 + 20 && m.y >= 165 && m.y <= 165 + 20) {
				//�������ڵ�����˳�
				if (m.message == WM_LBUTTONDOWN) {
					play_head_statu[2] = 0;//����״̬
				}
			}
			
		}
	}
}


void ready() //׼��
{
	//׼�� 235 * 71
	
	peekmessage(&m, EM_MOUSE, false);
	//m = getmessage(EM_MOUSE);
	if (game_option_statu[1]) {
		//����
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 313 && m.y <= 313 + 71) {
			//��������
			//�������
			if (m.message == WM_LBUTTONDOWN ) {
				//׼�� ���� ��С
				//���� ���Լ���׼����Ϣ
				sprintf(sendbuffer, "_myreadyinfor ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);

				game_option_statu[1] = 0;//׼�����º�׼��������

				putimage(5, 313, &game_option[18], 0x008800C6);//׼��_��������
				putimage(5, 313, &game_option[17], 0x00EE0086);//׼��_����
			}
			else {
				putimage(5, 313, &game_option[1], 0x008800C6);//׼��01
				putimage(5, 313, &game_option[2], 0x00EE0086);//׼��02
			}
		}
		else {
			//���� ����������
			putimage(5, 313, &game_option[1], 0x008800C6);//׼��01
			putimage(5, 313, &game_option[2], 0x00EE0086);//׼��02
		}
	}
	else {
		//������ ��ɫ
		putimage(5, 313, &game_option[1], 0x008800C6);//׼��01
		putimage(5, 313, &game_option[19], 0x00EE0086);//׼��_����
	}
}

void repentance() //����
{
	//BeginBatchDraw();
	//���� 235 * 71
	//m = getmessage(EM_MOUSE);
	peekmessage(&m, EM_MOUSE, false);

	if (game_option_statu[2] && lastx != -1) {
		//����
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 388 && m.y <= 388 + 71) {
			//��������
			//�������
			if (m.message == WM_LBUTTONDOWN) {
				//���� ���� ��С

				//���� ���Լ��Ļ�����Ϣ
				sprintf(sendbuffer, "_I_want_to_regret ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);
				game_option_statu[2] = 0;//���º���屻����

				putimage(5, 388, &game_option[9], 0x008800C6);
				putimage(5, 388, &game_option[11], 0x00EE0086);

			}
			else {
				putimage(5, 388, &game_option[10], 0x008800C6);
				putimage(5, 388, &game_option[12], 0x00EE0086);
			}
		}
		else {
			//���� ����������
			putimage(5, 388, &game_option[10], 0x008800C6);
			putimage(5, 388, &game_option[12], 0x00EE0086);
		}
	}
	else {
		//������ ��ɫ
		putimage(5, 388, &game_option[3], 0x008800C6);
		putimage(5, 388, &game_option[4], 0x00EE0086);
	}
	//FlushBatchDraw();
}
void positional_draw() //����
{
	//���� 235 * 71
	//m = getmessage(EM_MOUSE);
	peekmessage(&m, EM_MOUSE, false);
	
	if (game_option_statu[3]) {
		//����
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 463 && m.y <= 463 + 71) {
			//��������
			//�������
			if (m.message == WM_LBUTTONDOWN) {
				//���� ���� ��С
				//���ͺ�������
				sprintf(sendbuffer, "_I_want_to_draw ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);

				game_option_statu[3] = 0;//���º���屻����

				putimage(5, 463, &game_option[9], 0x008800C6);
				putimage(5, 463, &game_option[13], 0x00EE0086);
			}
			else {
				putimage(5, 463, &game_option[10], 0x008800C6);
				putimage(5, 463, &game_option[14], 0x00EE0086);
			}
		}
		else {
			//���� ����������
			putimage(5, 463, &game_option[10], 0x008800C6);
			putimage(5, 463, &game_option[14], 0x00EE0086);
		}
	}
	else {
		//������ ��ɫ
		putimage(5, 463, &game_option[5], 0x008800C6);
		putimage(5, 463, &game_option[6], 0x00EE0086);
	}
}

void concede()//����
{
	//���� 235 * 71
	//m = getmessage(EM_MOUSE);
	peekmessage(&m, EM_MOUSE, false);
	if (game_option_statu[4]) {
		//����
		if (m.x >= 5 && m.x <= 5 + 235 && m.y >= 538 && m.y <= 538 + 71) {
			//��������
			//�������
			if (m.message == WM_LBUTTONDOWN) {
				//���� ���� ��С
				// 
				//������������
				sprintf(sendbuffer, "_I_capitulate ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);

				game_option_statu[4] = 0;//���º����䱻����

				putimage(5, 538, &game_option[9], 0x008800C6);
				putimage(5, 538, &game_option[15], 0x00EE0086);
			}
			else {
				putimage(5, 538, &game_option[10], 0x008800C6);
				putimage(5, 538, &game_option[16], 0x00EE0086);
			}
		}
		else {
			//���� ����������
			putimage(5, 538, &game_option[10], 0x008800C6);
			putimage(5, 538, &game_option[16], 0x00EE0086);
		}
	}
	else {
		//������ ��ɫ
		putimage(5, 538, &game_option[7], 0x008800C6);
		putimage(5, 538, &game_option[8], 0x00EE0086);
	}
}

void setting() {
	//��1 280 * 80
	setcolor(BLACK);
	//fillrectangle(1080 - 5 - 280, 5, 1080 - 5, 5 + 80);
	rectangle(1080 - 5 - 280, 5, 1080 - 5, 5 + 80); 
	putimage(1080 - 5 - 279, 6, &sys_option[3]);//����

	
	//���ز˵�
	//peekmessage(&m, EM_MOUSE, true);
	
	ExMessage msg;
	

	//tmpMsg = m;
	//ExMessage msg2 = {0};
	peekmessage(&msg, EM_MOUSE, true);
	{
		//tmpMsg = msg;
		//���ز˵�
		putimage(1080 - 265, 10, &sys_option[6], 0x008800C6);
		putimage(1080 - 265, 10, &sys_option[7], 0x00EE0086);

		//��Ϸ˵��
		putimage(1080 - 180, 10, &sys_option[10], 0x008800C6);
		putimage(1080 - 180, 10, &sys_option[11], 0x00EE0086);

		//����
		putimage(1080 - 95, 10, &sys_option[14], 0x008800C6);
		putimage(1080 - 95, 10, &sys_option[15], 0x00EE0086);

		if (msg.message == WM_LBUTTONDOWN){
			if (msg.x >= 1080 - 265 && msg.x <= 1080 - 195 && msg.y >= 10 && msg.y <= 80) {
				//�������	�˵�
				menu_statu = 0;

				//����֮ǰ���Ѿ�������Ӧ��
				//��Ϸ������״̬����
				memset(borad, 0, sizeof borad);

				menu_statu = 0;//������Ϸ�˵�
				
				gamestart_statu = 0;//���ۿ�û��ʼ��Ϸ����Ϸ������

				game_option_statu[1] = 1;//����
				game_option_statu[2] = game_option_statu[3] = game_option_statu[4] = 0;//����

				user_1.ready_ok = 0;
				user_2.ready_ok = 0;
				//׼��״̬Ҫ���� 
				
				init_chessborad = 0;//��ʼ����״̬����

				//sendһ�������˳�����Ϣ��������
				sprintf(sendbuffer, "_I_return_to_menu ");
				send(fd, sendbuffer, strlen(sendbuffer), 0);
				//���� ��¼ ע��״̬����
				log_sign_statu = -1;
			}
			if (msg.x >= 1080 - 180 && msg.x <= 1080 - 110 && msg.y >= 10 && msg.y <= 80) {
				//�������	˵��
				MessageBox(GetHWnd(), L"ʤ�����������������˫���໥���ݣ�ֱ��һ�����ⷽ���������ʤ����\n�Ծ�ʹ���޽��ֹ��򣬻����������һ�Ρ�\n��˫����׼����ʼ�Ծ֣����ڹ涨ʱ�������������Ծ�\n", L"��Ϸ˵��", MB_OK);

			}
			if (msg.x >= 1080 - 95 && msg.x <= 1080 - 25 && msg.y >= 10 && msg.y <= 80) {
				//����
				int id  = MessageBox(GetHWnd(), L"�Ƿ������ǰ����ͼƬ?\n", L"��Ϸ����",MB_ICONQUESTION | MB_YESNO);
				if (id == 6)init_chessborad = (1 - init_chessborad);//��תͼƬ
			}
		}
		
	}
	
	
	
	{
		//���ز˵�
		putimage(1080 - 265, 10, &sys_option[6], 0x008800C6);
		putimage(1080 - 265, 10, &sys_option[7], 0x00EE0086);

		//��Ϸ˵��
		putimage(1080 - 180, 10, &sys_option[10], 0x008800C6);
		putimage(1080 - 180, 10, &sys_option[11], 0x00EE0086);

		//����
		putimage(1080 - 95, 10, &sys_option[14], 0x008800C6);
		putimage(1080 - 95, 10, &sys_option[15], 0x00EE0086);
	}
	
}



void gameview() {


	//235 * 150
	putimage(0, 0, &game_background);
	//����
	chessboard_view();
	
	//��������
	//���1 ��Ϣ
	Play1_view();
	//���2 ��Ϣ
	Play2_view();
	
	ready();//׼��
	repentance();//����
	positional_draw();//����
	concede();//����

	//��1 280 * 275
	chatbox();
	setting();

	FlushBatchDraw();
	flushmessage(EM_MOUSE);//�����Ϣ������
	
}



void process_user_message() //�����û���Ϣ
{
	//������Ϣ��������Ϣ
	//�����׽��ַ�����
	u_long iMode = 1;
	ioctlsocket(fd, FIONBIO, &iMode);
	memset(recvbuffer, 0, sizeof(recvbuffer));
	int ret = recv(fd, recvbuffer, sizeof(recvbuffer), 0);
	//domessage(); 
	if (ret > 0) {
		iMode = 0;
		domessage();
		//puts("23323 inter gameview");
		//gameview();//��Ϸ����
		ioctlsocket(fd, FIONBIO, &iMode);
	}
	else if (ret == -1 && errno==0) {
		//δ�յ���Ϣ ����ͼ��
		//������û�з�����Ϣ
		//gameview();//��Ϸ����
		//puts("inter gameview");
		gameview();//��Ϸ����
		//�û��ڴ˿��Ե��׼����ť���������������Ϣ��
			//��׼����Ϣ��������Ϣ���Բ����͸�����������Ϊ�Է���ʱ��û�н��뷿�䣬�Է�����������

	}
	else {
		//printf("errno %d\n", errno);s
		printf("������recv����\n");
		//�ͷ������Դ
	}
}


void showmain() {

	if (menu_statu == 0)startmenu();//��Ϸ�˵�
	else if (menu_statu == 1)login();//��½��Ϸ
	else if (menu_statu == 2)regis();//ע����Ϸ
	else if (menu_statu == 3) {
		process_user_message();
		FlushBatchDraw();
		//gameview();//��Ϸ����	
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

	//��ʼ����״̬
	setting_statu[1] = setting_statu[2] = setting_statu[3] = 1;

	while (true) {
		showmain();
		FlushBatchDraw();
	}
	EndBatchDraw();
	closegraph();
	closesocket(fd);
	WSACleanup();//ÿ�ν�������ʱ����Ҫ�ǵõ��������д��룬�����ͷ��������������Դ------------------------
	
	/*
	closesocket(fd);
	WSACleanup();//ÿ�ν�������ʱ����Ҫ�ǵõ��������д���
	*/
	return 0;
}