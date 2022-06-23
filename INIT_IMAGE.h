#ifndef INIT_IMAGE
#define INIT_IMAGE

#include <graphics.h>

IMAGE menu_bakeground, chessboard[4], login_bakeground, game_background;//菜单背景，棋盘，登录背景，游戏背景
IMAGE menu_option[10];//菜单选项图片
IMAGE sys_option[20];//说明  设置 菜单 设置背景
IMAGE game_option[30];//准备 悔棋 和棋 认输
IMAGE game_player_head[10];//游戏人物头像
IMAGE player_option_background;//玩家信息显示背景图
IMAGE chat_option[10];//聊天背景
IMAGE send_option[10];//发送按钮
IMAGE play_msg_exit[10];//玩家信息退出图片
IMAGE play_msg_ok[10];// 玩家准备状态图片
IMAGE chessboard_tracet_view[10];//棋盘追踪图片
IMAGE chessboard_piece_white[10];//棋盘白色棋子图片
IMAGE chessboard_piece_black[10];//棋盘黑色棋子图片

int init_chessborad = 0;//初始棋盘图片

void Insert_IMAGE() {//导入图片

	loadimage(&menu_bakeground, L".\\picture\\menu_background.jpg");//导入菜单背景图片
	loadimage(&menu_option[1], L".\\picture\\01.png");//导入菜单选项1图片
	loadimage(&menu_option[2], L".\\picture\\02.png");//导入菜单选项2图片
	loadimage(&menu_option[5], _T(".\\picture\\双人棋局3.png"));//导入菜单选项2图片


	loadimage(&menu_option[3], _T(".\\picture\\退出游戏1.png"));//导入菜单选项3图片
	loadimage(&menu_option[4], _T(".\\picture\\退出游戏2.png"));//导入菜单选项3图片
	loadimage(&menu_option[6], _T(".\\picture\\退出游戏3.png"));//导入菜单选项3图片


	loadimage(&chessboard[1], _T(".\\picture\\棋盘\\木质棋盘.png"));//导入棋盘图片
	loadimage(&chessboard[2], _T(".\\picture\\棋盘\\石质棋盘.png"));//导入棋盘图片

	loadimage(&login_bakeground, _T(".\\picture\\login_background.png"));
	loadimage(&game_background, _T(".\\picture\\game_background.jpg"));

	loadimage(&game_option[1], _T(".\\picture\\按钮\\准备01.png"));
	loadimage(&game_option[2], _T(".\\picture\\按钮\\准备02.png"));

	loadimage(&game_option[3], _T(".\\picture\\按钮\\悔棋01.png"));
	loadimage(&game_option[4], _T(".\\picture\\按钮\\悔棋02.png"));



	loadimage(&game_option[5], _T(".\\picture\\按钮\\和棋01.png"));
	loadimage(&game_option[6], _T(".\\picture\\按钮\\和棋02.png"));

	loadimage(&game_option[7], _T(".\\picture\\按钮\\认输01.png"));
	loadimage(&game_option[8], _T(".\\picture\\按钮\\认输02.png"));

	loadimage(&sys_option[1], _T(".\\picture\\说明.jpg"));
	loadimage(&sys_option[2], _T(".\\picture\\太极.jpg"));
	loadimage(&sys_option[3], _T(".\\picture\\设置背景3.png"));

	loadimage(&sys_option[4], _T(".\\picture\\.png"));//菜单
	loadimage(&sys_option[5], _T(".\\picture\\.png"));
	loadimage(&sys_option[6], _T(".\\picture\\返回按钮001.png"));
	loadimage(&sys_option[7], _T(".\\picture\\.png"));

	loadimage(&sys_option[8], _T(".\\picture\\.png"));//说明
	loadimage(&sys_option[9], _T(".\\picture\\.png"));
	loadimage(&sys_option[10], _T(".\\picture\\说明.jpg"));
	loadimage(&sys_option[11], _T(".\\picture\\.png"));

	loadimage(&sys_option[12], _T(".\\picture\\.png"));//设置
	loadimage(&sys_option[13], _T(".\\picture\\.png"));
	loadimage(&sys_option[14], _T(".\\picture\\设置按钮001.png"));
	loadimage(&sys_option[15], _T(".\\picture\\.png"));


	loadimage(&game_player_head[6], _T(".\\picture\\头像遮罩.png"));
	loadimage(&game_player_head[0], _T(".\\picture\\头像01.png"));
	loadimage(&game_player_head[1], _T(".\\picture\\头像02.png"));
	loadimage(&game_player_head[2], _T(".\\picture\\头像03.png"));
	loadimage(&game_player_head[3], _T(".\\picture\\头像04.png"));
	loadimage(&game_player_head[4], _T(".\\picture\\头像05.png"));

	loadimage(&game_option[9], _T(".\\picture\\按钮_按下遮罩.png"));
	loadimage(&game_option[10], _T(".\\picture\\按钮_遮罩.png"));
	loadimage(&game_option[11], _T(".\\picture\\悔棋_按下.png"));
	loadimage(&game_option[12], _T(".\\picture\\悔棋_启用.png"));


	loadimage(&game_option[13], _T(".\\picture\\和棋_按下.png"));
	loadimage(&game_option[14], _T(".\\picture\\和棋_启用.png"));

	loadimage(&game_option[15], _T(".\\picture\\认输_按下.png"));
	loadimage(&game_option[16], _T(".\\picture\\认输_启用.png"));

	loadimage(&game_option[17], _T(".\\picture\\准备_按下.png"));
	loadimage(&game_option[18], _T(".\\picture\\准备_按下遮罩.png"));
	loadimage(&game_option[19], _T(".\\picture\\准备_禁用.png"));

	loadimage(&player_option_background, _T(".\\picture\\玩家信息背景.jpg"));

	loadimage(&chat_option[1], _T(".\\picture\\聊天背景3.jpg"));

	loadimage(&send_option[1], _T(".\\picture\\发送按钮_按下01.png"));
	loadimage(&send_option[2], _T(".\\picture\\发送按钮_按下02.png"));
	loadimage(&send_option[3], _T(".\\picture\\发送按钮01.png"));
	loadimage(&send_option[4], _T(".\\picture\\发送按钮02.png"));

	loadimage(&play_msg_exit[1], _T(".\\picture\\关闭按钮遮罩.png"));
	loadimage(&play_msg_exit[2], _T(".\\picture\\关闭按钮.png"));

	loadimage(&play_msg_ok[1], _T(".\\picture\\准备状态遮罩.png"));
	loadimage(&play_msg_ok[2], _T(".\\picture\\准备状态.png"));

	loadimage(&chessboard_tracet_view[1], _T(".\\picture\\追踪显示遮罩.png"));
	loadimage(&chessboard_tracet_view[2], _T(".\\picture\\追踪显示期望.png"));

	loadimage(&chessboard_piece_white[1], _T(".\\picture\\棋盘\\大棋子遮罩.png"));
	loadimage(&chessboard_piece_white[2], _T(".\\picture\\棋盘\\大白棋.png"));

	loadimage(&chessboard_piece_black[1], _T(".\\picture\\棋盘\\大棋子遮罩.png"));
	loadimage(&chessboard_piece_black[2], _T(".\\picture\\棋盘\\大黑棋.png"));
}

#endif