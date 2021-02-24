#include "mainwindow.h"
#include "filesystemwatcher.h"
#include "myapp.h"

#include "mytreeview.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSettings>
#include <QUrl>

QStatusBar* myStatusBar;
QToolBar* myToolBar;

EditorTabsWidget* tabWidget;
ItemState* copy_state;
DomItem* copy_item;

ItemState* AddMoveTemp;

QAction* copyAction;
QAction* cutAction;
QAction* pasteAction;
QAction* pasteAsChildAction;
QAction* actionNewSibling;
QAction* actionNewChild;
QAction* actionSort;

QUndoGroup* undoGroup;

QString fileName;
QVector<QString> filelist;
QVector<QString> openFileList;

int red = 0;

bool defaultIcon = false;

bool SelfSaved = false;

int windowX = 0;
int windowY = 0;

extern bool loading;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    loading = true;

    myToolBar = ui->mainToolBar;
    myStatusBar = ui->statusBar;

    tabWidget = new EditorTabsWidget(this);

    ui->centralWidget->layout()->addWidget(tabWidget);

    QApplication::setApplicationName("PlistEDPlus");
    QApplication::setOrganizationName("PlistED");

    CurVerison = "1.0.34";
    ver = "PlistEDPlus  V" + CurVerison + "        ";
    setWindowTitle(ver);

    QDir dir;
    if (dir.mkpath(QDir::homePath() + "/.config/PlistEDPlus/")) { }

    //获取背景色
    QPalette pal = this->palette();
    QBrush brush = pal.window();
    red = brush.color().red();

    //初始化plist文本Dock并删除Title棒（暂时）
    QWidget* lTitleBar = ui->dockWidget->titleBarWidget();
    QWidget* lEmptyWidget = new QWidget();
    ui->dockWidget->setTitleBarWidget(lEmptyWidget);
    delete lTitleBar;
    ui->gridLayout->setMargin(1);
    ui->dockWidgetContents->layout()->setMargin(1);
    ui->textEdit->setReadOnly(true);
    resizeDocks({ ui->dockWidget }, { 150 }, Qt::Vertical);
    myHL = new MyHighLighter(ui->textEdit->document());
    myHL->rehighlight();

    //初始化查找与替换界面
    //QWidget* lTitleBar2 = ui->dockWidget->titleBarWidget();
    QWidget* lEmptyWidget2 = new QWidget();
    ui->dockWidgetFindReplace->setTitleBarWidget(lEmptyWidget2);
    //delete lTitleBar2;
    ui->gridLayoutFindReplace->setMargin(1);
    //ui->gridLayoutFindReplace->setSpacing(1);
    ui->dockWidgetContentsFindReplace->layout()->setMargin(1);
    //ui->dockWidgetContentsFindReplace->layout()->setSpacing(1);
    ui->dockWidgetFindReplace->close();
    ui->btnPrevious->setEnabled(false);
    ui->btnNext->setEnabled(false);
    ui->btnReplace->setEnabled(false);

    undoGroup = new QUndoGroup(this);

    // create undo and redo actions
    actionUndo = undoGroup->createUndoAction(this, tr("Undo"));
    actionRedo = undoGroup->createRedoAction(this, tr("Redo"));

    // set shortcuts
    actionUndo->setShortcuts(QKeySequence::Undo);
    actionRedo->setShortcuts(QKeySequence::Redo);
    actionUndo->setIconVisibleInMenu(false);
    actionRedo->setIconVisibleInMenu(false);

    // add actions to menu
    ui->menuEdit->addAction(actionUndo);
    ui->menuEdit->addAction(actionRedo);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_currentChanged(int)));

    connect(undoGroup, SIGNAL(cleanChanged(bool)), this, SLOT(onCleanChanged(bool)));

    //File
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::actionOpen_activated);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::actionNew_activated);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::actionSave_activated);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::actionSave_as_activated);

    connect(ui->actionFile1, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile2, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile3, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile4, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile5, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile6, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile7, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile8, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile9, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    connect(ui->actionFile10, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    updateRecentFiles();

    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::actionClose_activated);
    connect(ui->actionClose_all, &QAction::triggered, this, &MainWindow::actionClose_all_activated);

    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequest(int)));

    connect(ui->actionNew_Window, &QAction::triggered, this, &MainWindow::on_NewWindow);
    ui->actionNew_Window->setShortcut(tr("ctrl+alt+n"));

    //Edit
    ui->actionCopy->setShortcuts(QKeySequence::Copy);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::on_copyAction);

    ui->actionPaste->setShortcuts(QKeySequence::Paste);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::on_pasteAction);

    ui->actionPaste_as_child->setShortcut(tr("shift+ctrl+v"));

    ui->actionCut->setShortcuts(QKeySequence::Cut);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::on_cutAction);

    connect(ui->actionCopy_between_windows, &QAction::triggered, this, &MainWindow::on_copyBW);
    ui->actionCopy_between_windows->setShortcut(tr("ctrl+b"));
    connect(ui->actionPaste_between_windows, &QAction::triggered, this, &MainWindow::on_pasteBW);
    ui->actionPaste_between_windows->setShortcut(tr("ctrl+alt+b"));

    //Help
    connect(ui->actionCheck_Update, &QAction::triggered, this, &MainWindow::CheckUpdate);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::actionAbout_activated);

    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::actionAdd_activated);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::actionRemove_activated);
    connect(ui->actionExpand_all, SIGNAL(triggered()), this, SLOT(actionExpand_all_activated()));

    ui->actionNew->setIcon(QIcon(":/new/toolbar/res/new.png"));
    ui->mainToolBar->addAction(ui->actionNew);

    ui->mainToolBar->addAction(ui->actionOpen);
    ui->actionOpen->setIcon(QIcon(":/new/toolbar/res/open.png"));

    ui->mainToolBar->addAction(ui->actionSave);
    ui->actionSave->setIcon(QIcon(":/new/toolbar/res/save.png"));

    ui->mainToolBar->addAction(ui->actionSave_as);
    ui->actionSave_as->setIconVisibleInMenu(false);
    ui->actionSave_as->setIcon(QIcon(":/new/toolbar/res/saveas.png"));

    ui->mainToolBar->addSeparator();

    actionNewSibling = new QAction(tr("New Sibling"), this);
    ui->mainToolBar->addAction(actionNewSibling);
    actionNewSibling->setIcon(QIcon(":/new/toolbar/res/sibling.png"));
    connect(actionNewSibling, &QAction::triggered, this, &MainWindow::on_actionNewSibling);
    connect(ui->actionNew_Sibling, &QAction::triggered, this, &MainWindow::on_actionNewSibling);
    ui->actionNew_Sibling->setShortcut(tr("ctrl++"));

    actionNewChild = new QAction(tr("New Child"), this);
    ui->mainToolBar->addAction(actionNewChild);
    actionNewChild->setIcon(QIcon(":/new/toolbar/res/child.png"));
    connect(actionNewChild, &QAction::triggered, this, &MainWindow::on_actionNewChild);

    connect(ui->actionNew_Child, &QAction::triggered, this, &MainWindow::on_actionNewChild);
    ui->actionNew_Child->setShortcut(tr("+"));

    ui->mainToolBar->addAction(ui->actionRemove);
    ui->actionRemove->setShortcut(Qt::Key_Delete);
    ui->actionRemove_2->setShortcut(Qt::Key_Delete);
    ui->actionRemove_2->setShortcut(tr("-"));
    connect(ui->actionRemove_2, &QAction::triggered, this, &MainWindow::actionRemove_activated);

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(ui->actionExpand_all);
    ui->actionExpand_all->setIcon(QIcon(":/new/toolbar/res/exp.png"));

    ui->mainToolBar->addSeparator();

    QAction* actionMoveUp = new QAction(tr("Move up"));
    actionMoveUp->setIcon(QIcon(":/new/toolbar/res/up.png"));
    actionMoveUp->setShortcut(tr("ctrl+u"));
    ui->mainToolBar->addAction(actionMoveUp);
    connect(actionMoveUp, &QAction::triggered, this, &MainWindow::on_actionMoveUp);

    QAction* actionMoveDown = new QAction(tr("Move down"));
    actionMoveDown->setIcon(QIcon(":/new/toolbar/res/down.png"));
    actionMoveDown->setShortcut(tr("ctrl+d"));
    ui->mainToolBar->addAction(actionMoveDown);
    connect(actionMoveDown, &QAction::triggered, this, &MainWindow::on_actionMoveDown);

    ui->mainToolBar->addSeparator();

    actionSort = new QAction(tr("A->Z Sort"));
    actionSort->setIcon(QIcon(":/new/toolbar/res/sort.png"));
    ui->mainToolBar->addAction(actionSort);
    connect(actionSort, &QAction::triggered, this, &MainWindow::on_actionSort);

    ui->mainToolBar->addSeparator();

    actionUndo->setIcon(QIcon(":/new/toolbar/res/undo.png"));
    ui->mainToolBar->addAction(actionUndo);

    actionRedo->setIcon(QIcon(":/new/toolbar/res/redo.png"));
    ui->mainToolBar->addAction(actionRedo);

    ui->mainToolBar->addSeparator();

    QAction* findAction = new QAction(QIcon(":/new/toolbar/res/find.png"), tr(""), this);
    findAction->setToolTip(tr("Find and Replace"));
    findAction->setShortcut(tr("ctrl+F"));
    //ui->mainToolBar->addAction(findAction);
    connect(findAction, &QAction::triggered, this, &MainWindow::on_ShowFindReplace);

    lblFindCount = new QLabel("0"); //查找结果计数器
    ui->mainToolBar->addWidget(lblFindCount);
    findEdit = new QLineEdit();
    ui->editFind->setClearButtonEnabled(true);
    ui->editFind->setPlaceholderText(tr("Find"));
    ui->editReplace->setClearButtonEnabled(true);
    ui->editReplace->setPlaceholderText(tr("Replace"));

    //ui->mainToolBar->addWidget(ui->btnMisc);
    ui->mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu); //屏蔽默认的右键菜单
    ui->mainToolBar->addWidget(ui->editFind);
    //ui->mainToolBar->addAction(findAction);
    ui->mainToolBar->addWidget(ui->btnFind);
    ui->btnFind->setIcon(QIcon(":/new/toolbar/res/find.png"));
    //设置下拉菜单
    actCaseSensitive->setCheckable(true);
    btnFindMenu = new QMenu(this);
    btnFindMenu->addAction(actCaseSensitive);

    ui->btnFind->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->btnFind, &QPushButton::customContextMenuRequested, [=](const QPoint& pos) {
        //qDebug() << pos;
        Q_UNUSED(pos);
        btnFindMenu->exec(QCursor::pos());
    });

    connect(actClearList, &QAction::triggered, [=]() {
        ui->editFind->setText("");
        FindTextList.clear();
        on_editFind_returnPressed();
    });

    ui->mainToolBar->addWidget(ui->btnPrevious);
    ui->btnPrevious->setIcon(QIcon(":/new/toolbar/res/1.png"));
    ui->mainToolBar->addWidget(ui->btnNext);
    ui->btnNext->setIcon(QIcon(":/new/toolbar/res/2.png"));
    ui->mainToolBar->addWidget(ui->btnShowReplace);
    ui->btnMisc->setVisible(false);

    connect(findEdit, &QLineEdit::returnPressed, this, &MainWindow::findEdit_returnPressed);
    connect(findEdit, &QLineEdit::textChanged, this, &MainWindow::findEdit_textChanged);

    ui->menuEdit->addSeparator();
    QAction* expandAction = new QAction(tr("Expand") + "/" + tr("Collapse"), this);
    expandAction->setShortcut(tr("space"));
    ui->menuEdit->addAction(expandAction);
    connect(expandAction, &QAction::triggered, this, &MainWindow::on_expandAction);

    QAction* collapseAction = new QAction(tr("Collapse"), this);
    connect(collapseAction, &QAction::triggered, this, &MainWindow::on_collapseAction);

#ifdef Q_OS_WIN32
    reg_win();
    this->resize(QSize(1350, 750));
    win = true;
    ui->actionRemove_2->setShortcut(tr("ctrl+-"));
    ui->actionNew_Child->setShortcut(tr("alt++"));
#endif

#ifdef Q_OS_LINUX
    linuxOS = true;
#endif

#ifdef Q_OS_MAC
    ui->mainToolBar->setIconSize(QSize(28, 28));
    this->resize(QSize(1050, 600));
    mac = true;
    //ui->actionCheck_Update->setVisible(false);
#endif

    ui->actionSaveAndFind->setCheckable(true);
    ui->actionSaveAndFind->setVisible(false);
    QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
    QFileInfo fi(qfile);
    if (fi.exists()) {
        QSettings Reg(qfile, QSettings::IniFormat);
        defaultIcon = Reg.value("DefaultIcon").toBool();
        ui->actionDefaultNodeIcon->setChecked(defaultIcon);

        SaveAndFind = Reg.value("SaveAndFind").toBool();
        ui->actionSaveAndFind->setChecked(SaveAndFind);

        ui->actionExpandAllOpenFile->setChecked(Reg.value("ExpAll").toBool());

        bool restore = Reg.value("restore").toBool();
        ui->actionRestoreScene->setChecked(restore);
        if (restore) {
            int count = Reg.value("count").toInt();

            for (int i = 0; i < count; i++) {
                QString file = Reg.value(QString::number(i) + "/file").toString();

                QFileInfo fi(file);
                if (fi.exists()) {
                    openPlist(file);
                }
            }

            int index = Reg.value("index").toInt();
            if (index >= 0 && index < tabWidget->tabBar()->count())
                tabWidget->setCurrentIndex(index);
            if (index >= tabWidget->tabBar()->count())
                tabWidget->setCurrentIndex(tabWidget->tabBar()->count() - 1);
        }

        bool drag = Reg.value("drag").toBool();
        if (drag) {
            int x, y, w, h;
            x = Reg.value("x").toInt();
            y = Reg.value("y").toInt();
            windowX = x;
            windowY = y;
            QScreen* screen = QGuiApplication::primaryScreen();
            w = screen->size().width();
            h = screen->size().height();
            if (x > w - 300)
                x = w - 300;
            if (y > h - 300)
                y = h - 300;

            this->setGeometry(x, y, this->width(), this->height());
        }

        //Get search text list
        int count = Reg.value("FindTextListTotal").toInt();

        btnFindMenu->addSeparator();
        btnFindMenu->addAction(actClearList);
        QAction* actTotal = new QAction(tr("Total") + " : " + QString::number(count), this);
        btnFindMenu->addAction(actTotal);
        btnFindMenu->addSeparator();

        for (int i = 0; i < count; i++) {

            QString strList = Reg.value("FindTextList" + QString::number(i)).toString();
            FindTextList.append(strList);

            btnFindActionList.append(new QAction(strList, this));
            btnFindMenu->addAction(btnFindActionList.at(i));

            connect(btnFindActionList.at(i), &QAction::triggered, [=]() {
                qDebug() << "I'm btnFirstAction";
                ui->editFind->setText(btnFindActionList.at(i)->text());
                on_btnFind_clicked();
            });
        }

        QCompleter* editFindCompleter = new QCompleter(FindTextList, this);
        editFindCompleter->setCaseSensitivity(Qt::CaseSensitive);
        editFindCompleter->setCompletionMode(QCompleter::InlineCompletion);
        ui->editFind->setCompleter(editFindCompleter);

        //是否显示plist文本
        ui->actionShowPlistText->setChecked(Reg.value("ShowPlistText", 1).toBool());
        if (ui->actionShowPlistText->isChecked())
            ui->dockWidget->setHidden(false);
        else
            ui->dockWidget->setHidden(true);
    }

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    loading = false;
}

MainWindow::~MainWindow()
{
    delete undoGroup;
    delete ui;
}

void MainWindow::actionNew_activated()
{

    // create new model
    DomModel* model = new DomModel();

    // add root
    DomItem* i = model->getRoot()->addChild();

    // set root data
    i->setName("plist");
    i->setType("dict");

    // create tab with model
    tabWidget->createTab(model);

    EditorTab* tab = tabWidget->getCurentTab();
    //QTreeView* treeView = new QTreeView;
    //treeView = (QTreeView*)tab->children().at(1);
    tab->treeView->setCurrentIndex(model->index(0, 0)); //设置当前索引

    tab->treeView->setFocus();
}

void MainWindow::actionOpen_activated()
{
    openFiles();
}

void MainWindow::actionClose_activated()
{
    if (tabWidget->hasTabs()) {
        onTabCloseRequest();
    }
}

void MainWindow::actionClose_all_activated()
{
    if (tabWidget->hasTabs())
        tabWidget->closeAllTabs();
}

void MainWindow::openFiles(QStringList list)
{
    if (list.isEmpty()) {
        list = QFileDialog::getOpenFileNames(
            this, tr("Select files to open"),
            "", "Property list (*.plist);;All files(*.*)");
    }

    for (int i = 0; i < list.size(); ++i) {
        openPlist(list[i]);
    }
}

void MainWindow::openPlist(QString filePath)
{
    if (!filePath.isEmpty()) {

        /*bool opened = false;
        for(int i = 0; i < tabWidget->tabBar()->count(); i ++)
        {
            if(filePath == tabWidget->getTab(i)->getPath())
            {
                tabWidget->tabBar()->setCurrentIndex(i);
                opened = true;
            }
        }*/

        for (int i = 0; i < tabWidget->tabBar()->count(); i++) {
            if (filePath == tabWidget->getTab(i)->getPath()) {
                tabWidget->closeTab(i);
                break;
            }
        }

        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) // && !opened)
        {
            QDomDocument document;

            if (document.setContent(&file)) {
                //qDebug() << QString("File %1 opened").arg(filePath);

                DomModel* model = DomParser::fromDom(document);

                tabWidget->createTab(model, filePath);
            }
            file.close();
        }

        QString fn = QDir::homePath() + "/.config/PlistEDPlus/temp.plist";
        if (filePath != fn) {
            setRecentFiles(filePath);
            updateRecentFiles();
        }

        FileSystemWatcher::addWatchPath(filePath); //监控这个文件的变化

        bool re = false;
        for (int i = 0; i < openFileList.count(); i++) {
            if (openFileList.at(i) == filePath)
                re = true;
        }
        if (!re)
            openFileList.append(filePath);

        //列宽自动适应最长的条目
        EditorTab* tab = tabWidget->getCurentTab();

        //tab->treeView->resizeColumnToContents(0);

        tab->treeView->setCurrentIndex(tab->getModel()->index(0, 0));
        tab->treeView->setFocus();

        if (ui->actionExpandAllOpenFile->isChecked())
            actionExpand_all_activated();

        QFileInfo fi(filePath);
        tabWidget->tabBar()->setTabToolTip(tabWidget->currentIndex(), fi.fileName());

        loadText(filePath);
    }
}

void MainWindow::onTabCloseRequest(int i)
{
    if (i != -1)
        tabWidget->setCurrentIndex(i);

    if (!undoGroup->isClean()) {
        // make tab active
        //if (i != -1) tabWidget->setCurrentIndex(i);
        QString fn = tabWidget->getCurentTab()->getFileName();
        // messageobox for save
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("The document has been modified.") + "\n" + fn);
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setButtonText(QMessageBox::Save, QString(tr("Save")));
        msgBox.setButtonText(QMessageBox::Cancel, QString(tr("Cancel")));
        msgBox.setButtonText(QMessageBox::Discard, QString(tr("Discard")));
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Cancel:
            // Cancel was clicked
            close_flag = 0;
            return;

        case QMessageBox::Save:
            close_flag = 1;
            actionSave_activated();
            break;

        case QMessageBox::Discard:
            // Don't Save was clicked
            close_flag = 2;
            break;
        }
    }

    // get current stack
    QUndoStack* stack = tabWidget->getCurentTab()->getUndoStack();

    // remove stack from group
    undoGroup->removeStack(stack);

    QString file = tabWidget->getTab(i)->getPath();
    QFileInfo fi(file);
    if (fi.exists())
        FileSystemWatcher::removeWatchPath(file);
    for (int i = 0; i < openFileList.count(); i++) {
        if (file == openFileList.at(i)) {
            openFileList.remove(i);
            break;
        }
    }

    // close tab
    tabWidget->closeTab();

    if (tabWidget->currentIndex() != -1)
        tabWidget->getCurentTab()->treeView->setFocus();
}

void MainWindow::savePlist(QString filePath)
{
    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();

        // get parsed dom doc
        QDomDocument doc = DomParser::toDom(tab->getModel());

        // create and open file
        QFile file(filePath);
        file.open(QIODevice::WriteOnly);

        // create txt stream with file
        QTextStream out(&file);

        // write to file
        doc.save(out, 4);

        // close file
        file.close();

        // set new name
        tab->setPath(filePath);

        //get tab index
        int index = tabWidget->indexOf(tab);

        // get name
        QString name = tab->getFileName();

        // set text
        tabWidget->setTabText(index, name);

        // set stack clean
        undoGroup->activeStack()->clear();
        //undoGroup->activeStack()->setClean();

        SelfSaved = true;
        FileSystemWatcher::addWatchPath(filePath); //监控这个文件的变化

        bool re = false;
        for (int i = 0; i < openFileList.count(); i++) {
            if (openFileList.at(i) == filePath)
                re = true;
        }
        if (!re)
            openFileList.append(filePath);

        QFileInfo fi(filePath);
        tabWidget->tabBar()->setTabToolTip(tabWidget->currentIndex(), fi.fileName());

        loadText(filePath);
        goPlistText();
    }
}

void MainWindow::actionSave_activated()
{
    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();
        QString path = tab->getPath();
        if (!path.isEmpty())
            savePlist(path);
        else
            actionSave_as_activated();
    }
}

void MainWindow::actionSave_as_activated()
{
    if (tabWidget->hasTabs()) {
        QString cfile = tabWidget->getCurentTab()->getPath();

        QString str = QFileDialog::getSaveFileName(
            this, tr("Save as"), "", tr("Property list (*.plist)"));

        if (!str.isEmpty()) {
            QFileInfo fi(cfile);
            if (fi.exists())
                FileSystemWatcher::removeWatchPath(cfile);
            for (int i = 0; i < openFileList.count(); i++) {
                if (cfile == openFileList.at(i)) {
                    openFileList.removeOne(cfile);
                    break;
                }
            }

            FileSystemWatcher::addWatchPath(str);
            savePlist(str);

            this->setWindowTitle(ver + "[*] " + tabWidget->getCurentTab()->getPath());
        }
    }
}

void MainWindow::actionAdd_activated()
{

    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();
        const QModelIndex index = tab->currentIndex();
        DomModel* model = tab->getModel();

        QModelIndex index0 = model->index(index.row(), 0, index.parent());

        if (index0.data().toString() == "")
            return;

        if (index.isValid()) {

            QUndoCommand* addMoveCommand = new AddMoveCommand(model, index);
            undoGroup->activeStack()->push(addMoveCommand);
        }
    }
}

void MainWindow::actionRemove_activated()
{
    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();
        const QModelIndex index = tab->currentIndex();

        if (index.isValid()) {
            DomModel* model = tab->getModel();

            if (model->itemNotPlist(index)) {
                QUndoCommand* removeCommand = new RemoveCommand(model, index);
                undoGroup->activeStack()->push(removeCommand);
            }
        }
    }
}

void MainWindow::actionExpand_all_activated()
{
    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();

        tab->expand();

        setExpandText(tab);
    }
}

void MainWindow::actionAbout_activated()
{
    //QMessageBox::about(this, tr("About PlistEDPlus"),
    //                     "PlistEDPlus");
    QFileInfo appInfo(qApp->applicationFilePath());
    QString str;

    str = tr("Last modified: ");

    QString last = str + appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    QString str1 = "<a style='color:blue;' href = https://github.com/ic005k/PlistEDPlus>PlistEDPlus</a><br><br>";
    QString str2 = tr("Thanks: Yaroslav Sushkov (alpex92)");
    QString str3 = "<br>";

    QMessageBox::about(this, "About", str1 + str2 + str3 + last);
}

void MainWindow::tabWidget_currentChanged(int index)
{

    if (index >= 0) {
        if (tabWidget->hasTabs()) {
            // get tab widget
            EditorTab* tab = tabWidget->getCurentTab();

            setExpandText(tab);

            // set window title to filename
            //this->setWindowFilePath(tabWidget->tabText(tabWidget->indexOf(tab)) + "[*]");
            this->setWindowTitle(ver + "[*] " + tabWidget->getCurentTab()->getPath());

            // get undo stack
            QUndoStack* stack = tab->getUndoStack();

            // set active stack
            if (!undoGroup->stacks().contains(stack))
                undoGroup->addStack(stack);

            undoGroup->setActiveStack(stack);

            if (!loading) {
                loadText(tabWidget->getCurentTab()->getPath());
            }

            if (this->isVisible() && !loading) {
                //if (mac) {
                //goPlistText();
                //}
            }

            ui->btnPrevious->setEnabled(false);
            ui->btnNext->setEnabled(false);
            ui->btnReplace->setEnabled(false);
        }
        //?
        //else this->setWindowFilePath(" ");
    }
}

void MainWindow::onCleanChanged(bool clean)
{

    this->setWindowModified(!clean);
}

void MainWindow::setRecentFiles(const QString& fileName)
{
    QSettings settings("PlistEDPlus", "PlistEDPlus");

    QStringList files = settings.value("recentFileList").toStringList();

    // remove all inclusions
    files.removeAll(fileName);

    // add to begining
    files.prepend(fileName);

    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
}

void MainWindow::updateRecentFiles()
{
    QSettings settings("PlistEDPlus", "PlistEDPlus");
    //settings.clear();
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QAction* action = ui->menuRecent_files->actions().at(i);
        QString text = files.at(i);

        if (QFileInfo(text).exists()) {
            action->setText(text);
            action->setVisible(true);
        }
    }
}

void MainWindow::openRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString file = action->text();
        QFileInfo fi(file);
        if (fi.exists())
            openPlist(file);
    }
}

void MainWindow::setExpandText(EditorTab* tab)
{
    QString text = (!tab->isExpanded()) ? tr("Expand all") : tr("Collapse all");
    ui->actionExpand_all->setIconText(text);

    if (ui->actionExpand_all->iconText() == tr("Expand all")) {
        ui->actionExpand_all->setIcon(QIcon(":/new/toolbar/res/exp.png"));
        ui->actionExpand_all->setToolTip(tr("Expand all"));
    }

    if (ui->actionExpand_all->iconText() == tr("Collapse all")) {
        ui->actionExpand_all->setIcon(QIcon(":/new/toolbar/res/col.png"));
        ui->actionExpand_all->setToolTip(tr("Collapse all"));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasFormat("text/uri-list"))
        event->accept();

    // application/xml application/x-plist
}

void MainWindow::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QStringList list;
    for (int i = 0; i < urls.size(); ++i) {
        list.append(urls.at(i).toLocalFile());
    }

    openFiles(list);
}

void MainWindow::menu_aboutToShow() //目前废除，去掉1就可正常使用
{
    bool enabled = tabWidget->hasTabs();
    ui->actionClose->setEnabled(enabled);
    ui->actionClose_all->setEnabled(enabled);
    ui->actionSave->setEnabled(enabled);
    ui->actionSave_as->setEnabled(enabled);
}

void MainWindow::on_Find()
{

    ui->btnReplace->setEnabled(false);

    if (ui->editFind->text() == "")
        return;

    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();

        if (ui->actionSaveAndFind->isChecked()) {

            QString fn = tabWidget->getCurentTab()->getPath();
            if (QFileInfo(fn).exists()) {

                actionSave_activated();

                FileSystemWatcher::removeWatchPath(fn);

                //Open current plist
                QFile file(fn);
                if (file.open(QIODevice::ReadOnly)) {
                    QDomDocument document;

                    if (document.setContent(&file)) {

                        DomModel* model = DomParser::fromDom(document);

                        tab->setModel(model);
                    }
                    file.close();
                }

                FileSystemWatcher::addWatchPath(fn);
            }
        }

        QModelIndex index;

        DomModel* model = tab->getModel();

        //QTreeView* treeView = new QTreeView;
        //treeView = (QTreeView*)tab->children().at(1);

        //treeView->collapseAll();
        tab->treeView->expandToDepth(0);

        index = model->index(0, 0);
        tab->treeView->setCurrentIndex(index); //设置当前索引
        tab->treeView->setFocus();

        findCount = 0;
        lblFindCount->setText("  " + QString::number(findCount) + "  ");
        find = false;

        if (index.isValid()) {
            indexFindList.clear();
            indexCount = -1;

            QString strFind = ui->editFind->text().trimmed();
            if (!actCaseSensitive->isChecked())
                strFind = strFind.toLower();

            forEach(model, index, strFind);
        } else
            qDebug() << "index is no valid";
    }
}

void MainWindow::forEach(QAbstractItemModel* model, QModelIndex parent, QString str)
{

    for (int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index2 = model->index(r, 2, parent);
        QString value = model->data(index2, Qt::DisplayRole).toString();
        if (!actCaseSensitive->isChecked())
            value = value.toLower();

        QModelIndex index = model->index(r, 0, parent);
        //QVariant name = model->data(index);
        QString name = model->data(index, Qt::DisplayRole).toString();
        if (!actCaseSensitive->isChecked())
            name = name.toLower();

        //搜索值
        if (value.contains(str.trimmed()) && str.trimmed() != "") {

            EditorTab* tab = tabWidget->getCurentTab();
            //DomModel* model = tab->getModel();

            //QTreeView* treeView = new QTreeView;
            //treeView = (QTreeView*)tab->children().at(1);

            tab->treeView->selectionModel()->setCurrentIndex(index2, QItemSelectionModel::Select);
            //treeView->selectionModel()->setCurrentIndex(index2, QItemSelectionModel::SelectCurrent);
            findCount++;
            lblFindCount->setText("  " + QString::number(findCount) + "  ");
            find = true;

            actionSort->setEnabled(false);

            //treeView->expand(index2);
            //tab->view_expand(index2, model);

            indexFindList.append(index2);
        }
        //搜索键
        if (name.contains(str.trimmed()) && str.trimmed() != "") {

            EditorTab* tab = tabWidget->getCurentTab();
            //DomModel* model = tab->getModel();
            //QTreeView* treeView = new QTreeView;
            //treeView = (QTreeView*)tab->children().at(1);

            tab->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
            //treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
            findCount++;
            lblFindCount->setText("  " + QString::number(findCount) + "  ");
            find = true;

            actionSort->setEnabled(false);

            //treeView->expand(index);
            //tab->view_expand(index, model);

            indexFindList.append(index);
        }

        if (model->hasChildren(index)) {
            forEach(model, index, str);
        }
    }

    if (find) {
        ui->btnPrevious->setEnabled(true);
        ui->btnNext->setEnabled(true);
    }
}

void MainWindow::findEdit_textChanged(const QString& arg1)
{
    if (tabWidget->hasTabs()) {

        if (arg1 != "") {

            //on_Find();
        }

        if (arg1 == "" || !find) {
            findCount = 0;
            lblFindCount->setText("  " + QString::number(findCount) + "  ");

            EditorTab* tab = tabWidget->getCurentTab();
            //QTreeView* treeView = new QTreeView;
            //treeView = (QTreeView*)tab->children().at(1);
            QModelIndex index = tab->currentIndex();
            tab->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Clear);
        }
    }
}

void MainWindow::findEdit_returnPressed()
{
    on_Find();

    QString str = findEdit->text().trimmed();
    bool re = false;
    for (int i = 0; i < FindTextList.count(); i++) {
        if (FindTextList.at(i) == str) {
            re = true;
            break;
        }
    }

    if (!re)
        FindTextList.insert(0, str);

    QCompleter* editFindCompleter = new QCompleter(FindTextList, this);
    editFindCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editFindCompleter->setCompletionMode(QCompleter::InlineCompletion);
    findEdit->setCompleter(editFindCompleter);
}

void MainWindow::on_copyAction()
{

    if (tabWidget->hasTabs())
        tabWidget->getCurentTab()->on_copyAction();
}

void MainWindow::on_cutAction()
{

    if (tabWidget->hasTabs())
        tabWidget->getCurentTab()->on_cutAction();
}

void MainWindow::on_pasteAction()
{

    if (tabWidget->hasTabs())
        tabWidget->getCurentTab()->on_pasteAction();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    loading = true;

    //记录当前文件
    QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
    QFile file(qfile);
    QSettings Reg(qfile, QSettings::IniFormat);

    Reg.setValue("restore", ui->actionRestoreScene->isChecked());
    Reg.setValue("DefaultIcon", ui->actionDefaultNodeIcon->isChecked());
    Reg.setValue("SaveAndFind", ui->actionSaveAndFind->isChecked());
    Reg.setValue("ExpAll", ui->actionExpandAllOpenFile->isChecked());
    Reg.setValue("drag", false);

    //记录搜索文本列表
    //先读取，采用增量保存的方式，以免覆盖之前的已有数据
    QFileInfo fi(qfile);

    if (fi.exists()) {
        QStringList tempList;
        int count = Reg.value("FindTextListTotal").toInt();
        for (int i = 0; i < count; i++) {
            tempList.append(Reg.value("FindTextList" + QString::number(i)).toString());
        }

        if (FindTextList.count() < tempList.count()) {
            //FindTextList = tempList;
        }
    }

    int findTotal;
    if (FindTextList.count() > 50)
        findTotal = 50;
    else
        findTotal = FindTextList.count();
    Reg.setValue("FindTextListTotal", findTotal);
    for (int i = 0; i < FindTextList.count(); i++) {

        Reg.setValue("FindTextList" + QString::number(i), FindTextList.at(i));
    }

    //记录是否显示Plist文本
    Reg.setValue("ShowPlistText", ui->actionShowPlistText->isChecked());

    if (tabWidget->hasTabs()) {

        int count = tabWidget->count();

        Reg.setValue("restore", ui->actionRestoreScene->isChecked());
        Reg.setValue("DefaultIcon", ui->actionDefaultNodeIcon->isChecked());
        Reg.setValue("count", count);
        Reg.setValue("index", tabWidget->tabBar()->currentIndex());

        for (int i = 0; i < count; i++) {
            tabWidget->setCurrentIndex(i);
            QString fn = tabWidget->getCurentTab()->getPath();
            Reg.setValue(QString::number(i) + "/" + "file", fn);
        }

        for (int i = 0; i < count; i++) {
            tabWidget->setCurrentIndex(0);

            emit tabWidget->tabCloseRequested(0);
            if (close_flag == 0) //0取消、1保存、2放弃标志，为后面新增功能预留
            {
                event->ignore();
                close_flag = -1;

                loading = false;

                break; //如果第一个标签页选择取消，则直接终止关闭
            }
            if (close_flag == 1) {

                event->ignore();
                close_flag = -1;
            }
            if (close_flag == 2) {
                event->ignore();
                close_flag = -1;
            }
        }

        if (tabWidget->count() == 0)
            event->accept();
    } else {
        Reg.setValue("count", 0);
    }

    loading = false;
}

void MainWindow::reg_win()
{
    QString appPath = qApp->applicationFilePath();

    QString dir = qApp->applicationDirPath();
    // 注意路径的替换
    appPath.replace("/", "\\");
    QString type = "PlistEDPlus";
    QSettings* regType = new QSettings("HKEY_CLASSES_ROOT\\.plist", QSettings::NativeFormat);
    QSettings* regIcon = new QSettings("HKEY_CLASSES_ROOT\\.plist\\DefaultIcon", QSettings::NativeFormat);
    QSettings* regShell = new QSettings("HKEY_CLASSES_ROOT\\QtOpenCoreConfig\\shell\\open\\command", QSettings::NativeFormat);

    regType->remove("Default");
    regType->setValue("Default", type);

    regIcon->remove("Default");
    // 0 使用当前程序内置图标
    regIcon->setValue("Default", appPath + ",1");

    // 百分号问题
    QString shell = "\"" + appPath + "\" ";
    shell = shell + "\"%1\"";

    regShell->remove("Default");
    regShell->setValue("Default", shell);

    delete regIcon;
    delete regShell;
    delete regType;

    // 通知系统刷新
#ifdef Q_OS_WIN32
    //::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST|SHCNF_FLUSH, 0, 0);
#endif
}

void MainWindow::resizeEvent(QResizeEvent* event)
{

    Q_UNUSED(event);
}

void MainWindow::on_actionMoveUp()
{

    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        //QTreeView* treeView = new QTreeView;
        //treeView = (QTreeView*)tab->children().at(1);
        DomModel* model = tab->getModel();
        QModelIndex index, index_bak;
        index = tab->currentIndex();
        index_bak = index;

        if (!index.isValid())
            return;

        DomItem* items = model->itemForIndex(index.parent());

        DomItem* currentItem = model->itemForIndex(index);
        if (currentItem->getType() == "bool") {
            QString strBool = currentItem->getValue().trimmed();
            currentItem->setValue(strBool);
        }

        if (items == NULL)
            return;

        if (index.row() == 0)
            return;

        bool array = false;
        if (items->getType() == "array") {
            items->setType("dict");
            array = true;
        }

        on_cutAction();
        tab->treeView->setCurrentIndex(model->index(index_bak.row() - 1, 0, index.parent()));
        on_pasteAction();

        //之前的老方法
        /*AddMoveTemp = model->saveItemState(index);
        model->addMoveItem(index.parent(), index.row() - 1, AddMoveTemp);

        treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
        index = tab->currentIndex();
        index = model->index(index.row(), 0, index.parent());

        model->removeItem(index);*/

        if (array)
            items->setType("array");

        tab->treeView->setCurrentIndex(model->index(index_bak.row() - 1, 0, index.parent()));
    }
}
void MainWindow::on_actionMoveDown()
{
    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        //QTreeView* treeView = new QTreeView;
        //treeView = (QTreeView*)tab->children().at(1);
        DomModel* model = tab->getModel();
        QModelIndex index, index_bak;
        index = tab->currentIndex();
        index_bak = index;

        if (!index.isValid())
            return;

        DomItem* items = model->itemForIndex(index.parent());

        DomItem* currentItem = model->itemForIndex(index);
        if (currentItem->getType() == "bool") {
            QString strBool = currentItem->getValue().trimmed();
            currentItem->setValue(strBool);
        }

        if (items == NULL)
            return;

        if (index.row() == items->childCount() - 1)
            return;

        bool array = false;
        if (items->getType() == "array") {
            items->setType("dict");
            array = true;
        }

        //倒数第三行及以前
        if (index.row() <= items->childCount() - 3) {
            on_cutAction();
            tab->treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
            on_pasteAction();
        }

        //倒数第二行
        if (index.row() == items->childCount() - 2) {

            QUndoCommand* addMoveCommand = new AddMoveCommand(tab->getModel(), index.parent());
            undoGroup->activeStack()->push(addMoveCommand);

            on_cutAction();
            tab->treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
            on_pasteAction();
            tab->treeView->setCurrentIndex(model->index(index_bak.row() + 2, 0, index.parent()));
            actionRemove_activated();
        }

        //之前的老方法
        /*ItemState* temp = model->saveItemState(index);
        int row = index.row() + 2;
        model->addMoveItem(index.parent(), row, temp);

        model->removeItem(index_bak);*/

        if (array)
            items->setType("array");

        tab->treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
    }
}

void MainWindow::showMsg()
{
    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index;
    index = tab->currentIndex();
    DomModel* model = tab->getModel();
    DomItem* item = model->itemForIndex(index);

    QString str1, str2, str3, str4, str5;
    str1 = QObject::tr("Currently selected: ") + index.data().toString();
    str2 = "      " + QObject::tr("Row: ") + QString::number(index.row() + 1);
    str3 = "      " + QObject::tr("Column: ") + QString::number(index.column() + 1);
    str4 = "      " + QObject::tr("Parent level：") + index.parent().data().toString();
    str5 = "      " + QObject::tr("Children: ") + QString::number(item->childCount());

    myStatusBar->showMessage(str1 + str2 + str3 + str5 + str4);
}

QString MainWindow::getPlistTextValue(QString str)
{
    QString str0, str1, str2, str3, str4;
    str0 = str.trimmed();
    for (int i = 0; i < str0.length(); i++) {
        str1 = str0.mid(i, 1);
        if (str1 == ">") {
            str2 = str0.mid(i + 1, str0.length() - i);
            //qDebug() << str2;

            for (int j = 0; j < str2.length(); j++) {
                str3 = str2.mid(j, 1);
                if (str3 == "<") {
                    str4 = str2.mid(0, j);
                    //qDebug() << str4;
                    break;
                }
            }

            break;
        }
    }

    return str4;
}

void MainWindow::goPlistText()
{
    //转到plist文本
    if (!loading) {

        EditorTab* tab = tabWidget->getCurentTab();
        QModelIndex index;
        index = tab->currentIndex();
        DomModel* model = tab->getModel();
        DomItem* item = model->itemForIndex(index);

        QString name, datatype, val;
        name = item->getName();
        datatype = item->getType();
        val = item->getValue().trimmed();
        if (datatype == "data") {

            val = tab->HexStrToByte(val).toBase64().trimmed();
        }

        //QTextBlock block = ui->textEdit->document()->findBlockByNumber(0);
        //ui->textEdit->setTextCursor(QTextCursor(block));

        for (int i = 0; i < ui->textEdit->document()->lineCount(); i++) {

            QTextBlock block = ui->textEdit->document()->findBlockByNumber(i);
            ui->textEdit->setTextCursor(QTextCursor(block));
            QString lineText = ui->textEdit->document()->findBlockByNumber(i).text().trimmed();

            if (name.contains("Item")) {

                if (getPlistTextValue(lineText) == val) {
                    setBarMarkers();

                    break;
                }
            } else {

                if (datatype == "array" || datatype == "dict") {
                    if (getPlistTextValue(lineText) == name) {
                        setBarMarkers();

                        break;
                    }
                }

                if (datatype == "bool") {

                    if (index.column() == 0 || index.column() == 1) {

                        if (getPlistTextValue(lineText) == name) {

                            QString strNext = ui->textEdit->document()->findBlockByNumber(i + 1).text().trimmed();
                            QString strBool = strNext.mid(1, strNext.length() - 3);

                            if (strBool == val) {
                                setBarMarkers();

                                break;
                            }
                        }
                    }

                    if (index.column() == 2) {

                        if (lineText.mid(1, lineText.length() - 3) == val) {

                            QString strPrevious = ui->textEdit->document()->findBlockByNumber(i - 1).text().trimmed();

                            if (getPlistTextValue(strPrevious) == name) {

                                setBarMarkers();

                                break;
                            }
                        }
                    }
                }

                if (datatype == "integer" || datatype == "string" || datatype == "data" || datatype == "date" || datatype == "real") {
                    if (index.column() == 0 || index.column() == 1) {

                        if (getPlistTextValue(lineText) == name) {

                            QString strNext = ui->textEdit->document()->findBlockByNumber(i + 1).text().trimmed();
                            if (getPlistTextValue(strNext) == val) {

                                setBarMarkers();

                                break;
                            }
                        }
                    }

                    if (index.column() == 2) {

                        if (getPlistTextValue(lineText) == val) {

                            QString strPrevious = ui->textEdit->document()->findBlockByNumber(i - 1).text().trimmed();

                            if (getPlistTextValue(strPrevious) == name) {

                                setBarMarkers();

                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    this->repaint();
}

void MainWindow::setBarMarkers()
{
    QList<QTextEdit::ExtraSelection> extraSelection;
    QTextEdit::ExtraSelection selection;
    QColor lineColor;
    lineColor.setRgb(0, 255, 0, 30);
    //lineColor = QColor(Qt::gray).lighter(150);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = ui->textEdit->textCursor();
    selection.cursor.clearSelection();
    //将刚设置的 selection追加到链表当中
    extraSelection.append(selection);
    ui->textEdit->setExtraSelections(extraSelection);

    QScrollBar* vsBar = new QScrollBar;
    vsBar = ui->textEdit->verticalScrollBar();
    int vPos = ui->textEdit->verticalScrollBar()->sliderPosition();
    if (vPos > ui->textEdit->height() / 3)
        vsBar->setSliderPosition(vPos + ui->textEdit->height() / 2);
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    //获取背景色
    QPalette pal = this->palette();
    QBrush brush = pal.window();
    int c_red = brush.color().red();

    if (c_red != red) {
        red = c_red;
        myHL = new MyHighLighter(ui->textEdit->document());
        myHL->rehighlight();
        ui->textEdit->repaint();

        //qDebug() << "repaint";
    }
}

void MainWindow::on_expandAction()
{

    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        QModelIndex index;
        index = tab->currentIndex();
        DomModel* model = tab->getModel();

        //QTreeView* treeView = new QTreeView;
        //treeView = (QTreeView*)tab->children().at(1);

        if (!tab->treeView->isExpanded(index)) {
            tab->treeView->expand(index);
            //tab->view_expand(index, model);

        } else if (tab->treeView->isExpanded(index)) {
            QModelIndex index1 = model->index(index.row(), 0, index.parent());
            tab->view_collapse(index1.parent(), model);
            //treeView->setExpanded(index1, false);
        }
    }
}

void MainWindow::on_collapseAction()
{

    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        QModelIndex index;
        index = tab->currentIndex();
        DomModel* model = tab->getModel();

        tab->view_collapse(index.parent(), model);
    }
}

void MainWindow::on_actionNewSibling()
{
    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();
        tab->on_actionNewSibling();
    }
}

void MainWindow::on_actionNewChild()
{
    if (tabWidget->hasTabs()) {
        EditorTab* tab = tabWidget->getCurentTab();
        tab->on_actionNewChild();
    }
}

void MainWindow::CheckUpdate()
{

    QNetworkRequest quest;
    quest.setUrl(QUrl("https://api.github.com/repos/ic005k/PlistEDPlus/releases/latest"));
    quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    QString str = reply->readAll();
    QMessageBox box;
    box.setText(str);
    //box.exec();
    //qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    parse_UpdateJSON(str);

    reply->deleteLater();
}

int MainWindow::parse_UpdateJSON(QString str)
{

    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(str.toUtf8(), &err_rpt);

    if (err_rpt.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "", tr("Network error!"));
        return -1;
    }
    if (root_Doc.isObject()) {
        QJsonObject root_Obj = root_Doc.object();

        QString macUrl, winUrl, linuxUrl;
        QVariantList list = root_Obj.value("assets").toArray().toVariantList();
        for (int i = 0; i < list.count(); i++) {
            QVariantMap map = list[i].toMap();
            QFileInfo file(map["name"].toString());
            if (file.suffix().toLower() == "zip")
                macUrl = map["browser_download_url"].toString();

            if (file.suffix().toLower() == "7z")
                winUrl = map["browser_download_url"].toString();

            if (file.suffix() == "AppImage")
                linuxUrl = map["browser_download_url"].toString();
        }

        QJsonObject PulseValue = root_Obj.value("assets").toObject();
        QString Verison = root_Obj.value("tag_name").toString();
        QString Url;
        if (mac)
            Url = macUrl;
        if (win)
            Url = winUrl;
        if (linuxOS)
            Url = linuxUrl;

        QString UpdateTime = root_Obj.value("published_at").toString();
        QString ReleaseNote = root_Obj.value("body").toString();

        if (Verison > CurVerison) {
            QString warningStr = tr("New version detected!") + "\n" + tr("Version: ") + "V" + Verison + "\n" + tr("Published at: ") + UpdateTime + "\n" + tr("Release Notes: ") + "\n" + ReleaseNote;
            int ret = QMessageBox::warning(this, "", warningStr, tr("Download"), tr("Cancel"));
            if (ret == 0) {
                QDesktopServices::openUrl(QUrl(Url));
            }
        } else
            QMessageBox::information(this, "", tr("It is currently the latest version!"));
    }
    return 0;
}

void MainWindow::on_actionSort()
{
    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        QModelIndex index = tab->currentIndex();
        if (!index.isValid())
            return;

        DomModel* model = tab->getModel();

        //QSortFilterProxyModel* sortModel = new QSortFilterProxyModel(this);
        //sortModel->setSourceModel(model);
        //QTreeView* treeView = new QTreeView;
        //treeView = (QTreeView*)tab->children().at(1);
        //treeView->setModel(sortModel);

        model->sort(0, Qt::AscendingOrder);
    }
}

void MainWindow::on_NewWindow()
{

    QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
    QFile file(qfile);
    QSettings Reg(qfile, QSettings::IniFormat);

    Reg.setValue("drag", true);
    windowX = windowX + 25;
    windowY = windowY + 50;

    QScreen* screen = QGuiApplication::primaryScreen();
    int w = screen->size().width();

    if (windowX > w + this->width()) {
        windowX = 0;
        windowY = 0;
    }

    Reg.setValue("x", windowX);
    Reg.setValue("y", windowY);

    QFileInfo appInfo(qApp->applicationDirPath());
    QString pathSource = appInfo.filePath() + "/PlistEDPlus";
    QStringList arguments;
    QString fn = "";
    arguments << fn;
    QProcess* process = new QProcess;
    process->setEnvironment(process->environment());
    process->start(pathSource, arguments);
}

void MainWindow::on_copyBW()
{
    if (tabWidget->hasTabs()) {

        this->repaint();

        int ci = tabWidget->currentIndex();
        on_copyAction();
        actionNew_activated();
        on_actionNewChild();
        QModelIndex index = tabWidget->getCurentTab()->currentIndex();
        QModelIndex index1 = tabWidget->getCurentTab()->getModel()->index(0, 0, index);
        tabWidget->getCurentTab()->treeView->setCurrentIndex(index1);

        on_pasteAction();
        actionRemove_activated();

        QString fn = QDir::homePath() + "/.config/PlistEDPlus/temp.plist";
        savePlist(fn);

        tabWidget->getCurentTab()->setWindowModified(false);
        actionClose_activated();

        tabWidget->setCurrentIndex(ci);
        tabWidget->getCurentTab()->treeView->setFocus();

        this->repaint();
    }
}

void MainWindow::on_pasteBW()
{
    if (tabWidget->hasTabs()) {

        this->repaint();

        int ci = tabWidget->currentIndex();
        QString fn = QDir::homePath() + "/.config/PlistEDPlus/temp.plist";
        QFileInfo fi(fn);
        if (!fi.exists())
            return;
        openPlist(fn);

        QModelIndex index = tabWidget->getCurentTab()->currentIndex();
        QModelIndex index1 = tabWidget->getCurentTab()->getModel()->index(0, 0, index);
        if (index1.isValid())
            tabWidget->getCurentTab()->treeView->setCurrentIndex(index1);

        on_copyAction();
        actionClose_activated();

        tabWidget->setCurrentIndex(ci);
        tabWidget->getCurentTab()->treeView->setFocus();

        on_pasteAction();

        this->repaint();
    }
}

void MainWindow::loadText(QString textFile)
{

    QFileInfo fi(textFile);
    if (fi.exists()) {
        QFile file(textFile);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                tr("Cannot read file %1:\n%2.")
                    .arg(QDir::toNativeSeparators(fileName), file.errorString()));

        } else {

            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString text = in.readAll();
            ui->textEdit->setPlainText(text);
        }
    } else
        ui->textEdit->clear();
}

void MainWindow::on_actionShowPlistText_triggered(bool checked)
{
    if (checked)
        ui->dockWidget->setVisible(true);
    else
        ui->dockWidget->setVisible(false);
}

void MainWindow::on_actionPaste_as_child_triggered()
{
    if (tabWidget->hasTabs())
        tabWidget->getCurentTab()->on_pasteAsChildAction();
}

void MainWindow::on_editFind_returnPressed()
{
    on_Find();

    QString str = ui->editFind->text().trimmed();
    bool re = false;
    for (int i = 0; i < FindTextList.count(); i++) {
        if (FindTextList.at(i) == str) {
            re = true;
            break;
        }
    }

    if (!re && str != "")
        FindTextList.insert(0, str);

    QCompleter* editFindCompleter = new QCompleter(FindTextList, this);
    editFindCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editFindCompleter->setCompletionMode(QCompleter::InlineCompletion);
    ui->editFind->setCompleter(editFindCompleter);

    //更新动作
    btnFindMenu->clear();
    btnFindMenu->addAction(actCaseSensitive);
    btnFindMenu->addSeparator();
    btnFindMenu->addAction(actClearList);
    QAction* actTotal = new QAction(tr("Total") + " : " + QString::number(FindTextList.count()), this);
    btnFindMenu->addAction(actTotal);
    btnFindMenu->addSeparator();

    btnFindActionList.clear();

    for (int i = 0; i < FindTextList.count(); i++) {

        QString strList = FindTextList.at(i);

        btnFindActionList.append(new QAction(strList, this));
        btnFindMenu->addAction(btnFindActionList.at(i));

        connect(btnFindActionList.at(i), &QAction::triggered, [=]() {
            ui->editFind->setText(btnFindActionList.at(i)->text());
            on_btnFind_clicked();
        });
    }
}

void MainWindow::on_editFind_textChanged(const QString& arg1)
{
    if (tabWidget->hasTabs()) {

        if (arg1 != "") {

            //on_Find();
        }

        if (arg1 == "" || !find) {
            findCount = 0;
            lblFindCount->setText("  " + QString::number(findCount) + "  ");

            EditorTab* tab = tabWidget->getCurentTab();
            //QTreeView* treeView = new QTreeView;
            //treeView = (QTreeView*)tab->children().at(1);
            QModelIndex index = tab->currentIndex();
            tab->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Clear);
        }
    }
}

void MainWindow::on_ShowFindReplace()
{

    on_Find();
}

void MainWindow::on_btnFind_clicked()
{
    on_Find();
}

void MainWindow::on_btnHideFind_clicked()
{
    ui->dockWidgetFindReplace->close();
}

void MainWindow::on_btnPrevious_clicked()
{
    if (ui->editFind->text() == "" || indexFindList.count() == 0)
        return;

    if (findCount == 0)
        return;

    if (tabWidget->hasTabs()) {

        indexCount--;

        if (indexCount < 0) {
            indexCount = 0;
        }

        lblFindCount->setText("  " + QString::number(findCount) + " << " + QString::number(indexCount + 1) + "  ");

        EditorTab* tab = tabWidget->getCurentTab();
        DomModel* model = tab->getModel();
        QModelIndex index = model->index(0, 0);

        QModelIndex index0 = indexFindList.at(indexCount);
        index = model->index(index0.row(), index0.column(), index0.parent());
        tab->treeView->setCurrentIndex(index);

        tab->treeView->selectionModel()->setCurrentIndex(indexFindList.at(indexCount), QItemSelectionModel::SelectCurrent);
        tab->treeView->setFocus();

        DomItem* item = model->itemForIndex(index);
        QString name, val;
        name = item->getName();
        val = item->getValue();
        //qDebug() << name << val;

        oneReplace = false;
        ui->btnReplace->setEnabled(true);
    }
}

void MainWindow::on_btnNext_clicked()
{
    if (ui->editFind->text() == "" || indexFindList.count() == 0)
        return;

    if (findCount == 0)
        return;

    if (tabWidget->hasTabs()) {

        indexCount++;

        if (indexCount >= indexFindList.count()) {
            indexCount = indexFindList.count() - 1;
        }

        lblFindCount->setText("  " + QString::number(indexCount + 1) + " >> " + QString::number(findCount) + "  ");

        EditorTab* tab = tabWidget->getCurentTab();
        DomModel* model = tab->getModel();
        QModelIndex index = model->index(0, 0);

        QModelIndex index0 = indexFindList.at(indexCount);
        index = model->index(index0.row(), index0.column(), index0.parent());
        tab->treeView->setCurrentIndex(index);

        tab->treeView->selectionModel()->setCurrentIndex(indexFindList.at(indexCount), QItemSelectionModel::SelectCurrent);
        tab->treeView->setFocus();

        DomItem* item = model->itemForIndex(index);
        QString name, val;
        name = item->getName();
        val = item->getValue();
        //qDebug() << name << val;

        oneReplace = false;
        ui->btnReplace->setEnabled(true);
    }
}

void MainWindow::on_btnReplace_clicked()
{

    on_btnShowReplace_clicked();

    if (ui->editReplace->text().trimmed() == "" || indexFindList.count() == 0)
        return;

    if (ui->editReplace->text().trimmed().mid(0, 4).toLower() == "item" || ui->editFind->text().trimmed().mid(0, 4).toLower() == "item")
        return;

    if (tabWidget->hasTabs() && !oneReplace) {

        EditorTab* tab = tabWidget->getCurentTab();

        DomModel* model = tab->getModel();
        QString strFind = ui->editFind->text().trimmed();
        QString strReplace = ui->editReplace->text().trimmed();

        QString str = model->data(indexFindList.at(indexCount), Qt::DisplayRole).toString();

        QModelIndex index = tab->treeView->currentIndex();
        DomItem* item = model->itemForIndex(index);
        QString name, val;
        name = item->getName();
        val = item->getValue();

        QString newStr;
        QString strModify = str;
        if (!actCaseSensitive->isChecked()) {

            newStr = strModify.replace(strFind.toLower(), strReplace, Qt::CaseInsensitive);
        } else
            newStr = strModify.replace(strFind, strReplace);

        //qDebug() << str << name << val << newStr;

        if (str == name) {

            QModelIndex index_m = model->index(index.row(), 0, index.parent());
            tab->editorDataAboutToBeSet(index_m, newStr);

            item->setName(newStr);

            tab->treeView->doItemsLayout();
            tab->treeView->setFocus();

            indexFindList.remove(indexCount);
            findCount = indexFindList.count();
            indexCount--;
            oneReplace = true;
        }

        if (str == val) {

            QModelIndex index_m = model->index(index.row(), 2, index.parent());
            tab->editorDataAboutToBeSet(index_m, newStr);

            item->setValue(newStr);

            tab->treeView->doItemsLayout();
            tab->treeView->setFocus();

            indexFindList.remove(indexCount);
            findCount = indexFindList.count();
            indexCount--;
            oneReplace = true;
        }
    }
}

void MainWindow::on_btnReplaceAll_clicked()
{
    if (ui->editReplace->text().trimmed() == "" || ui->editFind->text().trimmed() == "")
        return;

    if (ui->editReplace->text().trimmed().mid(0, 4).toLower() == "item" || ui->editFind->text().trimmed().mid(0, 4).toLower() == "item")
        return;

    on_Find();

    for (int i = 0; i < indexFindList.count(); i++) {

        if (tabWidget->hasTabs()) {

            EditorTab* tab = tabWidget->getCurentTab();
            DomModel* model = tab->getModel();

            QModelIndex index0 = indexFindList.at(i);
            QModelIndex index = model->index(index0.row(), index0.column(), index0.parent());
            tab->treeView->setCurrentIndex(index);

            tab->treeView->selectionModel()->setCurrentIndex(indexFindList.at(i), QItemSelectionModel::SelectCurrent);
            tab->treeView->setFocus();

            QString strFind = ui->editFind->text().trimmed();
            QString strReplace = ui->editReplace->text().trimmed();
            QString str = model->data(indexFindList.at(i), Qt::DisplayRole).toString();

            DomItem* item = model->itemForIndex(index);
            QString name, val;
            name = item->getName();
            val = item->getValue();

            QString newStr;
            QString strModify = str;
            if (!actCaseSensitive->isChecked()) {
                newStr = strModify.replace(strFind.toLower(), strReplace, Qt::CaseInsensitive);
            } else
                newStr = strModify.replace(strFind, strReplace);

            //qDebug() << name << val << newStr;

            if (str == name) {

                QModelIndex index_m = model->index(index.row(), 0, index.parent());
                tab->editorDataAboutToBeSet(index_m, newStr);

                item->setName(newStr);

                tab->treeView->doItemsLayout();
                tab->treeView->setFocus();
            }

            if (str == val) {

                QModelIndex index_m = model->index(index.row(), 2, index.parent());
                tab->editorDataAboutToBeSet(index_m, newStr);

                item->setValue(newStr);

                tab->treeView->doItemsLayout();
                tab->treeView->setFocus();
            }
        }
    }

    indexFindList.clear();
    findCount = 0;
    lblFindCount->setText("  " + QString::number(findCount) + "  ");

    ui->btnPrevious->setEnabled(false);
    ui->btnNext->setEnabled(false);
}

void MainWindow::on_actionFind_triggered()
{
    on_ShowFindReplace();
}

void MainWindow::on_actionFindNext_triggered()
{
    on_btnNext_clicked();
}

void MainWindow::on_actionFindPrevious_triggered()
{
    on_btnPrevious_clicked();
}

void MainWindow::on_actionReplace_triggered()
{
    on_btnReplace_clicked();
}

void MainWindow::on_actionReplaceAll_triggered()
{
    on_btnReplaceAll_clicked();
}

void MainWindow::on_btnShowReplace_clicked()
{
    ui->dockWidgetFindReplace->show();
    ui->editReplace->setFocus();
}
