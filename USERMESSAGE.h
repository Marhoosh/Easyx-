#ifndef USERMESSAGE
#define USERMESSAGE

#define Namelen 20
#include<iostream>
struct User_text {

	/*��������ʾ������Ϣ����*/
	char username[Namelen];//�û���
	int score;//����
	int max_score;//��߻���
	double win_rate;//ʤ��
	int total_match;//�ܾ���
	int win_count;//ʤ��
	int lose_count;//����
	int draw_count;//ƽ��


	int head_id;//ͷ��id
	double remain_time;//ʣ��ʱ��
	double single_time;//��ʱ

	int ready_ok;//�Ƿ���׼��

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


User_text user_1, user_2;//user1�����1 user2�����2

int play_head_statu[3] = {-1,-1,-1};//ͷ�����¼�״̬��0��ʾ����ҳ�棬1��ʾ������Ϣҳ�� -1��ʾ��λ�û�δ���������


char myname[Namelen];

int myround = 0;//��ǰ�Ƿ����ҵĻغ�

#define user1_test_define (user1.username , user1.score , user1.max_score , user1.win_rate , user1.total_match , user1.win_count , user1.lose_count , user1.draw_count)
#define user2_test_define (user2.username , user2.score , user2.max_score , user2.win_rate , user2.total_match , user2.win_count , user2.lose_count , user2.draw_count)


#endif
