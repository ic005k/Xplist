#include "mytreeview.h"
#include <editortabswidget.h>

extern bool defaultIcon;

MyTreeView::MyTreeView(QWidget* parent)
    : QTreeView(parent)

{

    QString strStyle0 =

        "QTreeView::item:hover{background-color:rgba(127,255,0,50)}"

        "QTreeView::item:selected{background-color:rgba(0, 0, 255, 255); "
        "color:rgba(255,255,255,255);}";

    QString strStyle =

        "QTreeView::branch:hover {background-color:rgba(127,255,0,50)}"

        "QTreeView::branch:selected {background: rgb(135,206,250);selection-background-color:rgb(135,206,250);}"

        "QTreeView::item:hover{background-color:rgba(127,255,0,50)}"

        "QTreeView::item:selected{background-color:rgb(135,206,250); "
        "color:rgb(5,5,5);}"

        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings {image: url(:/new/toolbar/res/sub.png);}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {image: "
        "url(:/new/toolbar/res/main.png);}";

    if (!defaultIcon)
        setStyleSheet(strStyle);
    else {
#ifdef Q_OS_WIN32
        setStyleSheet(strStyle0);
#endif
    }

    setUniformRowHeights(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropOverwriteMode(false);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);

    setAlternatingRowColors(true);
}

void MyTreeView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {

    default:
        QTreeView::keyPressEvent(event);
        break;
    }
}

void MyTreeView::dragLeaveEvent(QDragLeaveEvent* event)
{

    QTreeView::dragLeaveEvent(event);
}

void MyTreeView::dragMoveEvent(QDragMoveEvent* event)
{

    setDropIndicatorShown(true);
    QTreeView::dragMoveEvent(event);
}

void MyTreeView::dropEvent(QDropEvent* event)
{

    bool dropOK = false;
    int dropIndicator = dropIndicatorPosition();

    switch (dropIndicator) {
    case QAbstractItemView::AboveItem:
        dropOK = true;
        break;
    case QAbstractItemView::BelowItem:
        dropOK = true;
        break;
    case QAbstractItemView::OnItem:
        dropOK = true;
        break;
    case QAbstractItemView::OnViewport:
        dropOK = false;
        break;
    }
    if (dropOK) {
    }

    QTreeView::dropEvent(event);
    setDropIndicatorShown(false);
}

void MyTreeView::dragEnterEvent(QDragEnterEvent* event)
{

    QTreeView::dragEnterEvent(event);
}
