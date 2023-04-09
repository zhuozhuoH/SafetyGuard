#include "widget.h"
#include "ui_widget.h"

#pragma execution_character_set("utf-8")

unordered_map<API_Type, const char*> TypeName { {MESSAGEBOXA, "MessageBoxA"},
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


Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("HUSTGuard");
    this->resize(QSize(1200, 735));

    ui->textEditInfo->setFixedHeight(30);

    connect(&thread, SIGNAL(send_api_info(API_Info&)), this, SLOT(show_api_time(API_Info&)), Qt::BlockingQueuedConnection);
    connect(&thread, SIGNAL(send_process(Process_Info&)), this, SLOT(show_process_info(Process_Info&)), Qt::BlockingQueuedConnection);
    connect(&thread, SIGNAL(warning_Info(QString)), this, SLOT(show_warning_Info(QString)), Qt::BlockingQueuedConnection);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::closeEvent(QCloseEvent *event)
{
    if (thread.isRunning())
    {
        thread.stopThread();
        thread.terminateProcess();
        thread.terminate();
        thread.wait();
    }
    event->accept();
}

void Widget::start()
{
    thread.startThread();
    thread.start();
}


void Widget::clear()
{
    ui->treeWidget->clear();
}


void Widget::on_btnStart_clicked()
{
    if (Injector_readtype && Dll_readtype && testCode_readtype)
    {
        start();
        ui->btnStart->setEnabled(false);
    }
    else
    {
        QMessageBox warnbox;
        warnbox.warning(this, "WARNING", "请设置文件路径");
    }
}

void Widget::on_btnClear_clicked()
{
    clear();
}


void Widget::on_btnInjector_clicked()
{
    QString Title = "选择注射器";
    QString Path = QDir::currentPath(); // 获取应用程序当前目录
    QString filter = "可执行文件(*.exe *.dll);;所有文件(*.*)";
    QString FileName = QFileDialog::getOpenFileName(this, Title, Path, filter);
    if (!FileName.isEmpty())
    {
        ui->textEditInjector->setText(FileName);
        thread.injector = FileName;
        Injector_readtype = true;
    }
}


void Widget::on_btnDll_clicked()
{
    QString Title = "选择DLL";
    QString Path = QCoreApplication::applicationDirPath(); // 获取应用程序可执行文件所在目录
    QString filter = "可执行文件(*.exe *.dll);;所有文件(*.*)";
    QString FileName = QFileDialog::getOpenFileName(this, Title, Path, filter);
    if (!FileName.isEmpty())
    {
        ui->textEditDll->setText(FileName);
        thread.dll = FileName;
        Dll_readtype = true;
    }
}

void Widget::on_btnTestCode_clicked()
{
    QString Title = "选择测试程序";
    QString Path = QCoreApplication::applicationFilePath(); // 获取应用程序可执行文件所在目录
    QString filter = "可执行文件(*.exe *.dll);;所有文件(*.*)";
    QString FileName = QFileDialog::getOpenFileName(this, Title, Path, filter);
    if (!FileName.isEmpty())
    {
        ui->textEditTestCode->setText(FileName);
        thread.testCode = FileName;
        testCode_readtype = true;
    }
}


void Widget::show_api_time(API_Info& api_hook)
{
    QTreeWidgetItem* api_all = new QTreeWidgetItem();
    char time[128] = "";
    sprintf(time, "%4d-%2d-%2d %-2d:%-2d:%-2d.%-3d",
            api_hook.time.wYear, api_hook.time.wMonth, api_hook.time.wDay,
            api_hook.time.wHour, api_hook.time.wMinute, api_hook.time.wSecond,api_hook.time.wMilliseconds);
    api_all->setData(0, 0, TypeName[api_hook.type]);
    api_all->setData(1, 0, time);
    for (int i = 0; i < api_hook.parameterNum; i++)
    {
        QTreeWidgetItem* api_info = new QTreeWidgetItem();
        api_info->setData(0, 0, api_hook.parameterName[i]);
        api_info->setData(1, 0, api_hook.parameterValue[i]);
        api_all->addChild(api_info);
    }
    ui->treeWidget->addTopLevelItem(api_all);
}

void Widget::show_process_info(Process_Info& process_info)
{
    ui->labelName->setText(process_info.process_name);
    ui->labelId->setText(QString::asprintf("%d", process_info.process_id));
    ui->labelPriority->setText(process_info.process_Priority);
    ui->textEditModules->setText(process_info.process_Modules);
}


void Widget::show_warning_Info(QString text)
{
    ui->textEditInfo->setText(text);
}
