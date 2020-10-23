#include "editortab.h"
#include "ui_editortab.h"

#include "comboboxdelegate.h"
#include "lineeditdelegate.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern QStatusBar *myStatusBar;
extern QToolBar *myToolBar;
extern ItemState *copy_state;
extern EditorTabsWidget *tabWidget;
extern DomItem *copy_item;

extern QAction *copyAction;
extern QAction *cutAction;
extern QAction *pasteAction;

EditorTab::EditorTab(DomModel *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTab)
{
    ui->setupUi(this);

    QFont font;
#ifdef Q_OS_WIN32

    font.setPointSize(9);

#endif

#ifdef Q_OS_LINUX
    font.setPointSize(11);

#endif

#ifdef Q_OS_MAC
    font.setPointSize(13);

#endif

    treeExpanded = false;

    model = NULL;

    setModel(m);

    undoStack = new QUndoStack();

    QTreeView *view = ui->treeView;

    LineEditDelegate *delegate1 = new LineEditDelegate(view);

    ComboBoxDelegate *delegate2 = new ComboBoxDelegate(view);

    view->setItemDelegateForColumn(0, delegate1);

    view->setItemDelegateForColumn(1, delegate2);

    view->setItemDelegateForColumn(2, delegate1);

    view->expandToDepth(0);

    view->setFont(font);

    //view->header()->setDefaultSectionSize(150);//表头默认列宽
    view->setColumnWidth(0, 200);
    view->header()->setMinimumHeight(25);//表头高度

    //view->header()->setDefaultAlignment(Qt::AlignCenter); //表头文字默认对齐方式
    //view->header()->setStretchLastSection(true);
    //view->header()->setSortIndicator(0,Qt::AscendingOrder);    //按第1列升序排序

    //view->setStyle(QStyleFactory::create("windows")); //连接的虚线
    //view->setSelectionBehavior(QAbstractItemView::SelectItems);//不选中一行，分单元格选择

    connect(model, SIGNAL(itemAdded(const QModelIndex&)), this, SLOT(onItemAded(const QModelIndex&)));

    connect(delegate1, SIGNAL(dataChanged(const QModelIndex&, QString)), this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));
    connect(delegate2, SIGNAL(dataChanged(const QModelIndex&, QString)), this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));

}

EditorTab::~EditorTab()
{
    clearModel();
    delete undoStack;
    delete ui;
}

#ifndef QT_NO_CONTEXTMENU
void EditorTab::contextMenuEvent(QContextMenuEvent *event)
{

    EditorTab *tab = tabWidget->getCurentTab();
    QTreeView *treeView = new QTreeView;
    treeView = (QTreeView*)tab->children().at(1);

    QMenu menu(treeView);

    QAction *expandAction = new QAction(tr("Expand"), this);
    expandAction->setIcon(QIcon(":/new/toolbar/res/exp.png"));
    menu.addAction(expandAction);

    QAction *collapseAction = new QAction(tr("Collapse"), this);
    collapseAction->setIcon(QIcon(":/new/toolbar/res/col.png"));
    menu.addAction(collapseAction);

    menu.addSeparator();

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setIcon(QIcon(":/new/toolbar/res/copy.png"));
    menu.addAction(copyAction);

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setIcon(QIcon(":/new/toolbar/res/cut.png"));
    menu.addAction(cutAction);

    menu.addSeparator();

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setIcon(QIcon(":/new/toolbar/res/paste.png"));
    menu.addAction(pasteAction);

    connect(copyAction, &QAction::triggered, this, &EditorTab::on_copyAction);
    connect(cutAction, &QAction::triggered, this, &EditorTab::on_cutAction);
    connect(pasteAction, &QAction::triggered, this, &EditorTab::on_pasteAction);
    connect(expandAction, &QAction::triggered, this, &EditorTab::on_expandAction);
    connect(collapseAction, &QAction::triggered, this, &EditorTab::on_collapseAction);

    menu.exec(event->globalPos());

}
#endif // QT_NO_CONTEXTMENU

void EditorTab::onItemAded(const QModelIndex &index)
{
    ui->treeView->expand(index);
}

bool EditorTab::isExpanded()
{
    return treeExpanded;
}

void EditorTab::expand()
{
    if (treeExpanded == false)
    {
        treeExpanded = true;
        //ui->treeView->expandAll(); //性能差

        EditorTab *tab = tabWidget->getCurentTab();
        DomModel *model = tab->getModel();
        QModelIndex index;
        index = model->index(0, 0);

        if(index.isValid())
        {

            view_expand(index, model);//性能好

        }

    }
    else //if(treeExpanded)
    {
        treeExpanded = false;
        ui->treeView->expandToDepth(0);  //性能好些



        EditorTab *tab = tabWidget->getCurentTab();
        DomModel *model = tab->getModel();
        QModelIndex index;
        index = model->index(0, 0);

        if(index.isValid())
        {
            //view_collapse(index, model); //性能略差
        }
    }

    on_treeView_expanded();

}
QModelIndex EditorTab::currentIndex()
{
    QModelIndex i = ui->treeView->currentIndex();
    return QModelIndex(i);
}

DomModel *EditorTab::getModel()
{
    return model;
}

void EditorTab::setModel(DomModel *m)
{
    if (model != NULL) clearModel();
    this->model = m;
    ui->treeView->setModel(m);
}

void EditorTab::clearModel()
{
    ui->treeView->setModel(NULL);
    delete model;
    model = NULL;
}

void EditorTab::on_treeView_expanded()
{
    ui->treeView->resizeColumnToContents(0);

}

void  EditorTab::on_treeView_collapsed()
{
    on_treeView_expanded();

}

QUndoStack *EditorTab::getUndoStack()
{
   return undoStack;
}

QString EditorTab::getPath()
{
    return fileInfo.filePath();
}

void EditorTab::setPath(QString p)
{
    fileInfo.setFile(p);
}

QString EditorTab::getFileName()
{
    //return path.section("/", -1, -1);
    return fileInfo.fileName();
}

void EditorTab::editorDataAboutToBeSet(const QModelIndex &index, QString val)
{
    if (model->itemNotPlist(QModelIndex(index)))
    {
        QUndoCommand *editCommand = new EditCommand(val, model, QModelIndex(index));
        undoStack->push(editCommand);
    }
}



void EditorTab::on_treeView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

}

void EditorTab::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

}

void EditorTab::on_treeView_clicked(const QModelIndex &index)
{
    QModelIndex cindex = index;//this->currentIndex();
    //DomModel *model = this->getModel();

    QString str1, str2, str3, str4;
    str1 = QObject::tr("Currently selected: ") + cindex.data().toString();
    str2 = "      " + QObject::tr("Row: ") + QString::number(cindex.row() + 1);
    str3 = "      " + QObject::tr("Column: ") + QString::number(cindex.column() + 1);
    str4 = "      " + QObject::tr("Parent level：") + cindex.parent().data().toString();

    //QString   top  =   getTopParent( c_index). data(). toString();
    //str  +=   QStringLiteral( "    顶层节点名：%1\n"). arg( top);

    myStatusBar->showMessage(str1 + str2 + str3 + str4);

}

QStandardItem* EditorTab::getTopParent(QStandardItem* item)
{
    QStandardItem* secondItem = item;
    while(item->parent()!= 0)
    {
        secondItem = item->parent();
        item = secondItem;
    }
    if(secondItem->index().column() != 0)
    {
         QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeView->model());
         secondItem = model->itemFromIndex(secondItem->index().sibling(secondItem->index().row(),0));
    }
    return secondItem;
}

QModelIndex EditorTab::getTopParent(QModelIndex itemIndex)
{
    QModelIndex secondItem = itemIndex;
    while(itemIndex.parent().isValid())
    {
        secondItem = itemIndex.parent();
        itemIndex = secondItem;
    }
    if(secondItem.column() != 0)
    {
         secondItem = secondItem.sibling(secondItem.row(),0);
    }
    return secondItem;
}

void EditorTab::on_copyAction()
{

    DomModel *model;
    QModelIndex index;
    EditorTab *tab = tabWidget->getCurentTab();
    index = tab->currentIndex();
    model = tab->getModel();

    if(index.isValid())
    {

        //当前节点
        //DomItem *pCurItem;
        //pCurItem = static_cast<DomItem*>(index.internalPointer());

        copy_item = NULL;
        copy_item = model->copyItem(index);

    }

}

void EditorTab::on_cutAction()
{

    DomModel *model;
    QModelIndex index;
    EditorTab *tab = tabWidget->getCurentTab();
    index = tab->currentIndex();
    model = tab->getModel();

    if(index.parent().data().toString() == "") //最顶层不允许剪切
        return;

    if(index.isValid())
    {

        copy_item = model->copyItem(index);//必须要有克隆的过程，否则粘贴出错

        model->removeItem(index);

    }

}

void EditorTab::on_pasteAction()
{

    if(copy_item == NULL)
        return;

    DomModel *model;
    QModelIndex index;
    /*EditorTab *tab = tabWidget->getCurentTab();
    index = tab->currentIndex();
    model = tab->getModel();*/
    index = this->currentIndex();
    model = this->model;

    const QModelIndex parent = index.parent();

    if(index.isValid())
    {
        //qDebug() << "粘贴的内容" << copy_item->getName();
        model->pasteItem(parent, -1, NULL);

    }

}

void EditorTab::forEach1(QAbstractItemModel* model, QModelIndex parent)
{

    for(int r = parent.row(); r < model->rowCount(parent); ++r) {
            QModelIndex index = model->index(0, 0, parent);
            //QVariant name = model->data(index);
            QString name = model->data(index, Qt::DisplayRole).toString();
            qDebug() << name;

            if( model->hasChildren(index) ) {
                forEach1(model, index);
            }
       }

}

void EditorTab::view_collapse(const QModelIndex index, DomModel *model)
{

    int childCount = model->rowCount(index);
    //qDebug() << childCount;
    for(int childNo=0;childNo<childCount;childNo++)
    {
        QModelIndex childIndex = index.child(childNo,0);
        if(model->rowCount(childIndex) > 0)
        {
            if(ui->treeView->isExpanded(childIndex))
            {
                ui->treeView->setExpanded(childIndex,false);

                view_collapse(childIndex, model);
            }

        }

    }

}

void EditorTab::view_expand(const QModelIndex index, DomModel *model)
{

    int childCount = model->rowCount(index);
    //qDebug() << childCount;
    for(int childNo = 0; childNo < childCount; childNo ++)
    {

        QModelIndex childIndex = index.child(childNo, 0);
        if(model->rowCount(childIndex) > 0)
        {
            if(!ui->treeView->isExpanded(childIndex))
            {
                ui->treeView->setExpanded(childIndex, true);
                //ui->treeView->expand(childIndex);

                view_expand(childIndex, model);

            }

        }
    }

}

void EditorTab::on_expandAction()
{
    QModelIndex index = this->currentIndex();
    DomModel *model = this->getModel();
    ui->treeView->expand(index);
    view_expand(index, model);
}
void EditorTab::on_collapseAction()
{
    QModelIndex index = this->currentIndex();
    DomModel *model = this->getModel();
    view_collapse(index.parent(), model);

}



