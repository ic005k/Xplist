#ifndef EDITORTABSWIDGET_H
#define EDITORTABSWIDGET_H

#include <QMainWindow>
#include <QMenu>
#include <QContextMenuEvent>
#include "editortab.h"



class EditorTabsWidget : public QTabWidget
{

Q_OBJECT

public:
    EditorTabsWidget(QWidget * parent = 0);
    ~EditorTabsWidget();

    EditorTab *createTab(DomModel *model, QString filePath = "");
    EditorTab *getCurentTab();
    EditorTab *getTab(int i = -1);
    bool hasTabs();

    void contextMenuEvent(QContextMenuEvent * event);

public slots:
    void closeTab(int i = -1);
    void closeTabsToRight();
    void closeOtherTabs();
    void closeAllTabs();

signals:
    void tabCloseRequested(int i);

private:
    void createMenu();

};

#endif // EDITORTABSWIDGET_H
