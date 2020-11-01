#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QWidget>
#include <QFileInfo>
#include <QUndoStack>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include <QStandardItem>
#include <QAction>
#include <QMenu>
#include <QStyleFactory>
#include <QObject>
#include <QClipboard>

#include "dommodel.h"
#include "commands.h"
#include "comboboxdelegatebool.h"
#include "lineeditdelegate.h"
#include "comboboxdelegate.h"

namespace Ui {
class EditorTab;
}

class EditorTab : public QWidget
{
    Q_OBJECT
    
public:

    explicit EditorTab(DomModel *model, QWidget *parent = 0);
    ~EditorTab();

    void forEach1(QAbstractItemModel* model, QModelIndex parent = QModelIndex());

    bool isExpanded();
    void expand();

    DomModel *getModel();
    void setModel(DomModel *m);

    QString getPath();
    QString getFileName();
    void setPath(QString p);

    QModelIndex currentIndex() const;

    QUndoStack *getUndoStack();

    void view_collapse(const QModelIndex index, DomModel *model);

    void view_expand(const QModelIndex index, DomModel *model);

    ComboBoxDelegateBool *delegate_bool;
    LineEditDelegate *delegate1;
    ComboBoxDelegate *delegate2;

    void setIcon();


protected:
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

#ifndef QT_NO_CONTEXTMENU
        void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU


public slots:

     void onItemAded(const QModelIndex &index);
     void on_treeView_expanded();
     void on_treeView_collapsed();

     void on_copyAction();
     void on_cutAction();
     void on_pasteAction();
     void on_expandAction();
     void on_collapseAction();

private slots:

     void editorDataAboutToBeSet(const QModelIndex &index, QString val);

     void on_treeView_doubleClicked(const QModelIndex &index);

     void on_treeView_clicked(const QModelIndex &index);

     void on_actionNewSibling();

     void on_actionNewChild();

     void on_chkBox();


private:

    QUndoStack *undoStack;

    Ui::EditorTab *ui;
    bool treeExpanded;
    DomModel *model;
    QFileInfo fileInfo;

    void clearModel();

    QStandardItem* getTopParent(QStandardItem* item);
    QModelIndex getTopParent(QModelIndex itemIndex);

    QCheckBox *chkBox;
    QModelIndex index_bool_bak;
    QString val_bool;
    DomItem *item_bool;

    QComboBox *comBox;


};


#endif // EDITORTAB_H
