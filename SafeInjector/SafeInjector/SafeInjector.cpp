#include<iostream>
#include<cstdio>
#include<windows.h>
#include<detours.h>
#include<thread>
#pragma comment(lib, "detours.lib")
using namespace std;
int main(int argc, char* argv[]) {
	WCHAR testCode[512] = {};
	MultiByteToWideChar(CP_ACP, 0, argv[0], strlen(argv[0]), testCode, sizeof(testCode));
	wprintf(L"%s\n", testCode);

	WCHAR DLL_Dir[512]{};// dll文件夹
	MultiByteToWideChar(CP_ACP, 0, argv[1], strlen(argv[1]), DLL_Dir, sizeof(DLL_Dir));
	wprintf(L"%s\n", DLL_Dir);

	char DLL[512]{};
	strcpy(DLL, argv[2]);
	printf("%s\n", DLL);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);

	if (DetourCreateProcessWithDllEx(testCode, NULL, NULL, NULL, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DLL_Dir,
		&si, &pi, DLL, NULL)) {
		// 恢复线程
		ResumeThread(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	else {
		char error[100];
		sprintf(error, "%d", GetLastError());
	}
	return 0;
}