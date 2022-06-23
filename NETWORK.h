#ifndef NETWORK
#define NETWORK

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>

//����ͨ�Ų���
#define SERVERIP "42.192.128.175"
//����ͨ�Ų���
SOCKET fd;//��ͻ���ͨ�ŵ��ļ�������
#define SENDBUFFERSIZE 2048  //����sendbuffer�Ĵ�С
#define RECVBUFFERSIZE 1024  //����recvbuffer�Ĵ�С

char sendbuffer[SENDBUFFERSIZE];
char recvbuffer[RECVBUFFERSIZE];
void network_connect() {

	//1. ȷ��Э��汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);//wsastartup():���ݵ�ǰ�汾������Ӧsocket��İ�
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)//���WAStartupȷ����Э��汾�Ƿ�ɹ�
	{
		printf("ȷ��Э��汾ʧ��!\n");
		return;
	}

	//2.����socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == INVALID_SOCKET) {
		printf("����SOCKETʧ��\n");
		WSACleanup();
		return;
	}

	//3.���ӷ�����
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &saddr.sin_addr.S_un.S_addr);//������Ҫ�ĳɱ�����IP��ַ--------------------
	saddr.sin_port = htons(11111);
	int ret = connect(fd, (sockaddr*)&saddr, sizeof(saddr));
	if (ret == SOCKET_ERROR) {
		printf("����ʧ��\n");
		int errorcode = WSAGetLastError();
		printf("errorcode:%d\n", errorcode);
		//FormatMessage("����ʧ��\n");
		WSACleanup();
		closesocket(fd);
		return;
	}
	printf("���ӳɹ�\n");

}

#endif