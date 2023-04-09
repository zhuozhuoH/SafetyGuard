#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#if _MSC_VER >= 1600 // MSVC2015>1899, MSVC VER=l4 . 0
#pragma execution_character_set("utf-8")
#endif

#include <QThread>
#include <QDebug>
#include <QDir>
#include <unordered_map>
#include <windows.h>
#include <string>
#include <psapi.h>
#include <unordered_set>
using namespace std;

enum API_Type {
    MESSAGEBOXA, CREATEFILE, WRITEFILE, READFILE, HEAPCREATE, HEAPALLOC, HEAPFREE, HEAPDESTORY,
    REGCREATEKEYEX, REGOPENKEYEX, REGSETVALUEEX, REGDELETEVALUE, REGCLOSEKEY,
    SOCKET_, BIND, CONNECT, SEND, RECV
};

enum PRIORITY_CLASS {
    ABOVE_NORMAL = ABOVE_NORMAL_PRIORITY_CLASS,
    BELOW_NORMAL = BELOW_NORMAL_PRIORITY_CLASS,
    HIGH = HIGH_PRIORITY_CLASS,
    IDLE = IDLE_PRIORITY_CLASS,
    NORMAL = NORMAL_PRIORITY_CLASS,
    REALTIME = REALTIME_PRIORITY_CLASS
};

struct Process_Info
{
    QString process_name;
    HANDLE process_handle;
    DWORD process_id;
    QString process_Modules;
    QString process_Priority;
};

struct API_Info
{
    API_Type type;
    int parameterNum;
    SYSTEMTIME time;
    char parameterName[10][30] = {};
    char parameterValue[10][70] = {};
};



class mythread : public QThread
{
    Q_OBJECT

public:
    QString injector;
    QString dll;
    QString testCode;
    QString cmdline;

    static unordered_map<API_Type, const char*> TypeName;
    static unordered_map<PRIORITY_CLASS, const char*> PriorityType;

    void startThread();
    void stopThread();
    void terminateProcess();

private:
    bool processing = false;
    bool m_flag = true;
    API_Info m_api_hook;
    Process_Info m_process_info;

    unordered_map<int, unordered_map<int, bool> > heap_info;
    unordered_set<QString> folder_set;

    bool analyse_process();

signals:
    void send_api_info(API_Info & api_hook);
    void send_process(Process_Info & process_info);
    void warning_Info(QString text);

protected:
    void run();
};














#endif // MYTHREAD_H
