#include "mainwindow.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Plist.hpp"
#include "filesystemwatcher.h"
#include "myapp.h"
#include "mytreeview.h"
#include "ui_mainwindow.h"

#ifdef __APPLE__
#include "OSXHideTitleBar.h"
#endif

using namespace std;

#include <QMessageBox>
#include <QSettings>
#include <QUrl>

QString CurVerison = "1.2.20";

QStatusBar* myStatusBar;
QToolBar* myToolBar;

EditorTabsWidget* tabWidget;
ItemState* copy_state;

ItemState* AddMoveTemp;

QUndoGroup* undoGroup;

QString fileName;
QVector<QString> openFileList;

bool defaultIcon = false;
bool binPlistFile = false;
bool Save = false;

int red = 0;
int windowX = 0;
int windowY = 0;

extern bool loading;
extern QString strRootType, tabStyleLight;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  installEventFilter(this);

  ver = "PlistEDPlus  V" + CurVerison + "        ";
  ver = "";
  setWindowTitle("PlistEDPlus");
  ui->lblTitle->setText("PlistEDPlus");

  loading = true;

  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);

  // 获取背景色
  QPalette pal = this->palette();
  QBrush brush = pal.window();
  red = brush.color().red();

  this->resize(QSize(1200, 650));

#ifdef Q_OS_WIN32
  reg_win();
  win = true;
#endif

#ifdef Q_OS_MAC
  mac = true;
#if (QT_VERSION <= QT_VERSION_CHECK(5, 9, 9))
  osx1012 = true;
  mac = false;
#endif
#endif

#ifdef Q_OS_LINUX
  linuxOS = true;
#endif

  tabWidget = new EditorTabsWidget(this);
  tabWidget->setHidden(true);
  dlgAutoUpdate = new AutoUpdateDialog(this);
  plistTextEditor = new CodeEditor(this);
  plistTextEditor->setFont(getFont());
  plistTextEditor->setReadOnly(true);

  init_UIStyle();

  ui->mainToolBar->setStyleSheet(

      "QToolButton:hover{ "
      "color:rgb(255, 255, 255); "
      "border-style:solid; "
      "border-top-left-radius:2px;  "
      "border-top-right-radius:2px; "
      "background:#bfbfbf; "
      "border:1px;"
      "border-radius:5px;padding:2px 4px; }"

      "QToolButton:pressed{ "
      "color:rgb(255, 255, 255); "
      "border-style:solid; "
      "border-top-left-radius:2px;  "
      "border-top-right-radius:2px; "
      "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 "
      "rgb(226,236,241),"
      "stop: 0.3 rgb(190,190,190),"
      "stop: 1 rgb(160,160,160));"
      "border:1px;"
      "border-radius:5px;padding:2px 4px; }"

  );
  myToolBar = ui->mainToolBar;
  myStatusBar = ui->statusBar;

  lblStaInfo0 = new QLabel(this);
  lblStaInfo1 = new QLabel(this);
  lblStaInfo0->setStyleSheet(
      "QLabel { background-color : lightblue; color : black; }");
  lblStaInfo1->setStyleSheet(
      "QLabel { background-color : yellow; color : black; }");
  lblStaInfo2 = new QLabel(this);
  lblStaInfo2->setStyleSheet(
      "QLabel { background-color : lightgreen; color : black; }");
  myStatusBar->addPermanentWidget(lblStaInfo0);
  myStatusBar->addPermanentWidget(lblStaInfo2);
  myStatusBar->addPermanentWidget(lblStaInfo1);
  lblStaInfo0->setHidden(true);
  lblStaInfo1->setHidden(true);
  lblStaInfo2->setHidden(true);

  QSplitter* splitter1 = new QSplitter(Qt::Vertical, this);
  ui->frameMain->layout()->addWidget(ui->frameTip);
  ui->frameMain->layout()->addWidget(ui->frameFind);
  ui->frameMain->layout()->addWidget(tabWidget);
  ui->frameMain->layout()->addWidget(ui->frameData);
  splitter1->addWidget(ui->frameMain);
  splitter1->addWidget(plistTextEditor);
  ui->centralWidget->layout()->addWidget(splitter1);
  int h0 = Reg.value("frameMainHeight", 350).toInt();
  int h1 = Reg.value("PlistTextDockHeight", 100).toInt();
  if (h0 < 150) h0 = 150;
  if (h1 < 100) h1 = 100;
  QList<int> list;
  list.append(h0);
  list.append(h1);
  splitter1->setSizes(list);

  QSplitter* splitterMain = new QSplitter(Qt::Horizontal, this);
  splitterMain->addWidget(splitter1);
  splitterMain->addWidget(ui->listFind_2);
  ui->centralWidget->layout()->addWidget(splitterMain);
  h0 = Reg.value("frameMainWidth", 500).toInt();
  h1 = Reg.value("dockFindWidth", 100).toInt();
  if (h0 < 150) h0 = 150;
  if (h1 < 100) h1 = 100;
  list.clear();
  list.append(h0);
  list.append(h1);
  splitterMain->setSizes(list);

  ui->frameTitle->setHidden(true);

  QApplication::setApplicationName("PlistEDPlus");
  QApplication::setOrganizationName("PlistED");

  QDir dir;
  if (dir.mkpath(QDir::homePath() + "/.config/PlistEDPlus/")) {
  }

  // 初始化文件被修改后的提示
  ui->frameTip->setAutoFillBackground(true);
  ui->frameTip->setPalette(QPalette(QColor(255, 204, 204)));
  ui->btnYes->setDefault(true);
  ui->frameTip->setHidden(true);

  // ui->frameData->setAutoFillBackground(true);
  // ui->frameData->setPalette(QPalette(QColor(250, 250, 224)));
  ui->frameData->setHidden(true);
  // ui->centralWidget->layout()->addWidget(ui->frameData);

  initFindReplace();

  initMenuToolsBar();

  init_iniData();

  manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(replyFinished(QNetworkReply*)));
  blAutoCheckUpdate = true;
  ui->actionAutoUpdateCheck->setChecked(
      Reg.value("AutoUpdateCheck", 1).toBool());
  if (ui->actionAutoUpdateCheck->isChecked()) CheckUpdate();
  readINIProxy();

  initPlistTextShow();

  loading = false;
}

MainWindow::~MainWindow() {
  delete undoGroup;
  delete ui;
}

void MainWindow::init_iniData() {
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);

  defaultIcon = Reg.value("DefaultIcon").toBool();
  ui->actionDefaultNodeIcon->setChecked(defaultIcon);

  ui->actionExpandAllOpenFile->setChecked(Reg.value("ExpAll").toBool());

  bool restore = Reg.value("restore", 1).toBool();
  ui->actionRestoreScene->setChecked(restore);
  restore = true;
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
    if (x > w - 300) x = w - 300;
    if (y > h - 300) y = h - 300;

    this->setGeometry(x, y, this->width(), this->height());
  }

  int count = Reg.value("FindTextListTotal").toInt();

  btnFindMenu->addSeparator();
  btnFindMenu->addAction(actClearList);
  QAction* actTotal =
      new QAction(tr("Total") + " : " + QString::number(count), this);
  btnFindMenu->addAction(actTotal);
  btnFindMenu->addSeparator();

  for (int i = 0; i < count; i++) {
    QString strList = Reg.value("FindTextList" + QString::number(i)).toString();
    FindTextList.append(strList);

    btnFindActionList.append(new QAction(strList, this));
    btnFindMenu->addAction(btnFindActionList.at(i));

    connect(btnFindActionList.at(i), &QAction::triggered, [=]() {
      ui->editFind->setText(btnFindActionList.at(i)->text());
      on_btnFind_clicked();
    });
  }

  QCompleter* editFindCompleter = new QCompleter(FindTextList, this);
  editFindCompleter->setCaseSensitivity(Qt::CaseSensitive);
  editFindCompleter->setCompletionMode(QCompleter::InlineCompletion);
  ui->editFind->setCompleter(editFindCompleter);

  //是否显示plist文本
  ui->actionShowPlistText->setChecked(Reg.value("ShowPlistText", 0).toBool());
  if (ui->actionShowPlistText->isChecked())
    plistTextEditor->setHidden(false);
  else
    plistTextEditor->setHidden(true);

  // 主窗口位置和大小
  int x, y, width, height;
  x = Reg.value("x", 0).toInt();
  y = Reg.value("y", 0).toInt();
  width = Reg.value("width", 1200).toInt();
  height = Reg.value("height", 600).toInt();
  if (x < 0) {
    width = width + x;
    x = 0;
  }
  if (y < 0) {
    height = height + y;
    y = 0;
  }
  QRect rect(x, y, width, height);
  move(rect.topLeft());
  resize(rect.size());
}

void MainWindow::initMenuToolsBar() {
  ui->mainToolBar->setHidden(true);
  // if (mac || osx1012) this->setUnifiedTitleAndToolBarOnMac(true);
  ui->menuBar->setMouseTracking(true);
  ui->actionDiscussion_Forum->setVisible(false);
  ui->actionRestoreScene->setVisible(false);
  ui->btnFind_Tool->setIcon(QIcon(":/new/toolbar/res/find.png"));
  ui->btnFind->setIcon(QIcon(":/new/toolbar/res/find.png"));
  ui->btnNew->setIcon(QIcon(":/new/toolbar/res/new.png"));
  ui->btnSave->setIcon(QIcon(":/new/toolbar/res/save.png"));
  ui->btnBrother->setIcon(QIcon(":/new/toolbar/res/sibling.png"));
  ui->btnChild->setIcon(QIcon(":/new/toolbar/res/child.png"));
  ui->btnDel->setIcon(QIcon(":/new/toolbar/res/del.png"));
  ui->btnSort->setIcon(QIcon(":/new/toolbar/res/sort.png"));
  ui->actionCollapse_All->setVisible(false);
  ui->btnBrother->setHidden(true);
  ui->btnChild->setHidden(true);
  ui->btnExpand->setHidden(true);
  ui->btnDel->setHidden(true);
  ui->btnSort->setHidden(true);
  QFont font;
  font.setBold(true);
  ui->lblTitle->setFont(font);

  ui->actionAbout->setMenuRole(QAction::AboutRole);
  ui->mainToolBar->setIconSize(QSize(26, 26));
  ui->mainToolBar->layout()->setMargin(1);
  ui->mainToolBar->layout()->setSpacing(1);

  ui->cboxFileType->setToolTip(tr("Select the file storage format"));
  ui->cboxFileType->addItem("XML");
  ui->cboxFileType->addItem("BIN");

  // create undo and redo actions
  undoGroup = new QUndoGroup(this);
  actionUndo = undoGroup->createUndoAction(this, tr("Undo"));
  actionRedo = undoGroup->createRedoAction(this, tr("Redo"));
  actionUndo->setIconVisibleInMenu(false);
  actionRedo->setIconVisibleInMenu(false);

  // set shortcuts
  actionUndo->setShortcuts(QKeySequence::Undo);
  actionRedo->setShortcuts(QKeySequence::Redo);

  // add actions to menu
  ui->menuEdit->addAction(actionUndo);
  ui->menuEdit->addAction(actionRedo);

  connect(tabWidget, SIGNAL(currentChanged(int)), this,
          SLOT(onTabWidget_currentChanged(int)));

  connect(undoGroup, SIGNAL(cleanChanged(bool)), this,
          SLOT(onCleanChanged(bool)));

  // File
  connect(ui->actionClose, SIGNAL(triggered()), this,
          SLOT(actionClose_activated()));
  connect(ui->actionClose_all, SIGNAL(triggered()), this,
          SLOT(actionClose_all_activated()));

  connect(tabWidget, SIGNAL(tabCloseRequested(int)), this,
          SLOT(on_TabCloseRequest(int)));

  connect(ui->actionNew_Window, SIGNAL(triggered()), this,
          SLOT(on_NewWindow()));
  ui->actionNew_Window->setShortcut(tr("ctrl+alt+n"));
  if (mac || osx1012) ui->actionNew_Window->setIconVisibleInMenu(false);

  // Quit
  ui->actionQuit->setMenuRole(QAction::QuitRole);

  // Edit
  ui->actionCopy->setShortcuts(QKeySequence::Copy);
  connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(on_copyAction()));

  ui->actionPaste->setShortcuts(QKeySequence::Paste);
  connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(on_pasteAction()));

  ui->actionPaste_as_child->setShortcut(tr("shift+ctrl+v"));

  ui->actionCut->setShortcuts(QKeySequence::Cut);

  ui->actionCopy_between_windows->setShortcut(tr("ctrl+b"));
  ui->actionPaste_between_windows->setShortcut(tr("ctrl+alt+b"));

  // 编辑菜单 展开/折叠
  ui->menuEdit->addSeparator();
  QAction* expandAction =
      new QAction(tr("Expand") + "/" + tr("Collapse"), this);
  expandAction->setShortcut(tr("space"));
  ui->menuEdit->addAction(expandAction);
  connect(expandAction, &QAction::triggered, this,
          &MainWindow::on_expandAction);

  QAction* collapseAction = new QAction(tr("Collapse"), this);
  connect(collapseAction, &QAction::triggered, this,
          &MainWindow::on_collapseAction);

  //最近打开的文件快捷通道
  QToolButton* btn0 = new QToolButton(this);
  btn0->setToolTip(tr("Open Recent..."));
  btn0->setIcon(QIcon(":/new/toolbar/res/rp.svg"));
  btn0->setPopupMode(QToolButton::InstantPopup);
  ui->mainToolBar->addWidget(btn0);
  reFileMenu = new QMenu(this);
  btn0->setMenu(reFileMenu);
  QAction* actRecentlyOpen = new QAction;
  actRecentlyOpen->setText(tr("Recently Open"));
  ui->menuFile->insertAction(ui->actionSave, actRecentlyOpen);
  actRecentlyOpen->setMenu(reFileMenu);

  //最近打开的文件
  QCoreApplication::setOrganizationName("ic005k");
  QCoreApplication::setOrganizationDomain("github.com/ic005k");
  QCoreApplication::setApplicationName("PlistEdPlus");

  m_recentFiles = new RecentFiles(this);
  // m_recentFiles->attachToMenuAfterItem(ui->menuFile, tr("Save as"),
  // SLOT(recentOpen(QString)));
  m_recentFiles->setNumOfRecentFiles(10);

  // 最近打开的文件快捷通道
  initRecentFilesForToolBar();

  // 增加同级项
  ui->actionNew_Sibling->setShortcut(tr("+"));

  // 增加子项
  connect(ui->actionNew_Child, &QAction::triggered, this,
          &MainWindow::on_actionNewChild);
  ui->actionNew_Child->setShortcut(tr("ctrl++"));

  // 删除
  ui->actionRemove_2->setShortcut(Qt::Key_Delete);
  ui->actionRemove_2->setShortcut(tr("-"));
  connect(ui->actionRemove_2, &QAction::triggered, this,
          &MainWindow::actionRemove_activated);

  // 全部展开与折叠
  // ui->mainToolBar->addAction(ui->actionExpand_all);
  ui->actionExpand_all->setIcon(QIcon(":/new/toolbar/res/exp.svg"));

  connect(ui->actionExpand_all, SIGNAL(triggered()), this,
          SLOT(actionExpand_all_activated()));

  // ui->mainToolBar->addSeparator();
  //条目上移
  QAction* actionMoveUp = new QAction(tr("Move up"));
  actionMoveUp->setIcon(QIcon(":/new/toolbar/res/up.svg"));
  // ui->mainToolBar->addAction(actionMoveUp);
  connect(actionMoveUp, &QAction::triggered, this,
          &MainWindow::on_actionMoveUp);

  // 条目下移
  QAction* actionMoveDown = new QAction(tr("Move down"));
  actionMoveDown->setIcon(QIcon(":/new/toolbar/res/down.svg"));
  connect(actionMoveDown, &QAction::triggered, this,
          &MainWindow::on_actionMoveDown);

  // Undo、Redo
  actionUndo->setIcon(QIcon(":/new/toolbar/res/undo.svg"));
  actionRedo->setIcon(QIcon(":/new/toolbar/res/redo.svg"));

  QAction* findAction =
      new QAction(QIcon(":/new/toolbar/res/find.svg"), tr(""), this);
  findAction->setToolTip(tr("Find and Replace"));
  findAction->setShortcut(tr("ctrl+F"));
  connect(findAction, &QAction::triggered, this,
          &MainWindow::on_ShowFindReplace);

  // 搜索框
  ui->editFind->setClearButtonEnabled(true);
  ui->editFind->setPlaceholderText(tr("Find"));
  ui->editReplace->setClearButtonEnabled(true);
  ui->editReplace->setPlaceholderText(tr("Replace"));

  ui->mainToolBar->setContextMenuPolicy(
      Qt::CustomContextMenu);  //屏蔽默认的右键菜单
  ui->editFind->setMinimumWidth(150);

  //设置下拉菜单
  actCaseSensitive->setCheckable(true);
  btnFindMenu = new QMenu(this);
  btnFindMenu->addAction(actCaseSensitive);

  ui->btnFind->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->btnFind, &QPushButton::customContextMenuRequested,
          [=](const QPoint& pos) {
            Q_UNUSED(pos);
            btnFindMenu->exec(QCursor::pos());
          });

  connect(actClearList, &QAction::triggered, [=]() {
    ui->editFind->setText("");
    FindTextList.clear();
    on_editFind_returnPressed();
  });

  ui->mainToolBar->addWidget(ui->btnShowReplace);
  ui->btnMisc->setVisible(false);
}

void MainWindow::recentOpen(QString filename) { openPlist(filename); }

void MainWindow::actionNew() {
  ui->cboxFileType->setCurrentIndex(0);
  binPlistFile = false;

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

  tab->treeView->setCurrentIndex(model->index(0, 0));  //设置当前索引

  tab->treeView->setFocus();

  plistTextEditor->clear();
  tabWidget->setHidden(false);
  loading = false;
}

void MainWindow::actionClose_activated() {
  if (tabWidget->hasTabs()) {
    on_TabCloseRequest();
  }
}

void MainWindow::actionClose_all_activated() {
  if (tabWidget->hasTabs()) {
    tabWidget->closeAllTabs();
    ui->btnHideFind->click();
  }
}

void MainWindow::openFiles(QStringList list) {
  if (list.isEmpty()) {
    list = QFileDialog::getOpenFileNames(
        this, tr("Select files to open"), "",
        "Property list (*.plist);;All files(*.*)");
  }

  for (int i = 0; i < list.size(); ++i) {
    openPlist(list[i]);
  }
}

void MainWindow::openPlist(QString filePath) {
  tabWidget->setHidden(false);
  ui->listFind_2->clear();  // 否则会导致App崩溃
  removeWatchFiles();

  map<string, boost::any> dict;
  string baseName;
  QString path;
  QDomDocument document;
  QString strConfigDir = QDir::homePath() + "/.config/PlistEDPlus";
  QString fn = QDir::homePath() + "/.config/PlistEDPlus/temp.plist";

  if (filePath == fn) {
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly)) {
      if (document.setContent(&file)) {
        DomModel* model = DomParser::fromDom(document);

        tabWidget->createTab(model, filePath);
      }
    }

    file.close();
  }

  QFileInfo fi(filePath);
  if (fi.exists() && filePath != fn) {
    filePath = QDir::fromNativeSeparators(filePath);
    binPlistFile = getBinPlist(filePath);

    path = fi.path();
    QDir dir;
    if (dir.exists(path)) {
      dir.setCurrent(path);
    }

    QString str = fi.fileName();
    baseName = string(str.toLocal8Bit());

    if (binPlistFile) {
      if (linuxOS) {
        /*try {
          Plist::readPlist(baseName.c_str(), dict);
        } catch (...) {
          QMessageBox box;
          box.setText(
              tr("There's something wrong with this file and it won't open!") +
              "\n\n[BIN]  " + filePath);
          box.exec();
          return;
        }

        if (dir.mkpath(strConfigDir)) {
        }
        dir.setCurrent(strConfigDir);
        Plist::writePlistXML("_temp.plist", dict);*/

        QFileInfo appInfo(qApp->applicationDirPath());
        QString strPath = appInfo.filePath();
        QString strSource = strConfigDir + "/_util.plist";
        QFile::remove(strSource);
        QFile::copy(filePath, strSource);
        QString strExec = strPath + "/plistutil-x86_64.AppImage";
        QString strTarget = strConfigDir + "/_temp.plist";
        QProcess::execute(
            strExec, QStringList() << "-i" << strSource << "-o" << strTarget);
        QFile::remove(strSource);
      }

      if (win) {
        QFileInfo appInfo(qApp->applicationDirPath());
        QString strPath = appInfo.filePath();
        QString strSource = strConfigDir + "/_util.plist";
        QFile::remove(strSource);
        QFile::copy(filePath, strSource);
        QString strExec = strPath + "/util/plistutil.exe";
        QString strTarget = strConfigDir + "/_temp.plist";
        QProcess::execute("cmd.exe", QStringList()
                                         << "/c" << strExec << "-i" << strSource
                                         << "-o" << strTarget);
        QFile::remove(strSource);
      }

      if (mac || osx1012) {
        QProcess::execute("plutil", QStringList()
                                        << "-convert"
                                        << "xml1" << filePath << "-o"
                                        << strConfigDir + "/_temp.plist");
      }

      ui->cboxFileType->setCurrentIndex(1);
    } else
      ui->cboxFileType->setCurrentIndex(0);

    QString strLoad;
    if (binPlistFile) {
      strLoad = strConfigDir + "/_temp.plist";
    } else {
      strLoad = filePath;
    }

    QFileInfo fiCheck(strLoad);
    QString pathCheck = fiCheck.path();
    QDir dirCheck(pathCheck);
    if (dirCheck.exists(pathCheck)) {
      dirCheck.setCurrent(pathCheck);
    }

    // string baseNameCheck = string(fiCheck.fileName().toLocal8Bit());
    try {
      // Plist::readPlist(baseNameCheck.c_str(), dict);  // 目前仅做测试使用
    } catch (...) {
    }

    QFile file(strLoad);
    if (file.open(QIODevice::ReadOnly)) {
      document.setContent(&file);
    }
    file.close();

    closeOpenedFile(filePath);

    // 确定dict或array
    QTextEdit* txtEdit = new QTextEdit;
    txtEdit->setPlainText(readText(strLoad));
    for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
      if (getTextEditLineText(txtEdit, i) == "<dict>") {
        strRootType = "dict";
        break;
      }
      if (getTextEditLineText(txtEdit, i) == "<array>") {
        strRootType = "array";
        break;
      }
    }

    DomModel* model;
    model = DomParser::fromDom(document);
    if (model == NULL) {
      QMessageBox box;
      box.setText(
          tr("There's something wrong with this file and it won't open!") +
          "\n\n[XML]  " + filePath);
      box.exec();
      return;
    }
    tabWidget->createTab(model, filePath);

    QString tempText = QDir::homePath() + "/.config/PlistEDPlus/tempText.plist";
    if (filePath != fn && filePath != tempText) {
      QSettings settings;
      QFileInfo fInfo(filePath);
      settings.setValue("currentDirectory", fInfo.absolutePath());
      // qDebug() << settings.fileName(); //最近打开的文件所保存的位置
      m_recentFiles->setMostRecentFile(filePath);
      initRecentFilesForToolBar();
    }

    EditorTab* tab = tabWidget->getCurentTab();
    if (tab->getModel()->index(0, 0).isValid()) {
      tab->treeView->setCurrentIndex(tab->getModel()->index(0, 0));
      showMsg();
    } else {
      return;
    }

    tab->treeView->resizeColumnToContents(0);  // 列宽自动适应最长的条目
    tab->treeView->setFocus();

    if (ui->actionExpandAllOpenFile->isChecked()) actionExpand_all_activated();

    if (tabWidget->currentIndex() >= 0)
      tabWidget->tabBar()->setTabToolTip(tabWidget->currentIndex(),
                                         fi.filePath());

    loadText(filePath);
    if (binPlistFile) QFile::remove(strLoad);
  }

  setOrgMD5(filePath);
  addWatchFiles();

  loading = false;
}

QString MainWindow::getTextEditLineText(QTextEdit* txtEdit, int i) {
  QTextBlock block = txtEdit->document()->findBlockByNumber(i);
  txtEdit->setTextCursor(QTextCursor(block));
  QString lineText = txtEdit->document()->findBlockByNumber(i).text().trimmed();
  return lineText;
}

QString MainWindow::readText(QString textFile) {
  QFileInfo fi(textFile);
  if (fi.exists()) {
    QFile file(textFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::warning(
          this, tr("Application"),
          tr("Cannot read file %1:\n%2.")
              .arg(QDir::toNativeSeparators(textFile), file.errorString()));

    } else {
      QTextStream in(&file);
      in.setCodec("UTF-8");
      QString text = in.readAll();
      return text;
    }
  }

  return "";
}

QDomDocument MainWindow::readXMLPlist(QDomDocument document, QString filePath) {
  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly)) {
    if (document.setContent(&file)) {
    }
  }
  file.close();

  return document;
}

bool MainWindow::getBinPlist(QString filePath) {
  QFile file(filePath);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(
        this, tr("Application"),
        tr("Cannot read file %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));

  } else {
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString text = in.readAll().trimmed();
    if (text.mid(0, 8) == "bplist00")
      return true;
    else
      return false;
  }

  return false;
}

void MainWindow::closeOpenedFile(QString file) {
  for (int i = 0; i < tabWidget->tabBar()->count(); i++) {
    if (file == tabWidget->getTab(i)->getPath()) {
      tabWidget->closeTab(i);
      break;
    }
  }
}

void MainWindow::removeWatchFiles() {
  for (int i = 0; i < openFileList.count(); i++) {
    FileSystemWatcher::removeWatchPath(openFileList.at(i));
  }
}

void MainWindow::addWatchFiles() {
  openFileList.clear();
  for (int i = 0; i < tabWidget->tabBar()->count(); i++) {
    QString file = tabWidget->getTab(i)->getPath();
    FileSystemWatcher::addWatchPath(file);
    openFileList.append(file);
  }
}

void MainWindow::on_TabCloseRequest(int i) {
  if (i != -1)
    tabWidget->setCurrentIndex(i);
  else
    return;

  QString fn = tabWidget->getCurentTab()->getFileName();

  if (!undoGroup->isClean()) {
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The document has been modified.") + "\n" + fn);
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
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
        actionSave();
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

  // close tab
  tabWidget->closeTab();

  if (tabWidget->currentIndex() != -1)
    tabWidget->getCurentTab()->treeView->setFocus();

  // 整体移除并添加
  if (!blExit) {
    removeWatchFiles();
    addWatchFiles();
  }
  if (tabWidget->tabBar()->count() == 0) {
    tabWidget->setHidden(true);
    lblStaInfo0->setHidden(true);
    lblStaInfo1->setHidden(true);
    lblStaInfo2->setHidden(true);
    setWindowTitle("PlistEDPlus");
    ui->lblTitle->setText("PlistEDPlus");
    plistTextEditor->clear();
    ui->btnHideFind->click();
  }
}

void MainWindow::savePlist(QString filePath) {
  removeWatchFiles();
  if (tabWidget->hasTabs()) {
    Save = true;

    QString fileTemp = QDir::homePath() + "/.config/PlistEDPlus/temp.plist";

    EditorTab* tab = tabWidget->getCurentTab();

    // get parsed dom doc
    QDomDocument doc = DomParser::toDom(tab->getModel());

    if (filePath == fileTemp) {
      QFile file(filePath);
      file.open(QIODevice::WriteOnly);
      QTextStream out(&file);
      out.setCodec("UTF-8");
      doc.save(out, 4, QDomNode::EncodingFromDocument);
      file.close();

    } else {  // 始终生成一个空文件，供另存使用
      QFile file(filePath);
      file.open(QIODevice::WriteOnly);
      file.close();
    }

    QFileInfo fi(filePath);
    if (fi.exists() && filePath != fileTemp) {
      map<string, boost::any> dict;

      QString path = fi.path();
      QDir dir;
      if (dir.exists(path)) dir.setCurrent(path);

      string baseName;
      QString str = fi.fileName();
      baseName = string(str.toLocal8Bit());
      // cout << baseName << endl;

      QString strData = doc.toString();
      std::string mystring = strData.toStdString();
      std::istringstream is(mystring);

      try {
        Plist::readPlist(is, dict);  // 兼容性较差
      } catch (...) {
      }

      tab->setPath(filePath);

      int index = tabWidget->indexOf(tab);

      QString name = tab->getFileName();

      // XML
      if (ui->cboxFileType->currentIndex() == 0) {
        if (useQtWriteXML) {
          QFile file(filePath);
          file.open(QIODevice::WriteOnly);
          QTextStream out(&file);
          out.setCodec("UTF-8");
          doc.save(out, 4, QDomNode::EncodingFromDocument);
          file.close();

        } else
          Plist::writePlistXML(baseName.c_str(), dict);

        tabWidget->setTabText(index, name);
      }

      // BIN
      if (ui->cboxFileType->currentIndex() == 1) {
        if (linuxOS) {
          // Plist::writePlistBinary(baseName.c_str(), dict);
          QString strConfigDir = QDir::homePath() + "/.config/PlistEDPlus";
          QString strSource = strConfigDir + "/_util.plist";

          QFile file(strSource);
          file.open(QIODevice::WriteOnly);
          QTextStream out(&file);
          out.setCodec("UTF-8");
          doc.save(out, 4, QDomNode::EncodingFromDocument);
          file.close();

          QFileInfo appInfo(qApp->applicationDirPath());
          QString strPath = appInfo.filePath();

          QString strExec = strPath + "/plistutil-x86_64.AppImage";
          QString strTarget = strConfigDir + "/_temp.plist";

          QProcess::execute(
              strExec, QStringList() << "-i" << strSource << "-o" << strTarget);

          QElapsedTimer t;
          t.start();
          while (!QFile::exists(strTarget)) {
            QCoreApplication::processEvents();
          }

          if (QFile::exists(strTarget)) {
            QFile::remove(filePath);

            if (QFile::copy(strTarget, filePath)) {
              QFile::remove(strSource);
              QFile::remove(strTarget);
            }
          }
        }

        if (win) {
          QString strConfigDir = QDir::homePath() + "/.config/PlistEDPlus";
          QString strSource = strConfigDir + "/_util.plist";

          QFile file(strSource);
          file.open(QIODevice::WriteOnly);
          QTextStream out(&file);
          out.setCodec("UTF-8");
          doc.save(out, 4, QDomNode::EncodingFromDocument);
          file.close();

          QFileInfo appInfo(qApp->applicationDirPath());
          QString strPath = appInfo.filePath();

          QString strExec = strPath + "/util/plistutil.exe";
          QString strTarget = strConfigDir + "/_temp.plist";

          QProcess::execute("cmd.exe", QStringList()
                                           << "/c" << strExec << "-i"
                                           << strSource << "-o" << strTarget);

          QElapsedTimer t;
          t.start();
          while (!QFile::exists(strTarget)) {
            QCoreApplication::processEvents();
          }

          if (QFile::exists(strTarget)) {
            QFile::remove(filePath);

            if (QFile::copy(strTarget, filePath)) {
              QFile::remove(strSource);
              QFile::remove(strTarget);
            }
          }
        }

        if (mac || osx1012) {
          QFile file(filePath);
          file.open(QIODevice::WriteOnly);
          QTextStream out(&file);
          out.setCodec("UTF-8");
          doc.save(out, 4, QDomNode::EncodingFromDocument);
          file.close();
          QProcess::execute("plutil", QStringList() << "-convert"
                                                    << "binary1" << filePath);
        }
        tabWidget->setTabText(index, "[BIN] " + name);
      }

      loadText(filePath);
    }

    undoGroup->activeStack()->clear();

    tabWidget->tabBar()->setTabToolTip(tabWidget->currentIndex(),
                                       fi.filePath());

    goPlistText();
  }

  setOrgMD5(filePath);
  addWatchFiles();
}

void MainWindow::actionSave() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    QString path = tab->getPath();
    if (!path.isEmpty())
      savePlist(path);
    else
      actionSaveAs();
  }
}

void MainWindow::actionSaveAs() {
  if (tabWidget->hasTabs()) {
    QString str = QFileDialog::getSaveFileName(this, tr("Save as"), "",
                                               tr("Property list (*.plist)"));

    if (!str.isEmpty()) {
      closeOpenedFile(str);

      savePlist(str);

      this->setWindowTitle(ver + "[*] " + tabWidget->getCurentTab()->getPath());
      ui->lblTitle->setText(this->windowTitle());

      QSettings settings;
      QFileInfo fInfo(str);
      settings.setValue("currentDirectory", fInfo.absolutePath());
      // qDebug() << settings.fileName(); //最近打开的文件所保存的位置
      m_recentFiles->setMostRecentFile(str);
      initRecentFilesForToolBar();
    }
  }
}

void MainWindow::actionAdd_activated() {
  //增加子项

  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index = tab->currentIndex();
    DomModel* model = tab->getModel();

    if (!index.isValid()) return;

    DomItem* item = model->itemForIndex(index);

    if (item->getType() != "dict" && item->getType() != "array") return;

    QModelIndex index0 = model->index(index.row(), 0, index.parent());

    if (index0.data().toString() == "") return;

    if (index.isValid()) {
      index = model->index(index.row(), 0, index.parent());

      QUndoCommand* addMoveCommand = new AddMoveCommand(model, index);
      undoGroup->activeStack()->push(addMoveCommand);

      // 转到该行
      QModelIndex childIndex = model->index(item->childCount() - 1, 0, index);
      tab->treeView->setCurrentIndex(childIndex);
    }
  }
}

void MainWindow::actionRemove_activated() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* model = tab->getModel();

    QItemSelectionModel* selections = tab->treeView->selectionModel();
    QModelIndexList selectedsList = selections->selectedRows();

    qSort(selectedsList.begin(), selectedsList.end(),
          qGreater<QModelIndex>());  // so that rows are removed from highest
                                     // index

    foreach (QModelIndex index, selectedsList) {
      if (index.isValid()) {
        if (model->itemNotPlist(index)) {
          QUndoCommand* removeCommand = new RemoveCommand(model, index);
          undoGroup->activeStack()->push(removeCommand);
        }
      }
    }

    QModelIndex index = tab->currentIndex();
    if (index.isValid()) tab->treeView->setCurrentIndex(index);
    tab->treeView->resizeColumnToContents(0);
  }
}

void MainWindow::actionExpand_all_activated() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();

    tab->expand();

    setExpandText(tab);
  }
}

void MainWindow::actionAbout_activated() {
  QFileInfo appInfo(qApp->applicationFilePath());
  QString str;

  str = tr("Last modified: ");

  QString last = str + appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
  QString str1 =
      "<a style='color:blue;' href = "
      "https://github.com/ic005k/PlistEDPlus>PlistEDPlus</a><br><br>";
  QString str2 = "V " + CurVerison;
  QString str3 = "<br><br>";

  QMessageBox::about(this, "About", str1 + str2 + str3 + last);
}

void MainWindow::onTabWidget_currentChanged(int index) {
  if (index >= 0) {
    if (tabWidget->hasTabs()) {
      EditorTab* tab = tabWidget->getCurentTab();
      setExpandText(tab);

      QString strBin = tabWidget->tabBar()->tabText(tabWidget->currentIndex());
      if (strBin.contains("[BIN]")) {
        ui->cboxFileType->setCurrentIndex(1);

      } else {
        ui->cboxFileType->setCurrentIndex(0);
      }

      QString strCurrentFile = tabWidget->getCurentTab()->getPath();
      this->setWindowTitle(ver + "[*] " + strCurrentFile);
      ui->lblTitle->setText(this->windowTitle());

      // get undo stack
      QUndoStack* stack = tab->getUndoStack();

      // set active stack
      if (!undoGroup->stacks().contains(stack)) undoGroup->addStack(stack);

      undoGroup->setActiveStack(stack);

      if (!loading) {
        loadText(tabWidget->getCurentTab()->getPath());
        goPlistText();
        showMsg();

        writeINITab();
      }

      ui->btnPrevious->setEnabled(false);
      ui->btnNext->setEnabled(false);
      ui->btnReplace->setEnabled(false);
      ui->listFind_2->clear();
      ui->frameData->setHidden(true);
    }
  }
}

void MainWindow::onCleanChanged(bool clean) { this->setWindowModified(!clean); }

void MainWindow::openRecentFile() {
  QAction* action = qobject_cast<QAction*>(sender());
  if (action) {
    QString file = action->text();
    QFileInfo fi(file);
    if (fi.exists()) openPlist(file);
  }
}

void MainWindow::setExpandText(EditorTab* tab) {
  QString text = (!tab->isExpanded()) ? tr("Expand all") : tr("Collapse all");
  ui->actionExpand_all->setIconText(text);

  if (ui->actionExpand_all->iconText() == tr("Expand all")) {
    ui->actionExpand_all->setIcon(QIcon(":/new/toolbar/res/exp.svg"));
    ui->actionExpand_all->setToolTip(tr("Expand all"));
  }

  if (ui->actionExpand_all->iconText() == tr("Collapse all")) {
    ui->actionExpand_all->setIcon(QIcon(":/new/toolbar/res/col.svg"));
    ui->actionExpand_all->setToolTip(tr("Collapse all"));
  }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
  const QMimeData* mimeData = event->mimeData();

  if (mimeData->hasFormat("text/uri-list")) event->accept();

  // application/xml application/x-plist
}

void MainWindow::dropEvent(QDropEvent* event) {
  QList<QUrl> urls = event->mimeData()->urls();
  if (urls.isEmpty()) return;

  QStringList list;

  for (int i = 0; i < urls.size(); ++i) {
    list.append(urls.at(i).toLocalFile());
  }

  QFileInfo fi(list.at(0));

  // plist
  if (fi.suffix().toLower() == "plist") {
    openFiles(list);
  }

  // aml
  if (fi.suffix().toLower() == "aml") {
    for (int i = 0; i < list.count(); i++) {
      QFileInfo fi(list.at(i));
      AddACPI(fi.fileName());
    }
  }

  // tools
  if (fi.suffix().toLower() == "efi") {
    for (int i = 0; i < list.count(); i++) {
      QFileInfo fi(list.at(i));
      AddMiscTools(fi.fileName(), fi.baseName());
    }
  }

  // uefi
  if (fi.suffix().toLower() == "efi") {
    for (int i = 0; i < list.count(); i++) {
      QFileInfo fi(list.at(i));
      AddUEFIDrivers(fi.fileName());
    }
  }

  // kext
  QString fileSuffix;
#ifdef Q_OS_WIN32
  fileSuffix = fi.suffix().toLower();
#endif

#ifdef Q_OS_LINUX
  fileSuffix = fi.suffix().toLower();
#endif

#ifdef Q_OS_MAC
  QString str1 = list.at(0);
  QString str2 = str1.mid(str1.length() - 5, 4);
  fileSuffix = str2.toLower();
#endif

  if (fileSuffix == "kext") {
    QStringList kextList;
    for (int i = 0; i < list.count(); i++) {
      QString str3 = list.at(i);
      QString str4;
#ifdef Q_OS_WIN32
      str4 = str3.mid(0, str3.length());
#endif

#ifdef Q_OS_LINUX
      str4 = str3.mid(0, str3.length());
#endif

#ifdef Q_OS_MAC
      str4 = str3.mid(0, str3.length() - 1);
#endif

      kextList.append(str4);
    }

    addKexts(kextList);
  }

  // 要有tab存在，否则打开有问题的文件会导致崩溃
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    tab->treeView->resizeColumnToContents(0);
  }
}

void MainWindow::menu_aboutToShow()  //目前废除，去掉1就可正常使用
{
  bool enabled = tabWidget->hasTabs();
  ui->actionClose->setEnabled(enabled);
  ui->actionClose_all->setEnabled(enabled);
  ui->actionSave->setEnabled(enabled);
  ui->actionSave_as->setEnabled(enabled);
}

void MainWindow::on_Find() {
  loading = true;

  ui->btnReplace->setEnabled(false);

  if (ui->editFind->text() == "") return;

  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();

    QModelIndex index;

    DomModel* model = tab->getModel();

    tab->treeView->expandToDepth(0);

    index = model->index(0, 0);
    tab->treeView->setCurrentIndex(index);  //设置当前索引

    findCount = 0;
    ui->lblFindCount->setText("  " + QString::number(findCount) + "  ");
    find = false;

    if (index.isValid()) {
      indexFindList.clear();
      ui->listFind_2->clear();
      indexCount = -1;

      QString strFind = ui->editFind->text().trimmed();
      if (!actCaseSensitive->isChecked()) strFind = strFind.toLower();

      forEach(model, index, strFind);
    } else
      qDebug() << "index is no valid";

    tab->treeView->resizeColumnToContents(0);
  }

  loading = false;
}

void MainWindow::forEach(QAbstractItemModel* model, QModelIndex parent,
                         QString str) {
  for (int r = 0; r < model->rowCount(parent); ++r) {
    QModelIndex index2 = model->index(r, 2, parent);
    QString value = model->data(index2, Qt::DisplayRole).toString();
    QString originalValue = value.trimmed();
    if (!actCaseSensitive->isChecked()) value = value.toLower();

    QModelIndex index = model->index(r, 0, parent);
    // QVariant name = model->data(index);
    QString name = model->data(index, Qt::DisplayRole).toString();
    QString originalName = name.trimmed();
    if (!actCaseSensitive->isChecked()) name = name.toLower();

    //搜索值
    if (value.contains(str.trimmed()) && str.trimmed() != "") {
      EditorTab* tab = tabWidget->getCurentTab();

      tab->treeView->selectionModel()->setCurrentIndex(
          index2, QItemSelectionModel::Select);
      // treeView->selectionModel()->setCurrentIndex(index2,
      // QItemSelectionModel::SelectCurrent);
      findCount++;
      ui->lblFindCount->setText("  " + QString::number(findCount) + "  ");
      find = true;

      ui->actionSort->setEnabled(false);

      indexFindList.append(index2);
      ui->listFind_2->addItem(originalValue);
    }
    //搜索键
    if (name.contains(str.trimmed()) && str.trimmed() != "") {
      EditorTab* tab = tabWidget->getCurentTab();

      tab->treeView->selectionModel()->setCurrentIndex(
          index, QItemSelectionModel::Select);
      // treeView->selectionModel()->setCurrentIndex(index,
      // QItemSelectionModel::SelectCurrent);
      findCount++;
      ui->lblFindCount->setText("  " + QString::number(findCount) + "  ");
      find = true;

      ui->actionSort->setEnabled(false);

      indexFindList.append(index);
      ui->listFind_2->addItem(originalName);
    }

    if (model->hasChildren(index)) {
      forEach(model, index, str);
    }
  }

  if (find) {
    ui->btnPrevious->setEnabled(true);
    ui->btnNext->setEnabled(true);
    ui->listFind_2->setCurrentRow(0);
    ui->listFind_2->setHidden(false);
    update();
    findTextChanged = false;
  }
}

void MainWindow::on_copyAction() {
  if (tabWidget->hasTabs()) tabWidget->getCurentTab()->on_copyAction();
}

void MainWindow::on_cutAction() {
  if (tabWidget->hasTabs()) tabWidget->getCurentTab()->on_cutAction();
}

void MainWindow::on_pasteAction() {
  if (tabWidget->hasTabs()) tabWidget->getCurentTab()->on_pasteAction();
}

void MainWindow::closeEvent(QCloseEvent* event) {
  loading = true;

  writeINITab();

  //记录当前文件
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);

  //记录是否显示Plist文本
  Reg.setValue("ShowPlistText", ui->actionShowPlistText->isChecked());
  if (plistTextEditor->isVisible()) {
    Reg.setValue("PlistTextDockHeight", plistTextEditor->height());
    Reg.setValue("frameMainHeight", ui->frameMain->height());
  }

  Reg.setValue("restore", ui->actionRestoreScene->isChecked());
  Reg.setValue("DefaultIcon", ui->actionDefaultNodeIcon->isChecked());
  Reg.setValue("ExpAll", ui->actionExpandAllOpenFile->isChecked());
  Reg.setValue("drag", false);
  Reg.setValue("AutoUpdateCheck", ui->actionAutoUpdateCheck->isChecked());

  if (!ui->listFind_2->isHidden()) {
    Reg.setValue("dockFindWidth", ui->listFind_2->width());
    Reg.setValue("frameMainWidth", ui->frameMain->width());
  }

  // 存储窗口大小和位置
  Reg.setValue("x", this->x());
  Reg.setValue("y", this->y());
  Reg.setValue("width", this->width());
  Reg.setValue("height", this->height());

  //记录搜索文本列表
  //先读取，采用增量保存的方式，以免覆盖之前的已有数据
  QFileInfo fi(qfile);

  if (fi.exists()) {
    QStringList tempList;
    int count = Reg.value("FindTextListTotal").toInt();
    for (int i = 0; i < count; i++) {
      tempList.append(
          Reg.value("FindTextList" + QString::number(i)).toString());
    }

    if (FindTextList.count() < tempList.count()) {
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

  // 处理TAB
  if (tabWidget->hasTabs()) {
    blExit = true;
    int count = tabWidget->count();
    for (int i = 0; i < count; i++) {
      tabWidget->setCurrentIndex(0);
      emit tabWidget->tabCloseRequested(0);
      if (close_flag == 0)  // 0取消、1保存、2放弃标志，为后面新增功能预留
      {
        event->ignore();
        close_flag = -1;
        loading = false;
        break;  //如果第一个标签页选择取消，则直接终止关闭
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

    if (tabWidget->count() == 0) event->accept();
  }

  loading = false;
  blExit = false;
}

void MainWindow::reg_win() {
  QString appPath = qApp->applicationFilePath();

  QString dir = qApp->applicationDirPath();
  // 注意路径的替换
  appPath.replace("/", "\\");
  QString type = "PlistEDPlus";
  QSettings* regType =
      new QSettings("HKEY_CLASSES_ROOT\\.plist", QSettings::NativeFormat);
  QSettings* regIcon = new QSettings("HKEY_CLASSES_ROOT\\.plist\\DefaultIcon",
                                     QSettings::NativeFormat);
  QSettings* regShell =
      new QSettings("HKEY_CLASSES_ROOT\\QtOpenCoreConfig\\shell\\open\\command",
                    QSettings::NativeFormat);

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

void MainWindow::resizeEvent(QResizeEvent* event) {
  Q_UNUSED(event);
  QString t = windowTitle();
  setWindowTitle("");
  setWindowTitle(t);
  this->update();
}

void MainWindow::on_actionMoveUp() {
  if (tabWidget->hasTabs()) {
    loading = true;

    EditorTab* tab = tabWidget->getCurentTab();

    DomModel* model = tab->getModel();
    QModelIndex index, index_bak;
    index = tab->currentIndex();
    index_bak = index;

    if (!index.isValid()) return;

    DomItem* items = model->itemForIndex(index.parent());

    if (items == NULL) return;

    if (index.row() == 0) return;

    tab->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    on_cutAction();
    tab->treeView->setCurrentIndex(
        model->index(index_bak.row() - 1, 0, index.parent()));
    on_pasteAction();

    tab->treeView->setCurrentIndex(
        model->index(index_bak.row() - 1, 0, index.parent()));

    tab->treeView_clicked(tab->treeView->currentIndex());

    tab->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    loading = false;
  }
}
void MainWindow::on_actionMoveDown() {
  if (tabWidget->hasTabs()) {
    loading = true;

    EditorTab* tab = tabWidget->getCurentTab();

    DomModel* model = tab->getModel();
    QModelIndex index, index_bak;
    index = tab->currentIndex();
    index_bak = index;

    if (!index.isValid()) return;

    DomItem* items = model->itemForIndex(index.parent());

    if (items == NULL) return;

    if (index.row() == items->childCount() - 1) return;

    tab->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    //倒数第三行及以前
    if (index.row() <= items->childCount() - 3) {
      on_cutAction();
      tab->treeView->setCurrentIndex(
          model->index(index_bak.row() + 1, 0, index.parent()));
      on_pasteAction();
    }

    //倒数第二行
    if (index.row() == items->childCount() - 2) {
      QUndoCommand* addMoveCommand =
          new AddMoveCommand(tab->getModel(), index.parent());
      undoGroup->activeStack()->push(addMoveCommand);

      on_cutAction();
      tab->treeView->setCurrentIndex(
          model->index(index_bak.row() + 1, 0, index.parent()));
      on_pasteAction();
      tab->treeView->setCurrentIndex(
          model->index(index_bak.row() + 2, 0, index.parent()));
      actionRemove_activated();
    }

    tab->treeView->setCurrentIndex(
        model->index(index_bak.row() + 1, 0, index.parent()));

    tab->treeView_clicked(tab->treeView->currentIndex());

    tab->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    loading = false;
  }
}

void MainWindow::showMsg() {
  EditorTab* tab = tabWidget->getCurentTab();
  QModelIndex index;
  index = tab->currentIndex();
  DomModel* model = tab->getModel();
  DomItem* item = model->itemForIndex(index);

  QString str1, str2, str3, str4, str5, str6, str7;
  str1 =
      QObject::tr("Currently selected: ") + index.data().toString().trimmed();
  str2 = "      " + QObject::tr("Row: ") + QString::number(index.row() + 1);
  str3 =
      "      " + QObject::tr("Column: ") + QString::number(index.column() + 1);
  str5 = "      " + QObject::tr("Children: ") +
         QString::number(item->childCount());
  if (index.parent().isValid())
    str4 = "      " + QObject::tr("Parent level：") +
           index.parent().data().toString();
  else
    str4 = "";
  if (index.parent().parent().isValid())
    str6 = " -> " + index.parent().parent().data().toString();
  else
    str6 = "";
  if (index.parent().parent().parent().isValid())
    str7 = " -> " + index.parent().parent().parent().data().toString();
  else
    str7 = "";

  str1 = index.data().toString().trimmed();
  if (str1.count() >= 100) str1 = str1.mid(0, 97) + "...";
  lblStaInfo0->setText(str1);
  lblStaInfo2->setText(str2 + str3 + str5);
  lblStaInfo1->setText(str4 + str6 + str7);

  if (lblStaInfo0->text().trimmed().length() > 0)
    lblStaInfo0->setHidden(false);
  else
    lblStaInfo0->setHidden(true);
  if (lblStaInfo1->text().trimmed().length() > 0)
    lblStaInfo1->setHidden(false);
  else
    lblStaInfo1->setHidden(true);
  if (lblStaInfo2->text().trimmed().length() > 0)
    lblStaInfo2->setHidden(false);
  else
    lblStaInfo2->setHidden(true);
}

QString MainWindow::getPlistTextValue(QString str) {
  QString str0, str1, str2, str3, str4;
  str0 = str.trimmed();
  for (int i = 0; i < str0.length(); i++) {
    str1 = str0.mid(i, 1);
    if (str1 == ">") {
      str2 = str0.mid(i + 1, str0.length() - i);

      for (int j = 0; j < str2.length(); j++) {
        str3 = str2.mid(j, 1);
        if (str3 == "<") {
          str4 = str2.mid(0, j);

          break;
        }
      }

      break;
    }
  }

  return str4;
}

void MainWindow::goPlistText() {
  //转到plist文本
  if (!loading) {
    if (!ui->actionShowPlistText->isChecked()) return;

    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index;
    index = tab->currentIndex();
    if (!index.isValid()) return;

    DomModel* model = tab->getModel();
    DomItem* item = model->itemForIndex(index);

    QString name, datatype, val;
    name = item->getName();
    datatype = item->getType();
    val = item->getValue().trimmed();
    if (datatype == "data") {
      val = tab->HexStrToByte(val).toBase64().trimmed();
    }

    QTextEdit* editTemp = new QTextEdit;
    editTemp->setPlainText(plistTextEditor->document()->toPlainText());

    for (int i = 0; i < editTemp->document()->lineCount(); i++) {
      QTextBlock block = editTemp->document()->findBlockByNumber(i);
      editTemp->setTextCursor(QTextCursor(block));
      QString lineText =
          editTemp->document()->findBlockByNumber(i).text().trimmed();

      if (name.mid(0, 4) == "Item") {
        if (getPlistTextValue(lineText) == val) {
          setBarMarkers(i);

          break;
        }
      } else {
        if (datatype == "array" || datatype == "dict") {
          if (getPlistTextValue(lineText) == name) {
            setBarMarkers(i);

            break;
          }
        }

        if (datatype == "bool") {
          if (index.column() == 0 || index.column() == 1) {
            if (getPlistTextValue(lineText) == name) {
              QString strNext = editTemp->document()
                                    ->findBlockByNumber(i + 1)
                                    .text()
                                    .trimmed();
              QString strBool = strNext.mid(1, strNext.length() - 4);
              QString strBool1 = strNext.mid(1, strNext.length() - 3);

              if (strBool == val || strBool1 == val) {
                setBarMarkers(i);

                break;
              }
            }
          }

          if (index.column() == 2) {
            if (lineText.mid(1, lineText.length() - 4) == val ||
                lineText.mid(1, lineText.length() - 3) == val) {
              QString strPrevious = editTemp->document()
                                        ->findBlockByNumber(i - 1)
                                        .text()
                                        .trimmed();

              if (getPlistTextValue(strPrevious) == name) {
                setBarMarkers(i);

                break;
              }
            }
          }
        }

        if (datatype == "integer" || datatype == "string" ||
            datatype == "data" || datatype == "date" || datatype == "real") {
          if (index.column() == 0 || index.column() == 1) {
            if (getPlistTextValue(lineText) == name) {
              QString strNext = editTemp->document()
                                    ->findBlockByNumber(i + 1)
                                    .text()
                                    .trimmed();
              if (getPlistTextValue(strNext) == val) {
                setBarMarkers(i);

                break;
              }
            }
          }

          if (index.column() == 2) {
            if (getPlistTextValue(lineText) == val) {
              QString strPrevious = editTemp->document()
                                        ->findBlockByNumber(i - 1)
                                        .text()
                                        .trimmed();

              if (getPlistTextValue(strPrevious) == name) {
                setBarMarkers(i);

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

void MainWindow::setBarMarkers(int line) {
  QTextBlock block = plistTextEditor->document()->findBlockByNumber(line);
  plistTextEditor->setTextCursor(QTextCursor(block));

  QList<QTextEdit::ExtraSelection> extraSelection;
  QTextEdit::ExtraSelection selection;
  QColor lineColor;
  lineColor.setRgb(0, 255, 0, 60);
  // lineColor = QColor(Qt::gray).lighter(150);
  selection.format.setBackground(lineColor);
  selection.format.setProperty(QTextFormat::FullWidthSelection, true);
  selection.cursor = plistTextEditor->textCursor();
  selection.cursor.clearSelection();
  //将刚设置的 selection追加到链表当中
  extraSelection.append(selection);
  plistTextEditor->setExtraSelections(extraSelection);

  QScrollBar* vsBar = new QScrollBar;
  vsBar = plistTextEditor->verticalScrollBar();
  int vPos = plistTextEditor->verticalScrollBar()->sliderPosition();
  // if (vPos > plistTextEditor->height() / 3)
  vsBar->setSliderPosition(vPos + 0);  // plistTextEditor->height() / 2);
}

void MainWindow::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  //获取背景色
  QPalette pal = ui->btnFind->palette();
  QBrush brush = pal.window();
  int c_red = brush.color().red();

  if (c_red != red) {
    red = c_red;

    init_UIStyle();
  }
}

void MainWindow::on_expandAction() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index;
    index = tab->currentIndex();
    DomModel* model = tab->getModel();

    index = model->index(index.row(), 0, index.parent());

    if (!tab->treeView->isExpanded(index)) {
      tab->treeView->expand(index);

    } else if (tab->treeView->isExpanded(index)) {
      tab->treeView->collapse(index);
    }

    tab->treeView->resizeColumnToContents(0);
  }
}

void MainWindow::on_collapseAction() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index;
    index = tab->currentIndex();
    DomModel* model = tab->getModel();

    tab->view_collapse(index.parent(), model);
  }
}

void MainWindow::on_actionNewSibling() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    tab->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    tab->on_actionNewSibling();
    tab->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  }
}

void MainWindow::on_actionNewChild() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    tab->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    tab->on_actionNewChild();
    tab->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  }
}

void MainWindow::CheckUpdate() {
  QNetworkRequest quest;
  quest.setUrl(
      QUrl("https://api.github.com/repos/ic005k/PlistEDPlus/releases/latest"));
  quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
  manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply* reply) {
  QString str = reply->readAll();
  QMessageBox box;
  box.setText(str);

  parse_UpdateJSON(str);

  reply->deleteLater();
}

QString MainWindow::getUrl(QVariantList list) {
  QString macUrl, winUrl, linuxUrl, osx1012Url;
  for (int i = 0; i < list.count(); i++) {
    QVariantMap map = list[i].toMap();
    QString fName = map["name"].toString();

    if (fName.contains("PlistEDPlus_Mac.dmg"))
      macUrl = map["browser_download_url"].toString();

    if (fName.contains("Win")) winUrl = map["browser_download_url"].toString();

    if (fName.contains("Linux"))
      linuxUrl = map["browser_download_url"].toString();

    if (fName.contains("below"))
      osx1012Url = map["browser_download_url"].toString();
  }

  QString Url;
  if (mac) Url = macUrl;
  if (win) Url = winUrl;
  if (linuxOS) Url = linuxUrl;
  if (osx1012) Url = osx1012Url;
  // if (Url == "") Url =
  // "https://github.com/ic005k/PlistEDPlus/releases/latest";

  return Url;
}

int MainWindow::parse_UpdateJSON(QString str) {
  QJsonParseError err_rpt;
  QJsonDocument root_Doc = QJsonDocument::fromJson(str.toUtf8(), &err_rpt);

  if (err_rpt.error != QJsonParseError::NoError) {
    if (!blAutoCheckUpdate)
      QMessageBox::critical(this, "", tr("Network error!"));
    blAutoCheckUpdate = false;
    return -1;
  }
  if (root_Doc.isObject()) {
    QJsonObject root_Obj = root_Doc.object();

    QVariantList list = root_Obj.value("assets").toArray().toVariantList();
    QString Url = getUrl(list);
    dlgAutoUpdate->strUrl = Url;

    QJsonObject PulseValue = root_Obj.value("assets").toObject();
    QString Verison = root_Obj.value("tag_name").toString();

    QString UpdateTime = root_Obj.value("published_at").toString();
    QString ReleaseNote = root_Obj.value("body").toString();

    if (Verison > CurVerison && Url != "") {
      QString warningStr = tr("New version detected!") + "\n" +
                           tr("Version: ") + "V" + Verison + "\n" +
                           tr("Published at: ") + UpdateTime + "\n" +
                           tr("Release Notes: ") + "\n" + ReleaseNote;
      int ret = QMessageBox::warning(this, "", warningStr, tr("Download"),
                                     tr("Cancel"));
      if (ret == 0) {
        // Url = "https://github.com/ic005k/PlistEDPlus/releases/latest";
        // QDesktopServices::openUrl(QUrl(Url));
        ShowAutoUpdateDlg(false);
      }

    } else {
      if (!blAutoCheckUpdate)
        QMessageBox::information(
            this, "", tr("You are currently using the latest version!"));
    }
  }
  blAutoCheckUpdate = false;
  return 0;
}

void MainWindow::on_actionSort() {
  if (tabWidget->hasTabs()) {
    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index = tab->currentIndex();
    if (!index.isValid()) return;

    DomModel* model = tab->getModel();

    DomItem* currentItem = model->itemForIndex(index);
    if (currentItem->getType() == "bool") {
      QString strBool = currentItem->getValue().trimmed();
      currentItem->setValue(strBool);

      QModelIndex col3Index = model->index(index.row(), 2, index.parent());
      tab->treeView->setIndexWidget(col3Index, NULL);
    }

    model->sort(0, Qt::AscendingOrder);

    tab->treeView_clicked(tab->treeView->currentIndex());
  }
}

void MainWindow::on_NewWindow() {
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

void MainWindow::on_copyBW() {
  if (tabWidget->hasTabs()) {
    DomModel* model;
    EditorTab* tab = tabWidget->getCurentTab();
    model = tab->getModel();
    DomItem* item;
    QModelIndex index = tab->currentIndex();
    item = model->itemForIndex(index);
    if (item->getName() == "plist") return;

    this->repaint();

    int ci = tabWidget->currentIndex();
    on_copyAction();
    actionNew();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    int count = tabWidget->tabBar()->count();
    tabWidget->tabBar()->setTabVisible(count - 1, false);
    tabWidget->setCurrentIndex(count - 1);
#endif

    on_actionNewChild();

    pasteBW = true;
    on_pasteAction();
    pasteBW = false;

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

void MainWindow::on_pasteBW() {
  if (tabWidget->hasTabs()) {
    this->repaint();

    int ci = tabWidget->currentIndex();
    QString fn = QDir::homePath() + "/.config/PlistEDPlus/temp.plist";
    QFileInfo fi(fn);
    if (!fi.exists()) return;

    bool bak = ui->actionExpandAllOpenFile->isChecked();
    ui->actionExpandAllOpenFile->setChecked(false);
    openPlist(fn);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    int count = tabWidget->tabBar()->count();
    tabWidget->tabBar()->setTabVisible(count - 1, false);
    tabWidget->setCurrentIndex(count - 1);
#endif

    ui->actionExpandAllOpenFile->setChecked(bak);

    QModelIndex index = tabWidget->getCurentTab()->currentIndex();
    QModelIndex index1 =
        tabWidget->getCurentTab()->getModel()->index(0, 0, index);
    if (index1.isValid())
      tabWidget->getCurentTab()->treeView->setCurrentIndex(index1);

    tabWidget->getCurentTab()->treeView->selectAll();
    on_copyAction();
    actionClose_activated();

    tabWidget->setCurrentIndex(ci);
    tabWidget->getCurentTab()->treeView->setFocus();

    on_pasteAction();

    this->repaint();
  }
}

void MainWindow::loadText(QString textFile) {
  QString strBin = tabWidget->tabBar()->tabText(tabWidget->currentIndex());
  if (strBin.contains("[BIN]")) {
    plistTextEditor->clear();
    plistTextEditor->setPlainText(
        tr("This is a file in binary format, with no text displayed."));
    return;
  }

  QFileInfo fi(textFile);
  if (fi.exists()) {
    QFile file(textFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::warning(
          this, tr("Application"),
          tr("Cannot read file %1:\n%2.")
              .arg(QDir::toNativeSeparators(fileName), file.errorString()));

    } else {
      QTextStream in(&file);
      in.setCodec("UTF-8");
      QString text = in.readAll();
      plistTextEditor->setPlainText(text);
    }
  } else
    plistTextEditor->clear();
}

void MainWindow::on_actionShowPlistText_triggered(bool checked) {
  if (checked)
    plistTextEditor->setVisible(true);
  else
    plistTextEditor->setVisible(false);
}

void MainWindow::on_actionPaste_as_child_triggered() {
  if (tabWidget->hasTabs()) tabWidget->getCurentTab()->on_pasteAsChildAction();
}

void MainWindow::on_editFind_returnPressed() {
  ui->editFind->selectAll();
  if (!findTextChanged) {
    on_btnNext_clicked();
    return;
  }

  on_Find();

  QString str = ui->editFind->text().trimmed();
  for (int i = 0; i < FindTextList.count(); i++) {
    if (FindTextList.at(i) == str) {
      FindTextList.removeAt(i);
      break;
    }
  }
  if (str != "") FindTextList.insert(0, str);

  QCompleter* editFindCompleter = new QCompleter(FindTextList, this);
  editFindCompleter->setCaseSensitivity(Qt::CaseSensitive);
  editFindCompleter->setCompletionMode(QCompleter::InlineCompletion);
  ui->editFind->setCompleter(editFindCompleter);

  //更新动作
  btnFindMenu->clear();
  btnFindMenu->addAction(actCaseSensitive);
  btnFindMenu->addSeparator();
  btnFindMenu->addAction(actClearList);
  QAction* actTotal = new QAction(
      tr("Total") + " : " + QString::number(FindTextList.count()), this);
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

void MainWindow::on_editFind_textChanged(const QString& arg1) {
  if (tabWidget->hasTabs()) {
    findTextChanged = true;

    if (arg1 != "") {
      // on_Find();
    }

    if (arg1 == "" || !find) {
      findCount = 0;
      ui->lblFindCount->setText("  " + QString::number(findCount) + "  ");
      ui->listFind_2->clear();

      EditorTab* tab = tabWidget->getCurentTab();

      QModelIndex index = tab->currentIndex();
      tab->treeView->selectionModel()->setCurrentIndex(
          index, QItemSelectionModel::Clear);
    }
  }
}

void MainWindow::on_ShowFindReplace() { on_Find(); }

void MainWindow::on_btnFind_clicked() { on_Find(); }

void MainWindow::on_btnHideFind_clicked() {
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("dockFindWidth", ui->listFind_2->width());
  Reg.setValue("frameMainWidth", ui->frameMain->width());

  ui->frameFind->close();
  ui->listFind_2->close();
}

void MainWindow::on_btnPrevious_clicked() {
  if (ui->listFind_2->count() == 0) return;

  int row = ui->listFind_2->currentRow();

  if (row - 1 == -1)
    row = 0;
  else
    row = row - 1;

  ui->listFind_2->setCurrentRow(row);
  on_listFind_2_itemClicked(NULL);
}

void MainWindow::on_btnNext_clicked() {
  if (ui->listFind_2->count() == 0) return;

  int row = ui->listFind_2->currentRow();

  if (row + 1 == ui->listFind_2->count())
    row = 0;
  else
    row = row + 1;

  ui->listFind_2->setCurrentRow(row);
  on_listFind_2_itemClicked(NULL);
}

void MainWindow::on_btnReplace_clicked() {
  if (ui->editReplace->text().trimmed() == "" || indexFindList.count() == 0)
    return;

  if (ui->editReplace->text().trimmed().mid(0, 4).toLower() == "item" ||
      ui->editFind->text().trimmed().mid(0, 4).toLower() == "item")
    return;

  if (tabWidget->hasTabs() && !oneReplace) {
    EditorTab* tab = tabWidget->getCurentTab();

    DomModel* model = tab->getModel();
    QString strFind = ui->editFind->text().trimmed();
    QString strReplace = ui->editReplace->text().trimmed();

    QString str =
        model->data(indexFindList.at(indexCount), Qt::DisplayRole).toString();

    QModelIndex index = tab->treeView->currentIndex();
    DomItem* item = model->itemForIndex(index);
    QString name, val;
    name = item->getName();
    val = item->getValue();

    QString newStr;
    QString strModify = str;
    if (!actCaseSensitive->isChecked()) {
      newStr =
          strModify.replace(strFind.toLower(), strReplace, Qt::CaseInsensitive);
    } else
      newStr = strModify.replace(strFind, strReplace);

    if (str == name) {
      QModelIndex index_m = model->index(index.row(), 0, index.parent());
      tab->editorDataAboutToBeSet(index_m, newStr);

      item->setName(newStr);

      tab->treeView->doItemsLayout();
      tab->treeView->setFocus();

      indexFindList.remove(indexCount);

      loading = true;
      ui->listFind_2->takeItem(indexCount);
      loading = false;

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

      loading = true;
      ui->listFind_2->takeItem(indexCount);
      loading = false;

      findCount = indexFindList.count();
      indexCount--;
      oneReplace = true;
    }
  }
}

void MainWindow::on_btnReplaceAll_clicked() {
  if (ui->editReplace->text().trimmed() == "" ||
      ui->editFind->text().trimmed() == "")
    return;

  if (ui->editReplace->text().trimmed().mid(0, 4).toLower() == "item" ||
      ui->editFind->text().trimmed().mid(0, 4).toLower() == "item")
    return;

  on_Find();

  for (int i = 0; i < indexFindList.count(); i++) {
    if (tabWidget->hasTabs()) {
      EditorTab* tab = tabWidget->getCurentTab();
      DomModel* model = tab->getModel();

      QModelIndex index0 = indexFindList.at(i);
      QModelIndex index =
          model->index(index0.row(), index0.column(), index0.parent());
      tab->treeView->setCurrentIndex(index);

      tab->treeView->selectionModel()->setCurrentIndex(
          indexFindList.at(i), QItemSelectionModel::SelectCurrent);
      tab->treeView->setFocus();

      QString strFind = ui->editFind->text().trimmed();
      QString strReplace = ui->editReplace->text().trimmed();
      QString str =
          model->data(indexFindList.at(i), Qt::DisplayRole).toString();

      DomItem* item = model->itemForIndex(index);
      QString name, val;
      name = item->getName();
      val = item->getValue();

      QString newStr;
      QString strModify = str;
      if (!actCaseSensitive->isChecked()) {
        newStr = strModify.replace(strFind.toLower(), strReplace,
                                   Qt::CaseInsensitive);
      } else
        newStr = strModify.replace(strFind, strReplace);

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
  ui->lblFindCount->setText("  " + QString::number(findCount) + "  ");

  ui->btnPrevious->setEnabled(false);
  ui->btnNext->setEnabled(false);
  loading = true;
  ui->listFind_2->clear();
  loading = false;
}

void MainWindow::on_actionFind_triggered() {
  ui->btnFind_Tool->click();
  on_ShowFindReplace();
}

void MainWindow::on_actionFindNext_triggered() { on_btnNext_clicked(); }

void MainWindow::on_actionFindPrevious_triggered() { on_btnPrevious_clicked(); }

void MainWindow::on_actionReplace_triggered() {
  if (ui->frameFind->isHidden()) {
    ui->frameFind->show();
    ui->editFind->setFocus();
  }

  on_btnReplace_clicked();
}

void MainWindow::on_actionReplaceAll_triggered() { on_btnReplaceAll_clicked(); }

void MainWindow::on_btnShowReplace_clicked() {
  if (ui->frameFind->isHidden()) {
    ui->frameFind->show();
    ui->editReplace->setFocus();
  } else {
    ui->frameFind->close();
  }
}

void MainWindow::on_actionCut_triggered() { on_cutAction(); }

void MainWindow::on_actionCopy_between_windows_triggered() { on_copyBW(); }

void MainWindow::on_actionPaste_between_windows_triggered() { on_pasteBW(); }

void MainWindow::on_actionCheck_Update_triggered() { CheckUpdate(); }

void MainWindow::on_actionAbout_triggered() { actionAbout_activated(); }

void MainWindow::on_actionAdd_triggered() { actionAdd_activated(); }

void MainWindow::on_actionNew_Sibling_triggered() { on_actionNewSibling(); }

void MainWindow::on_actionOpen_triggered() { openFiles(); }

void MainWindow::on_actionNew_triggered() { actionNew(); }

void MainWindow::on_actionSave_triggered() { actionSave(); }

void MainWindow::on_actionSave_as_triggered() { actionSaveAs(); }

void MainWindow::initFindReplace() {
  //初始化查找与替换界面
  ui->frameFind->layout()->setMargin(1);
  ui->frameFind->close();
  ui->btnPrevious->setEnabled(false);
  ui->btnNext->setEnabled(false);
  ui->btnReplace->setEnabled(false);

  ui->listFind_2->setHidden(true);
}

void MainWindow::initPlistTextShow() {}

void MainWindow::on_listFind_2_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
  if (ui->listFind_2->count() <= 0) return;

  if (tabWidget->hasTabs()) {
    bool focus = false;
    bool focus1 = false;
    if (ui->editFind->hasFocus()) focus = true;
    if (ui->listFind_2->hasFocus()) focus1 = true;

    indexCount = ui->listFind_2->currentRow();

    if (indexCount >= indexFindList.count()) {
      indexCount = indexFindList.count() - 1;
    }

    ui->lblFindCount->setText(QString::number(indexCount + 1) + " >> " +
                              QString::number(findCount));

    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex index = indexFindList.at(indexCount);
    tab->treeView->clearSelection();

    loading = true;
    tab->treeView->expandAll();
    loading = false;
    tab->treeView->resizeColumnToContents(0);

    if (ui->listFind_2->count() == 1) {
      QModelIndex index0 = tab->getModel()->index(0, 0);
      tab->treeView->setCurrentIndex(index0);
    }

    tab->treeView->selectionModel()->setCurrentIndex(
        index, QItemSelectionModel::SelectCurrent);

    oneReplace = false;
    ui->btnReplace->setEnabled(true);

    if (focus) ui->editFind->setFocus();
    if (focus1) ui->listFind_2->setFocus();
  }
}

void MainWindow::AddACPI(QString fileStr) {
  loading = true;

  QModelIndex currentIndex;
  EditorTab* tab = tabWidget->getCurentTab();
  DomModel* model = tab->getModel();
  DomItem* parentItem;
  DomItem* item;
  currentIndex = tab->currentIndex();

  item = model->itemForIndex(currentIndex);
  parentItem = model->itemForIndex(currentIndex.parent());

  currentIndex = model->index(item->row(), 0, currentIndex.parent());

  if (item->getName().trimmed() == "Add" && item->getType() == "array" &&
      parentItem->getName().trimmed() == "ACPI") {
    item = model->itemForIndex(currentIndex);
    QModelIndex childIndex;
    actionAdd_activated();
    childIndex = model->index(item->childCount() - 1, 0, currentIndex);

    if (!childIndex.isValid()) return;

    tab->treeView->setCurrentIndex(childIndex);

    item = model->itemForIndex(childIndex);
    item->setType("dict");

    // add aml
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);

    setItem(childIndex, 0, "Comment", "string", "");
    setItem(childIndex, 1, "Enabled", "bool", "true");
    setItem(childIndex, 2, "Path", "string", fileStr);

    tab->treeView->setCurrentIndex(currentIndex);

    tab->treeView->setFocus();
  }

  loading = false;
}

void MainWindow::setItem(QModelIndex parentIndex, int row, QString key,
                         QString type, QString value) {
  EditorTab* tab = tabWidget->getCurentTab();
  DomModel* model = tab->getModel();
  DomItem* item;

  QModelIndex nextChildIndex = model->index(row, 0, parentIndex);
  tab->treeView->setCurrentIndex(nextChildIndex);

  item = model->itemForIndex(nextChildIndex);
  item->setName(key);
  item->setType(type);
  item->setValue(value);
}

void MainWindow::addKexts(QStringList FileName) {
  int file_count = FileName.count();

  if (file_count == 0 || FileName[0] == "") return;

  for (int k = 0; k < FileName.count(); k++) {
    QString file = FileName.at(k);
    QFileInfo fi(file);
    if (fi.baseName().toLower() == "lilu") {
      FileName.removeAt(k);
      FileName.insert(0, file);
    }

    if (fi.baseName().toLower() == "virtualsmc") {
      FileName.removeAt(k);
      FileName.insert(1, file);
    }
  }

  for (int j = 0; j < file_count; j++) {
    QFileInfo fileInfo(FileName[j]);

    QFileInfo fileInfoList;
    QString filePath = fileInfo.absolutePath();

    QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/MacOS/");

    if (fileDir.exists())  //如果目录存在，则遍历里面的文件
    {
      fileDir.setFilter(QDir::Files);  //只遍历本目录
      QFileInfoList fileList = fileDir.entryInfoList();
      int fileCount = fileList.count();
      for (int i = 0; i < fileCount; i++)  //一般只有一个二进制文件
      {
        fileInfoList = fileList[i];
      }
    }

    QTableWidget* t = new QTableWidget;
    t->setColumnCount(8);

    int row = t->rowCount() + 1;

    t->setRowCount(row);
    t->setItem(row - 1, 0,
               new QTableWidgetItem(QFileInfo(FileName[j]).fileName()));
    t->setItem(row - 1, 1, new QTableWidgetItem(""));

    if (fileInfoList.fileName() != "")
      t->setItem(
          row - 1, 2,
          new QTableWidgetItem("Contents/MacOS/" + fileInfoList.fileName()));
    else
      t->setItem(row - 1, 2, new QTableWidgetItem(""));

    t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
    t->setItem(row - 1, 4, new QTableWidgetItem(""));
    t->setItem(row - 1, 5, new QTableWidgetItem(""));

    t->setItem(row - 1, 6, new QTableWidgetItem("true"));

    QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
    newItem1->setTextAlignment(Qt::AlignCenter);
    t->setItem(row - 1, 7, newItem1);

    initKextTable(row - 1, t);

    //如果里面还有PlugIns目录，则需要继续遍历插件目录
    QDir piDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/");

    if (piDir.exists()) {
      piDir.setFilter(QDir::Dirs);  //过滤器：只遍历里面的目录
      QFileInfoList fileList = piDir.entryInfoList();
      int fileCount = fileList.count();
      QVector<QString> kext_file;
      // qDebug() << fileCount;
      for (int i = 0; i < fileCount; i++)  //找出里面的kext文件(目录）
      {
        kext_file.push_back(fileList[i].fileName());
      }

      if (fileCount >= 3)  //里面有目录
      {
        for (int i = 0; i < fileCount - 2; i++) {
          QDir fileDir(filePath + "/" + fileInfo.fileName() +
                       "/Contents/PlugIns/" + kext_file[i + 2] +
                       "/Contents/MacOS/");
          if (fileDir.exists()) {
            fileDir.setFilter(QDir::Files);  //只遍历本目录里面的文件
            QFileInfoList fileList = fileDir.entryInfoList();
            int fileCount = fileList.count();
            for (int i = 0; i < fileCount; i++)  //一般只有一个二进制文件
            {
              fileInfoList = fileList[i];
            }

            //写入到表里
            int row = t->rowCount() + 1;

            t->setRowCount(row);
            t->setItem(
                row - 1, 0,
                new QTableWidgetItem(QFileInfo(FileName[j]).fileName() +
                                     "/Contents/PlugIns/" + kext_file[i + 2]));
            t->setItem(row - 1, 1, new QTableWidgetItem(""));

            t->setItem(row - 1, 2,
                       new QTableWidgetItem("Contents/MacOS/" +
                                            fileInfoList.fileName()));

            t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
            t->setItem(row - 1, 4, new QTableWidgetItem(""));
            t->setItem(row - 1, 5, new QTableWidgetItem(""));

            t->setItem(row - 1, 6, new QTableWidgetItem("true"));

            QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
            newItem1->setTextAlignment(Qt::AlignCenter);
            t->setItem(row - 1, 7, newItem1);

            initKextTable(row - 1, t);

          } else {  //不存在二进制文件，只存在一个Info.plist文件的情况

            QDir fileDir(filePath + "/" + fileInfo.fileName() +
                         "/Contents/PlugIns/" + kext_file[i + 2] +
                         "/Contents/");
            if (fileDir.exists()) {
              //写入到表里
              int row = t->rowCount() + 1;

              t->setRowCount(row);
              t->setItem(row - 1, 0,
                         new QTableWidgetItem(
                             QFileInfo(FileName[j]).fileName() +
                             "/Contents/PlugIns/" + kext_file[i + 2]));
              t->setItem(row - 1, 1, new QTableWidgetItem(""));
              t->setItem(row - 1, 2, new QTableWidgetItem(""));
              t->setItem(row - 1, 3,
                         new QTableWidgetItem("Contents/Info.plist"));
              t->setItem(row - 1, 4, new QTableWidgetItem(""));
              t->setItem(row - 1, 5, new QTableWidgetItem(""));
              // init_enabled_data(t, row - 1, 6, "true");
              t->setItem(row - 1, 6, new QTableWidgetItem("true"));

              QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
              newItem1->setTextAlignment(Qt::AlignCenter);
              t->setItem(row - 1, 7, newItem1);

              initKextTable(row - 1, t);
            }
          }
        }
      }
    }

    t->setFocus();
    t->setCurrentCell(row - 1, 0);
  }

  this->setWindowModified(true);
}

void MainWindow::initKextTable(int row, QTableWidget* w) {
  loading = true;

  QModelIndex currentIndex;
  EditorTab* tab = tabWidget->getCurentTab();
  DomModel* model = tab->getModel();
  DomItem* parentItem;
  DomItem* item;
  currentIndex = tab->currentIndex();

  item = model->itemForIndex(currentIndex);
  parentItem = model->itemForIndex(currentIndex.parent());

  currentIndex = model->index(item->row(), 0, currentIndex.parent());

  if (item->getName().trimmed() == "Add" && item->getType() == "array" &&
      parentItem->getName().trimmed() == "Kernel") {
    item = model->itemForIndex(currentIndex);
    QModelIndex childIndex;
    actionAdd_activated();
    childIndex = model->index(item->childCount() - 1, 0, currentIndex);

    if (!childIndex.isValid()) return;

    tab->treeView->setCurrentIndex(childIndex);

    item = model->itemForIndex(childIndex);
    item->setType("dict");

    // add kext
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);

    setItem(childIndex, 0, "Arch", "string", w->item(row, 7)->text());
    setItem(childIndex, 1, "BundlePath", "string", w->item(row, 0)->text());
    setItem(childIndex, 2, "Comment", "string", w->item(row, 1)->text());
    setItem(childIndex, 3, "Enabled", "bool", w->item(row, 6)->text());
    setItem(childIndex, 4, "ExecutablePath", "string", w->item(row, 2)->text());
    setItem(childIndex, 5, "MaxKernel", "string", w->item(row, 5)->text());
    setItem(childIndex, 6, "MinKernel", "string", w->item(row, 4)->text());
    setItem(childIndex, 7, "PlistPath", "string", w->item(row, 3)->text());

    tab->treeView->setCurrentIndex(currentIndex);

    tab->treeView->setFocus();
  }

  loading = false;
}

void MainWindow::init_enabled_data(QTableWidget* table, int row, int column,
                                   QString str) {
  QTableWidgetItem* chkbox = new QTableWidgetItem(str);

  table->setItem(row, column, chkbox);
  table->item(row, column)->setTextAlignment(Qt::AlignCenter);
  if (str == "true")

    chkbox->setCheckState(Qt::Checked);
  else

    chkbox->setCheckState(Qt::Unchecked);
}

void MainWindow::AddMiscTools(QString fileStr, QString fileStrBaseName) {
  loading = true;

  QModelIndex currentIndex;
  EditorTab* tab = tabWidget->getCurentTab();
  DomModel* model = tab->getModel();
  DomItem* parentItem;
  DomItem* item;
  currentIndex = tab->currentIndex();

  item = model->itemForIndex(currentIndex);
  parentItem = model->itemForIndex(currentIndex.parent());

  currentIndex = model->index(item->row(), 0, currentIndex.parent());

  if (item->getName().trimmed() == "Tools" && item->getType() == "array" &&
      parentItem->getName().trimmed() == "Misc") {
    item = model->itemForIndex(currentIndex);
    QModelIndex childIndex;
    actionAdd_activated();
    childIndex = model->index(item->childCount() - 1, 0, currentIndex);

    if (!childIndex.isValid()) return;

    tab->treeView->setCurrentIndex(childIndex);

    item = model->itemForIndex(childIndex);
    item->setType("dict");

    // add misc tools
    for (int i = 0; i < 9; i++) {
      actionAdd_activated();
      tab->treeView->setCurrentIndex(childIndex);
    }

    setItem(childIndex, 0, "Arguments", "string", "");
    setItem(childIndex, 1, "Auxiliary", "bool", "false");
    setItem(childIndex, 2, "Comment", "string", "");
    setItem(childIndex, 3, "Enabled", "bool", "true");
    setItem(childIndex, 4, "Flavour", "string", "Auto");
    setItem(childIndex, 5, "Name", "string", fileStrBaseName);
    setItem(childIndex, 6, "Path", "string", fileStr);
    setItem(childIndex, 7, "RealPath", "bool", "false");
    setItem(childIndex, 8, "TextMode", "bool", "false");

    tab->treeView->setCurrentIndex(currentIndex);

    tab->treeView->setFocus();
  }

  loading = false;
}

void MainWindow::AddUEFIDrivers(QString fileStr) {
  loading = true;

  QModelIndex currentIndex;
  EditorTab* tab = tabWidget->getCurentTab();
  DomModel* model = tab->getModel();
  DomItem* parentItem;
  DomItem* item;
  currentIndex = tab->currentIndex();

  item = model->itemForIndex(currentIndex);
  parentItem = model->itemForIndex(currentIndex.parent());

  currentIndex = model->index(item->row(), 0, currentIndex.parent());

  if (item->getName().trimmed() == "Drivers" && item->getType() == "array" &&
      parentItem->getName().trimmed() == "UEFI") {
    item = model->itemForIndex(currentIndex);
    QModelIndex childIndex;

    actionAdd_activated();

    tab->treeView->setCurrentIndex(currentIndex);

    childIndex = model->index(item->childCount() - 1, 0, currentIndex);

    if (!childIndex.isValid()) return;

    tab->treeView->setCurrentIndex(childIndex);

    item = model->itemForIndex(childIndex);
    item->setType("dict");

    // item->setValue(fileStr); //OC0.7.3之前

    // add efi
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);
    actionAdd_activated();
    tab->treeView->setCurrentIndex(childIndex);

    setItem(childIndex, 0, "Arguments", "string", "");
    setItem(childIndex, 1, "Comment", "string", "");
    setItem(childIndex, 2, "Enabled", "bool", "true");
    setItem(childIndex, 3, "Path", "string", fileStr);

    tab->treeView->setCurrentIndex(currentIndex);

    tab->treeView->setFocus();
  }

  loading = false;
}

void MainWindow::on_actionBug_Report_triggered() {
  QUrl url(QString("https://github.com/ic005k/PlistEDPlus/issues"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionDiscussion_Forum_triggered() {
  QUrl url(QString(
      "https://www.insanelymac.com/forum/topic/"
      "345512-open-source-cross-platform-plist-file-editor-plistedplus/"));
  QDesktopServices::openUrl(url);
}

void MainWindow::initRecentFilesForToolBar() {
  QStringList rfList = m_recentFiles->getRecentFiles();
  reFileMenu->clear();
  for (int i = 0; i < rfList.count(); i++) {
    QFileInfo fi(rfList.at(i));
    QAction* act = new QAction(QString::number(i + 1) + " . " + fi.filePath());
    reFileMenu->addAction(act);
    connect(act, &QAction::triggered,
            [=]() { openPlist(m_recentFiles->getRecentFiles().at(i)); });
  }
}

void MainWindow::on_actionQuit_triggered() { this->close(); }

void MainWindow::on_listFind_2_itemSelectionChanged() {
  if (!loading) on_listFind_2_itemClicked(NULL);
}

void MainWindow::on_actionFont_triggered() {
  bool ok;
  QFontDialog fd;
  font = getFont();
  font = fd.getFont(&ok, font);

  if (ok) {
    if (tabWidget->hasTabs()) {
      int index = tabWidget->currentIndex();
      for (int i = 0; i < tabWidget->tabBar()->count(); i++) {
        tabWidget->setCurrentIndex(i);
        tabWidget->getCurentTab()->treeView->setFont(font);
      }

      tabWidget->setCurrentIndex(index);
    }

    plistTextEditor->setFont(font);

    //存储字体信息
    QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
    QFile file(qfile);

    QSettings Reg(qfile, QSettings::IniFormat);
    Reg.setValue("FontName", font.family());
    Reg.setValue("FontSize", font.pointSize());
    Reg.setValue("FontBold", font.bold());
    Reg.setValue("FontItalic", font.italic());
    Reg.setValue("FontUnderline", font.underline());

    file.close();
  }
}

QFont MainWindow::getFont() {
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QFileInfo fi(qfile);
  QFont font;
  if (fi.exists()) {
    QSettings Reg(qfile, QSettings::IniFormat);
    if (Reg.value("FontName").toString() != "") {
      font.setFamily(Reg.value("FontName").toString());
      font.setPointSize(Reg.value("FontSize").toInt());
      font.setBold(Reg.value("FontBold").toBool());
      font.setItalic(Reg.value("FontItalic").toBool());
      font.setUnderline(Reg.value("FontUnderline").toBool());

      return font;
    }
  }

  return font;
}

void MainWindow::on_listFind_2_currentRowChanged(int currentRow) {
  Q_UNUSED(currentRow)
}

void MainWindow::on_btnNo_clicked() {
  ui->frameTip->setHidden(true);
  reLoadByModiList.removeOne(strModiFile);

  checkReloadFilesByModi();
}

void MainWindow::on_btnYes_clicked() {
  ui->frameTip->setHidden(true);
  reLoadByModiList.removeOne(strModiFile);
  openPlist(strModiFile);

  checkReloadFilesByModi();
}

void MainWindow::checkReloadFilesByModi() {
  if (reLoadByModiList.count() > 0) {
    strModiFile = reLoadByModiList.at(0);
    QString elideNote;
    QFontMetrics fontWidth(ui->lblTip->font());
    elideNote =
        fontWidth.elidedText(strModiFile, Qt::ElideRight, width() - 200);
    ui->lblTip->setText(tr("The file has been modified by another "
                           "program. Do you want to reload?") +
                        "\n\n" + QString("%1").arg(elideNote));
    ui->frameTip->setHidden(false);
  }
}

void MainWindow::on_actionDownload_Upgrade_Packages_triggered() {
  ShowAutoUpdateDlg(false);
}

void MainWindow::ShowAutoUpdateDlg(bool Database) {
  if (dlgAutoUpdate->strUrl == "") return;
  if (dlgAutoUpdate->isVisible()) return;

  // dlgAutoUpdate->setWindowFlags(dlgAutoUpdate->windowFlags() |
  //                              Qt::WindowStaysOnTopHint);
  dlgAutoUpdate->setModal(true);
  dlgAutoUpdate->show();
  dlgAutoUpdate->startDownload(Database);
}

int MainWindow::deleteDirfile(QString dirName) {
  QDir directory(dirName);
  if (!directory.exists()) {
    return true;
  }

  QString srcPath = QDir::toNativeSeparators(dirName);
  if (!srcPath.endsWith(QDir::separator())) srcPath += QDir::separator();

  QStringList fileNames = directory.entryList(
      QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
  bool error = false;
  for (QStringList::size_type i = 0; i != fileNames.size(); ++i) {
    QString filePath = srcPath + fileNames.at(i);
    QFileInfo fileInfo(filePath);
    if (fileInfo.isFile() || fileInfo.isSymLink()) {
      QFile::setPermissions(filePath, QFile::WriteOwner);
      if (!QFile::remove(filePath)) {
        error = true;
      }
    } else if (fileInfo.isDir()) {
      if (!deleteDirfile(filePath)) {
        error = true;
      }
    }
  }

  if (!directory.rmdir(QDir::toNativeSeparators(directory.path()))) {
    error = true;
  }
  return !error;
}

QString MainWindow::getMD5(QString targetFile) {
  QCryptographicHash hashTest(QCryptographicHash::Md5);
  QFile f2(targetFile);
  f2.open(QFile::ReadOnly);
  hashTest.reset();  // 重置（很重要）
  hashTest.addData(&f2);
  QString targetHash = hashTest.result().toHex();
  f2.close();
  return targetHash;
}

void MainWindow::setOrgMD5(QString fileName) {
  listOrgMD5.clear();
  for (int i = 0; i < tabWidget->tabBar()->count(); i++) {
    fileName = tabWidget->getTab(i)->getPath();
    listOrgMD5.append(fileName + "|" + getMD5(fileName));
  }
}

QString MainWindow::getOrgMD5(QString fileName) {
  for (int i = 0; i < listOrgMD5.count(); i++) {
    QString strListAt = listOrgMD5.at(i);
    QStringList strListSplit = strListAt.split("|");
    if (strListSplit.count() == 2) {
      if (strListSplit.at(0) == fileName) {
        return getMD5(strListSplit.at(1));
      }
    }
  }

  return "";
}

void MainWindow::TextEditToFile(QTextEdit* txtEdit, QString fileName) {
  QFile* file;
  file = new QFile;
  file->setFileName(fileName);
  bool ok = file->open(QIODevice::WriteOnly);
  if (ok) {
    QTextStream out(file);
    out.setCodec("UTF-8");
    out << txtEdit->toPlainText();
    file->close();
    delete file;
  }
}

void MainWindow::on_btnUpdateASCII_clicked() {
  if (!ui->editASCII->isModified()) return;

  QString str = ui->editASCII->text();
  QString strHex = str.toUtf8().toHex().toUpper();

  EditorTab* tab = tabWidget->getCurentTab();
  QModelIndex index = tab->currentIndex();

  tab->editorDataAboutToBeSet(index, strHex);
  tab->treeView->doItemsLayout();

  tab->treeView_clicked(index);
}

void MainWindow::on_btnUpdateBase64_clicked() {
  if (!ui->editBase64->isModified()) return;

  QString str = ui->editBase64->text();
  QString strHex;
  QByteArray bytes = QByteArray::fromBase64(str.toUtf8());
  strHex = bytes.toHex().toUpper();

  EditorTab* tab = tabWidget->getCurentTab();
  QModelIndex index = tab->currentIndex();

  tab->editorDataAboutToBeSet(index, strHex);
  tab->treeView->doItemsLayout();

  tab->treeView_clicked(index);
}

void MainWindow::on_btnUpdateHex_clicked() {
  if (!ui->editHex->isModified()) return;

  QString str = ui->editHex->text();
  QString strHex = str.remove(QRegExp("\\s")).toUpper();

  EditorTab* tab = tabWidget->getCurentTab();
  QModelIndex index = tab->currentIndex();

  tab->editorDataAboutToBeSet(index, strHex);
  tab->treeView->doItemsLayout();

  tab->treeView_clicked(index);
}

void MainWindow::writeINITab() {
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  int count = tabWidget->count();
  Reg.setValue("count", count);
  if (tabWidget->hasTabs()) {
    Reg.setValue("index", tabWidget->tabBar()->currentIndex());
    for (int i = 0; i < count; i++) {
      QString fn = tabWidget->getTab(i)->getPath();
      Reg.setValue(QString::number(i) + "/" + "file", fn);
    }
  }
}

void MainWindow::on_actProxy1_triggered() {
  ui->actProxy1->setChecked(true);
  ui->actProxy2->setChecked(false);
  ui->actProxy3->setChecked(false);
  ui->actProxy4->setChecked(false);
  ui->actProxy5->setChecked(false);
  writeINIProxy();
}

void MainWindow::on_actProxy2_triggered() {
  ui->actProxy1->setChecked(false);
  ui->actProxy2->setChecked(true);
  ui->actProxy3->setChecked(false);
  ui->actProxy4->setChecked(false);
  ui->actProxy5->setChecked(false);
  writeINIProxy();
}

void MainWindow::on_actProxy3_triggered() {
  ui->actProxy1->setChecked(false);
  ui->actProxy2->setChecked(false);
  ui->actProxy3->setChecked(true);
  ui->actProxy4->setChecked(false);
  ui->actProxy5->setChecked(false);
  writeINIProxy();
}

void MainWindow::on_actProxy4_triggered() {
  ui->actProxy1->setChecked(false);
  ui->actProxy2->setChecked(false);
  ui->actProxy3->setChecked(false);
  ui->actProxy4->setChecked(true);
  ui->actProxy5->setChecked(false);
  writeINIProxy();
}

void MainWindow::on_actProxy5_triggered() {
  ui->actProxy1->setChecked(false);
  ui->actProxy2->setChecked(false);
  ui->actProxy3->setChecked(false);
  ui->actProxy4->setChecked(false);
  ui->actProxy5->setChecked(true);
  writeINIProxy();
}

void MainWindow::writeINIProxy() {
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("proxy1", ui->actProxy1->isChecked());
  Reg.setValue("proxy2", ui->actProxy2->isChecked());
  Reg.setValue("proxy3", ui->actProxy3->isChecked());
  Reg.setValue("proxy4", ui->actProxy4->isChecked());
  Reg.setValue("proxy5", ui->actProxy5->isChecked());
}

void MainWindow::readINIProxy() {
  QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
  QSettings Reg(qfile, QSettings::IniFormat);

  if (!Reg.allKeys().contains("proxy1")) {
    QLocale locale;
    if (locale.language() != QLocale::Chinese) {
      ui->actProxy1->setChecked(Reg.value("proxy1", true).toBool());
      on_actProxy1_triggered();
    }
  } else
    ui->actProxy1->setChecked(Reg.value("proxy1").toBool());

  if (!Reg.allKeys().contains("proxy3")) {
    QLocale locale;
    if (locale.language() == QLocale::Chinese) {
      ui->actProxy3->setChecked(Reg.value("proxy3", true).toBool());
      on_actProxy3_triggered();
    }
  } else
    ui->actProxy3->setChecked(Reg.value("proxy3").toBool());

  ui->actProxy2->setChecked(Reg.value("proxy2").toBool());
  ui->actProxy4->setChecked(Reg.value("proxy4").toBool());
  ui->actProxy5->setChecked(Reg.value("proxy5").toBool());
}

QString MainWindow::getProxy() {
  if (ui->actProxy1->isChecked()) return ui->actProxy1->text();
  if (ui->actProxy2->isChecked()) return ui->actProxy2->text();
  if (ui->actProxy3->isChecked()) return ui->actProxy3->text();
  if (ui->actProxy4->isChecked()) return ui->actProxy4->text();
  if (ui->actProxy5->isChecked()) return ui->actProxy5->text();

  return "";
}

void MainWindow::on_actionMove_Up_triggered() { on_actionMoveUp(); }

void MainWindow::on_actionMove_Down_triggered() { on_actionMoveDown(); }

void MainWindow::on_actionSort_triggered() { on_actionSort(); }

void MainWindow::on_actionExpand_All_triggered() {
  actionExpand_all_activated();
}

void MainWindow::on_actionCollapse_All_triggered() {}

void MainWindow::on_btnFind_Tool_clicked() {
  ui->frameFind->show();
  ui->editFind->setFocus();
  if (ui->editFind->text().length() > 0) ui->editFind->selectAll();
}

void MainWindow::on_btnReplaceFind_clicked() {
  if (!ui->btnReplace->isEnabled()) return;
  if (ui->editReplace->text().trimmed() == "") return;
  ui->btnReplace->clicked();
  ui->btnNext->clicked();
}

void MainWindow::on_btnNew_clicked() { actionNew(); }

void MainWindow::on_btnSave_clicked() { actionSave(); }

void MainWindow::mousePressEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) {
    isDrag = true;
    m_position = e->globalPos() - this->pos();
    e->accept();
  }
}

void MainWindow::mouseMoveEvent(QMouseEvent* e) {
  if (isDrag & (e->buttons() & Qt::LeftButton)) {
    move(e->globalPos() - m_position);
    e->accept();
  }
}

void MainWindow::mouseReleaseEvent(QMouseEvent*) { isDrag = false; }

void MainWindow::on_btnClose_clicked() { close(); }

void MainWindow::on_btnMax_clicked() {}

bool MainWindow::eventFilter(QObject* o, QEvent* e) {
  if (e->type() == QEvent::ActivationChange) {
    return QWidget::eventFilter(o, e);
    if (QApplication::activeWindow() != this) {
      if (red > 55) {
        this->setStyleSheet(
            "QMainWindow { background-color: rgb(246,246,246);}");
      } else {
        this->setStyleSheet("QMainWindow { background-color: rgb(45,45,45);}");
      }
    } else {
      if (red > 55) {
        this->setStyleSheet(
            "QMainWindow { background-color: rgb(212,212,212);}");
      } else {
        this->setStyleSheet("QMainWindow { background-color: rgb(42,42,42);}");
      }
    }
  }
  return QWidget::eventFilter(o, e);
}

void MainWindow::init_UIStyle() {
  myHL = new MyHighLighter(plistTextEditor->document());
  myHL->rehighlight();

  if (mac || osx1012) {
    if (red > 55) {
      ui->statusBar->setStyleSheet(sbarStyleLight);
      // this->setStyleSheet("QMainWindow { background-color:
      // rgb(212,212,212);}");
      tabWidget->setStyleSheet(tabStyleLight);

    } else {
      ui->statusBar->setStyleSheet(sbarStyleDark);
      // this->setStyleSheet("QMainWindow { background-color: rgb(42,42,42);}");
      tabWidget->setStyleSheet(ui->tabWidget->styleSheet());
    }
  }

  if (win || linuxOS) {
    QString tabBarStyle = "QTabBar::tab{min-height:35px;min-width:80px;}";
    tabWidget->setStyleSheet(tabBarStyle);
  }
}

void MainWindow::changeEvent(QEvent* e) {
  Q_UNUSED(e);
#ifdef __APPLE__
  return;
  OSXHideTitleBar::HideTitleBar(winId());
#endif
}
