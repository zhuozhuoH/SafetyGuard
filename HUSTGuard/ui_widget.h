/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QWidget *w_bg;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_18;
    QTextEdit *textEditDll;
    QPushButton *btnDll;
    QHBoxLayout *horizontalLayout_14;
    QTextEdit *textEditInjector;
    QPushButton *btnInjector;
    QHBoxLayout *horizontalLayout_17;
    QTextEdit *textEditTestCode;
    QPushButton *btnTestCode;
    QVBoxLayout *verticalLayout_8;
    QPushButton *pushButton_info;
    QTextEdit *textEditInfo;
    QVBoxLayout *verticalLayout_7;
    QPushButton *pushButton_pm;
    QTextEdit *textEditModules;
    QVBoxLayout *verticalLayout;
    QLabel *title_label;
    QHBoxLayout *horizontalLayout_19;
    QToolButton *btnStart;
    QToolButton *btnClear;
    QVBoxLayout *label_all_2;
    QLabel *label_2;
    QLabel *labelName;
    QLabel *label_4;
    QLabel *labelPriority;
    QLabel *label_6;
    QLabel *labelId;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1041, 602);
        Widget->setMinimumSize(QSize(0, 0));
        Widget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        w_bg = new QWidget(Widget);
        w_bg->setObjectName(QString::fromUtf8("w_bg"));
        horizontalLayout = new QHBoxLayout(w_bg);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeWidget = new QTreeWidget(w_bg);
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/name.png"), QSize(), QIcon::Normal, QIcon::Off);
        QFont font;
        font.setFamily(QString::fromUtf8("Perpetua"));
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/property.png"), QSize(), QIcon::Normal, QIcon::Off);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(1, font);
        __qtreewidgetitem->setIcon(1, icon1);
        __qtreewidgetitem->setFont(0, font);
        __qtreewidgetitem->setIcon(0, icon);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setStyleSheet(QString::fromUtf8("QTreeWidget{\n"
"border: 1px solid grey ;  \n"
"border-radius:20px;\n"
"padding: 1px 12px 1px 12px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout->addWidget(treeWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        textEditDll = new QTextEdit(w_bg);
        textEditDll->setObjectName(QString::fromUtf8("textEditDll"));
        textEditDll->setMaximumSize(QSize(16777215, 35));
        textEditDll->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_18->addWidget(textEditDll);

        btnDll = new QPushButton(w_bg);
        btnDll->setObjectName(QString::fromUtf8("btnDll"));
        btnDll->setMaximumSize(QSize(16777215, 35));
        btnDll->setCursor(QCursor(Qt::PointingHandCursor));
        btnDll->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("icons/gear.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDll->setIcon(icon2);
        btnDll->setIconSize(QSize(20, 20));
        btnDll->setAutoDefault(false);
        btnDll->setFlat(false);

        horizontalLayout_18->addWidget(btnDll);

        horizontalLayout_18->setStretch(0, 3);
        horizontalLayout_18->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_18);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        textEditInjector = new QTextEdit(w_bg);
        textEditInjector->setObjectName(QString::fromUtf8("textEditInjector"));
        textEditInjector->setMaximumSize(QSize(16777215, 35));
        textEditInjector->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_14->addWidget(textEditInjector);

        btnInjector = new QPushButton(w_bg);
        btnInjector->setObjectName(QString::fromUtf8("btnInjector"));
        btnInjector->setMaximumSize(QSize(16777215, 35));
        btnInjector->setCursor(QCursor(Qt::PointingHandCursor));
        btnInjector->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("icons/syringe.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnInjector->setIcon(icon3);
        btnInjector->setIconSize(QSize(20, 20));
        btnInjector->setAutoDefault(false);
        btnInjector->setFlat(false);

        horizontalLayout_14->addWidget(btnInjector);

        horizontalLayout_14->setStretch(0, 3);
        horizontalLayout_14->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        textEditTestCode = new QTextEdit(w_bg);
        textEditTestCode->setObjectName(QString::fromUtf8("textEditTestCode"));
        textEditTestCode->setMaximumSize(QSize(16777215, 35));
        textEditTestCode->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_17->addWidget(textEditTestCode);

        btnTestCode = new QPushButton(w_bg);
        btnTestCode->setObjectName(QString::fromUtf8("btnTestCode"));
        btnTestCode->setMaximumSize(QSize(16777215, 35));
        btnTestCode->setCursor(QCursor(Qt::PointingHandCursor));
        btnTestCode->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTestCode->setIcon(icon4);
        btnTestCode->setIconSize(QSize(20, 20));
        btnTestCode->setAutoDefault(false);
        btnTestCode->setFlat(false);

        horizontalLayout_17->addWidget(btnTestCode);

        horizontalLayout_17->setStretch(0, 3);
        horizontalLayout_17->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_17);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        pushButton_info = new QPushButton(w_bg);
        pushButton_info->setObjectName(QString::fromUtf8("pushButton_info"));
        pushButton_info->setMinimumSize(QSize(0, 10));
        pushButton_info->setMaximumSize(QSize(16777215, 40));
        pushButton_info->setStyleSheet(QString::fromUtf8("font-size:12pt;"));

        verticalLayout_8->addWidget(pushButton_info);

        textEditInfo = new QTextEdit(w_bg);
        textEditInfo->setObjectName(QString::fromUtf8("textEditInfo"));
        textEditInfo->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:20px;\n"
"padding: 1px 20px 1px 20px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        verticalLayout_8->addWidget(textEditInfo);


        verticalLayout_2->addLayout(verticalLayout_8);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        pushButton_pm = new QPushButton(w_bg);
        pushButton_pm->setObjectName(QString::fromUtf8("pushButton_pm"));
        pushButton_pm->setMaximumSize(QSize(16777215, 40));
        pushButton_pm->setStyleSheet(QString::fromUtf8("font-size:12pt;"));

        verticalLayout_7->addWidget(pushButton_pm);

        textEditModules = new QTextEdit(w_bg);
        textEditModules->setObjectName(QString::fromUtf8("textEditModules"));
        textEditModules->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:20px;\n"
"padding: 1px 20px 1px 20px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        verticalLayout_7->addWidget(textEditModules);


        verticalLayout_2->addLayout(verticalLayout_7);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        title_label = new QLabel(w_bg);
        title_label->setObjectName(QString::fromUtf8("title_label"));
        title_label->setStyleSheet(QString::fromUtf8("font-family: \"Arial\";"));
        title_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(title_label);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        btnStart = new QToolButton(w_bg);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));
        btnStart->setMinimumSize(QSize(70, 70));
        btnStart->setCursor(QCursor(Qt::PointingHandCursor));
        btnStart->setStyleSheet(QString::fromUtf8(""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnStart->setIcon(icon5);
        btnStart->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnStart);

        btnClear = new QToolButton(w_bg);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));
        btnClear->setMinimumSize(QSize(70, 70));
        btnClear->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("icons/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClear->setIcon(icon6);
        btnClear->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnClear);


        verticalLayout->addLayout(horizontalLayout_19);

        label_all_2 = new QVBoxLayout();
        label_all_2->setObjectName(QString::fromUtf8("label_all_2"));
        label_2 = new QLabel(w_bg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        label_all_2->addWidget(label_2);

        labelName = new QLabel(w_bg);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        labelName->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        label_all_2->addWidget(labelName);

        label_4 = new QLabel(w_bg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        label_all_2->addWidget(label_4);

        labelPriority = new QLabel(w_bg);
        labelPriority->setObjectName(QString::fromUtf8("labelPriority"));
        labelPriority->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        label_all_2->addWidget(labelPriority);

        label_6 = new QLabel(w_bg);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        label_all_2->addWidget(label_6);

        labelId = new QLabel(w_bg);
        labelId->setObjectName(QString::fromUtf8("labelId"));
        labelId->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        label_all_2->addWidget(labelId);


        verticalLayout->addLayout(label_all_2);

        verticalSpacer_6 = new QSpacerItem(223, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_5);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_15);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 3);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(2, 1);

        gridLayout->addWidget(w_bg, 0, 0, 1, 1);


        retranslateUi(Widget);

        btnDll->setDefault(false);
        btnInjector->setDefault(false);
        btnTestCode->setDefault(false);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("Widget", "Time/Parameters", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("Widget", "API_Type", nullptr));
        btnDll->setText(QCoreApplication::translate("Widget", "Dll", nullptr));
        btnInjector->setText(QCoreApplication::translate("Widget", "Injector", nullptr));
        btnTestCode->setText(QCoreApplication::translate("Widget", "App", nullptr));
        pushButton_info->setText(QCoreApplication::translate("Widget", "Info", nullptr));
        pushButton_pm->setText(QCoreApplication::translate("Widget", "Process Modules", nullptr));
        title_label->setText(QCoreApplication::translate("Widget", "HUSTGuard", nullptr));
        btnStart->setText(QCoreApplication::translate("Widget", "Start", nullptr));
        btnClear->setText(QCoreApplication::translate("Widget", "Clear", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\342\234\247ProcessName\342\234\247", nullptr));
        labelName->setText(QString());
        label_4->setText(QCoreApplication::translate("Widget", "\342\234\247ProcessPriority\342\234\247", nullptr));
        labelPriority->setText(QString());
        label_6->setText(QCoreApplication::translate("Widget", "\342\234\247ProcessID\342\234\247", nullptr));
        labelId->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
