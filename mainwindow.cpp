#include "mainwindow.h"
#include "filesystemwatcher.h"
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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    myToolBar = ui->mainToolBar;
    myStatusBar = ui->statusBar;

    tabWidget = new EditorTabsWidget(this);

    ui->centralWidget->layout()->addWidget(tabWidget);

    QApplication::setApplicationName("PlistEDPlus");
    QApplication::setOrganizationName("PlistED");

    CurVerison = "1.0.23";
    ver = "PlistEDPlus  V" + CurVerison + "        ";
    setWindowTitle(ver);

    QDir dir;
    if (dir.mkpath(QDir::homePath() + "/.config/PlistEDPlus/")) { }

    //获取背景色
    QPalette pal = this->palette();
    QBrush brush = pal.window();
    red = brush.color().red();

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

    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequest(int)));

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::actionOpen_activated);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::actionNew_activated);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::actionSave_activated);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::actionSave_as_activated);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::actionClose_activated);
    connect(ui->actionClose_all, &QAction::triggered, this, &MainWindow::actionClose_all_activated);

    connect(ui->actionCheck_Update, &QAction::triggered, this, &MainWindow::CheckUpdate);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::actionAbout_activated);

    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::actionAdd_activated);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::actionRemove_activated);
    connect(ui->actionExpand_all, SIGNAL(triggered()), this, SLOT(actionExpand_all_activated()));

    updateRecentFiles();

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
    ui->actionNew_Sibling->setShortcut(tr("ctrl+="));

    actionNewChild = new QAction(tr("New Child"), this);
    ui->mainToolBar->addAction(actionNewChild);
    actionNewChild->setIcon(QIcon(":/new/toolbar/res/child.png"));
    connect(actionNewChild, &QAction::triggered, this, &MainWindow::on_actionNewChild);
    ui->actionNew_Child->setShortcut(tr("="));
    connect(ui->actionNew_Child, &QAction::triggered, this, &MainWindow::on_actionNewChild);

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

    ui->actionCopy->setShortcuts(QKeySequence::Copy);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::on_copyAction);

    ui->actionPaste->setShortcuts(QKeySequence::Paste);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::on_pasteAction);

    ui->actionCut->setShortcuts(QKeySequence::Cut);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::on_cutAction);

    actionUndo->setIcon(QIcon(":/new/toolbar/res/undo.png"));
    ui->mainToolBar->addAction(actionUndo);

    actionRedo->setIcon(QIcon(":/new/toolbar/res/redo.png"));
    ui->mainToolBar->addAction(actionRedo);

    ui->mainToolBar->addSeparator();

    lblFindCount = new QLabel(tr("Count"));
    ui->mainToolBar->addWidget(lblFindCount);
    findEdit = new QLineEdit(this);
    findEdit->setClearButtonEnabled(true);
    findEdit->setPlaceholderText(tr("Find"));
    ui->mainToolBar->addWidget(findEdit);
    connect(findEdit, &QLineEdit::returnPressed, this, &MainWindow::findEdit_returnPressed);
    connect(findEdit, &QLineEdit::textChanged, this, &MainWindow::findEdit_textChanged);

    QAction* findAction = new QAction(QIcon(":/new/toolbar/res/find.png"), tr(""), this);
    findAction->setToolTip(tr("Find"));
    ui->mainToolBar->addAction(findAction);
    connect(findAction, &QAction::triggered, this, &MainWindow::on_Find);

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
    ui->actionNew_Child->setShortcut(tr("alt+="));
#endif

#ifdef Q_OS_LINUX
    linuxOS = true;
#endif

#ifdef Q_OS_MAC
    ui->mainToolBar->setIconSize(QSize(28, 28));
    this->resize(QSize(1050, 600));
    mac = true;
    ui->actionCheck_Update->setVisible(true);
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
    }

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
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
    QTreeView* treeView = new QTreeView;
    treeView = (QTreeView*)tab->children().at(1);
    treeView->setCurrentIndex(model->index(0, 0)); //设置当前索引

    treeView->setFocus();
}

void MainWindow::actionOpen_activated()
{
    openFiles();
}

void MainWindow::actionClose_activated()
{
    if (tabWidget->hasTabs())
        onTabCloseRequest();
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

    for (int i = 0; i < list.size(); ++i)
        openPlist(list[i]);
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

        setRecentFiles(filePath);
        updateRecentFiles();

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
        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);
        treeView->resizeColumnToContents(0);

        treeView->setCurrentIndex(tab->getModel()->index(0, 0));
        treeView->setFocus();

        QFileInfo fi(filePath);
        tabWidget->tabBar()->setTabToolTip(tabWidget->currentIndex(), fi.fileName());
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

        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);

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

        action->setText(text);
        action->setVisible(true);
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

    if (findEdit->text() == "")
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

        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);

        //treeView->collapseAll();
        treeView->expandToDepth(0);

        index = model->index(0, 0);
        treeView->setCurrentIndex(index); //设置当前索引
        treeView->setFocus();

        findCount = 0;
        lblFindCount->setText("  " + QString::number(findCount) + "  ");
        find = false;

        if (index.isValid()) {
            forEach(model, index, findEdit->text().trimmed());
        } else
            qDebug() << "index is no valid";
    }
}

void MainWindow::forEach(QAbstractItemModel* model, QModelIndex parent, QString str)
{

    for (int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index2 = model->index(r, 2, parent);
        QString value = model->data(index2, Qt::DisplayRole).toString();

        QModelIndex index = model->index(r, 0, parent);
        //QVariant name = model->data(index);
        QString name = model->data(index, Qt::DisplayRole).toString();

        //搜索值
        if (value.toLower().contains(str.trimmed().toLower()) && str.trimmed() != "") {

            EditorTab* tab = tabWidget->getCurentTab();
            //DomModel* model = tab->getModel();

            QTreeView* treeView = new QTreeView;
            treeView = (QTreeView*)tab->children().at(1);

            treeView->selectionModel()->setCurrentIndex(index2, QItemSelectionModel::Select);
            //treeView->selectionModel()->setCurrentIndex(index2, QItemSelectionModel::SelectCurrent);
            findCount++;
            lblFindCount->setText("  " + QString::number(findCount) + "  ");
            find = true;

            actionSort->setEnabled(false);

            //treeView->expand(index2);
            //tab->view_expand(index2, model);
        }
        //搜索键
        if (name.toLower().contains(str.trimmed().toLower()) && str.trimmed() != "") {

            EditorTab* tab = tabWidget->getCurentTab();
            //DomModel* model = tab->getModel();
            QTreeView* treeView = new QTreeView;
            treeView = (QTreeView*)tab->children().at(1);

            treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
            //treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
            findCount++;
            lblFindCount->setText("  " + QString::number(findCount) + "  ");
            find = true;

            actionSort->setEnabled(false);

            //treeView->expand(index);
            //tab->view_expand(index, model);
        }

        if (model->hasChildren(index)) {
            forEach(model, index, str);
        }
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
            QTreeView* treeView = new QTreeView;
            treeView = (QTreeView*)tab->children().at(1);
            QModelIndex index = tab->currentIndex();
            treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Clear);
        }
    }
}

void MainWindow::findEdit_returnPressed()
{
    on_Find();
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

    //记录当前文件
    QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
    QFile file(qfile);
    QSettings Reg(qfile, QSettings::IniFormat);

    Reg.setValue("restore", ui->actionRestoreScene->isChecked());
    Reg.setValue("DefaultIcon", ui->actionDefaultNodeIcon->isChecked());
    Reg.setValue("SaveAndFind", ui->actionSaveAndFind->isChecked());

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
        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);
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
        treeView->setCurrentIndex(model->index(index_bak.row() - 1, 0, index.parent()));
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

        treeView->setCurrentIndex(model->index(index_bak.row() - 1, 0, index.parent()));
        showMsg();
    }
}
void MainWindow::on_actionMoveDown()
{
    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);
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
            treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
            on_pasteAction();
        }

        //倒数第二行
        if (index.row() == items->childCount() - 2) {

            QUndoCommand* addMoveCommand = new AddMoveCommand(tab->getModel(), index.parent());
            undoGroup->activeStack()->push(addMoveCommand);

            on_cutAction();
            treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
            on_pasteAction();
            treeView->setCurrentIndex(model->index(index_bak.row() + 2, 0, index.parent()));
            actionRemove_activated();
        }

        //之前的老方法
        /*ItemState* temp = model->saveItemState(index);
        int row = index.row() + 2;
        model->addMoveItem(index.parent(), row, temp);

        model->removeItem(index_bak);*/

        if (array)
            items->setType("array");

        treeView->setCurrentIndex(model->index(index_bak.row() + 1, 0, index.parent()));
        showMsg();
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

void MainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    //获取背景色
    QPalette pal = this->palette();
    QBrush brush = pal.window();
    int c_red = brush.color().red();
    if (c_red != red) {
        red = c_red;
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

        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);

        if (!treeView->isExpanded(index)) {
            treeView->expand(index);
            //tab->view_expand(index, model);

        } else if (treeView->isExpanded(index)) {
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

        QTreeView* treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);

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
