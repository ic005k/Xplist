#include "mytreeview.h"

#include <editortabswidget.h>

//extern EditorTabsWidget* tabWidget;
//extern int dragRow;
//extern int currentRow;
//extern int childCount;

MyTreeView::MyTreeView(QWidget* parent)
    : QTreeView(parent)

{
    setUniformRowHeights(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    //setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropOverwriteMode(false);

    setDragEnabled(true);
    setAcceptDrops(true);
    //viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);

    setAlternatingRowColors(true);
}

void MyTreeView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    //case Qt::Key_Down:
    //case Qt::Key_Up:
    //    break;
    default:
        QTreeView::keyPressEvent(event);
        break;
    }
}

void MyTreeView::dragLeaveEvent(QDragLeaveEvent* event)
{

    //qDebug() << "dragLeaveEvent";
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
