#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMimeData>
#include <QPalette>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>
#include <QUndoGroup>

//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
//JSON相关头文件
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "domparser.h"
#include "editortabswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    Ui::MainWindow* ui;

    QLineEdit* findEdit;

    void forEach(QAbstractItemModel* model, QModelIndex parent, QString str);

    void openPlist(QString filePath = "");

    int close_flag = -1;

    QAction* actionUndo;

    QAction* actionRedo;

public slots:
    void on_Find();
    void on_actionMoveUp();
    void on_actionMoveDown();
    void actionAdd_activated();
    void actionRemove_activated();

private slots:

    void actionNew_activated();
    void actionOpen_activated();
    void actionClose_activated();
    void actionClose_all_activated();

    void actionSave_activated();
    void actionSave_as_activated();

    void actionExpand_all_activated();
    void actionAbout_activated();

    void tabWidget_currentChanged(int index);
    void menu_aboutToShow();

    void onCleanChanged(bool clean);

    void openRecentFile();

    void onTabCloseRequest(int i = -1);

    void findEdit_textChanged(const QString& arg1);

    void findEdit_returnPressed();

    void on_copyAction();
    void on_cutAction();
    void on_pasteAction();

    void on_expandAction();
    void on_collapseAction();

    void on_actionNewSibling();

    void on_actionNewChild();

    void replyFinished(QNetworkReply* reply);
    void CheckUpdate();

private:
    QNetworkAccessManager* manager;
    int parse_UpdateJSON(QString str);
    bool mac = false;
    bool win = false;
    bool linuxOS = false;
    QString CurVerison = "V1.0";

    void setExpandText(EditorTab* tab);

    void openFiles(QStringList list = QStringList());

    void savePlist(QString filePath);

    enum { MaxRecentFiles = 10 };
    void setRecentFiles(const QString& fileName);
    void updateRecentFiles();

    int findCount = 0;
    QLabel* lblFindCount;
    bool find = false;

    void showMsg();

    void reg_win();

    QString ver;

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);
};

#endif // MAINWINDOW_H
