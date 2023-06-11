/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAdd;
    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionExpand_all;
    QAction *actionNew;
    QAction *actionAbout;
    QAction *actionClose_all;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionRestoreScene;
    QAction *actionDefaultNodeIcon;
    QAction *actionCheck_Update;
    QAction *actionNew_Sibling;
    QAction *actionNew_Child;
    QAction *actionRemove_2;
    QAction *actionExpandAllOpenFile;
    QAction *actionNew_Window;
    QAction *actionCopy_between_windows;
    QAction *actionPaste_between_windows;
    QAction *actionShowPlistText;
    QAction *actionPaste_as_child;
    QAction *actionFind;
    QAction *actionFindNext;
    QAction *actionFindPrevious;
    QAction *actionReplace;
    QAction *actionReplaceAll;
    QAction *actionBug_Report;
    QAction *actionDiscussion_Forum;
    QAction *actionQuit;
    QAction *actionFont;
    QAction *actionDownload_Upgrade_Packages;
    QAction *actProxy1;
    QAction *actProxy2;
    QAction *actProxy3;
    QAction *actProxy4;
    QAction *actProxy5;
    QAction *actionMove_Up;
    QAction *actionMove_Down;
    QAction *actionSort;
    QAction *actionExpand_All;
    QAction *actionCollapse_All;
    QAction *actionAutoUpdateCheck;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QListWidget *listFind;
    QFrame *frameMain;
    QGridLayout *gridLayout_8;
    QFrame *frameTip;
    QGridLayout *gridLayout_7;
    QPushButton *btnNo;
    QLabel *lblTip;
    QPushButton *btnYes;
    QFrame *frameFind;
    QGridLayout *gridLayout_4;
    QToolButton *btnFind;
    QToolButton *btnNext;
    QToolButton *btnShowReplace;
    QToolButton *btnHideFind;
    QToolButton *btnReplaceAll;
    QToolButton *btnReplaceFind;
    QLineEdit *editFind;
    QToolButton *btnPrevious;
    QLineEdit *editReplace;
    QToolButton *btnReplace;
    QToolButton *btnMisc;
    QLabel *lblFindCount;
    QFrame *frameData;
    QGridLayout *gridLayout_6;
    QLabel *lblBytes;
    QLineEdit *editHex;
    QToolButton *btnUpdateHex;
    QLabel *label;
    QLineEdit *editASCII;
    QToolButton *btnUpdateASCII;
    QLabel *label_2;
    QLineEdit *editBase64;
    QToolButton *btnUpdateBase64;
    QFrame *wTools;
    QGridLayout *gridLayout;
    QToolButton *btnChild;
    QToolButton *btnBrother;
    QToolButton *btnSort;
    QToolButton *btnExpand;
    QToolButton *btnNew;
    QToolButton *btnDel;
    QToolButton *btnFind_Tool;
    QToolButton *btnSave;
    QGridLayout *gridLayout_2;
    QComboBox *cboxFileType;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QWidget *tab;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QMenu *menuUpgrade_Download_Proxy;
    QMenu *menuPreferences;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 805);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/toolbar/res/my.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("/*QMainWindow { background-color: lightgray; }*/\n"
"\n"
"/*QMainWindow { background-color: rgb(66,66,66); }*/"));
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionAdd = new QAction(MainWindow);
        actionAdd->setObjectName("actionAdd");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/toolbar/res/Add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd->setIcon(icon1);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/toolbar/res/open.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionOpen->setIconVisibleInMenu(false);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName("actionClose");
        actionClose->setIconVisibleInMenu(false);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/toolbar/res/save.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionSave->setIconVisibleInMenu(false);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName("actionSave_as");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/toolbar/res/saveas.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_as->setIcon(icon4);
        actionSave_as->setIconVisibleInMenu(false);
        actionExpand_all = new QAction(MainWindow);
        actionExpand_all->setObjectName("actionExpand_all");
        actionExpand_all->setCheckable(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/toolbar/res/Transfer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExpand_all->setIcon(icon5);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/toolbar/res/new.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon6);
        actionNew->setIconVisibleInMenu(false);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionClose_all = new QAction(MainWindow);
        actionClose_all->setObjectName("actionClose_all");
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName("actionCopy");
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName("actionCut");
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName("actionPaste");
        actionRestoreScene = new QAction(MainWindow);
        actionRestoreScene->setObjectName("actionRestoreScene");
        actionRestoreScene->setCheckable(true);
        actionRestoreScene->setChecked(true);
        actionDefaultNodeIcon = new QAction(MainWindow);
        actionDefaultNodeIcon->setObjectName("actionDefaultNodeIcon");
        actionDefaultNodeIcon->setCheckable(true);
        actionCheck_Update = new QAction(MainWindow);
        actionCheck_Update->setObjectName("actionCheck_Update");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/toolbar/res/cu.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionCheck_Update->setIcon(icon7);
        actionCheck_Update->setIconVisibleInMenu(false);
        actionNew_Sibling = new QAction(MainWindow);
        actionNew_Sibling->setObjectName("actionNew_Sibling");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/new/toolbar/res/sibling.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Sibling->setIcon(icon8);
        actionNew_Sibling->setIconVisibleInMenu(false);
        actionNew_Child = new QAction(MainWindow);
        actionNew_Child->setObjectName("actionNew_Child");
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/new/toolbar/res/child.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Child->setIcon(icon9);
        actionNew_Child->setIconVisibleInMenu(false);
        actionRemove_2 = new QAction(MainWindow);
        actionRemove_2->setObjectName("actionRemove_2");
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/new/toolbar/res/remove.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove_2->setIcon(icon10);
        actionRemove_2->setIconVisibleInMenu(false);
        actionExpandAllOpenFile = new QAction(MainWindow);
        actionExpandAllOpenFile->setObjectName("actionExpandAllOpenFile");
        actionExpandAllOpenFile->setCheckable(true);
        actionNew_Window = new QAction(MainWindow);
        actionNew_Window->setObjectName("actionNew_Window");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/new/toolbar/res/nw.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Window->setIcon(icon11);
        actionNew_Window->setIconVisibleInMenu(false);
        actionCopy_between_windows = new QAction(MainWindow);
        actionCopy_between_windows->setObjectName("actionCopy_between_windows");
        actionPaste_between_windows = new QAction(MainWindow);
        actionPaste_between_windows->setObjectName("actionPaste_between_windows");
        actionShowPlistText = new QAction(MainWindow);
        actionShowPlistText->setObjectName("actionShowPlistText");
        actionShowPlistText->setCheckable(true);
        actionShowPlistText->setChecked(false);
        actionPaste_as_child = new QAction(MainWindow);
        actionPaste_as_child->setObjectName("actionPaste_as_child");
        actionFind = new QAction(MainWindow);
        actionFind->setObjectName("actionFind");
        actionFind->setIconVisibleInMenu(true);
        actionFindNext = new QAction(MainWindow);
        actionFindNext->setObjectName("actionFindNext");
        actionFindNext->setIconVisibleInMenu(false);
        actionFindPrevious = new QAction(MainWindow);
        actionFindPrevious->setObjectName("actionFindPrevious");
        actionFindPrevious->setIconVisibleInMenu(false);
        actionReplace = new QAction(MainWindow);
        actionReplace->setObjectName("actionReplace");
        actionReplaceAll = new QAction(MainWindow);
        actionReplaceAll->setObjectName("actionReplaceAll");
        actionBug_Report = new QAction(MainWindow);
        actionBug_Report->setObjectName("actionBug_Report");
        actionDiscussion_Forum = new QAction(MainWindow);
        actionDiscussion_Forum->setObjectName("actionDiscussion_Forum");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        actionFont = new QAction(MainWindow);
        actionFont->setObjectName("actionFont");
        actionDownload_Upgrade_Packages = new QAction(MainWindow);
        actionDownload_Upgrade_Packages->setObjectName("actionDownload_Upgrade_Packages");
        actProxy1 = new QAction(MainWindow);
        actProxy1->setObjectName("actProxy1");
        actProxy1->setCheckable(true);
        actProxy2 = new QAction(MainWindow);
        actProxy2->setObjectName("actProxy2");
        actProxy2->setCheckable(true);
        actProxy3 = new QAction(MainWindow);
        actProxy3->setObjectName("actProxy3");
        actProxy3->setCheckable(true);
        actProxy4 = new QAction(MainWindow);
        actProxy4->setObjectName("actProxy4");
        actProxy4->setCheckable(true);
        actProxy5 = new QAction(MainWindow);
        actProxy5->setObjectName("actProxy5");
        actProxy5->setCheckable(true);
        actionMove_Up = new QAction(MainWindow);
        actionMove_Up->setObjectName("actionMove_Up");
        actionMove_Down = new QAction(MainWindow);
        actionMove_Down->setObjectName("actionMove_Down");
        actionSort = new QAction(MainWindow);
        actionSort->setObjectName("actionSort");
        actionExpand_All = new QAction(MainWindow);
        actionExpand_All->setObjectName("actionExpand_All");
        actionCollapse_All = new QAction(MainWindow);
        actionCollapse_All->setObjectName("actionCollapse_All");
        actionAutoUpdateCheck = new QAction(MainWindow);
        actionAutoUpdateCheck->setObjectName("actionAutoUpdateCheck");
        actionAutoUpdateCheck->setCheckable(true);
        actionAutoUpdateCheck->setChecked(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        listFind = new QListWidget(centralWidget);
        listFind->setObjectName("listFind");
        listFind->setMinimumSize(QSize(150, 0));
        listFind->setMaximumSize(QSize(400, 16777215));
        listFind->setBaseSize(QSize(150, 0));
        listFind->setFrameShape(QFrame::NoFrame);
        listFind->setFrameShadow(QFrame::Plain);

        gridLayout_3->addWidget(listFind, 0, 0, 1, 1);

        frameMain = new QFrame(centralWidget);
        frameMain->setObjectName("frameMain");
        frameMain->setFrameShape(QFrame::NoFrame);
        frameMain->setFrameShadow(QFrame::Plain);
        gridLayout_8 = new QGridLayout(frameMain);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName("gridLayout_8");
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        frameTip = new QFrame(frameMain);
        frameTip->setObjectName("frameTip");
        frameTip->setFrameShape(QFrame::NoFrame);
        frameTip->setFrameShadow(QFrame::Plain);
        gridLayout_7 = new QGridLayout(frameTip);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName("gridLayout_7");
        gridLayout_7->setContentsMargins(1, 1, 1, 1);
        btnNo = new QPushButton(frameTip);
        btnNo->setObjectName("btnNo");
        btnNo->setMaximumSize(QSize(75, 16777215));

        gridLayout_7->addWidget(btnNo, 0, 1, 1, 1);

        lblTip = new QLabel(frameTip);
        lblTip->setObjectName("lblTip");

        gridLayout_7->addWidget(lblTip, 0, 0, 1, 1);

        btnYes = new QPushButton(frameTip);
        btnYes->setObjectName("btnYes");
        btnYes->setMaximumSize(QSize(75, 16777215));

        gridLayout_7->addWidget(btnYes, 0, 2, 1, 1);


        gridLayout_8->addWidget(frameTip, 0, 0, 1, 1);

        frameFind = new QFrame(frameMain);
        frameFind->setObjectName("frameFind");
        frameFind->setFrameShape(QFrame::NoFrame);
        frameFind->setFrameShadow(QFrame::Sunken);
        frameFind->setLineWidth(1);
        gridLayout_4 = new QGridLayout(frameFind);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        btnFind = new QToolButton(frameFind);
        btnFind->setObjectName("btnFind");

        gridLayout_4->addWidget(btnFind, 0, 3, 1, 1);

        btnNext = new QToolButton(frameFind);
        btnNext->setObjectName("btnNext");

        gridLayout_4->addWidget(btnNext, 0, 5, 1, 1);

        btnShowReplace = new QToolButton(frameFind);
        btnShowReplace->setObjectName("btnShowReplace");
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/new/toolbar/res/replace.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btnShowReplace->setIcon(icon12);

        gridLayout_4->addWidget(btnShowReplace, 0, 11, 1, 1);

        btnHideFind = new QToolButton(frameFind);
        btnHideFind->setObjectName("btnHideFind");

        gridLayout_4->addWidget(btnHideFind, 0, 10, 1, 1);

        btnReplaceAll = new QToolButton(frameFind);
        btnReplaceAll->setObjectName("btnReplaceAll");

        gridLayout_4->addWidget(btnReplaceAll, 0, 9, 1, 1);

        btnReplaceFind = new QToolButton(frameFind);
        btnReplaceFind->setObjectName("btnReplaceFind");

        gridLayout_4->addWidget(btnReplaceFind, 0, 8, 1, 1);

        editFind = new QLineEdit(frameFind);
        editFind->setObjectName("editFind");

        gridLayout_4->addWidget(editFind, 0, 2, 1, 1);

        btnPrevious = new QToolButton(frameFind);
        btnPrevious->setObjectName("btnPrevious");

        gridLayout_4->addWidget(btnPrevious, 0, 4, 1, 1);

        editReplace = new QLineEdit(frameFind);
        editReplace->setObjectName("editReplace");

        gridLayout_4->addWidget(editReplace, 0, 6, 1, 1);

        btnReplace = new QToolButton(frameFind);
        btnReplace->setObjectName("btnReplace");

        gridLayout_4->addWidget(btnReplace, 0, 7, 1, 1);

        btnMisc = new QToolButton(frameFind);
        btnMisc->setObjectName("btnMisc");

        gridLayout_4->addWidget(btnMisc, 0, 1, 1, 1);

        lblFindCount = new QLabel(frameFind);
        lblFindCount->setObjectName("lblFindCount");
        lblFindCount->setMinimumSize(QSize(45, 0));
        lblFindCount->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblFindCount, 0, 0, 1, 1);


        gridLayout_8->addWidget(frameFind, 1, 0, 1, 1);

        frameData = new QFrame(frameMain);
        frameData->setObjectName("frameData");
        frameData->setFrameShape(QFrame::NoFrame);
        frameData->setFrameShadow(QFrame::Plain);
        frameData->setLineWidth(0);
        gridLayout_6 = new QGridLayout(frameData);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_6->setVerticalSpacing(2);
        gridLayout_6->setContentsMargins(2, 2, 2, 2);
        lblBytes = new QLabel(frameData);
        lblBytes->setObjectName("lblBytes");

        gridLayout_6->addWidget(lblBytes, 0, 0, 1, 1);

        editHex = new QLineEdit(frameData);
        editHex->setObjectName("editHex");

        gridLayout_6->addWidget(editHex, 0, 2, 1, 1);

        btnUpdateHex = new QToolButton(frameData);
        btnUpdateHex->setObjectName("btnUpdateHex");

        gridLayout_6->addWidget(btnUpdateHex, 0, 3, 1, 1);

        label = new QLabel(frameData);
        label->setObjectName("label");

        gridLayout_6->addWidget(label, 1, 0, 1, 1);

        editASCII = new QLineEdit(frameData);
        editASCII->setObjectName("editASCII");

        gridLayout_6->addWidget(editASCII, 1, 1, 1, 2);

        btnUpdateASCII = new QToolButton(frameData);
        btnUpdateASCII->setObjectName("btnUpdateASCII");

        gridLayout_6->addWidget(btnUpdateASCII, 1, 3, 1, 1);

        label_2 = new QLabel(frameData);
        label_2->setObjectName("label_2");

        gridLayout_6->addWidget(label_2, 2, 0, 1, 2);

        editBase64 = new QLineEdit(frameData);
        editBase64->setObjectName("editBase64");

        gridLayout_6->addWidget(editBase64, 2, 2, 1, 1);

        btnUpdateBase64 = new QToolButton(frameData);
        btnUpdateBase64->setObjectName("btnUpdateBase64");

        gridLayout_6->addWidget(btnUpdateBase64, 2, 3, 1, 1);


        gridLayout_8->addWidget(frameData, 2, 0, 1, 1);

        wTools = new QFrame(frameMain);
        wTools->setObjectName("wTools");
        wTools->setFrameShape(QFrame::NoFrame);
        wTools->setFrameShadow(QFrame::Plain);
        gridLayout = new QGridLayout(wTools);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 12, 0, 12);
        btnChild = new QToolButton(wTools);
        btnChild->setObjectName("btnChild");
        btnChild->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnChild, 0, 6, 1, 1);

        btnBrother = new QToolButton(wTools);
        btnBrother->setObjectName("btnBrother");
        btnBrother->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnBrother, 0, 5, 1, 1);

        btnSort = new QToolButton(wTools);
        btnSort->setObjectName("btnSort");
        btnSort->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnSort, 0, 9, 1, 1);

        btnExpand = new QToolButton(wTools);
        btnExpand->setObjectName("btnExpand");
        btnExpand->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnExpand, 0, 8, 1, 1);

        btnNew = new QToolButton(wTools);
        btnNew->setObjectName("btnNew");
        btnNew->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnNew, 0, 2, 1, 1);

        btnDel = new QToolButton(wTools);
        btnDel->setObjectName("btnDel");
        btnDel->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnDel, 0, 7, 1, 1);

        btnFind_Tool = new QToolButton(wTools);
        btnFind_Tool->setObjectName("btnFind_Tool");
        btnFind_Tool->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnFind_Tool, 0, 4, 1, 1);

        btnSave = new QToolButton(wTools);
        btnSave->setObjectName("btnSave");
        btnSave->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(btnSave, 0, 3, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(-1, 0, -1, -1);
        cboxFileType = new QComboBox(wTools);
        cboxFileType->setObjectName("cboxFileType");
        cboxFileType->setFrame(true);

        gridLayout_2->addWidget(cboxFileType, 1, 0, 1, 1);


        gridLayout->addLayout(gridLayout_2, 0, 1, 1, 1);


        gridLayout_8->addWidget(wTools, 3, 0, 1, 1);

        tabWidget = new QTabWidget(frameMain);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"    border: 1px solid gray;\n"
"    background: rgb(60, 60, 60);\n"
"}\n"
"\n"
"QTabBar::close-button:hover {\n"
"image: url(:/new/toolbar/res/c0.png);\n"
"subcontrol-origin: padding;\n"
"subcontrol-position: bottom right;\n"
"}\n"
"\n"
"QTabBar::close-button {\n"
"image: url(:/new/toolbar/res/c.png);\n"
"subcontrol-origin: padding;\n"
"subcontrol-position: bottom right;\n"
"} \n"
"\n"
"QTabWidget::tab-bar:top {\n"
"    top: 1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar:bottom {\n"
"    bottom: 1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar:left {\n"
"    right: 1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar:right {\n"
"    left: 1px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    border: 1px solid gray;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: rgb(60, 60, 60);\n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    background: rgb(26.26,26); \n"
"}\n"
"\n"
"QTabBar::tab:!selected:hover {\n"
"    /*background:  silver;\n"
"    color: black;*/\n"
"	\n"
"}\n"
"\n"
"QTabBar::tab:top:!selected {\n"
"    margin"
                        "-top: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:bottom:!selected {\n"
"    margin-bottom: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:top, QTabBar::tab:bottom {\n"
"    min-width: 8ex;\n"
"    margin-right: -1px;\n"
"    padding: 8px 20px 8px 20px;\n"
"}\n"
"\n"
"QTabBar::tab:top:selected {\n"
"    border-bottom-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:bottom:selected {\n"
"    border-top-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:top:last, QTabBar::tab:bottom:last,\n"
"QTabBar::tab:top:only-one, QTabBar::tab:bottom:only-one {\n"
"    margin-right: 0;\n"
"}\n"
"\n"
"QTabBar::tab:left:!selected {\n"
"    margin-right: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:right:!selected {\n"
"    margin-left: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:left, QTabBar::tab:right {\n"
"    min-height: 8ex;\n"
"    margin-bottom: -1px;\n"
"    padding: 10px 5px 10px 5px;\n"
"}\n"
"\n"
"QTabBar::tab:left:selected {\n"
"    border-left-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:right:selected {\n"
"    border-right-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:left:last, QTabB"
                        "ar::tab:right:last,\n"
"QTabBar::tab:left:only-one, QTabBar::tab:right:only-one {\n"
"    margin-bottom: 0;\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"/*QTabWidget::pane {\n"
"    border: 1px solid lightgray;\n"
"    background: rgb(236, 236, 236);\n"
"}\n"
"\n"
"QTabBar::close-button {\n"
"image: url(:/new/toolbar/res/c.png);\n"
"subcontrol-origin: padding;\n"
"subcontrol-position: bottom right;\n"
"} \n"
"\n"
"QTabWidget::tab-bar:top {\n"
"    top: 1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar:bottom {\n"
"    bottom: 1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar:left {\n"
"    right: 1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar:right {\n"
"    left: 1px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    border: 1px solid lightgray;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: rgb(236, 236, 236);\n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    background: lightgray; \n"
"}\n"
"\n"
"QTabBar::tab:!selected:hover {\n"
"    background: silver;\n"
"}\n"
"\n"
"QTabBar::tab:top:!selected {\n"
"    margin-top: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:"
                        "bottom:!selected {\n"
"    margin-bottom: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:top, QTabBar::tab:bottom {\n"
"    min-width: 8ex;\n"
"    margin-right: -1px;\n"
"    padding: 8px 20px 8px 20px;\n"
"}\n"
"\n"
"QTabBar::tab:top:selected {\n"
"    border-bottom-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:bottom:selected {\n"
"    border-top-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:top:last, QTabBar::tab:bottom:last,\n"
"QTabBar::tab:top:only-one, QTabBar::tab:bottom:only-one {\n"
"    margin-right: 0;\n"
"}\n"
"\n"
"QTabBar::tab:left:!selected {\n"
"    margin-right: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:right:!selected {\n"
"    margin-left: 3px;\n"
"}\n"
"\n"
"QTabBar::tab:left, QTabBar::tab:right {\n"
"    min-height: 8ex;\n"
"    margin-bottom: -1px;\n"
"    padding: 10px 5px 10px 5px;\n"
"}\n"
"\n"
"QTabBar::tab:left:selected {\n"
"    border-left-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:right:selected {\n"
"    border-right-color: none;\n"
"}\n"
"\n"
"QTabBar::tab:left:last, QTabBar::tab:right:last,\n"
"QTabBar::tab:le"
                        "ft:only-one, QTabBar::tab:right:only-one {\n"
"    margin-bottom: 0;\n"
"}*/\n"
""));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setUsesScrollButtons(false);
        tabWidget->setTabsClosable(true);
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        tabWidget->addTab(tab, QString());

        gridLayout_8->addWidget(tabWidget, 4, 0, 1, 1);


        gridLayout_3->addWidget(frameMain, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1000, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName("menuEdit");
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName("menuHelp");
        menuUpgrade_Download_Proxy = new QMenu(menuHelp);
        menuUpgrade_Download_Proxy->setObjectName("menuUpgrade_Download_Proxy");
        menuPreferences = new QMenu(menuBar);
        menuPreferences->setObjectName("menuPreferences");
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        statusBar->setStyleSheet(QString::fromUtf8("/*QStatusBar { background: rgb(236, 236, 236);}\n"
"QStatusBar::item {\n"
"         border: 1px solid blue;\n"
"         border-radius: 3px;}*/\n"
"\n"
"/*QStatusBar { background: rgb(64, 65, 66);}\n"
"QStatusBar::item {\n"
"         border: 1px solid red;\n"
"         border-radius: 3px;}*/"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        mainToolBar->setMovable(false);
        mainToolBar->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
        mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        mainToolBar->setFloatable(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuPreferences->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuFile->addAction(actionClose_all);
        menuFile->addSeparator();
        menuFile->addAction(actionNew_Window);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionPaste_as_child);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCopy_between_windows);
        menuEdit->addAction(actionPaste_between_windows);
        menuEdit->addSeparator();
        menuEdit->addAction(actionNew_Sibling);
        menuEdit->addAction(actionNew_Child);
        menuEdit->addAction(actionRemove_2);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFind);
        menuEdit->addAction(actionFindPrevious);
        menuEdit->addAction(actionFindNext);
        menuEdit->addSeparator();
        menuEdit->addAction(actionReplace);
        menuEdit->addAction(actionReplaceAll);
        menuEdit->addSeparator();
        menuEdit->addAction(actionMove_Up);
        menuEdit->addAction(actionMove_Down);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSort);
        menuEdit->addSeparator();
        menuEdit->addAction(actionExpand_All);
        menuEdit->addAction(actionCollapse_All);
        menuEdit->addSeparator();
        menuHelp->addAction(actionCheck_Update);
        menuHelp->addAction(menuUpgrade_Download_Proxy->menuAction());
        menuHelp->addAction(actionDownload_Upgrade_Packages);
        menuHelp->addSeparator();
        menuHelp->addAction(actionBug_Report);
        menuHelp->addAction(actionAbout);
        menuUpgrade_Download_Proxy->addAction(actProxy1);
        menuUpgrade_Download_Proxy->addAction(actProxy2);
        menuUpgrade_Download_Proxy->addAction(actProxy3);
        menuUpgrade_Download_Proxy->addAction(actProxy4);
        menuUpgrade_Download_Proxy->addAction(actProxy5);
        menuPreferences->addAction(actionRestoreScene);
        menuPreferences->addAction(actionExpandAllOpenFile);
        menuPreferences->addAction(actionShowPlistText);
        menuPreferences->addAction(actionAutoUpdateCheck);
        menuPreferences->addSeparator();
        menuPreferences->addAction(actionFont);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        actionAdd->setText(QString());
        actionAdd->setIconText(QString());
#if QT_CONFIG(tooltip)
        actionAdd->setToolTip(QCoreApplication::translate("MainWindow", "Add", nullptr));
#endif // QT_CONFIG(tooltip)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionOpen->setIconText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionClose->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        actionClose->setIconText(QCoreApplication::translate("MainWindow", "Close", nullptr));
#if QT_CONFIG(shortcut)
        actionClose->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+W", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionSave->setIconText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_as->setText(QCoreApplication::translate("MainWindow", "Save As...", nullptr));
        actionSave_as->setIconText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_as->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExpand_all->setText(QCoreApplication::translate("MainWindow", "Expand all", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
        actionNew->setIconText(QCoreApplication::translate("MainWindow", "New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionClose_all->setText(QCoreApplication::translate("MainWindow", "Close All", nullptr));
        actionClose_all->setIconText(QCoreApplication::translate("MainWindow", "Close all", nullptr));
#if QT_CONFIG(shortcut)
        actionClose_all->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+W", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
        actionRestoreScene->setText(QCoreApplication::translate("MainWindow", "RestoreScene", nullptr));
        actionDefaultNodeIcon->setText(QCoreApplication::translate("MainWindow", "Default Node Icon", nullptr));
        actionCheck_Update->setText(QCoreApplication::translate("MainWindow", "Update Check", nullptr));
        actionNew_Sibling->setText(QCoreApplication::translate("MainWindow", "New Sibling", nullptr));
        actionNew_Child->setText(QCoreApplication::translate("MainWindow", "New Child", nullptr));
        actionRemove_2->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        actionExpandAllOpenFile->setText(QCoreApplication::translate("MainWindow", "Expand All When Opening File", nullptr));
        actionNew_Window->setText(QCoreApplication::translate("MainWindow", "New Window", nullptr));
        actionCopy_between_windows->setText(QCoreApplication::translate("MainWindow", "Copy Between Windows", nullptr));
        actionPaste_between_windows->setText(QCoreApplication::translate("MainWindow", "Paste Between Windows", nullptr));
        actionShowPlistText->setText(QCoreApplication::translate("MainWindow", "Show Plist Text", nullptr));
        actionPaste_as_child->setText(QCoreApplication::translate("MainWindow", "Paste As Child", nullptr));
        actionFind->setText(QCoreApplication::translate("MainWindow", "Find", nullptr));
#if QT_CONFIG(shortcut)
        actionFind->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFindNext->setText(QCoreApplication::translate("MainWindow", "Next", nullptr));
#if QT_CONFIG(shortcut)
        actionFindNext->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+2", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFindPrevious->setText(QCoreApplication::translate("MainWindow", "Previous", nullptr));
#if QT_CONFIG(shortcut)
        actionFindPrevious->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionReplace->setText(QCoreApplication::translate("MainWindow", "Replace", nullptr));
#if QT_CONFIG(shortcut)
        actionReplace->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionReplaceAll->setText(QCoreApplication::translate("MainWindow", "Replace All", nullptr));
#if QT_CONFIG(shortcut)
        actionReplaceAll->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBug_Report->setText(QCoreApplication::translate("MainWindow", "Bug Report", nullptr));
        actionDiscussion_Forum->setText(QCoreApplication::translate("MainWindow", "Discussion Forum", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFont->setText(QCoreApplication::translate("MainWindow", "Font", nullptr));
        actionDownload_Upgrade_Packages->setText(QCoreApplication::translate("MainWindow", "Download Upgrade Packages", nullptr));
        actProxy1->setText(QCoreApplication::translate("MainWindow", "https://github.com/", nullptr));
        actProxy2->setText(QCoreApplication::translate("MainWindow", "https://ghproxy.com/https://github.com/", nullptr));
        actProxy3->setText(QCoreApplication::translate("MainWindow", "https://download.fastgit.org/", nullptr));
        actProxy4->setText(QCoreApplication::translate("MainWindow", "https://gh.api.99988866.xyz/https://github.com/", nullptr));
        actProxy5->setText(QCoreApplication::translate("MainWindow", "https://github.rc1844.workers.dev/", nullptr));
        actionMove_Up->setText(QCoreApplication::translate("MainWindow", "Move Up", nullptr));
#if QT_CONFIG(shortcut)
        actionMove_Up->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Up", nullptr));
#endif // QT_CONFIG(shortcut)
        actionMove_Down->setText(QCoreApplication::translate("MainWindow", "Move Down", nullptr));
#if QT_CONFIG(shortcut)
        actionMove_Down->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Down", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSort->setText(QCoreApplication::translate("MainWindow", "Sort", nullptr));
#if QT_CONFIG(shortcut)
        actionSort->setShortcut(QCoreApplication::translate("MainWindow", "Alt+T", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExpand_All->setText(QCoreApplication::translate("MainWindow", "Expand / Collapse All", nullptr));
#if QT_CONFIG(shortcut)
        actionExpand_All->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCollapse_All->setText(QCoreApplication::translate("MainWindow", "Collapse All", nullptr));
        actionAutoUpdateCheck->setText(QCoreApplication::translate("MainWindow", "Automatic Update Checking", nullptr));
        btnNo->setText(QCoreApplication::translate("MainWindow", "No", nullptr));
        lblTip->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        btnYes->setText(QCoreApplication::translate("MainWindow", "Yes", nullptr));
#if QT_CONFIG(tooltip)
        btnFind->setToolTip(QCoreApplication::translate("MainWindow", "Find", nullptr));
#endif // QT_CONFIG(tooltip)
        btnFind->setText(QCoreApplication::translate("MainWindow", "Find", nullptr));
#if QT_CONFIG(tooltip)
        btnNext->setToolTip(QCoreApplication::translate("MainWindow", "Go to the next", nullptr));
#endif // QT_CONFIG(tooltip)
        btnNext->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
#if QT_CONFIG(tooltip)
        btnShowReplace->setToolTip(QCoreApplication::translate("MainWindow", "Replace", nullptr));
#endif // QT_CONFIG(tooltip)
        btnShowReplace->setText(QCoreApplication::translate("MainWindow", ">>", nullptr));
        btnHideFind->setText(QCoreApplication::translate("MainWindow", "Hide", nullptr));
        btnReplaceAll->setText(QCoreApplication::translate("MainWindow", "All", nullptr));
        btnReplaceFind->setText(QCoreApplication::translate("MainWindow", "Replace && Find", nullptr));
#if QT_CONFIG(tooltip)
        btnPrevious->setToolTip(QCoreApplication::translate("MainWindow", "Go to the previous", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPrevious->setText(QCoreApplication::translate("MainWindow", "<", nullptr));
        btnReplace->setText(QCoreApplication::translate("MainWindow", "Replace", nullptr));
        btnMisc->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        lblFindCount->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblBytes->setText(QCoreApplication::translate("MainWindow", "Bytes", nullptr));
        btnUpdateHex->setText(QCoreApplication::translate("MainWindow", "^", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ASCII", nullptr));
        btnUpdateASCII->setText(QCoreApplication::translate("MainWindow", "^", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Base64", nullptr));
        btnUpdateBase64->setText(QCoreApplication::translate("MainWindow", "^", nullptr));
        btnChild->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        btnBrother->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        btnSort->setText(QCoreApplication::translate("MainWindow", "Sort", nullptr));
        btnExpand->setText(QCoreApplication::translate("MainWindow", "E", nullptr));
        btnNew->setText(QCoreApplication::translate("MainWindow", "N", nullptr));
        btnDel->setText(QCoreApplication::translate("MainWindow", "D", nullptr));
        btnFind_Tool->setText(QCoreApplication::translate("MainWindow", "F", nullptr));
        btnSave->setText(QCoreApplication::translate("MainWindow", "S", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuUpgrade_Download_Proxy->setTitle(QCoreApplication::translate("MainWindow", "Select the Mirror Server for Upgrade Download", nullptr));
        menuPreferences->setTitle(QCoreApplication::translate("MainWindow", "Preferences", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
