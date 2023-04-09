#include<windows.h>
#include<stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
using namespace std;
void showMenu();
void Client();
void Server();

int main() {
	int flag;
	showMenu();
	scanf("%d", &flag);
	while (flag) {
		switch (flag)
		{
			case 0:
				printf("关闭!\n");
				break;
			case 1:
				Client();
				break;

			case 2:
				Server();
				break;
			default:
				break;
				// exit
		}
		showMenu();
		scanf("%d", &flag);
	}
	return 0;
}		
void showMenu() {
	//printf("\n*************************************************************************************\n");
	printf("************************************选择功能************************************\n");
	printf("1.客户端\n");
	printf("2.服务器\n");

}

void Client() {
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	Sleep(500);
	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//输出接收到的数据
	printf("来自服务器的数据: %s\n", szBuffer);
	//关闭套接字
	closesocket(sock);
	//终止使用 DLL
	WSACleanup();
}
void Server() {
	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//进入监听状态
	listen(servSock, 20);
	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	//向客户端发送数据
	char str[32] = "Socket Test!";
	send(clntSock, str, strlen(str) + sizeof(char), NULL);
	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);
	//终止 DLL 的使用
	WSACleanup();
}
