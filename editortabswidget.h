#ifndef EDITORTABSWIDGET_H
#define EDITORTABSWIDGET_H

#include "editortab.h"

#include <QContextMenuEvent>
#include <QMainWindow>
#include <QMenu>
#include <QSortFilterProxyModel>

class EditorTabsWidget : public QTabWidget {

    Q_OBJECT

public:
    EditorTabsWidget(QWidget* parent = 0);
    ~EditorTabsWidget();

    EditorTab* createTab(DomModel* model, QString filePath = "");
    EditorTab* getCurentTab();
    EditorTab* getTab(int i = -1);
    bool hasTabs();

    void contextMenuEvent(QContextMenuEvent* event) override;

protected:
public slots:
    void closeTab(int i = -1);
    void closeTabsToRight();
    void closeOtherTabs();
    void closeAllTabs();

signals:
    void tabCloseRequested(int i);

private:
    void createMenu();
    void initTabBar();
    void popPage(QWidget* page);
    void takeNormalPage(QWidget* page);
    int appendNormalPage(QWidget* page);
    bool indexValid(int index) const;
    //保存不能拖出、没有关闭按钮的tab页（如主页）
    QList<QWidget*> fixedPage;
};

#endif // EDITORTABSWIDGET_H
