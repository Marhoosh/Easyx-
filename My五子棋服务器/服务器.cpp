#pragma warning( disable : 4996)
#include <winsock2.h>
#include <WS2tcpip.h>
#include<stdio.h>
#include<conio.h>
#include<mysql.h>
#include<time.h>
#pragma comment(lib,"Ws2_32.lib")

//网络通信部分
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
需要发送的信息：
1.准备状态
2.对方战绩
3.聊天信息
4.落子、悔棋、求和、认输等
*/


//数据库部分
MYSQL mysql;
MYSQL_ROW row;//char **二维数组,存放一条条记录

const char DataBase_UserName[] = "root";
const char DataBase_Password[] = "123456";
const char DataBase_Host[] = "localhost";//数据库连接地址
const char DataBase_Name[] = "test";
const unsigned int DataBase_Port = 3306;

#define QUERYBUFFERSIZE 1024
char querybuffer[QUERYBUFFERSIZE];

#define USERNAMELENGTH 10  //用户名最大长度
#define PASSWORDLENGTH 10  //密码最大长度




//其他
int ret;
bool cliuserlog[2];//用户是否登录
bool cliuserready[2];//用户是否准备
int cliuserheadid[2];//用户头像id
char cliusername[2][USERNAMELENGTH];//用户名
char cliuserinforpos[2][22] = {
	"firstpos",
	"secondpos"
};//用户信息位置
char cliusercolor[2][11] = {
	"black",
	"white"
};

#define BOARDHEIGHT 15
#define BOARDWIDTH 15
int chessboard[BOARDHEIGHT][BOARDWIDTH];

int chessexitnum = 0;



void dosign(int clinum) {//处理用户注册
	//提取用户名，密码
	char username[USERNAMELENGTH];//用户名
	char password[PASSWORDLENGTH];//密码
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
		printf("mysql 查询错误\n");
		return;
	}
	MYSQL_RES* res;//结果结构体
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);

	//判断用户名是否存在
	if (rownum == 0) {//用户名不存在
		//memset(querybuffer, 0, sizeof(querybuffer));
		sprintf(querybuffer, "insert into user values('%s','%s')", username, password);//插入用户名和密码
		ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
		if (ret != 0) {
			printf("mysql 插入错误\n");
			return;
		}
		//memset(sendbuffer,0,sizeof(sendbuffer));
		sprintf(sendbuffer, "_sign sign_success ",username);
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);

		//为新用户添加用户信息
		sprintf(querybuffer, "insert into userinfor values('%s',0,0,0,0,0,0,0)",username);
		ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
		if (ret != 0) {
			printf("mysql 插入错误\n");
			return;
		}
	}
	else {//用户名存在
		//memset(sendbuffer,0,sizeof(sendbuffer));
		sprintf(sendbuffer, "_sign username_exist ",username);
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	}
	mysql_free_result(res);
}

void douserinfor(int clifrnum,int clitonum) {//把clifrnum的信息发送给clitonum

	sprintf(querybuffer, "select * from userinfor where username='%s'", cliusername[clifrnum]);
	ret=mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql 查询错误\n");
		return;
	}
	MYSQL_RES* res;//结果结构体
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("查找用户信息错误\n");
		mysql_free_result(res);
		return;
	}

	//发送用户信息
	sprintf(sendbuffer, "_userinfor username %s %s score %s max_score %s win_rate %s total_match %s win_count %s lose_count %s draw_count %s head_id %d ",
		cliusername[clifrnum],
		cliuserinforpos[clifrnum],
		row[1], row[2], row[3], row[4], row[5], row[6], row[7],
		cliuserheadid[clifrnum]);
	send(clifd[clitonum], sendbuffer, strlen(sendbuffer),0);
	printf("sendbuffer:%s\n", sendbuffer);
	printf("将用户%s发送给用户%s信息成功\n",cliusername[clifrnum],cliusername[clitonum]);

	mysql_free_result(res);
	//printf("log username:%s\n", username);
	
}


void dolog(int clinum) {//处理用户登陆

	//提取用户名，密码
	char username[USERNAMELENGTH];//用户名
	char password[PASSWORDLENGTH];//密码
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
		printf("mysql 查询错误\n");
		return;
	}
	MYSQL_RES* res;//结果结构体
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);

	//判断用户名是否存在
	if (rownum == 0) {//不存在用户名
		//memset(sendbuffer,0,sizeof(sendbuffer));
		sprintf(sendbuffer, "_log username_wrong ",username);
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	}
	else {//用户名存在
		//密码是否正确
		row = mysql_fetch_row(res);
		if (strcmp(row[1], password) != 0) {//密码错误
			//memset(sendbuffer,0,sizeof(sendbuffer));
			sprintf(sendbuffer, "_log password_wrong ",username);
			send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
			mysql_free_result(res);
			return;
		}
		else {//密码正确
			//如果用户已经登录
			if (strcmp(username, cliusername[1 - clinum]) == 0){
				sprintf(sendbuffer, "_log user_loged ", username);
				send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
				printf("用户%s已登录\n",cliusername[1-clinum]);
				mysql_free_result(res);
				return;
			}

			//生成随机头像id
			srand((unsigned)time(NULL));//加上这一行是为了防止rand()的随机规律化
			if (cliuserheadid[clinum] == -1) {
				cliuserheadid[clinum] = rand() % 5;

				//避免生成一样的头像id
				if (cliuserheadid[clinum] == cliuserheadid[1 - clinum]) {
					cliuserheadid[clinum] = (cliuserheadid[clinum] + 1) % 5;
				}
			}
			
			cliuserlog[clinum] = true;

			//memset(sendbuffer,0,sizeof(sendbuffer));
			sprintf(sendbuffer, "_log password_right ",username);
			send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
			sprintf(cliusername[clinum], "%s", username);
			
			////告诉客户端自己是第几个进来  老版本需求  现在不用发
			//if (clifd[1-clinum] == -1) {
			//	sprintf(sendbuffer, "_%s firstentry ",username);
			//}
			//else {
			//	sprintf(sendbuffer, "_%s secondentry ",username);
			//}

			//send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
			//printf("sendbuffer:%s\n", sendbuffer);
			//printf("给用户%s发送第几个进来消息成功\n",cliusername[clinum]);

			//如果对方已经准备，向此用户发送对方已准备
			if (cliuserready[1 - clinum]){
				sprintf(sendbuffer, "_readyinfor %s ", cliuserinforpos[1-clinum]);
				send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
				printf("sendbuffer:%s\n", sendbuffer);
				printf("向用户%s发送用户%s 已准备消息成功\n",cliusername[clinum],cliusername[1-clinum]);
			}
			//发送用户信息
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
	//printf("收到ready消息\n");
	sprintf(sendbuffer, "_readyinfor %s ", cliuserinforpos[clinum]);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer:%s\n", sendbuffer);
	printf("发送ready消息给用户%s成功\n",cliusername[clinum]);
	if (cliuserlog[1 - clinum]){
		sprintf(sendbuffer, "_readyinfor %s ", cliuserinforpos[clinum]);
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer:%s\n", sendbuffer);
		printf("发送ready消息给用户%s成功\n",cliusername[1-clinum]);
	}
}

void dogamestart() {
	sprintf(sendbuffer, "_gamestart ");
	send(clifd[0], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送游戏开始消息成功\n", cliusername[0]);
	send(clifd[1], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送游戏开始消息成功\n", cliusername[1]);
}

void dochat(int clinum) {
	//"_chat username %s inputtext %s"
	char username[33];
	char chatbuffer[111];

	//过滤掉username标识符
	//memset(username, 0, sizeof(username));
	sscanf(recvbuffer + recvbufferidx, "%s", username);
	recvbufferidx += strlen(username) + 1;

	//读取出username
	//memset(username, 0, sizeof(username));
	sscanf(recvbuffer + recvbufferidx, "%s", username);
	recvbufferidx += strlen(username) + 1;

	//过滤掉inputext标识符
	//memset(chatbuffer, 0, sizeof(chatbuffer));
	sscanf(recvbuffer + recvbufferidx, "%s", chatbuffer);
	recvbufferidx += strlen(chatbuffer) + 1;

	//读取出聊天内容--------------------------------------------------------注意不能有空格---------------------------------
	//memset(chatbuffer, 0, sizeof(chatbuffer));
	sscanf(recvbuffer + recvbufferidx, "%s", chatbuffer);
	recvbufferidx += strlen(chatbuffer) + 1;
	
	//向双方发送聊天消息
	sprintf(sendbuffer, "_chat username %s inputtext %s ",username,chatbuffer);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("%s\n", sendbuffer);
	printf("向用户%s发送聊天消息成功\n", cliusername[clinum]);
	if (cliuserlog[1 - clinum]) {//如果对方登录了  才能看到消息   且看不到登录之前的消息
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("%s\n", sendbuffer);
		printf("向用户%s发送聊天消息成功\n", cliusername[1 - clinum]);
	}
}

void dowin(int cliwinnum) {

	//向两个客户端发送消息
	sprintf(sendbuffer, "_win %s ", cliusername[cliwinnum]);
	send(clifd[cliwinnum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送游戏胜利消息成功\n", cliusername[cliwinnum]);
	send(clifd[1 - cliwinnum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送游戏失败消息成功\n", cliusername[1-cliwinnum]);

}

bool isgg(int x,int y,int color) {
	int pa, pb;
	//横
	pa = pb = y;
	while (pa>=0 && chessboard[x][pa] == color) { pa--; }
	while (pb<BOARDWIDTH && chessboard[x][pb] == color) { pb++; }
	if (pb - pa - 1 >= 5) {
		return true;
	}

	//竖
	pa = pb = y;
	while (pa >= 0 && chessboard[pa][y] == color) { pa--; }
	while (pb < BOARDHEIGHT && chessboard[pb][y] == color) { pb++; }
	if (pb - pa - 1 >= 5) {
		return true;
	}

	//右斜
	pa = pb = 1;
	while (x-pa>=0 && y+pa<BOARDWIDTH && chessboard[x-pa][y+pa] == color) { pa++; }
	while (x+pb < BOARDHEIGHT && y-pb>=0 && chessboard[x+pb][y-pb] == color) { pb++; }
	if (pb + pa - 1 >= 5) {
		return true;
	}

	//左斜
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
		printf("mysql 查询错误\n");
		return;
	}
	MYSQL_RES* res;//结果结构体
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("mysql 更新用户胜利消息时，未查询到用户结果\n");
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
		printf("mysql 更新错误\n");
		return;
	}
}

void updatelose(int clinum) {
	sprintf(querybuffer, "select score,max_score,total_match,win_count from userinfor where username='%s'", cliusername[clinum]);
	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql 查询错误\n");
		return;
	}
	MYSQL_RES* res;//结果结构体
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("mysql 更新用户失败消息时，未查询到用户结果\n");
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
		printf("mysql 更新错误\n");
		return;
	}
}

void updatedraw(int clinum) {
	sprintf(querybuffer, "select total_match,win_count from userinfor where username='%s'", cliusername[clinum]);
	ret = mysql_real_query(&mysql, querybuffer, (unsigned long)strlen(querybuffer));
	if (ret != 0) {
		printf("mysql 查询错误\n");
		return;
	}
	MYSQL_RES* res;//结果结构体
	res = mysql_store_result(&mysql);
	int rownum = mysql_num_rows(res);
	if (rownum == 0) {
		printf("mysql 更新用户失败消息时，未查询到用户结果\n");
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
		printf("mysql 更新错误\n");
		return;
	}
}


void domove(int clinum,int &x,int &y) {
	//服务器接受"_move _clientsend %d %d "
	//服务器发送"_move _serversend %s %d %d "

	//提取出棋子的x,y坐标
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

	//向两个客户端发送落子消息
	sprintf(sendbuffer, "_move serversend %s %s %s ", cliusercolor[clinum], coordinatex, coordinatey);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送落子消息成功\n", cliusername[clinum]);
	sprintf(sendbuffer, "_move serversend %s %s %s ", cliusercolor[clinum], coordinatex, coordinatey);
	send(clifd[1-clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送落子消息成功\n", cliusername[1-clinum]);

}

void doround(int clinum) {
	sprintf(sendbuffer, "_round %s ", cliusername[1-clinum]);
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送轮到用户%s回合消息成功\n", cliusername[clinum], cliusername[1 - clinum]);
	send(clifd[1-clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送轮到用户%s回合消息成功\n", cliusername[1-clinum], cliusername[1 - clinum]);
}

void dodrawrequest(int clinum) {
	sprintf(sendbuffer, "_other_want_to_draw ");
	send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer ; %s\n", sendbuffer);
	printf("向用户%s发送和棋请求成功\n", cliusername[1 - clinum]);
}

bool dodrawrespond(int clinum) {
	int result = -1;
	sscanf(recvbuffer + recvbufferidx, "%d", &result);
	recvbufferidx += 2;
	if (!(result == 0 || result == 1)) {
		printf("result = %d 获取和棋请求错误\n", result);
		return false;
	}
	if (result == 0) {
		sprintf(sendbuffer, "_draw_is_fail ");
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer : %s\n", sendbuffer);
		printf("向用户%s发送不同意和棋消息成功\n", cliusername[1 - clinum]);
		return false;
	}
	if (result == 1) {
		sprintf(sendbuffer, "_draw_is_ok ");
		send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer : %s\n", sendbuffer);
		printf("向用户%s发送同意和棋消息成功\n", cliusername[clinum]);
		send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
		printf("sendbuffer : %s\n", sendbuffer);
		printf("向用户%s发送同意和棋消息成功\n", cliusername[1 - clinum]);
		return true;
	}
	return true;
}

void dodogfall() {
	sprintf(sendbuffer, "_dogfall ");
	send(clifd[0], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送平局消息成功\n", cliusername[0]);
	send(clifd[1], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送平局消息成功\n", cliusername[1]);
}

void docapitulate(int clinum) {
	sprintf(sendbuffer, "_capitulate_is_ok ");
	send(clifd[clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送认输消息成功\n", cliusername[clinum]);
	sprintf(sendbuffer, "_other_is_capitulate ");
	send(clifd[1-clinum], sendbuffer, strlen(sendbuffer), 0);
	printf("sendbuffer : %s\n", sendbuffer);
	printf("向用户%s发送认输消息成功\n", cliusername[1-clinum]);
}

void doregret(int clinum) {

}

bool doreturnmenu(int clinum) {//游戏开始返回true;否则，返回false;

	//如果对方已经在房间内，向对方发送自己返回菜单的消息
	if (cliuserlog[1 - clinum]) {
		if (!cliuserready[0] || !cliuserready[1]) {//游戏未开始
			sprintf(sendbuffer, "_other_return_to_menu _no_game_start ", cliuserinforpos[clinum]);
			send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
			printf("sendbuffer");
			printf("向用户%s发送用户%s已返回菜单的消息成功\n", cliusername[1 - clinum], cliusername[clinum]);
			return false;
		}
		else {//游戏已经开始
			sprintf(sendbuffer, "_other_return_to_menu _game_start ", cliuserinforpos[clinum]);
			send(clifd[1 - clinum], sendbuffer, strlen(sendbuffer), 0);
			printf("sendbuffer");
			printf("向用户%s发送用户%s已返回菜单的消息成功\n", cliusername[1 - clinum], cliusername[clinum]);
			return true;
		}
	}
	return false;
}


void domessage(int clinum) {//clinum是向服务器发送请求的客户端；flag标志：是否向对方服务器发送请求
	recvbufferidx = 0;
	//使用sscanf处理recvbuffer.
	char waybuffer[22];
	while (recvbufferidx<strlen(recvbuffer)){
		printf("recvbufferidx : %d,  recvbufferlen : %d\n", recvbufferidx, strlen(recvbuffer));
		//memset(waybuffer, 0, sizeof(waybuffer));
		sscanf(recvbuffer + recvbufferidx, "%s", waybuffer);
		printf("waybuffer ： %s\n", waybuffer);
		recvbufferidx += strlen(waybuffer)+1;
		if (strcmp(waybuffer, "_sign") == 0) {//注册
			printf("注册\n");
			dosign(clinum);
		}
		if (strcmp(waybuffer, "_log") == 0) {//登录
			printf("登录\n");
			dolog(clinum);
		}
		
		if (strcmp(waybuffer, "_myreadyinfor") == 0) {//准备
			doready(clinum);

			//如果双方都准备
			if (cliuserready[0] && cliuserready[1]) {
				//发送游戏开始的消息
				dogamestart();
				doround(1);
			}
		}
		if (strcmp(waybuffer, "_chat") == 0) {//聊天
			dochat(clinum);
		}
		if (strcmp(waybuffer, "_move") == 0) {//落子
			int chessx;int chessy;
			domove(clinum,chessx,chessy);
			chessexitnum++;

			//判断游戏是否结束
			if (isgg(chessx, chessy, clinum + 1)) {
				//发送平局消息
				dodogfall();

				//更新用户积分
				updatedraw(clinum);
				updatedraw(1-clinum);

				//发送用户信息  
				douserinfor(0, 0);
				douserinfor(0, 1);
				douserinfor(1, 1);
				douserinfor(1, 0);

				//初始化棋局
				cliuserready[0] = cliuserready[1] = false;
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//继续处理未处理完的消息
			}

			//判断是否不能再下棋
			if (chessexitnum == BOARDHEIGHT * BOARDWIDTH) {

				//更新用户积分
				updatedraw(clinum);
				updatedraw(1 - clinum);

				//发送用户信息  
				douserinfor(0, 0);
				douserinfor(0, 1);
				douserinfor(1, 1);
				douserinfor(1, 0);

				//初始化棋局
				cliuserready[0] = cliuserready[1] = false;
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//继续处理未处理完的消息
			}

			//告诉客户端是谁的回合
			doround(clinum);
		}
		if (strcmp(waybuffer, "_I_want_to_draw") == 0) {//求和请求
			dodrawrequest(clinum);
		}
		if (strcmp(waybuffer, "_answer_draw_result") == 0) {//求和响应
			if (dodrawrespond(clinum)) {

				//更新用户积分
				updatedraw(clinum);
				updatedraw(1 - clinum);

				//发送用户信息  
				douserinfor(0, 0);
				douserinfor(0, 1);
				douserinfor(1, 1);
				douserinfor(1, 0);

				//初始化棋局
				cliuserready[0] = cliuserready[1] = false;
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//继续处理未处理完的消息
			}
		}
		if (strcmp(waybuffer, "_I_capitulate") == 0) {//认输

			//处理认输消息
			docapitulate(clinum);

			//更新用户积分
			updatewin(1-clinum);
			updatelose(clinum);

			//发送用户信息  
			douserinfor(0, 0);
			douserinfor(0, 1);
			douserinfor(1, 1);
			douserinfor(1, 0);

			//初始化棋局
			cliuserready[0] = cliuserready[1] = false;
			memset(chessboard, 0, sizeof(chessboard));
			chessexitnum = 0;

			continue;//继续处理未处理完的消息
		}

		if (strcmp(waybuffer, "_regret") == 0) {//悔棋
			doregret(clinum);
		}
		
		if (strcmp(waybuffer, "_I_return_to_menu") == 0) {//返回菜单

			//处理消息
			if (doreturnmenu(clinum)) {

				//更新用户积分
				updatewin(1 - clinum);
				updatelose(clinum);

				//发送用户信息  
				douserinfor(1 - clinum, 1 - clinum);

				//初始化棋局
				cliuserready[0] = cliuserready[1] = false;
				cliuserlog[clinum] = false;
				cliuserheadid[clinum] = -1;
				memset(cliusername[clinum], 0, sizeof(cliusername[clinum]));
				memset(chessboard, 0, sizeof(chessboard));
				chessexitnum = 0;

				continue;//继续处理未处理完的消息
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

//游戏结束和断开连接后后记得将相关变量初始化

int main() {

	//数据库初始化
	mysql_init(&mysql);
	//连接mysql,数据库
	if (!(mysql_real_connect(&mysql, DataBase_Host, DataBase_UserName, DataBase_Password, DataBase_Name, DataBase_Port, NULL, 0))) {
		printf("error connecting to database:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}

	//1.创建socket
	SOCKET listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == listenfd)
	{
		WSACleanup();
		printf("创建socket失败\n");
		return -1;
	}

	//2.绑定socket
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &saddr.sin_addr.S_un.S_addr);
	saddr.sin_port = htons(9999);
	ret = bind(listenfd, (sockaddr*)&saddr, sizeof(saddr));
	if (ret == -1) {
		printf("绑定失败\n");
		WSACleanup();
		closesocket(listenfd);
		return -1;
	}

	//3.监听socket
	ret = listen(listenfd, 5);
	if (ret == -1) {
		printf("监听失败\n");

		WSACleanup();
		closesocket(listenfd);
		return -1;
	}

	//网络连接初始化
	conninit();

	fd_set readfds,tmpfds;
	FD_ZERO(&readfds);
	FD_SET(listenfd, &readfds);
	while (1) {
		tmpfds = readfds;
		ret = select(0, &tmpfds, NULL, NULL, NULL);
		printf("readfds中有%d个fd\n",readfds.fd_count);
		if (ret > 0) {
			printf("ret=%d\n", ret);
			if (FD_ISSET(listenfd, &tmpfds)) {//连接请求
				if (clifd[0] == -1) {//先连接的是先手
					clifd[0] = accept(listenfd, (sockaddr*)&cliaddr[0], &cliaddrlen);
					
					//printf("%d\n", clifd[0]);
					if (clifd[0] == INVALID_SOCKET) {
						printf("accept连接失败\n");
					}
					else {
						printf("0号连接成功\n");
						FD_SET(clifd[0], &readfds);
						//memset(sendbuffer, 0, sizeof(sendbuffer));

					}
					
				}
				else if (clifd[1]==-1) {
					clifd[1] = accept(listenfd, (sockaddr*)&cliaddr[1], &cliaddrlen);
					if (clifd[1] == INVALID_SOCKET) {
						printf("accept连接失败\n");
					}
					else {
						printf("1号连接成功\n");
						FD_SET(clifd[1], &readfds);
						//memset(sendbuffer, 0, sizeof(sendbuffer));

					}
				}
			}
			for (int i = 0;i < 2;i++) {
				if (clifd[i] != -1 && FD_ISSET(clifd[i], &tmpfds)) {//有请求
					memset(recvbuffer, 0, sizeof(recvbuffer));
					int len = recv(clifd[i], recvbuffer, RECVBUFFERSIZE, 0);
					printf("recvbuffer:%s\n", recvbuffer);
					if (len > 0) {
						
						domessage(i);

					}

					else if (len == 0) {//连接正常断开  对方关闭socket的情况
						cliuserready[i] = false;
						cliuserlog[i] = false;
						cliuserheadid[i] = -1;
						memset(cliusername[i], 0, sizeof(cliusername[i]));
						memset(chessboard, 0, sizeof(chessboard));
						chessexitnum = 0;
 						printf("连接正常断开\n");
						FD_CLR(clifd[i], &readfds);
						clifd[i] = -1;
					}
					else if (len == -1) {//连接意外断开 对方未关闭socket就断开连接了
						printf("连接意外断开\n");

						//如果对方已经在房间内，向对方发送自己断开连接的消息
						if (cliuserlog[1 - i]) {
							if (!cliuserready[0] || !cliuserready[1]) {//游戏未开始
								sprintf(sendbuffer, "_exit %s _no_game_start ", cliuserinforpos[i]);
								send(clifd[1 - i], sendbuffer, strlen(sendbuffer), 0);
								printf("sendbuffer");
								printf("向用户%s发送用户%s已断开连接的消息成功\n", cliusername[1 - i], cliusername[i]);

								//初始化棋局
								cliuserready[i] = false;
								cliuserlog[i] = false;
								cliuserheadid[i] = -1;
								memset(cliusername[i], 0, sizeof(cliusername[i]));
								memset(chessboard, 0, sizeof(chessboard));
								chessexitnum = 0;
							}
							else {//游戏已经开始
								sprintf(sendbuffer, "_exit %s _game_start ", cliuserinforpos[i]);
								send(clifd[1 - i], sendbuffer, strlen(sendbuffer), 0);
								printf("sendbuffer");
								printf("向用户%s发送用户%s已断开连接的消息成功\n", cliusername[1 - i], cliusername[i]);

								//更新用户积分
								updatewin(1 - i);
								updatelose(i);

								//发送用户信息  
								douserinfor(1 - i, 1 - i);

								//初始化棋局
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
			if (m.message == WM_LBUTTONDOWN) {
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

*/