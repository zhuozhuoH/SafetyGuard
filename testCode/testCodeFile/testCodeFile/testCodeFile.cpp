#include<windows.h>
#include<stdio.h>
#include <stdlib.h>
//#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
//#include <wchar.h>
#define PAGE_SIZE	4096
using namespace std;

void showMenu();
void write();
void read();
void ExProgram();
void Replication();
void AnotherFolder();

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
				write();
				break;
			case 2:
				read();
				break;
			case 3:
				ExProgram();
				break;
			case 4:
				Replication();
				break;
			case 5:
				AnotherFolder();
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

void showMenu()
{
	//printf("\n*************************************************************************************\n");
	printf("***********************************选择功能***********************************\n");
	printf("--正常操作--:\n");
	printf("1.写文件  2.读文件   \n");

	printf("--危险操作--:\n");
	printf("3.修改可执行文件   4.自我复制    5.打开多个文件夹\n");
	printf("");
}

void write()
{
	CHAR* pBuffer;
	char writebuf[100];
	bool flag;
	HANDLE hOpenFile = (HANDLE)CreateFile(L"test.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hOpenFile == INVALID_HANDLE_VALUE)
	{
		hOpenFile = NULL;
		return;
	}
	printf("输入字符:");
	scanf("%s", writebuf);
	flag = WriteFile(hOpenFile, writebuf, strlen(writebuf), NULL, NULL);
	if (flag) {
		printf("已经写入\n");
	}
	FlushFileBuffers(hOpenFile);
	CloseHandle(hOpenFile);
}
void read() {
	CHAR* pBuffer;
	int filelength = 0;
	bool flag;
	HANDLE hOpenFile = (HANDLE)CreateFile(L"test.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	if (hOpenFile == INVALID_HANDLE_VALUE)
	{
		hOpenFile = NULL;
		return;
	}
	filelength = GetFileSize(hOpenFile, NULL);
	pBuffer = (char*)malloc((filelength + 1) * sizeof(char));
	flag = ReadFile(hOpenFile, pBuffer, filelength, NULL, NULL);
	pBuffer[filelength] = 0;
	if (flag) {
		printf("成功读取到字符串:%s\n", pBuffer);
	}
	free(pBuffer);
	CloseHandle(hOpenFile);
}


void ExProgram() {
	HANDLE hOpenFile = (HANDLE)CreateFile(L"test.exe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	CloseHandle(hOpenFile);
}
void Replication() {
	HANDLE hOpenFile = (HANDLE)CreateFile(L"testCodeFile.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	CloseHandle(hOpenFile);
}

void AnotherFolder() {
	HANDLE hOpenFile = (HANDLE)CreateFile(L"./test/test.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, NULL, nullptr);
	printf("(HANDLE)hOpenFile: %08X\n", hOpenFile);
	CloseHandle(hOpenFile);
}
