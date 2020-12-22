
#include "domitem.h"
#include <QTime>

DomItem::DomItem(int row, QString n, DomItem* parent)
{
    // Record the item's location within its parent.
    rowNumber = row;
    parentItem = parent;

    // fill data
    type = "string";
    value = "";
    name = n;
}

DomItem::DomItem() { }

DomItem::~DomItem()
{
    while (!childItems.isEmpty())
        delete childItems.takeFirst();
}

DomItem* DomItem::parent()
{
    return parentItem;
}

DomItem* DomItem::child(int i)
{
    if ((!childItems.count()) <= i)
        return childItems.at(i);

    return 0;
}

DomItem* DomItem::addChild(int i, DomItem* item)
{
    if (i == -1)
        i = childItems.size();
    if (item == NULL) {
        item = new DomItem(i, QObject::tr("NewItem") + " " + QString::number(i + 1), this);
    }

    if (item->parent()->getType() == "array") //数组单独采用"Item"加空格的方式
    {
        if (item == NULL)
            item = new DomItem(i, "Item " + QString::number(i + 1), this);
    }

    childItems.insert(i, item);
    return item;
}

void DomItem::removeChild(int i)
{
    //DomItem *item = childItems[i]; //此处无需重新定义item，特别是后面的delete item
    childItems.removeAt(i);
    //delete item;
}

void DomItem::removeFromParent(int row)
{
    parentItem->removeChild(row);
}

int DomItem::childCount() const
{
    return childItems.count();
}

int DomItem::row() const
{
    return rowNumber;
}

QString DomItem::getName()
{
    return name;
}

QString DomItem::getType()
{
    return type;
}

QString DomItem::getValue()
{
    return value;
}

void DomItem::setName(QString n)
{
    name = n;
}

void DomItem::setType(QString t)
{
    type = t;
}

void DomItem::setValue(QString v)
{
    value = v;
}

void DomItem::setData(QString n, QString t, QString v)
{
    name = n;
    type = t;
    value = v;
}

DomItem* DomItem::clone()
{
    // create new item
    DomItem* newItem = new DomItem();

    // fill the fields
    newItem->rowNumber = this->rowNumber;
    newItem->parentItem = this->parentItem;

    newItem->type = this->type;
    newItem->value = this->value;
    newItem->name = this->name;

    // copy children
    for (int i = 0; i < childItems.size(); ++i) {
        DomItem* item = childItems[i]->clone();
        newItem->childItems.append(item);
        item->parentItem = newItem;
    }

    return newItem;
}

bool DomItem::lessThan_key(const DomItem* item1, const DomItem* item2)
{

    return (item1->name < item2->name);
}

// 对key列执行排序
void DomItem::sort_key(Qt::SortOrder order)
{

    QList<DomItem*> _children; // 子节点
    if (order == Qt::AscendingOrder) // 升序
    {
        std::sort(_children.begin(), _children.end(), lessThan_key);

    } else // 降序
    {
        std::sort(_children.rbegin(), _children.rend(), lessThan_key);
    }
}

void DomItem::sort(int column, Qt::SortOrder order)
{

    if (column == 0)
        sort_key(order);
}
