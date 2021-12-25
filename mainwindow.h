#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCompleter>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDropEvent>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMimeData>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTextBlock>
#include <QTreeView>
#include <QUndoGroup>

//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
// JSON相关头文件
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "autoupdatedialog.h"
#include "codeeditor.h"
#include "domparser.h"
#include "editortabswidget.h"
#include "myhighlighter.h"
#include "recentfiles.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  Ui::MainWindow* ui;

  bool mac = false;
  bool win = false;
  bool linuxOS = false;
  AutoUpdateDialog* dlgAutoUpdate;
  QLabel* lblStaInfo0;
  QLabel* lblStaInfo1;
  QLabel* lblStaInfo2;
  QStringList reLoadByModiList;
  QFont font;
  bool findTextChanged = false;
  CodeEditor* plistTextEditor;
  QComboBox* cboxFileType;
  bool osx1012 = false;
  bool pasteBW = false;
  QString strModiFile;
  bool blAutoCheckUpdate = false;
  void addWatchFiles();
  void removeWatchFiles();

  void AddACPI(QString fileStr);
  void addKexts(QStringList FileName);
  void init_enabled_data(QTableWidget* table, int row, int column, QString str);
  void initKextTable(int row, QTableWidget* w);
  void AddUEFIDrivers(QString fileStr);
  void AddMiscTools(QString fileStr, QString fileStrBaseName);
  void setItem(QModelIndex parentIndex, int row, QString key, QString type,
               QString value);

  void forEach(QAbstractItemModel* model, QModelIndex parent, QString str);
  QModelIndex indexFind;
  QVector<QModelIndex> indexFindList;
  int indexCount = 0;

  void openPlist(QString filePath = "");

  int close_flag = -1;

  QAction* actionUndo;

  QAction* actionRedo;

  void showMsg();

  void loadText(QString textFile);

  void goPlistText();

  void closeOpenedFile(QString file);

  QFont getFont();
  void checkReloadFilesByModi();
  void ShowAutoUpdateDlg(bool Database);
  int deleteDirfile(QString dirName);
  QString getUrl(QVariantList list);
  bool getBinPlist(QString filePath);
  QDomDocument readXMLPlist(QDomDocument document, QString filePath);
  QString getMD5(QString targetFile);
  QString strOrgMD5;
  QString strByModiMD5;
  QStringList listOrgMD5;

  void setOrgMD5(QString fileName);
  QString getOrgMD5(QString fileName);

  QString getTextEditLineText(QTextEdit* txtEdit, int i);
  QString readText(QString textFile);
  void TextEditToFile(QTextEdit* txtEdit, QString fileName);
  void writeINITab();
  void writeINIProxy();
  QString getProxy();
 public slots:
  void on_Find();
  void on_actionMoveUp();
  void on_actionMoveDown();
  void actionAdd_activated();
  void actionRemove_activated();

  void on_NewWindow();

 private slots:
  void recentOpen(QString filename);
  void on_copyBW();
  void on_pasteBW();

  void actionNew();

  void actionClose_activated();
  void actionClose_all_activated();

  void actionSave();
  void actionSaveAs();

  void actionExpand_all_activated();
  void actionAbout_activated();

  void on_TabWidget_currentChanged(int index);
  void menu_aboutToShow();

  void onCleanChanged(bool clean);

  void openRecentFile();

  void on_TabCloseRequest(int i = -1);

  void on_copyAction();
  void on_cutAction();
  void on_pasteAction();

  void on_expandAction();
  void on_collapseAction();

  void on_actionNewSibling();

  void on_actionNewChild();

  void replyFinished(QNetworkReply* reply);
  void CheckUpdate();

  void on_actionSort();

  void on_actionShowPlistText_triggered(bool checked);

  void on_actionPaste_as_child_triggered();

  void on_editFind_returnPressed();

  void on_editFind_textChanged(const QString& arg1);

  void on_ShowFindReplace();

  void on_btnFind_clicked();

  void on_btnHideFind_clicked();

  void on_btnNext_clicked();

  void on_btnPrevious_clicked();

  void on_btnReplace_clicked();

  void on_btnReplaceAll_clicked();

  void on_actionFind_triggered();

  void on_actionFindNext_triggered();

  void on_actionFindPrevious_triggered();

  void on_actionReplace_triggered();

  void on_actionReplaceAll_triggered();

  void on_btnShowReplace_clicked();

  void on_actionCut_triggered();

  void on_actionCopy_between_windows_triggered();

  void on_actionPaste_between_windows_triggered();

  void on_actionCheck_Update_triggered();

  void on_actionAbout_triggered();

  void on_actionAdd_triggered();

  void on_actionNew_Sibling_triggered();

  void on_actionOpen_triggered();

  void on_actionNew_triggered();

  void on_actionSave_triggered();

  void on_actionSave_as_triggered();

  void on_listFind_itemClicked(QListWidgetItem* item);

  void on_actionBug_Report_triggered();

  void on_actionDiscussion_Forum_triggered();

  void on_actionQuit_triggered();

  void on_listFind_itemSelectionChanged();

  void on_actionFont_triggered();

  void on_listFind_currentRowChanged(int currentRow);

  void on_btnNo_clicked();

  void on_btnYes_clicked();

  void on_actionDownload_Upgrade_Packages_triggered();

  void on_btnUpdateASCII_clicked();

  void on_btnUpdateBase64_clicked();

  void on_btnUpdateHex_clicked();

  void on_actProxy1_triggered();

  void on_actProxy2_triggered();

  void on_actProxy3_triggered();

  void on_actProxy4_triggered();

  void on_actProxy5_triggered();

  void on_actionMove_Up_triggered();

  void on_actionMove_Down_triggered();

  void on_actionSort_triggered();

  void on_actionExpand_All_triggered();

  void on_actionCollapse_All_triggered();

  void on_btnFind_Tool_clicked();

  void on_btnReplaceFind_clicked();

  void on_btnNew_clicked();

  void on_btnSave_clicked();

 private:
  bool useQtWriteXML = true;
  RecentFiles* m_recentFiles;
  QMenu* reFileMenu;
  void initRecentFilesForToolBar();
  void initMenuToolsBar();
  void initFindReplace();
  void initPlistTextShow();

  QNetworkAccessManager* manager;
  int parse_UpdateJSON(QString str);
  bool blExit = false;
  void setExpandText(EditorTab* tab);

  void openFiles(QStringList list = QStringList());

  void savePlist(QString filePath);

  enum { MaxRecentFiles = 10 };

  int findCount = 0;

  bool find = false;

  void reg_win();

  QString ver;

  QStringList FindTextList;

  void setBarMarkers(int line);
  QString getPlistTextValue(QString str);

  MyHighLighter* myHL;

  bool oneReplace = false;

  QMenu* btnFindMenu;
  QAction* actCaseSensitive = new QAction(tr("Case sensitive"), this);
  QAction* actClearList = new QAction(tr("Clear List"), this);
  QVector<QAction*> btnFindActionList;

  void init_iniData();

  void readINIProxy();

 protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;
  void closeEvent(QCloseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
};

#endif  // MAINWINDOW_H
