#ifndef NETWORK
#define NETWORK

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>

//网络通信部分
#define SERVERIP "42.192.128.175"
//网络通信部分
SOCKET fd;//与客户端通信的文件描述符
#define SENDBUFFERSIZE 2048  //数组sendbuffer的大小
#define RECVBUFFERSIZE 1024  //数组recvbuffer的大小

char sendbuffer[SENDBUFFERSIZE];
char recvbuffer[RECVBUFFERSIZE];
void network_connect() {

	//1. 确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);//wsastartup():根据当前版本进行相应socket库的绑定
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)//检查WAStartup确定的协议版本是否成功
	{
		printf("确定协议版本失败!\n");
		return;
	}

	//2.创建socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == INVALID_SOCKET) {
		printf("创建SOCKET失败\n");
		WSACleanup();
		return;
	}

	//3.连接服务器
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &saddr.sin_addr.S_un.S_addr);//这里需要改成本机的IP地址--------------------
	saddr.sin_port = htons(11111);
	int ret = connect(fd, (sockaddr*)&saddr, sizeof(saddr));
	if (ret == SOCKET_ERROR) {
		printf("连接失败\n");
		int errorcode = WSAGetLastError();
		printf("errorcode:%d\n", errorcode);
		//FormatMessage("连接失败\n");
		WSACleanup();
		closesocket(fd);
		return;
	}
	printf("连接成功\n");

}

#endif