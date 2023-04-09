#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#if _MSC_VER >= 1600 // MSVC2015>1899, MSVC VER=l4 . 0
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
//最小化相关
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>

#include "mythread.h"
#include "messagebox.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();


private:
    Ui::Widget* ui;
    //自定义窗口通信参数
    int mbox_return;

    messagebox* mbox;
    mythread thread;

    bool Injector_readtype;
    bool Dll_readtype;
    bool testCode_readtype;

    void start();
    void clear();
    void analyse();


signals:
    void btnchicked(int num);

private slots:
    void on_btnStart_clicked();
    void on_btnClear_clicked();
    void on_btnInjector_clicked();
    void on_btnDll_clicked();
    void on_btnTestCode_clicked();

    void show_api_time(API_Info & api_hook);
    void show_process_info(Process_Info& process_info);
    void show_warning_Info(QString text);

protected:
    void closeEvent(QCloseEvent *event);

};
#endif // WIDGET_H
