#include<windows.h>
#include<stdio.h>
#include <stdlib.h>
//#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
//#include <wchar.h>
#define PAGE_SIZE	4096
using namespace std;
void CreateAndDestory();
void showMenu();
void TwiceRelease();
int main() {
	int flag;
	showMenu();
	scanf("%d", &flag);
	while (flag) {
		switch (flag)
		{
			case 0:
				printf("bye!\n");
				break;
			case 1: 
				CreateAndDestory();
				break;
			case 2: 
				TwiceRelease();
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
	printf("--------------------------------please select an option--------------------------------\n");
	printf("--正常操作--:\n");
	printf("1.CreateAndDestory\n");
	printf("--危险操作--:\n");
	printf("2.二次释放\n");
}
void CreateAndDestory() {
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
	int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 15);
	for (int i = 0; i < 15; ++i)
	{
		pArr[i] = i + 1;
	}
	for (int i = 0; i < 15; ++i)
	{
		if (i % 5 == 0)
			printf_s("\n");
		printf("%3d ", pArr[i]);
	}
	getchar();
	HeapFree(hHeap, 0, pArr);
	HeapDestroy(hHeap);
}

void TwiceRelease() {
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
	int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 15);
	for (int i = 0; i < 15; ++i)
	{
		pArr[i] = i + 1;
	}
	for (int i = 0; i < 15; ++i)
	{
		if (i % 5 == 0)
			printf_s("\n");
		printf("%3d ", pArr[i]);
	}
	printf_s("\n\n");
	printf("请输入任意键，完成第一次释放!\n");
	getchar();
	HeapFree(hHeap, 0, pArr);
	printf("请输入任意键，完成第二次释放!\n");
	getchar();
	HeapFree(hHeap, 0, pArr);
	HeapDestroy(hHeap);
}
