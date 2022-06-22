#pragma warning( disable : 4996)
#include <winsock2.h>
#include <WS2tcpip.h>
#include<stdio.h>
#include<conio.h>
#include<mysql.h>
#include<time.h>
#pragma comment(lib,"Ws2_32.lib")

//����ͨ�Ų���
#define SERVERIP "172.17.122.148"
#define RECVBUFFERSIZE 1024
#define SENDBUFFERSIZE 2048
char recvbuffer[RECVBUFFERSIZE];
char sendbuffer[SENDBUFFERSIZE];

int cliaddrlen = sizeof(SOCKADDR_IN);
SOCKADDR_IN cliaddr[2];
SOCKET clifd[2];

int recvbufferidx;

/*
��Ҫ���͵���Ϣ��
1.׼��״̬
2.�Է�ս��
3.������Ϣ
4.���ӡ����塢��͡������
*/


//���ݿⲿ��
MYSQL mysql;
MYSQL_ROW row;//char **��ά����,���һ������¼

const char DataBase_UserName[] = "root";
const char DataBase_Password[] = "123456";
const char DataBase_Host[] = "localhost";//���ݿ����ӵ�ַ
const char DataBase_Name[] = "test";
const unsigned int DataBase_Port = 3306;

#define QUERYBUFFERSIZE 1024
char querybuffer[QUERYBUFFERSIZE];

#define USERNAMELENGTH 10  //�û�����󳤶�
#define PASSWORDLENGTH 10  //������󳤶�




//����
int ret;
bool cliuserlog[2];//�û��Ƿ��¼
bool cliuserready[2];//�û��Ƿ�׼��
int cliuserheadid[2];//�û�ͷ��id
char cliusername[2][USERNAMELENGTH];//�û���
char cliuserinforpos[2][22] = {
	"firstpos",
	"secondpos"
};//�û���Ϣλ��
char cliusercolor[2][11] = {
	"black",
	"white"
};

#define BOARDHEIGHT 15
#define BOARDWIDTH 15
int chessboard[BOARDHEIGHT][BOARDWIDTH];

int chessexitnum = 0;



void dosign(int clinum) {//�����û�ע��
	//��ȡ�û���������
	char username[USERNAMELENGTH];//�û���
	char password[PASSWORDLENGTH];//����
	//memset(username, 0, sizeof(username));
	sscanf(recvbuffer + recvbufferidx, "%s", username);
	recvbufferidx += strlen(username)+1;
	printf("sign username:%s\n", username);
	//memset(password, 0, sizeof(password));
	sscanf(recvbuffer + recvbufferidx, "%s", password);
	recvbufferidx += strlen(password)+1;
	printf("sign password:%s\n", password);

	sprintf(querybuffer, "select * from user where username='%s'", username);
	ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ��ѯ����\n");
		return;
	}
	MYSQL_RES* res;//����ṹ��
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);

	//�ж��û����Ƿ����
	if (rownum == 0) {//�û���������
		//memset(querybuffer, 0, sizeof(querybuffer));
		sprintf(querybuffer, "insert into user values('%s','%s')", username, password);//�����û���������
		ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
		if (ret != 0) {
			printf("mysql �������\n");
			return;
		}
		//memset(sendbuffer,0,sizeof(sendbuffer));
		sprintf(sendbuffer, "_sign sign_success ",username);
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);

		//Ϊ���û�����û���Ϣ
		sprintf(querybuffer, "insert into userinfor values('%s',0,0,0,0,0,0,0)",username);
		ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
		if (ret != 0) {
			printf("mysql �������\n");
			return;
		}
	}
	else {//�û�������
		//memset(sendbuffer,0,sizeof(sendbuffer));
		sprintf(sendbuffer, "_sign username_exist ",username);
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	}
	mysql_free_result(res);
}

void douserinfor(int clifrnum,int clitonum) {//��clifrnum����Ϣ���͸�clitonum

	sprintf(querybuffer, "select * from userinfor where username='%s'", cliusername[clifrnum]);
	ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ��ѯ����\n");
		return;
	}
	MYSQL_RES* res;//����ṹ��
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("�����û���Ϣ����\n");
		mysql_free_result(res);
		return;
	}

	//�����û���Ϣ
	sprintf(sendbuffer, "_userinfor username %s %s score %s max_score %s win_rate %s total_match %s win_count %s lose_count %s draw_count %s head_id %d ",
		cliusername[clifrnum],
		cliuserinforpos[clifrnum],
		row[1], row[2], row[3], row[4], row[5], row[6], row[7],
		cliuserheadid[clifrnum]);
	send(clifd[clitonum], sendbuffer, strlen(sendbuffer),0);
	printf("sendbuffer:%s\n", sendbuffer);
	printf("���û�%s���͸��û�%s��Ϣ�ɹ�\n",cliusername[clifrnum],cliusername[clitonum]);

	mysql_free_result(res);
	//printf("log username:%s\n", username);
	
}


void dolog(int clinum) {//�����û���½

	//��ȡ�û���������
	char username[USERNAMELENGTH];//�û���
	char password[PASSWORDLENGTH];//����
	//memset(username, 0, sizeof(username));
	sscanf(recvbuffer + recvbufferidx, "%s", username);
	recvbufferidx += strlen(username)+1;
	printf("log username:%s\n", username);
	//memset(password, 0, sizeof(password));
	sscanf(recvbuffer + recvbufferidx, "%s", password);
	recvbufferidx += strlen(password)+1;
	printf("log password:%s\n", password);

	sprintf(querybuffer, "select * from user where username='%s'", username);
	ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ��ѯ����\n");
		return;
	}
	MYSQL_RES* res;//����ṹ��
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);

	//�ж��û����Ƿ����
	if (rownum == 0) {//�������û���
		//memset(sendbuffer,0,sizeof(sendbuffer));
		sprintf(sendbuffer, "_log username_wrong ",username);
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	}
	else {//�û�������
		//�����Ƿ���ȷ
		row = mysql_fetch_row(res);
		if (strcmp(row[1], password) != 0) {//�������
			//memset(sendbuffer,0,sizeof(sendbuffer));
			sprintf(sendbuffer, "_log password_wrong ",username);
			send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
			mysql_free_result(res);
			return;
		}
		else {//������ȷ
			//����û��Ѿ���¼
			if (strcmp(username, cliusername[1 - clinum]) == 0){
				sprintf(sendbuffer, "_log user_loged ", username);
				send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
				printf("�û�%s�ѵ�¼\n",cliusername[1-clinum]);
				mysql_free_result(res);
				return;
			}

			//�������ͷ��id
			srand((unsigned)time(NULL));//������һ����Ϊ�˷�ֹrand()��������ɻ�
			if (cliuserheadid[clinum] == -1) {
				cliuserheadid[clinum] = rand() % 5;

				//��������һ����ͷ��id
				if (cliuserheadid[clinum] == cliuserheadid[1 - clinum]) {
					cliuserheadid[clinum] = (cliuserheadid[clinum] + 1) % 5;
				}
			}
			
			cliuserlog[clinum] = true;

			//memset(sendbuffer,0,sizeof(sendbuffer));
			sprintf(sendbuffer, "_log password_right ",username);
			send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
			sprintf(cliusername[clinum], "%s", username);
			
			////���߿ͻ����Լ��ǵڼ�������  �ϰ汾����  ���ڲ��÷�
			//if (clifd[1-clinum] == -1) {
			//	sprintf(sendbuffer, "_%s firstentry ",username);
			//}
			//else {
			//	sprintf(sendbuffer, "_%s secondentry ",username);
			//}

			//send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
			//printf("sendbuffer:%s\n", sendbuffer);
			//printf("���û�%s���͵ڼ���������Ϣ�ɹ�\n",cliusername[clinum]);

			//����Է��Ѿ�׼��������û����ͶԷ���׼��
			if (cliuserready[1 - clinum]){
				sprintf(sendbuffer, "_readyinfor %s ", cliuserinforpos[1-clinum]);
				send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
				printf("sendbuffer:%s\n", sendbuffer);
				printf("���û�%s�����û�%s ��׼����Ϣ�ɹ�\n",cliusername[clinum],cliusername[1-clinum]);
			}
			//�����û���Ϣ
			if (cliuserlog[1 - clinum]){
				douserinfor(1-clinum,clinum);
				douserinfor(clinum, 1 - clinum);
			}
			douserinfor(clinum,clinum);
		}
	}

	mysql_free_result(res);
}

void doready(int clinum) {
	cliuserready[clinum] = true;
	//printf("�յ�ready��Ϣ\n");
	sprintf(sendbuffer, "_readyinfor %s ", cliuserinforpos[clinum]);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer:%s\n", sendbuffer);
	printf("����ready��Ϣ���û�%s�ɹ�\n",cliusername[clinum]);
	if (cliuserlog[1 - clinum]){
		sprintf(sendbuffer, "_readyinfor %s ", cliuserinforpos[clinum]);
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer:%s\n", sendbuffer);
		printf("����ready��Ϣ���û�%s�ɹ�\n",cliusername[1-clinum]);
	}
}

void dogamestart() {
	sprintf(sendbuffer, "_gamestart ");
	send(clifd[0], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s������Ϸ��ʼ��Ϣ�ɹ�\n", cliusername[0]);
	send(clifd[1], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s������Ϸ��ʼ��Ϣ�ɹ�\n", cliusername[1]);
}

void dochat(int clinum) {
	//"_chat username %s inputtext %s"
	char username[33];
	char chatbuffer[111];

	//���˵�username��ʶ��
	//memset(username, 0, sizeof(username));
	sscanf(recvbuffer + recvbufferidx, "%s", username);
	recvbufferidx += strlen(username) + 1;

	//��ȡ��username
	//memset(username, 0, sizeof(username));
	sscanf(recvbuffer + recvbufferidx, "%s", username);
	recvbufferidx += strlen(username) + 1;

	//���˵�inputext��ʶ��
	//memset(chatbuffer, 0, sizeof(chatbuffer));
	sscanf(recvbuffer + recvbufferidx, "%s", chatbuffer);
	recvbufferidx += strlen(chatbuffer) + 1;

	//��ȡ����������--------------------------------------------------------ע�ⲻ���пո�---------------------------------
	//memset(chatbuffer, 0, sizeof(chatbuffer));
	sscanf(recvbuffer + recvbufferidx, "%s", chatbuffer);
	recvbufferidx += strlen(chatbuffer) + 1;
	
	//��˫������������Ϣ
	sprintf(sendbuffer, "_chat username %s inputtext %s ",username,chatbuffer);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("%s\n", sendbuffer);
	printf("���û�%s����������Ϣ�ɹ�\n", cliusername[clinum]);
	if (cliuserlog[1 - clinum]) {//����Է���¼��  ���ܿ�����Ϣ   �ҿ�������¼֮ǰ����Ϣ
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("%s\n", sendbuffer);
		printf("���û�%s����������Ϣ�ɹ�\n", cliusername[1 - clinum]);
	}
}

void dowin(int cliwinnum) {

	//�������ͻ��˷�����Ϣ
	sprintf(sendbuffer, "_win %s ", cliusername[cliwinnum]);
	send(clifd[cliwinnum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s������Ϸʤ����Ϣ�ɹ�\n", cliusername[cliwinnum]);
	send(clifd[1 - cliwinnum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s������Ϸʧ����Ϣ�ɹ�\n", cliusername[1-cliwinnum]);

}

bool isgg(int x,int y,int color) {
	int pa, pb;
	//��
	pa = pb = y;
	while (pa>=0 && chessboard[x][pa] == color) { pa--; }
	while (pb<BOARDWIDTH && chessboard[x][pb] == color) { pb++; }
	if (pb - pa - 1 >= 5) {
		return true;
	}

	//��
	pa = pb = y;
	while (pa >= 0 && chessboard[pa][y] == color) { pa--; }
	while (pb < BOARDHEIGHT && chessboard[pb][y] == color) { pb++; }
	if (pb - pa - 1 >= 5) {
		return true;
	}

	//��б
	pa = pb = 1;
	while (x-pa>=0 && y+pa<BOARDWIDTH && chessboard[x-pa][y+pa] == color) { pa++; }
	while (x+pb < BOARDHEIGHT && y-pb>=0 && chessboard[x+pb][y-pb] == color) { pb++; }
	if (pb + pa - 1 >= 5) {
		return true;
	}

	//��б
	pa = pb = 1;
	while (x - pa >= 0 && y - pa >=0 && chessboard[x - pa][y - pa] == color) { pa++; }
	while (x + pb < BOARDHEIGHT && y + pb <BOARDWIDTH && chessboard[x + pb][y + pb] == color) { pb++; }
	if (pb + pa - 1 >= 5) {
		return true;
	}

	return false;
}

void updatewin(int clinum) {
	sprintf(querybuffer, "select score,max_score,total_match,win_count from userinfor where username='%s'", cliusername[clinum]);
	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ��ѯ����\n");
		return;
	}
	MYSQL_RES* res;//����ṹ��
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("mysql �����û�ʤ����Ϣʱ��δ��ѯ���û����\n");
		mysql_free_result(res);
		return;
	}
	row = mysql_fetch_row(res);
	int score = atoi(row[0]);
	int max_score = atoi(row[1]);
	int total_match = atoi(row[2]) + 1;
	int win_count = atoi(row[3]) + 1;
	double win_rate = (double)win_count / (double)total_match;
	int flag = 0;
	if (score + 1 > max_score) {
		flag = 1;
	}
	mysql_free_result(res);
	sprintf(querybuffer, "update userinfor set score=score+1,max_score=max_score+%d,win_rate=%lf,total_match=total_match+1,win_count=win_count+1 where username='%s'",
		flag,
		win_rate,
		cliusername[clinum]
	);

	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ���´���\n");
		return;
	}
}

void updatelose(int clinum) {
	sprintf(querybuffer, "select score,max_score,total_match,win_count from userinfor where username='%s'", cliusername[clinum]);
	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ��ѯ����\n");
		return;
	}
	MYSQL_RES* res;//����ṹ��
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("mysql �����û�ʧ����Ϣʱ��δ��ѯ���û����\n");
		mysql_free_result(res);
		return;
	}
	row = mysql_fetch_row(res);
	int score = atoi(row[0]);
	int max_score = atoi(row[1]);
	int total_match = atoi(row[2]) + 1;
	int win_count = atoi(row[3]);
	double win_rate = (double)win_count / (double)total_match;
	mysql_free_result(res);
	sprintf(querybuffer, "update userinfor set score=score-1,win_rate=%lf,total_match=total_match+1,lose_count=lose_count+1 where username='%s'",
		win_rate,
		cliusername[clinum]
	);

	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ���´���\n");
		return;
	}
}

void updatedraw(int clinum) {
	sprintf(querybuffer, "select total_match,win_count from userinfor where username='%s'", cliusername[clinum]);
	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ��ѯ����\n");
		return;
	}
	MYSQL_RES* res;//����ṹ��
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("mysql �����û�ʧ����Ϣʱ��δ��ѯ���û����\n");
		mysql_free_result(res);
		return;
	}
	row = mysql_fetch_row(res);
	int total_match = atoi(row[0]) + 1;
	int win_count = atoi(row[1]);
	double win_rate = (double)win_count / (double)total_match;
	mysql_free_result(res);
	sprintf(querybuffer, "update userinfor set win_rate=%lf,total_match=total_match+1,draw_count=draw_count+1 where username='%s'",
		win_rate,
		cliusername[clinum]
	);

	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql ���´���\n");
		return;
	}
}


void domove(int clinum,int &x,int &y) {
	//����������"_move _clientsend %d %d "
	//����������"_move _serversend %s %d %d "

	//��ȡ�����ӵ�x,y����
	char tmpbuffer[22];
	//memset(tmpbuffer, 0, sizeof(tmpbuffer));
	sscanf(recvbuffer + recvbufferidx, "%s", tmpbuffer);
	recvbufferidx += strlen(tmpbuffer) + 1;
	char coordinatex[11];
	char coordinatey[11];
	sscanf(recvbuffer + recvbufferidx, "%s", coordinatex);
	recvbufferidx += strlen(coordinatex) + 1;
	x = atoi(coordinatex);
	sscanf(recvbuffer + recvbufferidx, "%s", coordinatey);
	recvbufferidx += strlen(coordinatey) + 1;
	y = atoi(coordinatey);

	chessboard[x][y] = clinum + 1;

	//�������ͻ��˷���������Ϣ
	sprintf(sendbuffer, "_move serversend %s %s %s ", cliusercolor[clinum], coordinatex, coordinatey);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s����������Ϣ�ɹ�\n", cliusername[clinum]);
	sprintf(sendbuffer, "_move serversend %s %s %s ", cliusercolor[clinum], coordinatex, coordinatey);
	send(clifd[1-clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s����������Ϣ�ɹ�\n", cliusername[1-clinum]);

}

void doround(int clinum) {
	sprintf(sendbuffer, "_round %s ", cliusername[1-clinum]);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s�����ֵ��û�%s�غ���Ϣ�ɹ�\n", cliusername[clinum], cliusername[1 - clinum]);
	send(clifd[1-clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s�����ֵ��û�%s�غ���Ϣ�ɹ�\n", cliusername[1-clinum], cliusername[1 - clinum]);
}

void dodrawrequest(int clinum) {
	sprintf(sendbuffer, "_other_want_to_draw ");
	send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer ; %s\n", sendbuffer);
	printf("���û�%s���ͺ�������ɹ�\n", cliusername[1 - clinum]);
}

bool dodrawrespond(int clinum) {
	int result = -1;
	sscanf(recvbuffer + recvbufferidx, "%d", &result);
	recvbufferidx += 2;
	if (!(result == 0 || result == 1)) {
		printf("result = %d ��ȡ�����������\n", result);
		return false;
	}
	if (result == 0) {
		sprintf(sendbuffer, "_draw_is_fail ");
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer : %s\n", sendbuffer);
		printf("���û�%s���Ͳ�ͬ�������Ϣ�ɹ�\n", cliusername[1 - clinum]);
		return false;
	}
	if (result == 1) {
		sprintf(sendbuffer, "_draw_is_ok ");
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer : %s\n", sendbuffer);
		printf("���û�%s����ͬ�������Ϣ�ɹ�\n", cliusername[clinum]);
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer : %s\n", sendbuffer);
		printf("���û�%s����ͬ�������Ϣ�ɹ�\n", cliusername[1 - clinum]);
		return true;
	}
	return true;
}

void dodogfall() {
	sprintf(sendbuffer, "_dogfall ");
	send(clifd[0], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s����ƽ����Ϣ�ɹ�\n", cliusername[0]);
	send(clifd[1], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s����ƽ����Ϣ�ɹ�\n", cliusername[1]);
}

void docapitulate(int clinum) {
	sprintf(sendbuffer, "_capitulate_is_ok ");
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s����������Ϣ�ɹ�\n", cliusername[clinum]);
	sprintf(sendbuffer, "_other_is_capitulate ");
	send(clifd[1-clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("���û�%s����������Ϣ�ɹ�\n", cliusername[1-clinum]);
}

void doregret(int clinum) {

}

bool doreturnmenu(int clinum) {//��Ϸ��ʼ����true;���򣬷���false;

	//����Է��Ѿ��ڷ����ڣ���Է������Լ����ز˵�����Ϣ
	if (cliuserlog[1 - clinum]) {
		if (!cliuserready[0] || !cliuserready[1]) {//��Ϸδ��ʼ
			sprintf(sendbuffer, "_other_return_to_menu _no_game_start ", cliuserinforpos[clinum]);
			send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
			printf("sendbuffer");
			printf("���û�%s�����û�%s�ѷ��ز˵�����Ϣ�ɹ�\n", cliusername[1 - clinum], cliusername[clinum]);
			return false;
		}
		else {//��Ϸ�Ѿ���ʼ
			sprintf(sendbuffer, "_other_return_to_menu _game_start ", cliuserinforpos[clinum]);
			send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
			printf("sendbuffer");
			printf("���û�%s�����û�%s�ѷ��ز˵�����Ϣ�ɹ�\n", cliusername[1 - clinum], cliusername[clinum]);
			return true;
		}
	}
	return false;
}


void domessage(int clinum) {//clinum�����������������Ŀͻ��ˣ�flag��־���Ƿ���Է���������������
	recvbufferidx = 0;
	//ʹ��sscanf����recvbuffer.
	char waybuffer[22];
	while (recvbufferidx<strlen(recvbuffer)){
		printf("recvbufferidx : %d,  recvbufferlen : %d\n", recvbufferidx, strlen(recvbuffer));
		//memset(waybuffer, 0, sizeof(waybuffer));
		sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
		printf("waybuffer �� %s\n", waybuffer);
		recvbufferidx += strlen(waybuffer)+1;
		if (strcmp(waybuffer, "_sign") == 0) {//ע��
			printf("ע��\n");
			dosign(clinum);
		}
		if (strcmp(waybuffer, "_log") == 0) {//��¼
			printf("��¼\n");
			dolog(clinum);
		}
		
		if (strcmp(waybuffer, "_myreadyinfor") == 0) {//׼��
			doready(clinum);

			//���˫����׼��
			if (cliuserready[0] && cliuserready[1]) {
				//������Ϸ��ʼ����Ϣ
				dogamestart();
				doround(1);
			}
		}
		if (strcmp(waybuffer, "_chat") == 0) {//����
			dochat(clinum);
		}
		if (strcmp(waybuffer, "_move") == 0) {//����
			int chessx;int chessy;
			domove(clinum,chessx,chessy);
			chessexitnum++;

			//�ж���Ϸ�Ƿ����
			if (isgg(chessx, chessy, clinum + 1)) {
				//����ƽ����Ϣ
				dodogfall();

				//�����û�����
				updatedraw(clinum);
				updatedraw(1-clinum);

				//�����û���Ϣ  
				douserinfor(0, 0);
				douserinfor(0, 1);
				douserinfor(1, 1);
				douserinfor(1, 0);

				//��ʼ�����
				cliuserready[0] = cliuserready[1] = false;
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//��������δ���������Ϣ
			}

			//�ж��Ƿ���������
			if (chessexitnum == BOARDHEIGHT * BOARDWIDTH) {

				//�����û�����
				updatedraw(clinum);
				updatedraw(1 - clinum);

				//�����û���Ϣ  
				douserinfor(0, 0);
				douserinfor(0, 1);
				douserinfor(1, 1);
				douserinfor(1, 0);

				//��ʼ�����
				cliuserready[0] = cliuserready[1] = false;
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//��������δ���������Ϣ
			}

			//���߿ͻ�����˭�Ļغ�
			doround(clinum);
		}
		if (strcmp(waybuffer, "_I_want_to_draw") == 0) {//�������
			dodrawrequest(clinum);
		}
		if (strcmp(waybuffer, "_answer_draw_result") == 0) {//�����Ӧ
			if (dodrawrespond(clinum)) {

				//�����û�����
				updatedraw(clinum);
				updatedraw(1 - clinum);

				//�����û���Ϣ  
				douserinfor(0, 0);
				douserinfor(0, 1);
				douserinfor(1, 1);
				douserinfor(1, 0);

				//��ʼ�����
				cliuserready[0] = cliuserready[1] = false;
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//��������δ���������Ϣ
			}
		}
		if (strcmp(waybuffer, "_I_capitulate") == 0) {//����

			//����������Ϣ
			docapitulate(clinum);

			//�����û�����
			updatewin(1-clinum);
			updatelose(clinum);

			//�����û���Ϣ  
			douserinfor(0, 0);
			douserinfor(0, 1);
			douserinfor(1, 1);
			douserinfor(1, 0);

			//��ʼ�����
			cliuserready[0] = cliuserready[1] = false;
			memset(chessboard, 0, sizeof(chessboard));
			chessexitnum = 0;

			continue;//��������δ���������Ϣ
		}

		if (strcmp(waybuffer, "_regret") == 0) {//����
			doregret(clinum);
		}
		
		if (strcmp(waybuffer, "_I_return_to_menu") == 0) {//���ز˵�

			//������Ϣ
			if (doreturnmenu(clinum)) {

				//�����û�����
				updatewin(1 - clinum);
				updatelose(clinum);

				//�����û���Ϣ  
				douserinfor(1 - clinum, 1 - clinum);

				//��ʼ�����
				cliuserready[0] = cliuserready[1] = false;
				cliuserlog[clinum] = false;
				cliuserheadid[clinum] = -1;
				memset(cliusername[clinum], 0, sizeof(cliusername[clinum]));
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//��������δ���������Ϣ
			}
		}
	}
}



void conninit() {
	chessexitnum = 0;
	memset(cliusername, 0, sizeof(cliusername));
	memset(cliuserheadid, -1, sizeof(cliuserheadid));
	memset(cliuserlog, 0, sizeof(cliuserlog));
	memset(clifd, -1, sizeof(clifd));
	for (int i = 0;i < 2;i++) {
		cliaddr[i].sin_family = AF_INET;
		inet_pton(AF_INET, SERVERIP, &cliaddr[i].sin_addr.S_un.S_addr);
		cliaddr[i].sin_port = htons(9999);
	}
}

//��Ϸ�����ͶϿ����Ӻ��ǵý���ر�����ʼ��

int main() {

	//���ݿ��ʼ��
	mysql_init(&mysql);
	//����mysql,���ݿ�
	if (!(mysql_real_connect(&mysql, DataBase_Host, DataBase_UserName, DataBase_Password, DataBase_Name, DataBase_Port, NULL, 0))) {
		printf("error connecting to database:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}

	//1.����socket
	SOCKET listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == listenfd)
	{
		WSACleanup();
		printf("����socketʧ��\n");
		return -1;
	}

	//2.��socket
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &saddr.sin_addr.S_un.S_addr);
	saddr.sin_port = htons(9999);
	ret = bind(listenfd, (sockaddr*)&saddr, sizeof(saddr));
	if (ret == -1) {
		printf("��ʧ��\n");
		WSACleanup();
		closesocket(listenfd);
		return -1;
	}

	//3.����socket
	ret = listen(listenfd, 5);
	if (ret == -1) {
		printf("����ʧ��\n");

		WSACleanup();
		closesocket(listenfd);
		return -1;
	}

	//�������ӳ�ʼ��
	conninit();

	fd_set readfds,tmpfds;
	FD_ZERO(&readfds);
	FD_SET(listenfd, &readfds);
	while (1) {
		tmpfds = readfds;
		ret = select(0, &tmpfds, NULL, NULL, NULL);
		printf("readfds����%d��fd\n",readfds.fd_count);
		if (ret > 0) {
			printf("ret=%d\n", ret);
			if (FD_ISSET(listenfd, &tmpfds)) {//��������
				if (clifd[0] == -1) {//�����ӵ�������
					clifd[0] = accept(listenfd, (sockaddr*)&cliaddr[0], &cliaddrlen);
					
					//printf("%d\n", clifd[0]);
					if (clifd[0] == INVALID_SOCKET) {
						printf("accept����ʧ��\n");
					}
					else {
						printf("0�����ӳɹ�\n");
						FD_SET(clifd[0], &readfds);
						//memset(sendbuffer, 0, sizeof(sendbuffer));

					}
					
				}
				else if (clifd[1]==-1) {
					clifd[1] = accept(listenfd, (sockaddr*)&cliaddr[1], &cliaddrlen);
					if (clifd[1] == INVALID_SOCKET) {
						printf("accept����ʧ��\n");
					}
					else {
						printf("1�����ӳɹ�\n");
						FD_SET(clifd[1], &readfds);
						//memset(sendbuffer, 0, sizeof(sendbuffer));

					}
				}
			}
			for (int i = 0;i < 2;i++) {
				if (clifd[i] != -1 && FD_ISSET(clifd[i], &tmpfds)) {//������
					memset(recvbuffer, 0, sizeof(recvbuffer));
					int len = recv(clifd[i], recvbuffer, RECVBUFFERSIZE, 0);
					printf("recvbuffer:%s\n", recvbuffer);
					if (len > 0) {
						
						domessage(i);

					}

					else if (len == 0) {//���������Ͽ�  �Է��ر�socket�����
						cliuserready[i] = false;
						cliuserlog[i] = false;
						cliuserheadid[i] = -1;
						memset(cliusername[i], 0, sizeof(cliusername[i]));
						memset(chessboard, 0, sizeof(chessboard));
						chessexitnum = 0;
 						printf("���������Ͽ�\n");
						FD_CLR(clifd[i], &readfds);
						clifd[i] = -1;
					}
					else if (len == -1) {//��������Ͽ� �Է�δ�ر�socket�ͶϿ�������
						printf("��������Ͽ�\n");

						//����Է��Ѿ��ڷ����ڣ���Է������Լ��Ͽ����ӵ���Ϣ
						if (cliuserlog[1 - i]) {
							if (!cliuserready[0] || !cliuserready[1]) {//��Ϸδ��ʼ
								sprintf(sendbuffer, "_exit %s _no_game_start ", cliuserinforpos[i]);
								send(clifd[1 - i], sendbuffer, strlen(sendbuffer), 0);
								printf("sendbuffer");
								printf("���û�%s�����û�%s�ѶϿ����ӵ���Ϣ�ɹ�\n", cliusername[1 - i], cliusername[i]);

								//��ʼ�����
								cliuserready[i] = false;
								cliuserlog[i] = false;
								cliuserheadid[i] = -1;
								memset(cliusername[i], 0, sizeof(cliusername[i]));
								memset(chessboard, 0, sizeof(chessboard));
								chessexitnum = 0;
							}
							else {//��Ϸ�Ѿ���ʼ
								sprintf(sendbuffer, "_exit %s _game_start ", cliuserinforpos[i]);
								send(clifd[1 - i], sendbuffer, strlen(sendbuffer), 0);
								printf("sendbuffer");
								printf("���û�%s�����û�%s�ѶϿ����ӵ���Ϣ�ɹ�\n", cliusername[1 - i], cliusername[i]);

								//�����û�����
								updatewin(1 - i);
								updatelose(i);

								//�����û���Ϣ  
								douserinfor(1 - i, 1 - i);

								//��ʼ�����
								cliuserready[0] = cliuserready[1] = false;
								cliuserlog[i] = false;
								cliuserheadid[i] = -1;
								memset(cliusername[i], 0, sizeof(cliusername[i]));
								memset(chessboard, 0, sizeof(chessboard));
								chessexitnum = 0;
							}
						}

						

						FD_CLR(clifd[i], &readfds);
						clifd[i] = -1;
					}
					
				}
			}
		}

	}


	_getch();
	WSACleanup();
	for (int i = 0;i < 2;i++) {
		closesocket(clifd[i]);
	}
	closesocket(listenfd);
	return 0;
}


/*
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
			if (m.message == WM_LBUTTONDOWN) {
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

*/