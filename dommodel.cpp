
#include <QtGui>
#include <QtXml>

#include "dommodel.h"
#include "editortab.h"
#include "editortabswidget.h"
#include "mainwindow.h"
extern EditorTabsWidget* tabWidget;
extern QVector<DomItem*> copy_item;
extern bool paste;
extern MainWindow* mw_one;
extern int childCount;
extern QUndoGroup* undoGroup;
extern int currentRow;
extern bool loading;

DomModel::DomModel(QObject* parent)
    : QAbstractItemModel(parent)

{
    rootItem = new DomItem(0, "Root");
    changed = false;
}

DomModel::~DomModel()
{
    delete rootItem;
}

DomItem* DomModel::getRoot()
{
    return rootItem;
}

int DomModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

QVariant DomModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole) {
        if (role == Qt::TextAlignmentRole) {
            if (index.column() == 1) // 设置第二列数据的对齐方式
                return QVariant(Qt::AlignCenter);
        }

        return QVariant();
    }

    DomItem* item = itemForIndex(index);

    QString name = item->getName();
    QString type = item->getType();
    QString value = item->getValue();

    switch (index.column()) {
    case 0:
        return name;
    case 1:
        return type;
    case 2:
        return value;
    default:
        return QVariant();
    }
}

bool DomModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

    if (role) { }

    if (!index.isValid())
        return false;
    else {
        DomItem* item = getItem(index); //itemForIndex(index);

        QString str = value.toString();

        switch (index.column()) {
        case 0:
            item->setName(str);
            break;
        case 1:
            item->setType(str);
            break;
        case 2:
            item->setValue(str);
        };

        emit dataChanged(index, index);
        return true;
    }
}

Qt::ItemFlags DomModel::flags(const QModelIndex& index) const
{

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (role == Qt::TextAlignmentRole)
        return QVariant();

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {

        switch (section) {
        case 0:
            return tr("Key");
        case 1:

            return tr("Type");
        case 2:
            return tr("Value");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex& parent) const

{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem* parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = itemForIndex(parent);

    DomItem* childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DomModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem* childItem = itemForIndex(child);
    DomItem* parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    int row = parentItem->row();

    if (parentItem)
        return createIndex(row, 0, parentItem);

    return QModelIndex();
}

int DomModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem* parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = itemForIndex(parent);

    return parentItem->childCount();
}

QModelIndex DomModel::addItem(const QModelIndex& parent, int row, ItemState* state)
{

    if (parent.isValid()) {
        //QModelIndex index = this->index(parent.row(), 0, parent.parent()); //原始,有导致app崩溃的情况发生
        QModelIndex index = parent; //粘贴时使用，否则导致新建子级项无法撤销（索引无效）
        DomItem* item = itemForIndex(index);

        if (row == -1)
            row = item->childCount();
        DomItem* child = NULL;

        beginInsertRows(index, row, row);

        if (state != NULL) {
            child = state->getState()->clone();
        }
        child = item->addChild(row, child);

        endInsertRows();

        if (item->getType() == "array") {

            for (int i = 0; i < item->childCount(); i++) {

                item->child(i)->setName("Item " + QString::number(i + 1));
            }
        }

        emit itemAdded(index); //通知treeView自适应列宽和展开节点

        return this->index(child->row(), 0, index);
    }

    return QModelIndex();
}

QModelIndex DomModel::addMoveItem(const QModelIndex& parent, int row, ItemState* state)
{

    if (parent.isValid()) {
        //QModelIndex index = this->index(parent.row(), 0, parent.parent()); //原始
        QModelIndex index = parent; //单独用来移动条目
        DomItem* item = itemForIndex(index);

        if (row == -1)
            row = item->childCount();
        DomItem* child = NULL;

        beginInsertRows(index, row, row);

        if (state != NULL) {
            child = state->getState()->clone();
        }
        child = item->addChild(row, child);

        endInsertRows();

        if (item->getType() == "array") {

            for (int i = 0; i < item->childCount(); i++) {

                item->child(i)->setName("Item " + QString::number(i + 1));
            }
        }

        emit itemAdded(index); //通知treeView自适应列宽和展开节点

        return this->index(child->row(), 0, index);
    }

    return QModelIndex();
}

DomItem* DomModel::copyItem(const QModelIndex& parent)
{
    const QModelIndex index = this->index(parent.row(), 0, parent.parent());
    DomItem* item = itemForIndex(index)->clone();

    return item;
}

QModelIndex DomModel::pasteItem(const QModelIndex& parent, int row, ItemState* state, DomItem* copy_item)
{

    Q_UNUSED(state);

    if (parent.isValid()) {

        QModelIndex index = parent;
        DomItem* item = itemForIndex(parent); //父级

        if (row == -1)
            row = item->childCount();

        DomItem* child = NULL;

        beginInsertRows(index, row, row);

        child = item->addChild(row, child);
        QString Name = copy_item->getName();
        if (!mw_one->pasteBW) {
            if (Name.mid(0, 4) == "Item" && item->getType() != "array")
                Name = "_" + Name;
        }
        QString Type = copy_item->getType();
        QString Value = copy_item->getValue();
        if (Type == "bool")
            Value = Value.trimmed();

        int total = 0; //查重
        bool re = false;
        int child_count = item->childCount();
        for (int i = 0; i < child_count; i++) {
            QString str = this->index(i, 0, index).data().toString();
            if (str == Name)
                re = true;
        }

        if (re) {
            for (int i = 0; i < child_count; i++) {
                QString str = this->index(i, 0, index).data().toString();
                if (str.contains(Name)) {
                    total++;
                }
            }
        }

        if (!re)
            child->setData(Name, Type, Value);
        else
            child->setData(Name + "-" + QString::number(total), Type, Value);

        if (item->getType() == "array") {

            child->setData("Item " + QString::number(child_count + 1), copy_item->getType(), copy_item->getValue());

            for (int i = 0; i < item->childCount(); i++) {

                item->child(i)->setName("Item " + QString::number(i + 1));
            }
        }

        DomItem* item0 = NULL;
        DomItem* item1 = NULL;
        DomItem* item2 = NULL;
        DomItem* item3 = NULL;
        DomItem* item4 = NULL;
        DomItem* sub1 = NULL;
        DomItem* sub2 = NULL;
        DomItem* sub3 = NULL;
        DomItem* sub4 = NULL;

        for (int i = 0; i < copy_item->childCount(); i++) {
            QString name = copy_item->childItems.at(i)->getName();
            QString type = copy_item->childItems.at(i)->getType();
            QString value = copy_item->childItems.at(i)->getValue();
            if (type == "bool")
                value = value.trimmed();

            item0 = NULL;
            item0 = child->addChild(i, item0);
            item0->setData(name, type, value); //先增加，再设置数据

            sub1 = copy_item->childItems.at(i);
            if (sub1->childCount() > 0) {
                for (int j = 0; j < sub1->childCount(); j++) {
                    QString name = sub1->childItems.at(j)->getName();
                    QString type = sub1->childItems.at(j)->getType();
                    QString value = sub1->childItems.at(j)->getValue();
                    if (type == "bool")
                        value = value.trimmed();

                    item1 = NULL;
                    item1 = item0->addChild(j, item1);
                    item1->setData(name, type, value); //先增加，再设置数据

                    sub2 = sub1->childItems.at(j);
                    if (sub2->childCount() > 0) {
                        for (int k = 0; k < sub2->childCount(); k++) {
                            QString name = sub2->childItems.at(k)->getName();
                            QString type = sub2->childItems.at(k)->getType();
                            QString value = sub2->childItems.at(k)->getValue();
                            if (type == "bool")
                                value = value.trimmed();

                            item2 = NULL;
                            item2 = item1->addChild(k, item2);
                            item2->setData(name, type, value); //先增加，再设置数据

                            sub3 = sub2->childItems.at(k);
                            if (sub3->childCount() > 0) {
                                for (int l = 0; l < sub3->childCount(); l++) {
                                    QString name = sub3->childItems.at(l)->getName();
                                    QString type = sub3->childItems.at(l)->getType();
                                    QString value = sub3->childItems.at(l)->getValue();
                                    if (type == "bool")
                                        value = value.trimmed();

                                    item3 = NULL;
                                    item3 = item2->addChild(l, item3);
                                    item3->setData(name, type, value); //先增加，再设置数据

                                    sub4 = sub3->childItems.at(l);
                                    if (sub4->childCount() > 0) {
                                        for (int n = 0; n < sub4->childCount(); n++) {
                                            QString name = sub4->childItems.at(n)->getName();
                                            QString type = sub4->childItems.at(n)->getType();
                                            QString value = sub4->childItems.at(n)->getValue();
                                            if (type == "bool")
                                                value = value.trimmed();
                                            item4 = NULL;
                                            item4 = item3->addChild(n, item4);
                                            item4->setData(name, type, value); //先增加，再设置数据
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        endInsertRows();

        emit itemAdded(index); //通知treeView自适应列宽和展开节点

        return this->index(child->row(), 0, index);
    }

    return QModelIndex();
}

void DomModel::removeItem(const QModelIndex& index)
{

    if (index.isValid()) {

        DomItem* item = itemForIndex(index);

        int row = index.row();
        const QModelIndex parent = index.parent();

        //这个函数在父级为index的结构中的第position行删除rows行。形式如下：
        //beginRemoveRows(index, position, position + rows - 1);

        beginRemoveRows(parent, row, row);

        item->removeFromParent(index.row());

        endRemoveRows();

        item = itemForIndex(index.parent());
        if (item->getType() == "array") {

            for (int i = 0; i < item->childCount(); i++) {

                item->child(i)->setName("Item " + QString::number(i + 1));
            }
        }
    }

    //emit itemAdded(index); //通知treeView自适应列宽和展开节点
}

ItemState* DomModel::saveItemState(const QModelIndex& index)
{
    return new ItemState(itemForIndex(index));
}

DomItem* DomModel::itemForIndex(const QModelIndex& index) const
{
    return static_cast<DomItem*>(index.internalPointer());
}

bool DomModel::itemNotPlist(const QModelIndex& index)
{
    return itemForIndex(QModelIndex(index))->getName() != "plist";
}

DomItem* DomModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        DomItem* item = static_cast<DomItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

void DomModel::sort(int column, Qt::SortOrder order)
{

    if (tabWidget->hasTabs()) {

        EditorTab* tab = tabWidget->getCurentTab();
        QModelIndex index = tab->currentIndex();
        if (!index.isValid())
            return;
        index = index.parent();
        if (!index.isValid())
            return;

        loading = true;

        QModelIndex indexBak = tab->currentIndex();
        int rowBak = indexBak.row();

        DomItem* item;
        item = itemForIndex(index);
        DomItem* item1;
        DomItem* item2;

        int count = item->childCount();

        emit layoutAboutToBeChanged();

        if (item->getType() != "array") {
            for (int i = 0; i < count; i++) {
                //tab->treeView->setCurrentIndex(index.child(i, column));
                tab->treeView->setCurrentIndex(tab->getModel()->index(i, column, index));

                item1 = item->child(i);

                if (i + 1 < count)
                    item2 = item->child(i + 1);

                if (i + 1 == count) {

                    break;
                }

                if (order == Qt::AscendingOrder) {

                    if (item1->getName() > item2->getName()) {
                        i = -1;

                        mw_one->on_actionMoveDown();

                        loading = true;
                    }
                }
            }
        }

        emit layoutChanged();

        indexBak = this->index(rowBak, 0, index);
        tab->treeView->setCurrentIndex(indexBak);
        tab->treeView->setFocus();

        loading = false;

        mw_one->showMsg();
    }
}

Qt::DropActions DomModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList DomModel::mimeTypes() const
//当items中的data从model导出时，他们被编译成一个或多个Mime类型
{
    QStringList types;
    types << "application/vnd.text.list";
    //表示plain text，纯文本
    return types;
}

QMimeData* DomModel::mimeData(const QModelIndexList& indexes) const
//将items中的数据，编译成plain text并存储在QMimeData对象中
{

    EditorTab* tab = tabWidget->getCurentTab();

    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;
    //存储“8 bit、以'\0'结尾的 字符串”，比const char*更方便；
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    //二进制的stream，100%的独立于操作系统。
    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }
    mimeData->setData("application/vnd.text.list", encodedData);

    currentRow = indexes.at(0).row();

    if (indexes.at(0).parent().isValid())
        childCount = tab->getModel()->itemForIndex(indexes.at(0).parent())->childCount();

    return mimeData;
}

bool DomModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    Q_UNUSED(data);
    Q_UNUSED(action);

    //将“拖放的数据(dropped data)”插入到Model中

    if (!parent.isValid())
        return false;

    loading = true;

    EditorTab* tab = tabWidget->getCurentTab();
    QModelIndex indexNew;
    indexNew = tab->treeView->currentIndex();

    if (tab->treeView->currentIndex().parent() != parent) //是否同级
        return false;

    if (row == -1) {

        tab->treeView->setFocus();

        tab->treeView->setCurrentIndex(indexNew);
        return false;
    }

    if (row > currentRow) {
        indexNew = tab->getModel()->index(row - 1, column, parent);
    }

    if (row < currentRow) {
        indexNew = tab->getModel()->index(row, column, parent);
    }

    if (row != -1) {
        tab->on_cutAction();
        tab->treeView->setCurrentIndex(indexNew);

        tab->on_pasteAction();
    }

    if (row > currentRow) {
        indexNew = tab->getModel()->index(row - 1, column, parent);
    }
    if (row < currentRow) {
        indexNew = tab->getModel()->index(row, column, parent);
    }

    tab->treeView->setCurrentIndex(indexNew);

    tab->treeView_clicked(tab->treeView->currentIndex());

    loading = false;

    return true;
}
