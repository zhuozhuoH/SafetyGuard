#include<windows.h>
#include<stdio.h>
#include <stdlib.h>
//#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
//#include <wchar.h>
#define PAGE_SIZE	4096
using namespace std;

void showMenu();
void regCreateSetValue();
void regOpenDelValue();
void StartupRegistry();

int main() {
	int flag = 0;
	showMenu();
	scanf("%d", &flag);
	while (flag) {
		switch (flag)
		{
			case 0: {
				printf("退出\n");
				break;
			}
			case 1: 
				regCreateSetValue();
				break;
			case 2: 
				regOpenDelValue();
				break;
			case 3: 
				StartupRegistry();
				break;
			}
	}
	return 0;
}
void showMenu() {
	printf("**************************************选择功能**************************************\n");
	printf("--正常操作--:\n");
	printf("1.注册表创建设值   2.注册表开启和删除\n");
	printf("--危险操作--:\n");
	printf("3.修改注册表启动项\n");
}

void regCreateSetValue() {
	// 创建注册表并设置键值
	HKEY hKey = NULL;
	TCHAR Data[254];
	memset(Data, 0, sizeof(Data));
	wcsncpy_s(Data, TEXT("test"), 254);

	size_t lRet = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"TestKey", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (lRet == ERROR_SUCCESS) {
		printf("创建成功!\n");
	}
	else {
		printf("创建失败!\n");
	}
	// 修改注册表键值，没有则创建它
	size_t iLen = wcslen(Data);
	// 设置键值
	lRet = RegSetValueEx(hKey, L"test", 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
	if (lRet == ERROR_SUCCESS)
	{
		printf("设置值成功!\n");
		return;
	}
	else {
		printf("设值失败!\n");
	}
	RegCloseKey(hKey);
}

void regOpenDelValue() {
	HKEY hKey = NULL;
	size_t lRet = RegOpenKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"TestKey", 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS) {
		printf("打开成功!\n");
	}
	else {
		printf("打开失败\n");
	}
	lRet = RegDeleteValue(hKey, L"test");
	if (lRet == ERROR_SUCCESS) {
		printf("删除成功!\n");
	}
	else {
		printf("删除失败!\n");
	}
	RegCloseKey(hKey);
}
void StartupRegistry() {
	HKEY hKey = NULL;
	size_t lRet = RegOpenKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);
	if (lRet == ERROR_SUCCESS) {
		printf("打开成功!\n");
	}
	else {
		printf("打开失败\n");
	}
	RegCloseKey(hKey);
}
