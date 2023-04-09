#include<windows.h>
#include<stdio.h>
#include <stdlib.h>
//#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
//#include <wchar.h>
#define PAGE_SIZE	4096
using namespace std;
void showMenu();
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
				MessageBoxA(NULL, "Test MessageBoxA", "MessageBoxA", MB_OK);
				break;
			default:
				break;
		}
		showMenu();
		scanf("%d", &flag);
	}
	return 0;
}
void showMenu() {
	printf("*************************************选择操作*************************************\n");
	printf("1.MessageBoxA\n");
}
