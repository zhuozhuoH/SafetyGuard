#pragma execution_character_set("utf-8")
#include "mythread.h"


unordered_map<API_Type, const char*> mythread::TypeName{ {MESSAGEBOXA, "MessageBoxA"},
                                          {CREATEFILE, "CreateFile"},
                                          {WRITEFILE, "WriteFile"},
                                          {READFILE, "ReadFile"},
                                          {HEAPCREATE, "HeapCreate"},
                                          {HEAPALLOC, "HeapAlloc"},
                                          {HEAPFREE, "HeapFree"},
                                          {HEAPDESTORY,"HeapDestory"},
                                          {REGCREATEKEYEX, "RegCreateKeyex"},
                                          {REGOPENKEYEX, "RegOpenKeyex"},
                                          {REGSETVALUEEX, "RegSetValueex"},
                                          {REGDELETEVALUE, "RegDeleteValue"},
                                          {REGCLOSEKEY, "RegCloseKey"},
                                          {SOCKET_, "Socket"},
                                          {BIND, "Bind"},
                                          {CONNECT, "Connect"},
                                          {SEND, "Send"},
                                          {RECV, "Recv"} };

unordered_map<PRIORITY_CLASS, const char*> mythread::PriorityType
{
    {ABOVE_NORMAL, "ABOVE_NORMAL"},
    {BELOW_NORMAL, "BELOW_NORMAL"},
    {HIGH, "HIGH"},
    {IDLE, "IDLE"},
    {NORMAL, "NORMAL"},
    {REALTIME, "REALTIME"}
};


void mythread::run() {

    HANDLE hSemaphoreA = CreateSemaphore(NULL, 1, 1, L"SemaphoreA");
    HANDLE hSemaphoreB = CreateSemaphore(NULL, 0, 1, L"SemaphoreB");
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(API_Info), L"ShareMemory");
    LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    cmdline = testCode + ' ' + dll.left(dll.lastIndexOf("/")) + ' ' + dll;
    CreateProcessA(injector.toStdString().c_str(), cmdline.toLocal8Bit().data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);


    WaitForSingleObject(hSemaphoreB, INFINITE);
    memcpy(&m_process_info.process_id, lpBase, sizeof(m_process_info.process_id));
    m_process_info.process_name = testCode;
    m_process_info.process_name = m_process_info.process_name.right(m_process_info.process_name.size() - m_process_info.process_name.lastIndexOf("/") - 1);
    m_process_info.process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_process_info.process_id); //
    m_process_info.process_Priority = PriorityType[(PRIORITY_CLASS)GetPriorityClass(m_process_info.process_handle)];
    HMODULE hMod[100];
    DWORD cbNeeded;
    char modulepath[256];
    int moduleNum;
    m_process_info.process_Modules = "";
    if (EnumProcessModules(m_process_info.process_handle, hMod, sizeof(hMod), &cbNeeded))
    {
        moduleNum = cbNeeded / sizeof(HMODULE);
        for (int i = 0; i < moduleNum; i++)
        {
            if(GetModuleFileNameA(hMod[i], modulepath, 256))
            {
                m_process_info.process_Modules += modulepath;
                m_process_info.process_Modules += "\n";
            }
        }
    }
    CloseHandle(m_process_info.process_handle);
    emit send_process(m_process_info);
    ReleaseSemaphore(hSemaphoreA, 1, NULL);

    while (processing)
    {
        WaitForSingleObject(hSemaphoreB, INFINITE);
        memcpy(&m_api_hook, lpBase, sizeof(API_Info));
        analyse_process();
        ReleaseSemaphore(hSemaphoreA, 1, NULL);
    }
    quit();
}

void mythread::startThread()
{
    processing = true;
}

void mythread::stopThread()
{
    processing = false;
}

void mythread::terminateProcess()
{
    auto pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_process_info.process_id);
    TerminateProcess(pHandle, 0);
}


bool mythread::analyse_process()
{
    int hHeap;
    int lpMem;
    bool flag=true;
    unsigned dwDesiredAccess;
    QDir file_pre;
    QString filepath;
    QString filename;
    QString foldername;
    switch (m_api_hook.type)
    {
        case MESSAGEBOXA:
        case WRITEFILE:
        case READFILE:
        case SOCKET_:
        case BIND:
        case SEND:
        case CONNECT:
        case RECV:
        case REGCLOSEKEY:
        case REGOPENKEYEX:
        case REGSETVALUEEX:
        case REGDELETEVALUE:
            emit send_api_info(m_api_hook);
            break;



        case HEAPCREATE:
            hHeap = strtoul(m_api_hook.parameterValue[3], NULL, 16);
            heap_info[hHeap][0] = false;
            emit send_api_info(m_api_hook);
            break;
        case HEAPALLOC:
            hHeap = strtoul(m_api_hook.parameterValue[0], NULL, 16);
            if (heap_info.find(hHeap) != heap_info.end()){
                lpMem = strtoul(m_api_hook.parameterValue[3], NULL, 16);
                heap_info[hHeap][lpMem] = true;
                emit send_api_info(m_api_hook);
            }
            break;
        case HEAPFREE:
            hHeap = strtoul(m_api_hook.parameterValue[0], NULL, 16);
            if (heap_info.find(hHeap) != heap_info.end()){
                lpMem = strtoul(m_api_hook.parameterValue[2], NULL, 16);
                if (heap_info[hHeap].find(lpMem) != heap_info[hHeap].end())
                {
                    if (heap_info[hHeap][lpMem]){
                        heap_info[hHeap][lpMem] = false;
                    }
                    else{
                        emit warning_Info("重复释放");
                    }
                }
                else{
                    emit warning_Info("释放未申请内存");
                }
                emit send_api_info(m_api_hook);
            }
            break;
        case HEAPDESTORY:
            hHeap = strtoul(m_api_hook.parameterValue[0], NULL, 16);
            heap_info.erase(hHeap);
            emit send_api_info(m_api_hook);
            break;

        case CREATEFILE:
            dwDesiredAccess = strtoul(m_api_hook.parameterValue[1], NULL, 16);
            file_pre.setPath(QString(m_api_hook.parameterValue[0]));
            filepath = file_pre.absolutePath();
            sprintf(m_api_hook.parameterValue[0], "%s",filepath.toStdString().c_str());
            filename = filepath.right(filepath.size() - filepath.lastIndexOf("/") - 1);
            foldername = filepath.left(filepath.lastIndexOf("/"));

            if (dwDesiredAccess & GENERIC_WRITE)
            {
               if (filename.endsWith(".exe") || filename.endsWith(".dll") || filename.endsWith(".ocx"))
               {
                   emit warning_Info("修改可执行文件");
               }
            }
            if (dwDesiredAccess & GENERIC_READ)
            {
               if (filepath == testCode)
               {
                   emit warning_Info("自我复制");
               }
            }
            if (folder_set.find(foldername) == folder_set.end()){
                folder_set.insert(foldername);
            }
            if (folder_set.size() >= 2){
                emit warning_Info("修改多个文件夹");
            }
            emit send_api_info(m_api_hook);
            break;

        case REGCREATEKEYEX:
            if (QString(m_api_hook.parameterValue[1]) ==  "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"){
                emit warning_Info("注册表启动项");
            }
            emit send_api_info(m_api_hook);
            break;
        default:
            break;
    }
    return flag;
}




















