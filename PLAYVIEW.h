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
	//头像位置
	//circle(60, 60, 50);
	//状态栏
	//circle(95, 95, 12);

	peekmessage(&m, EM_MOUSE, false);

	//检测是否在圆内
	if (play_head_statu[1] == 0 && (60 - m.x) * (60 - m.x) + (60 - m.y) * (60 - m.y) <= 50 * 50) {

		if (m.message == WM_LBUTTONDOWN) {
			play_head_statu[1] = 1;//切换到个人信息页面
		}

	}

	if (play_head_statu[1] == 0) {
		putimage(10, 10, &game_player_head[6], 0x008800C6);
		putimage(10, 10, &game_player_head[user_1.head_id], 0x00EE0086);
		//时间框
		//setcolor(RED);
		//fillrectangle(115, 10, 235, 145);
	}
	else {


		//昵称
		roundrect(10, 20, 110, 40, 10, 10);
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		settextstyle(15, 0, _T("微软黑体"));
		outtextxy(13, 24, _T("昵称:  Bob"));
		//积分框
		roundrect(10, 50, 110, 140, 10, 10);
		outtextxy(13, 63, _T("积分:  10"));
		settextstyle(14, 0, _T("微软黑体"));
		outtextxy(13, 93, _T("最高积分:  15"));
		//胜率
		//roundrect(125, 70, 230, 135, 10, 10);
		settextstyle(19, 0, _T("楷体"));
		outtextxy(120, 15, _T("胜率:  50%"));
		outtextxy(120, 45, _T("总局数:40"));
		roundrect(120, 70, 225, 135, 10, 10);
		settextstyle(18, 0, _T("楷体"));
		outtextxy(125, 75, _T("胜场：15"));
		outtextxy(125, 95, _T("负场：15"));
		outtextxy(125, 115, _T("平局：10"));

	}


}
void Play2_view() {
	setcolor(BLACK);
	fillrectangle(5, 160, 241, 306);
	fillrectangle(115, 165, 235, 300);

	putimage(6, 161, &player_option_background);

	peekmessage(&m, EM_MOUSE, false);

	//检测是否在圆内
	printf("circle check %d\n", (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50);
	if (play_head_statu[2] == 0 && (60 - m.x) * (60 - m.x) + (215 - m.y) * (215 - m.y) <= 50 * 50) {

		if (m.message == WM_LBUTTONDOWN) {
			play_head_statu[2] = 1;//切换到个人信息页面
		}

	}
	printf("play_head_statu check %d\n", play_head_statu[2]);

	//头像位置
	//circle(60, 215, 50);
	if (play_head_statu[2] == 0) {
		//状态栏
		//circle(95, 250, 12);
		putimage(10, 165, &game_player_head[6], 0x008800C6);
		putimage(10, 165, &game_player_head[user_2.head_id], 0x00EE0086);
		//时间框
		//setcolor(RED);
		//fillrectangle(115, 165, 235, 300);
	}
	else {
		//昵称
		roundrect(10, 180, 110, 200, 10, 10);
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		settextstyle(15, 0, _T("微软黑体"));
		outtextxy(13, 184, _T("昵称:  Alice"));
		//积分框
		roundrect(10, 210, 110, 295, 10, 10);
		outtextxy(13, 223, _T("积分:  15"));
		settextstyle(14, 0, _T("微软黑体"));
		outtextxy(13, 253, _T("最高积分:  20"));
		//胜率
		//roundrect(125, 225, 230, 290, 10, 10);
		settextstyle(19, 0, _T("楷体"));
		outtextxy(120, 170, _T("胜率:  50%"));
		outtextxy(120, 200, _T("总局数:50"));
		roundrect(120, 225, 225, 290, 10, 10);
		settextstyle(18, 0, _T("楷体"));
		outtextxy(125, 230, _T("胜场：20"));
		outtextxy(125, 250, _T("负场：20"));
		outtextxy(125, 270, _T("平局：10"));
	}

}

#endif