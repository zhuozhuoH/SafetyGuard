// dllmain.cpp : 定义 DLL 应用程序的入口点。
// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "framework.h"
#include "stdio.h"
#include "stdarg.h"
#include "windows.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <WinSock2.h>
#include "detours.h"

#pragma comment(lib, "detours.lib")
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll


enum API_Type {
	MESSAGEBOXA, CREATEFILE, WRITEFILE, READFILE, HEAPCREATE, HEAPALLOC, HEAPFREE, HEAPDESTORY,
	REGCREATEKEYEX, REGOPENKEYEX, REGSETVALUEEX, REGDELETEVALUE, REGCLOSEKEY,
	SOCKET_, BIND, CONNECT, SEND, RECV
};
using namespace std;


struct API_info {
	API_Type type{};
	int parameterNum{};
	SYSTEMTIME time{};
	char parameterName[10][30]{};
	char parameterValue[10][70]{};
};




API_info api_hook;

HANDLE hSemaphoreA = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"SemaphoreA");
HANDLE hSemaphoreB = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"SemaphoreB");
HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, L"ShareMemory");
LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(API_info));



// 第一和引入需要hool的函数和替换的函数
// 定义需要hook的函数
static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;
// 定义需要替换的新的函数
extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
	// 向api_hook中写入数据
	api_hook.type = MESSAGEBOXA;
	api_hook.parameterNum = 4;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hWnd");
	sprintf(api_hook.parameterName[1], "lpText");
	sprintf(api_hook.parameterName[2], "lpCaption");
	sprintf(api_hook.parameterName[3], "uType");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hWnd);
	sprintf(api_hook.parameterValue[1], "%s", lpText);
	sprintf(api_hook.parameterValue[2], "%s", lpCaption);
	sprintf(api_hook.parameterValue[3], "%08X", uType);

	WaitForSingleObject(hSemaphoreA, INFINITE); 
	memcpy(lpBase, &api_hook, sizeof(API_info)); 
	ReleaseSemaphore(hSemaphoreB, 1, NULL); 
	WaitForSingleObject(hSemaphoreA, INFINITE); 
	ReleaseSemaphore(hSemaphoreA, 1, NULL); 

	return OldMessageBoxA(hWnd, lpText, lpCaption, uType);

}


// 打开文件
static HANDLE(WINAPI* OldCreateFile)(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	) = CreateFile;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
)
{
	HANDLE hFile = OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	if (GetFileType(hFile) == FILE_TYPE_DISK) {
		char temp[70];
		api_hook.type = CREATEFILE;
		api_hook.parameterNum = 7;
		GetLocalTime(&(api_hook.time));
		// 参数名
		sprintf(api_hook.parameterName[0], "lpFileName");
		sprintf(api_hook.parameterName[1], "dwDesiredAccess");
		sprintf(api_hook.parameterName[2], "dwShareMode");
		sprintf(api_hook.parameterName[3], "lpSecurityAttributes");
		sprintf(api_hook.parameterName[4], "dwCreationDisposition");
		sprintf(api_hook.parameterName[5], "dwFlagsAndAttributes");
		sprintf(api_hook.parameterName[6], "hTemplateFile");
		// 参数值
		// 宽字节转char
		memset(temp, 0, sizeof(temp));
		WideCharToMultiByte(CP_ACP, 0, lpFileName, wcslen(lpFileName), temp, sizeof(temp), NULL, NULL);
		strcpy(api_hook.parameterValue[0], temp);
		sprintf(api_hook.parameterValue[1], "%08X", dwDesiredAccess);
		sprintf(api_hook.parameterValue[2], "%08X", dwShareMode);
		sprintf(api_hook.parameterValue[3], "%08X", lpSecurityAttributes);
		sprintf(api_hook.parameterValue[4], "%08X", dwCreationDisposition);
		sprintf(api_hook.parameterValue[5], "%08X", dwFlagsAndAttributes);
		sprintf(api_hook.parameterValue[6], "%08X", hTemplateFile);

		WaitForSingleObject(hSemaphoreA, INFINITE);
		memcpy(lpBase, &api_hook, sizeof(API_info));
		ReleaseSemaphore(hSemaphoreB, 1, NULL);
		WaitForSingleObject(hSemaphoreA, INFINITE);
		ReleaseSemaphore(hSemaphoreA, 1, NULL);
	}
	return hFile;
}

// 写文件
static BOOL(WINAPI* OldWriteFile)(
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
	) = WriteFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
)
{
	if (GetFileType(hFile) == FILE_TYPE_DISK)
	{
		api_hook.type = WRITEFILE;
		api_hook.parameterNum = 5;
		GetLocalTime(&(api_hook.time));
		// 参数名
		sprintf(api_hook.parameterName[0], "hFile");
		sprintf(api_hook.parameterName[1], "lpBuffer");
		sprintf(api_hook.parameterName[2], "nNumberOfBytesToWrite");
		sprintf(api_hook.parameterName[3], "lpNumberOfBytesWritten");
		sprintf(api_hook.parameterName[4], "lpOverlapped");
		// 参数值
		sprintf(api_hook.parameterValue[0], "%08X", hFile);
		sprintf(api_hook.parameterValue[1], "%08X", lpBuffer);
		sprintf(api_hook.parameterValue[2], "%08X", nNumberOfBytesToWrite);
		sprintf(api_hook.parameterValue[3], "%08X", lpNumberOfBytesWritten);
		sprintf(api_hook.parameterValue[4], "%08X", lpOverlapped);

		WaitForSingleObject(hSemaphoreA, INFINITE);
		memcpy(lpBase, &api_hook, sizeof(API_info));
		ReleaseSemaphore(hSemaphoreB, 1, NULL);
		WaitForSingleObject(hSemaphoreA, INFINITE);
		ReleaseSemaphore(hSemaphoreA, 1, NULL);
	}
	return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

static BOOL(WINAPI* OldReadFile)(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
	) = ReadFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
)
{
	if (GetFileType(hFile) == FILE_TYPE_DISK) {
		api_hook.type = READFILE;
		api_hook.parameterNum = 5;
		GetLocalTime(&(api_hook.time));
		// 参数名
		sprintf(api_hook.parameterName[0], "hFile");
		sprintf(api_hook.parameterName[1], "lpBuffer");
		sprintf(api_hook.parameterName[2], "nNumberOfBytesToRead");
		sprintf(api_hook.parameterName[3], "lpNumberOfBytesRead");
		sprintf(api_hook.parameterName[4], "lpOverlapped");
		// 参数值
		sprintf(api_hook.parameterValue[0], "%08X", hFile);
		sprintf(api_hook.parameterValue[1], "%08X", lpBuffer);
		sprintf(api_hook.parameterValue[2], "%08X", nNumberOfBytesToRead);
		sprintf(api_hook.parameterValue[3], "%08X", lpNumberOfBytesRead);
		sprintf(api_hook.parameterValue[4], "%08X", lpOverlapped);

		WaitForSingleObject(hSemaphoreA, INFINITE);
		memcpy(lpBase, &api_hook, sizeof(API_info));
		ReleaseSemaphore(hSemaphoreB, 1, NULL);
		WaitForSingleObject(hSemaphoreA, INFINITE);
		ReleaseSemaphore(hSemaphoreA, 1, NULL);
	}
	return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

// 堆操作
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;
extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
	HANDLE handle = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
	api_hook.type = HEAPCREATE;
	api_hook.parameterNum = 4;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "fIOoptions");
	sprintf(api_hook.parameterName[1], "dwInitialSize");
	sprintf(api_hook.parameterName[2], "dwMaximumSize");
	sprintf(api_hook.parameterName[3], "HANDLE");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", fIOoptions);
	sprintf(api_hook.parameterValue[1], "%08X", dwInitialSize);
	sprintf(api_hook.parameterValue[2], "%08X", dwMaximumSize);
	sprintf(api_hook.parameterValue[3], "%08X", handle);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return handle;
}

static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) = HeapAlloc;

extern "C" __declspec(dllexport)LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
	auto lpMem = OldHeapAlloc(hHeap, dwFlags, dwBytes);
	api_hook.type = HEAPALLOC;
	api_hook.parameterNum = 3;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hHeap");
	sprintf(api_hook.parameterName[1], "dwFlags");
	sprintf(api_hook.parameterName[2], "dwBytes");
	sprintf(api_hook.parameterName[3], "lpMem");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hHeap);
	sprintf(api_hook.parameterValue[1], "%08X", dwFlags);
	sprintf(api_hook.parameterValue[2], "%08X", dwBytes);
	sprintf(api_hook.parameterValue[3], "%08X", lpMem);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return lpMem;
}

static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) {
	api_hook.type = HEAPFREE;
	api_hook.parameterNum = 3;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hHeap");
	sprintf(api_hook.parameterName[1], "dwFlags");
	sprintf(api_hook.parameterName[2], "lpMem");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hHeap);
	sprintf(api_hook.parameterValue[1], "%08X", dwFlags);
	sprintf(api_hook.parameterValue[2], "%08X", lpMem);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return OldHeapFree(hHeap, dwFlags, lpMem);
}

static BOOL(WINAPI* OldHeapDestory)(HANDLE) = HeapDestroy;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestory(HANDLE hHeap)
{
	api_hook.type = HEAPDESTORY;
	api_hook.parameterNum = 1;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hHeap");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hHeap);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return OldHeapDestory(hHeap);
}

static LSTATUS(WINAPI* OldRegCreateKeyEx)(
	HKEY                        hKey,
	LPCWSTR                     lpSubKey,
	DWORD                       Reserved,
	LPWSTR                      lpClass,
	DWORD                       dwOptions,
	REGSAM                      samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY                       phkResult,
	LPDWORD                     lpdwDisposition) = RegCreateKeyEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyEx(
	HKEY                        hKey,
	LPCWSTR                     lpSubKey,
	DWORD                       Reserved,
	LPWSTR                      lpClass,
	DWORD                       dwOptions,
	REGSAM                      samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY                       phkResult,
	LPDWORD                     lpdwDisposition
) {
	char temp[70];
	api_hook.type = REGCREATEKEYEX;
	api_hook.parameterNum = 9;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hKey");
	sprintf(api_hook.parameterName[1], "lpSubKey");
	sprintf(api_hook.parameterName[2], "Reserved");
	sprintf(api_hook.parameterName[3], "lpClass");
	sprintf(api_hook.parameterName[4], "dwOptions");
	sprintf(api_hook.parameterName[5], "samDesired");
	sprintf(api_hook.parameterName[6], "lpSecurityAttributes");
	sprintf(api_hook.parameterName[7], "phkResult");
	sprintf(api_hook.parameterName[8], "lpdwDisposition");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hKey);
	// 宽字节转char
	memset(temp, 0, sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), temp, sizeof(temp), NULL, NULL);
	strcpy(api_hook.parameterValue[1], temp);
	sprintf(api_hook.parameterValue[2], "%08X", Reserved);
	sprintf(api_hook.parameterValue[3], "%08X", lpClass);
	sprintf(api_hook.parameterValue[4], "%08X", dwOptions);
	sprintf(api_hook.parameterValue[5], "%08X", samDesired);
	sprintf(api_hook.parameterValue[6], "%08X", lpSecurityAttributes);
	sprintf(api_hook.parameterValue[7], "%08X", phkResult);
	sprintf(api_hook.parameterValue[8], "%08X", lpdwDisposition);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);

	return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);

}

static LSTATUS(WINAPI* OldRegSetValueEx)(
	HKEY       hKey,
	LPCWSTR    lpValueName,
	DWORD      Reserved,
	DWORD      dwType,
	const BYTE* lpData,
	DWORD      cbData
	) = RegSetValueEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueEx(
	HKEY       hKey,
	LPCWSTR    lpValueName,
	DWORD      Reserved,
	DWORD      dwType,
	const BYTE * lpData,
	DWORD      cbData)
{
	char temp[70];
	api_hook.type = REGSETVALUEEX;
	api_hook.parameterNum = 6;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hKey");
	sprintf(api_hook.parameterName[1], "lpValueName");
	sprintf(api_hook.parameterName[2], "Reserved");
	sprintf(api_hook.parameterName[3], "dwType");
	sprintf(api_hook.parameterName[4], "lpData");
	sprintf(api_hook.parameterName[5], "cbData");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hKey);
	// 宽字节转char
	memset(temp, 0, sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), temp, sizeof(temp), NULL, NULL);
	strcpy(api_hook.parameterValue[1], temp);
	sprintf(api_hook.parameterValue[2], "%08X", Reserved);
	sprintf(api_hook.parameterValue[3], "%08X", dwType);
	memset(temp, 0, sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)lpData, wcslen((LPCWCH)lpData), temp, sizeof(temp), NULL, NULL);
	strcpy(api_hook.parameterValue[4], temp);
	//strcpy(api_hook.parameterValue[4], (const char *)lpData);
	//sprintf(api_hook.parameterValue[4], "%08X", lpData);
	sprintf(api_hook.parameterValue[5], "%08X", cbData);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey)
{
	api_hook.type = REGCLOSEKEY;
	api_hook.parameterNum = 1;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hKey");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hKey);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return OldRegCloseKey(hKey);
}

static LSTATUS(WINAPI* OldRegOpenKeyEx)(
	HKEY    hKey,
	LPCWSTR lpSubKey,
	DWORD   ulOptions,
	REGSAM  samDesired,
	PHKEY   phkResult
	) = RegOpenKeyEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
	HKEY    hKey,
	LPCWSTR lpSubKey,
	DWORD   ulOptions,
	REGSAM  samDesired,
	PHKEY   phkResult)
{
	char temp[70];
	api_hook.type = REGOPENKEYEX;
	api_hook.parameterNum = 5;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hKey");
	sprintf(api_hook.parameterName[1], "lpSubKey");
	sprintf(api_hook.parameterName[2], "ulOptions");
	sprintf(api_hook.parameterName[3], "samDesired");
	sprintf(api_hook.parameterName[4], "phkResult");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hKey);
	// 宽字节转char
	memset(temp, 0, sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), temp, sizeof(temp), NULL, NULL);
	strcpy(api_hook.parameterValue[1], temp);
	sprintf(api_hook.parameterValue[2], "%08X", ulOptions);
	sprintf(api_hook.parameterValue[3], "%08X", samDesired);
	sprintf(api_hook.parameterValue[4], "%08X", phkResult);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

static LSTATUS(WINAPI* OldRegDeleteValue)(
	HKEY    hKey,
	LPCWSTR lpValueName
	) = RegDeleteValue;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValue(
	HKEY    hKey,
	LPCWSTR lpValueName)
{
	char temp[70];
	api_hook.type = REGDELETEVALUE;
	api_hook.parameterNum = 2;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "hKey");
	sprintf(api_hook.parameterName[1], "lpValueName");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", hKey);
	// 宽字节转char
	memset(temp, 0, sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), temp, sizeof(temp), NULL, NULL);
	strcpy(api_hook.parameterValue[1], temp);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return OldRegDeleteValue(hKey, lpValueName);
}

static SOCKET(WINAPI* Oldsocket)(
	int af,
	int type,
	int protocol
	) = socket;
extern "C" __declspec(dllexport)SOCKET WINAPI Newsocket(int af, int type, int protocol) {
	api_hook.type = SOCKET_;
	api_hook.parameterNum = 3;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "af");
	sprintf(api_hook.parameterName[1], "type");
	sprintf(api_hook.parameterName[2], "protocol");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", af);
	sprintf(api_hook.parameterValue[1], "%08X", type);
	sprintf(api_hook.parameterValue[2], "%08X", protocol);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return Oldsocket(af, type, protocol);
}

static int (WINAPI* Oldbind)(
	SOCKET         s,
	const sockaddr* name,
	int            namelen
	) = bind;
extern "C" __declspec(dllexport)int WINAPI Newbind(SOCKET s, const sockaddr * name, int namelen) {
	//name->sa_data
	api_hook.type = BIND;
	api_hook.parameterNum = 5;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "s");
	sprintf(api_hook.parameterName[1], "name");
	sprintf(api_hook.parameterName[2], "namelen");
	sprintf(api_hook.parameterName[3], "IP");
	sprintf(api_hook.parameterName[4], "port");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", s);
	sprintf(api_hook.parameterValue[1], "%08X", name);
	sprintf(api_hook.parameterValue[2], "%08X", namelen);
	struct sockaddr_in* sock = (struct sockaddr_in*)name;
	int port = ntohs(sock->sin_port);
	sprintf(api_hook.parameterValue[3], "%s", inet_ntoa(sock->sin_addr));
	sprintf(api_hook.parameterValue[4], "%d", port);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return Oldbind(s, name, namelen);

}

static int (WINAPI* Oldsend)(
	SOCKET     s,
	const char* buf,
	int        len,
	int        flags
	) = send;
extern "C" __declspec(dllexport)int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags) {
	api_hook.type = SEND;
	api_hook.parameterNum = 5;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "s");
	sprintf(api_hook.parameterName[1], "buf");
	sprintf(api_hook.parameterName[2], "len");
	sprintf(api_hook.parameterName[3], "flags");
	sprintf(api_hook.parameterName[4], "data");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", s);
	sprintf(api_hook.parameterValue[1], "%08X", buf);
	sprintf(api_hook.parameterValue[2], "%08X", len);
	sprintf(api_hook.parameterValue[3], "%08X", flags);
	sprintf(api_hook.parameterValue[4], "%s", buf);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return Oldsend(s, buf, len, flags);

}

static int (WINAPI* Oldconnect)(
	SOCKET         s,
	const sockaddr* name,
	int            namelen
	) = connect;
extern "C" __declspec(dllexport)int WINAPI Newconnect(SOCKET s, const sockaddr * name, int namelen) {
	//name->sa_data
	api_hook.type = CONNECT;
	api_hook.parameterNum = 5;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "s");
	sprintf(api_hook.parameterName[1], "name");
	sprintf(api_hook.parameterName[2], "namelen");
	sprintf(api_hook.parameterName[3], "IP");
	sprintf(api_hook.parameterName[4], "port");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", s);
	sprintf(api_hook.parameterValue[1], "%08X", name);
	sprintf(api_hook.parameterValue[2], "%08X", namelen);
	struct sockaddr_in* sock = (struct sockaddr_in*)name;
	int port = ntohs(sock->sin_port);
	sprintf(api_hook.parameterValue[3], "%s", inet_ntoa(sock->sin_addr));
	sprintf(api_hook.parameterValue[4], "%d", port);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return Oldconnect(s, name, namelen);

}

static int (WINAPI* Oldrecv)(
	SOCKET     s,
	char* buf,
	int        len,
	int        flags
	) = recv;
extern "C" __declspec(dllexport)int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags) {
	api_hook.type = RECV;
	api_hook.parameterNum = 4;
	GetLocalTime(&(api_hook.time));
	// 参数名
	sprintf(api_hook.parameterName[0], "s");
	sprintf(api_hook.parameterName[1], "buf");
	sprintf(api_hook.parameterName[2], "len");
	sprintf(api_hook.parameterName[3], "flags");
	// 参数值
	sprintf(api_hook.parameterValue[0], "%08X", s);
	sprintf(api_hook.parameterValue[1], "%08X", buf);
	sprintf(api_hook.parameterValue[2], "%08X", len);
	sprintf(api_hook.parameterValue[3], "%08X", flags);

	WaitForSingleObject(hSemaphoreA, INFINITE);
	memcpy(lpBase, &api_hook, sizeof(API_info));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);
	WaitForSingleObject(hSemaphoreA, INFINITE);
	ReleaseSemaphore(hSemaphoreA, 1, NULL);
	return Oldrecv(s, buf, len, flags);
}


BOOL WINAPI DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
)
{
	WaitForSingleObject(hSemaphoreA, INFINITE);
	auto process_Id = GetCurrentProcessId(); // 当前进程id
	memcpy(lpBase, &process_Id, sizeof(process_Id));
	ReleaseSemaphore(hSemaphoreB, 1, NULL);

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
			DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
			DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
			DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
			DetourAttach(&(PVOID&)OldHeapDestory, NewHeapDestory);
			DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
			DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
			DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
			DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
			DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
			DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
			DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
			DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
			DetourAttach(&(PVOID&)Oldsocket, Newsocket);
			DetourAttach(&(PVOID&)Oldbind, Newbind);
			DetourAttach(&(PVOID&)Oldsend, Newsend);
			DetourAttach(&(PVOID&)Oldconnect, Newconnect);
			DetourAttach(&(PVOID&)Oldrecv, Newrecv);

			DetourTransactionCommit();
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
			DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
			DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
			DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
			DetourDetach(&(PVOID&)OldHeapDestory, NewHeapDestory);
			DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
			DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
			DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
			DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
			DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
			DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
			DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
			DetourDetach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
			DetourDetach(&(PVOID&)Oldsocket, Newsocket);
			DetourDetach(&(PVOID&)Oldbind, Newbind);
			DetourDetach(&(PVOID&)Oldsend, Newsend);
			DetourDetach(&(PVOID&)Oldconnect, Newconnect);
			DetourDetach(&(PVOID&)Oldrecv, Newrecv);

			DetourTransactionCommit();
			UnmapViewOfFile(lpBase);
			CloseHandle(hMapFile);
			break;
		}
	}
	return true;
}
