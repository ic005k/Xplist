#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QAbstractItemView>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QObject>
#include <QTreeView>
#include <QWidget>

typedef struct _NodeP {
    int nRowNo;
    std::vector<_NodeP> arChildren;
} NodeExpand;

class MyTreeView : public QTreeView {
    Q_OBJECT

    enum DropIndicatorPosition {
        OnItem,
        AboveItem,
        BelowItem,
        OnViewport
    };

protected:
    void keyPressEvent(QKeyEvent* event) override;

    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;

public:
    explicit MyTreeView(QWidget* parent);

private:
};

#endif // MYTREEVIEW_H
