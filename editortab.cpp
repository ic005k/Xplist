#include "editortab.h"
#include "ui_editortab.h"

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

extern QUndoGroup *undoGroup;

extern bool defaultIcon;

EditorTab::EditorTab(DomModel *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTab)
{

    ui->setupUi(this);

    chkBox = new QCheckBox(this);
    chkBox->setVisible(false);

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

    delegate1 = new LineEditDelegate(view);

    delegate2 = new ComboBoxDelegate(view);

    delegate_bool = new ComboBoxDelegateBool(view);

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
    if(!defaultIcon)
    view->setStyleSheet(//"QTreeView{background-color: transparent;color:white;font: bold 14px;outline:none;}"
                                        "QTreeView::branch:open:has-children:!has-siblings,"
                                        "QTreeView::branch:open:has-children:has-siblings {image: url(:/new/toolbar/res/sub.png);}"
                                        "QTreeView::branch:has-children:!has-siblings:closed,"
                                        "QTreeView::branch:closed:has-children:has-siblings {image: url(:/new/toolbar/res/main.png);}"
                                        //"QTreeView::item:hover {background-color:rgb(8,52,127);}"
                                        //"QTreeView::item:selected {background-color:rgb(8,52,127);border:1px solid #08347F;}"
                        );
    const QSize size(4, 4);
    view->setIconSize(size);

    connect(model, SIGNAL(itemAdded(const QModelIndex&)), this, SLOT(onItemAded(const QModelIndex&)));

    connect(delegate1, SIGNAL(dataChanged(const QModelIndex&, QString)), this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));
    connect(delegate2, SIGNAL(dataChanged(const QModelIndex&, QString)), this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));
    connect(delegate_bool, SIGNAL(dataChanged(const QModelIndex&, QString)), this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));

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
    //copyAction->setShortcuts(QKeySequence::Copy);
    menu.addAction(copyAction);

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setIcon(QIcon(":/new/toolbar/res/cut.png"));

    menu.addAction(cutAction);

    menu.addSeparator();

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setIcon(QIcon(":/new/toolbar/res/paste.png"));
    //pasteAction->setShortcut(tr("ctrl+v"));
    menu.addAction(pasteAction);

    menu.addSeparator();

    QAction *actionNewSibling = new QAction(tr("New Sibling"), this);
    actionNewSibling->setIcon(QIcon(":/new/toolbar/res/sibling.png"));
    menu.addAction(actionNewSibling);

    QAction *actionNewChild = new QAction(tr("New Child"), this);
    actionNewChild->setIcon(QIcon(":/new/toolbar/res/child.png"));
    menu.addAction(actionNewChild);

    connect(copyAction, &QAction::triggered, this, &EditorTab::on_copyAction);
    connect(cutAction, &QAction::triggered, this, &EditorTab::on_cutAction);
    connect(pasteAction, &QAction::triggered, this, &EditorTab::on_pasteAction);
    connect(expandAction, &QAction::triggered, this, &EditorTab::on_expandAction);
    connect(collapseAction, &QAction::triggered, this, &EditorTab::on_collapseAction);
    connect(actionNewSibling, &QAction::triggered, this, &EditorTab::on_actionNewSibling);
    connect(actionNewChild, &QAction::triggered, this, &EditorTab::on_actionNewChild);

    menu.exec(event->globalPos());

}
#endif // QT_NO_CONTEXTMENU

void EditorTab::onItemAded(const QModelIndex &index)
{
    ui->treeView->resizeColumnToContents(0);

    ui->treeView->expand(index);
    ui->treeView->doItemsLayout();//重要：刷新数据的显示

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
QModelIndex EditorTab::currentIndex() const
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
    DomModel *model = this->model;
    DomItem *item = model->itemForIndex(index);

    if(item->getType() == "bool" && index.column() == 2)
    {
         //connect(chkBox, &QCheckBox::stateChanged, this, &EditorTab::on_chkBox);
        if(!chkBox->isChecked())
        {
            //val_bool = "false";
            //item->setValue("     " + val_bool);

        }
         return;
    }


    //if(item->getType() == "bool")
    //    ui->treeView->setItemDelegateForColumn(2, delegate_bool);
    //else
    //    ui->treeView->setItemDelegateForColumn(2, delegate1);


    /*if(index != index_bool_bak)
    {
        if(index_bool_bak.isValid())
            ui->treeView->setIndexWidget(index_bool_bak, NULL);
    }

    if(index.column() == 1)
    {
        comBox = new QComboBox;
        QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
        comBox->setItemDelegate(itemDelegate);

        //comBox->setMaximumHeight(16);
        QStringList list;
        list << "array" << "dict" << "integer" << "real" << "string" << "data" << "bool" << "date";
        comBox->insertItems(0, list);
        ui->treeView->setIndexWidget(index, comBox);
        index_bool_bak = index;
        QString value = index.data().toString();
        int n = comBox->findText(value);
        comBox->setCurrentIndex(n);
        comBox->showPopup();
    }*/

}

void EditorTab::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

}

void EditorTab::on_treeView_clicked(const QModelIndex &index)
{

    DomModel *model = this->model;
    DomItem *item = model->itemForIndex(index);

    //if(item->getType() == "bool")
    //    ui->treeView->setItemDelegateForColumn(2, delegate_bool);
    //else
    //    ui->treeView->setItemDelegateForColumn(2, delegate1);

    QString str1, str2, str3, str4, str5;
    str1 = QObject::tr("Currently selected: ") + index.data().toString();
    str2 = "      " + QObject::tr("Row: ") + QString::number(index.row() + 1);
    str3 = "      " + QObject::tr("Column: ") + QString::number(index.column() + 1);
    str4 = "      " + QObject::tr("Parent level：") + index.parent().data().toString();
    str5 = "      " + QObject::tr("Children: ") + QString::number(item->childCount());

    //QString   top  =   getTopParent( c_index). data(). toString();
    //str  +=   QStringLiteral( "    顶层节点名：%1\n"). arg( top);

    myStatusBar->showMessage(str1 + str2 + str3 + str5 + str4);

    if(index != index_bool_bak)
    {

        if(index_bool_bak.isValid())
        {
            item_bool  = model->itemForIndex(index_bool_bak);
            item_bool->setValue(val_bool.trimmed());
            //editorDataAboutToBeSet(index_bool_bak, val_bool.trimmed());
            ui->treeView->setIndexWidget(index_bool_bak, NULL);


        }

    }



    if(item->getType() == "bool")
    {

        chkBox = new QCheckBox(this);
        ui->treeView->setFocus();

        connect(chkBox, &QCheckBox::clicked, this, &EditorTab::on_chkBox);
        QModelIndex index_m = model->index(index.row(), 2, index.parent());

        ui->treeView->setIndexWidget(index_m, chkBox);
        chkBox->setGeometry(chkBox->x() + 100, chkBox->y(), chkBox->width(), chkBox->height());
        val_bool = item->getValue();

        QPalette p = chkBox->palette();
        p.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
        p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::white);
        chkBox->setPalette(p);


        if(item->getValue() == "false")
        {
            chkBox->setChecked(false);

            item->setValue("     " + val_bool);

        }
        else if(item->getValue() == "true")
        {
            chkBox->setChecked(true);
            item->setValue("     " + val_bool);


        }

        chkBox->setText("          ");


        index_bool_bak = index_m;

        if(item->getValue().trimmed() == "true" && index.column() == 2)
        {

            chkBox->setChecked(true);

        }
    }


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

        if (model->itemNotPlist(index))
        {
            QUndoCommand *removeCommand = new RemoveCommand(model, index);
            undoGroup->activeStack()->push(removeCommand);
        }

    }
}

void EditorTab::on_pasteAction()
{

    if(copy_item == NULL)
        return;

    DomModel *model;
    QModelIndex index;
    EditorTab *tab = tabWidget->getCurentTab();
    index = tab->currentIndex();
    model = tab->getModel();

    if(index.isValid())
    {

        QUndoCommand *pasteCommand = new PasteCommand(model, index);
        undoGroup->activeStack()->push(pasteCommand);

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

void EditorTab::on_actionNewSibling()
{
    EditorTab *tab = tabWidget->getCurentTab();
    const QModelIndex index = tab->currentIndex();

    if(index.parent().data().toString() == "")
        return;

    if (index.isValid())
    {

        QUndoCommand *addCommand = new AddCommand(tab->getModel(), index.parent());
        undoGroup->activeStack()->push(addCommand);

    }

}

void EditorTab::on_actionNewChild()
{
    EditorTab *tab = tabWidget->getCurentTab();
    const QModelIndex index = tab->currentIndex();

    if (index.isValid())
    {

        QUndoCommand *addCommand = new AddCommand(tab->getModel(), index);
        undoGroup->activeStack()->push(addCommand);

    }

}

void EditorTab::setIcon()
{
    ui->treeView->setIconSize(QSize(6, 6));
}

void EditorTab::on_chkBox()
{
    /*QPalette p = chkBox->palette();
    p.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::white);
    chkBox->setPalette(p);*/

    if(chkBox->isChecked())
    {
        val_bool = "true";
        //chkBox->setText("    ");

    }
    else //if(!chkBox->isChecked())
    {
        val_bool = "false";
        //chkBox->setText("     ");


    }

    chkBox->setText("          ");

    DomModel *model = this->model;
    QModelIndex index = this->currentIndex();


    QModelIndex index_m = model->index(index.row(), 2, index.parent());
    DomItem *item = model->itemForIndex(index_m);

    item->setValue("     " + val_bool);
    //editorDataAboutToBeSet(index, val_bool.trimmed());
    QTreeView *treeView = new QTreeView;
    treeView = (QTreeView*)tabWidget->getCurentTab()->children().at(1);
    treeView->doItemsLayout();



}






