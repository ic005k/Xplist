#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QScrollArea>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStyleFactory>
#include <QTextBrowser>
#include <QTextEdit>
#include <QUndoStack>
#include <QUndoView>
#include <QWidget>

#include "comboboxdelegate.h"
#include "comboboxdelegatebool.h"
#include "commands.h"

#include "dommodel.h"
#include "lineeditdelegate.h"
#include "mytreeview.h"

namespace Ui {
class EditorTab;
}

class EditorTab : public QWidget {
    Q_OBJECT

public:
    explicit EditorTab(DomModel* model, QWidget* parent = 0);
    ~EditorTab();

    void changeDataType(QString txt);
    void setTipsFixedHeight();

    void initBoolWidget(QModelIndex index);

    QTextBrowser* lblTips;

    MyTreeView* treeView;

    int getCurrentRow();

    QSortFilterProxyModel* proxyModel;

    bool isExpanded();
    void expand();

    DomModel* getModel();
    void setModel(DomModel* m);

    QString getPath();
    QString getFileName();
    void setPath(QString p);

    QModelIndex currentIndex() const;

    QUndoStack* getUndoStack();
    QUndoView* undoView;

    void view_collapse(const QModelIndex index, DomModel* model);

    void view_expand(const QModelIndex index, DomModel* model);

    ComboBoxDelegateBool* delegate_bool;
    LineEditDelegate* delegate1;
    ComboBoxDelegate* delegate2;

    void setIcon();

    void clearModel();

    QByteArray HexStrToByte(QString value);

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent* event) override;
#endif // QT_NO_CONTEXTMENU

public slots:

    void onItemAdded(const QModelIndex& index);
    void treeView_expanded();
    void treeView_collapsed();

    void on_copyAction();
    void on_cutAction();
    void on_pasteAction();
    void on_pasteAsChildAction();
    void on_expandAction();
    void on_collapseAction();

    void on_actionNewSibling();

    void on_actionNewChild();

    void editorDataAboutToBeSet(const QModelIndex& index, QString val);

    void treeView_clicked(const QModelIndex& index);

private slots:

    void treeView_doubleClicked(const QModelIndex& index);

    void slotCurrentRowChanged(const QModelIndex index, const QModelIndex& previous);

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

    int hex_to_ascii(QString str);
};

#endif // EDITORTAB_H
