#ifndef INIT_IMAGE
#define INIT_IMAGE

#include <graphics.h>

IMAGE menu_bakeground, chessboard[4], login_bakeground, game_background;//�˵����������̣���¼��������Ϸ����
IMAGE menu_option[10];//�˵�ѡ��ͼƬ
IMAGE sys_option[20];//˵��  ���� �˵� ���ñ���
IMAGE game_option[30];//׼�� ���� ���� ����
IMAGE game_player_head[10];//��Ϸ����ͷ��
IMAGE player_option_background;//�����Ϣ��ʾ����ͼ
IMAGE chat_option[10];//���챳��
IMAGE send_option[10];//���Ͱ�ť
IMAGE play_msg_exit[10];//�����Ϣ�˳�ͼƬ
IMAGE play_msg_ok[10];// ���׼��״̬ͼƬ
IMAGE chessboard_tracet_view[10];//����׷��ͼƬ
IMAGE chessboard_piece_white[10];//���̰�ɫ����ͼƬ
IMAGE chessboard_piece_black[10];//���̺�ɫ����ͼƬ

int init_chessborad = 0;//��ʼ����ͼƬ

void Insert_IMAGE() {//����ͼƬ

	loadimage(&menu_bakeground, L".\\picture\\menu_background.jpg");//����˵�����ͼƬ
	loadimage(&menu_option[1], L".\\picture\\01.png");//����˵�ѡ��1ͼƬ
	loadimage(&menu_option[2], L".\\picture\\02.png");//����˵�ѡ��2ͼƬ
	loadimage(&menu_option[5], _T(".\\picture\\˫�����3.png"));//����˵�ѡ��2ͼƬ


	loadimage(&menu_option[3], _T(".\\picture\\�˳���Ϸ1.png"));//����˵�ѡ��3ͼƬ
	loadimage(&menu_option[4], _T(".\\picture\\�˳���Ϸ2.png"));//����˵�ѡ��3ͼƬ
	loadimage(&menu_option[6], _T(".\\picture\\�˳���Ϸ3.png"));//����˵�ѡ��3ͼƬ


	loadimage(&chessboard[1], _T(".\\picture\\����\\ľ������.png"));//��������ͼƬ
	loadimage(&chessboard[2], _T(".\\picture\\����\\ʯ������.png"));//��������ͼƬ

	loadimage(&login_bakeground, _T(".\\picture\\login_background.png"));
	loadimage(&game_background, _T(".\\picture\\game_background.jpg"));

	loadimage(&game_option[1], _T(".\\picture\\��ť\\׼��01.png"));
	loadimage(&game_option[2], _T(".\\picture\\��ť\\׼��02.png"));

	loadimage(&game_option[3], _T(".\\picture\\��ť\\����01.png"));
	loadimage(&game_option[4], _T(".\\picture\\��ť\\����02.png"));



	loadimage(&game_option[5], _T(".\\picture\\��ť\\����01.png"));
	loadimage(&game_option[6], _T(".\\picture\\��ť\\����02.png"));

	loadimage(&game_option[7], _T(".\\picture\\��ť\\����01.png"));
	loadimage(&game_option[8], _T(".\\picture\\��ť\\����02.png"));

	loadimage(&sys_option[1], _T(".\\picture\\˵��.jpg"));
	loadimage(&sys_option[2], _T(".\\picture\\̫��.jpg"));
	loadimage(&sys_option[3], _T(".\\picture\\���ñ���3.png"));

	loadimage(&sys_option[4], _T(".\\picture\\.png"));//�˵�
	loadimage(&sys_option[5], _T(".\\picture\\.png"));
	loadimage(&sys_option[6], _T(".\\picture\\���ذ�ť001.png"));
	loadimage(&sys_option[7], _T(".\\picture\\.png"));

	loadimage(&sys_option[8], _T(".\\picture\\.png"));//˵��
	loadimage(&sys_option[9], _T(".\\picture\\.png"));
	loadimage(&sys_option[10], _T(".\\picture\\˵��.jpg"));
	loadimage(&sys_option[11], _T(".\\picture\\.png"));

	loadimage(&sys_option[12], _T(".\\picture\\.png"));//����
	loadimage(&sys_option[13], _T(".\\picture\\.png"));
	loadimage(&sys_option[14], _T(".\\picture\\���ð�ť001.png"));
	loadimage(&sys_option[15], _T(".\\picture\\.png"));


	loadimage(&game_player_head[6], _T(".\\picture\\ͷ������.png"));
	loadimage(&game_player_head[0], _T(".\\picture\\ͷ��01.png"));
	loadimage(&game_player_head[1], _T(".\\picture\\ͷ��02.png"));
	loadimage(&game_player_head[2], _T(".\\picture\\ͷ��03.png"));
	loadimage(&game_player_head[3], _T(".\\picture\\ͷ��04.png"));
	loadimage(&game_player_head[4], _T(".\\picture\\ͷ��05.png"));

	loadimage(&game_option[9], _T(".\\picture\\��ť_��������.png"));
	loadimage(&game_option[10], _T(".\\picture\\��ť_����.png"));
	loadimage(&game_option[11], _T(".\\picture\\����_����.png"));
	loadimage(&game_option[12], _T(".\\picture\\����_����.png"));


	loadimage(&game_option[13], _T(".\\picture\\����_����.png"));
	loadimage(&game_option[14], _T(".\\picture\\����_����.png"));

	loadimage(&game_option[15], _T(".\\picture\\����_����.png"));
	loadimage(&game_option[16], _T(".\\picture\\����_����.png"));

	loadimage(&game_option[17], _T(".\\picture\\׼��_����.png"));
	loadimage(&game_option[18], _T(".\\picture\\׼��_��������.png"));
	loadimage(&game_option[19], _T(".\\picture\\׼��_����.png"));

	loadimage(&player_option_background, _T(".\\picture\\�����Ϣ����.jpg"));

	loadimage(&chat_option[1], _T(".\\picture\\���챳��3.jpg"));

	loadimage(&send_option[1], _T(".\\picture\\���Ͱ�ť_����01.png"));
	loadimage(&send_option[2], _T(".\\picture\\���Ͱ�ť_����02.png"));
	loadimage(&send_option[3], _T(".\\picture\\���Ͱ�ť01.png"));
	loadimage(&send_option[4], _T(".\\picture\\���Ͱ�ť02.png"));

	loadimage(&play_msg_exit[1], _T(".\\picture\\�رհ�ť����.png"));
	loadimage(&play_msg_exit[2], _T(".\\picture\\�رհ�ť.png"));

	loadimage(&play_msg_ok[1], _T(".\\picture\\׼��״̬����.png"));
	loadimage(&play_msg_ok[2], _T(".\\picture\\׼��״̬.png"));

	loadimage(&chessboard_tracet_view[1], _T(".\\picture\\׷����ʾ����.png"));
	loadimage(&chessboard_tracet_view[2], _T(".\\picture\\׷����ʾ����.png"));

	loadimage(&chessboard_piece_white[1], _T(".\\picture\\����\\����������.png"));
	loadimage(&chessboard_piece_white[2], _T(".\\picture\\����\\�����.png"));

	loadimage(&chessboard_piece_black[1], _T(".\\picture\\����\\����������.png"));
	loadimage(&chessboard_piece_black[2], _T(".\\picture\\����\\�����.png"));
}

#endif