#include "mytreeview.h"
#include <editortabswidget.h>

extern bool defaultIcon;

MyTreeView::MyTreeView(QWidget* parent)
    : QTreeView(parent)

{
    setUniformRowHeights(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropOverwriteMode(false);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);

    setAlternatingRowColors(true);

    QString strStyle = //"QTreeView{background: rgb(220, 124, 221);}"
        //"QTreeView::item:selected:!active{color:white;background:rgb(220, 124, 221);}"
        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings {image: url(:/new/toolbar/res/sub.png);}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {image: url(:/new/toolbar/res/main.png);}";

    if (!defaultIcon)
        setStyleSheet(strStyle);
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
