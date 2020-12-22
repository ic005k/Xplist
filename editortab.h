#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStyleFactory>
#include <QUndoStack>
#include <QWidget>

#include "comboboxdelegate.h"
#include "comboboxdelegatebool.h"
#include "commands.h"
#include "dommodel.h"
#include "lineeditdelegate.h"

namespace Ui {
class EditorTab;
}

class EditorTab : public QWidget {
    Q_OBJECT

public:
    explicit EditorTab(DomModel* model, QWidget* parent = 0);
    ~EditorTab();

    QSortFilterProxyModel* proxyModel;

    void forEach1(QAbstractItemModel* model, QModelIndex parent = QModelIndex());

    bool isExpanded();
    void expand();

    DomModel* getModel();
    void setModel(DomModel* m);

    QString getPath();
    QString getFileName();
    void setPath(QString p);

    QModelIndex currentIndex() const;

    QUndoStack* getUndoStack();

    void view_collapse(const QModelIndex index, DomModel* model);

    void view_expand(const QModelIndex index, DomModel* model);

    ComboBoxDelegateBool* delegate_bool;
    LineEditDelegate* delegate1;
    ComboBoxDelegate* delegate2;

    void setIcon();

    void clearModel();

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void dropEvent(QDropEvent* event) override; //放下动作
    void dragEnterEvent(QDragEnterEvent* event) override; //托到进入窗口动作
    void dragMoveEvent(QDragMoveEvent* event) override; //拖着物体在窗口移动
    void dragLeaveEvent(QDragLeaveEvent* event) override; //拖走了没有释放

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent* event) override;
#endif // QT_NO_CONTEXTMENU

public slots:

    void onItemAded(const QModelIndex& index);
    void on_treeView_expanded();
    void on_treeView_collapsed();

    void on_copyAction();
    void on_cutAction();
    void on_pasteAction();
    void on_expandAction();
    void on_collapseAction();

    void on_actionNewSibling();

    void on_actionNewChild();

    void editorDataAboutToBeSet(const QModelIndex& index, QString val);

    void on_treeView_clicked(const QModelIndex& index);

private slots:

    void on_treeView_doubleClicked(const QModelIndex& index);

    void on_chkBox();

private:
    QUndoStack* undoStack;

    Ui::EditorTab* ui;
    bool treeExpanded;
    DomModel* model;
    QFileInfo fileInfo;

    QStandardItem* getTopParent(QStandardItem* item);
    QModelIndex getTopParent(QModelIndex itemIndex);

    QModelIndex index_bool_bak;
    QString val_bool;
    DomItem* item_bool;

    QComboBox* comBox;

    QByteArray HexStrToByte(QString value);
    int hex_to_ascii(QString str);
};

#endif // EDITORTAB_H
