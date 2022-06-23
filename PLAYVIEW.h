#ifndef PLAYVIEW
#define PLAYVIEW

#include "INIT_IMAGE.h"
#include "USERMESSAGE.h"
#include <graphics.h>


ExMessage m;

void Play1_view() {
	setcolor(BLACK);
	fillrectangle(5, 5, 241, 151);
	fillrectangle(115, 10, 235, 145);

	putimage(6, 6, &player_option_background);
	//ͷ��λ��
	//circle(60, 60, 50);
	//״̬��
	//circle(95, 95, 12);

	peekmessage(&m, EM_MOUSE, false);

	//����Ƿ���Բ��
	if (play_head_statu[1] == 0 && (60 - m.x) * (60 - m.x) + (60 - m.y) * (60 - m.y) <= 50 * 50) {

		if (m.message == WM_LBUTTONDOWN) {
			play_head_statu[1] = 1;//�л���������Ϣҳ��
		}

	}

	if (play_head_statu[1] == 0) {
		putimage(10, 10, &game_player_head[6], 0x008800C6);
		putimage(10, 10, &game_player_head[user_1.head_id], 0x00EE0086);
		//ʱ���
		//setcolor(RED);
		//fillrectangle(115, 10, 235, 145);
	}
	else {


		//�ǳ�
		roundrect(10, 20, 110, 40, 10, 10);
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		settextstyle(15, 0, _T("΢�����"));
		outtextxy(13, 24, _T("�ǳ�:  Bob"));
		//���ֿ�
		roundrect(10, 50, 110, 140, 10, 10);
		outtextxy(13, 63, _T("����:  10"));
		settextstyle(14, 0, _T("΢�����"));
		outtextxy(13, 93, _T("��߻���:  15"));
		//ʤ��
		//roundrect(125, 70, 230, 135, 10, 10);
		settextstyle(19, 0, _T("����"));
		outtextxy(120, 15, _T("ʤ��:  50%"));
		outtextxy(120, 45, _T("�ܾ���:40"));
		roundrect(120, 70, 225, 135, 10, 10);
		settextstyle(18, 0, _T("����"));
		outtextxy(125, 75, _T("ʤ����15"));
		outtextxy(125, 95, _T("������15"));
		outtextxy(125, 115, _T("ƽ�֣�10"));

	}


}
void Play2_view() {
	setcolor(BLACK);
	fillrectangle(5, 160, 241, 306);
	fillrectangle(115, 165, 235, 300);

	putimage(6, 161, &player_option_background);

	peekmessage(&m, EM_MOUSE, false);

	//����Ƿ���Բ��
	printf("circle check %d\n", (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50);
	if (play_head_statu[2] == 0 && (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50) {

		if (m.message == WM_LBUTTONDOWN) {
			play_head_statu[2] = 1;//�л���������Ϣҳ��
		}

	}
	printf("play_head_statu check %d\n", play_head_statu[2]);

	//ͷ��λ��
	//circle(60, 215, 50);
	if (play_head_statu[2] == 0) {
		//״̬��
		//circle(95, 250, 12);
		putimage(10, 165, &game_player_head[6], 0x008800C6);
		putimage(10, 165, &game_player_head[user_2.head_id], 0x00EE0086);
		//ʱ���
		//setcolor(RED);
		//fillrectangle(115, 165, 235, 300);
	}
	else {
		//�ǳ�
		roundrect(10, 180, 110, 200, 10, 10);
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		settextstyle(15, 0, _T("΢�����"));
		outtextxy(13, 184, _T("�ǳ�:  Alice"));
		//���ֿ�
		roundrect(10, 210, 110, 295, 10, 10);
		outtextxy(13, 223, _T("����:  15"));
		settextstyle(14, 0, _T("΢�����"));
		outtextxy(13, 253, _T("��߻���:  20"));
		//ʤ��
		//roundrect(125, 225, 230, 290, 10, 10);
		settextstyle(19, 0, _T("����"));
		outtextxy(120, 170, _T("ʤ��:  50%"));
		outtextxy(120, 200, _T("�ܾ���:50"));
		roundrect(120, 225, 225, 290, 10, 10);
		settextstyle(18, 0, _T("����"));
		outtextxy(125, 230, _T("ʤ����20"));
		outtextxy(125, 250, _T("������20"));
		outtextxy(125, 270, _T("ƽ�֣�10"));
	}

}

#endif