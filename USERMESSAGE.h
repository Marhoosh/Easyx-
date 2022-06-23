#ifndef USERMESSAGE
#define USERMESSAGE

#define Namelen 20
#include<iostream>
struct User_text {

	/*以下是显示个人信息部分*/
	char username[Namelen];//用户名
	int score;//积分
	int max_score;//最高积分
	double win_rate;//胜率
	int total_match;//总局数
	int win_count;//胜场
	int lose_count;//负场
	int draw_count;//平局


	int head_id;//头像id
	double remain_time;//剩余时间
	double single_time;//步时

	int ready_ok;//是否已准备

	void debug() {
		std::cout << "+++++++++++++++++++++++" << "\n";
		std::cout << "username:" << username << "\n";
		std::cout << "score:" << score << "\n";
		std::cout << "max_score:" << max_score << "\n";
		std::cout << "win_rate:" << win_rate << "\n";
		std::cout << "total_match:" << total_match << "\n";
		std::cout << "win_count:" << win_count << "\n";
		std::cout << "lose_count:" << lose_count << "\n";
		std::cout << "draw_count:" << draw_count << "\n";
		std::cout << "head_id:" << head_id << "\n";

	}
};


User_text user_1, user_2;//user1是玩家1 user2是玩家2

int play_head_statu[3] = {-1,-1,-1};//头像点击事件状态，0表示正常页面，1表示个人信息页面 -1表示该位置还未有玩家连接


char myname[Namelen];

int myround = 0;//当前是否是我的回合

#define user1_test_define (user1.username , user1.score , user1.max_score , user1.win_rate , user1.total_match , user1.win_count , user1.lose_count , user1.draw_count)
#define user2_test_define (user2.username , user2.score , user2.max_score , user2.win_rate , user2.total_match , user2.win_count , user2.lose_count , user2.draw_count)


#endif
