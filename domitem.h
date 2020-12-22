
#ifndef DOMITEM_H
#define DOMITEM_H

#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QString>

class DomItem {
public:
    DomItem(int row, QString n, DomItem* parent = 0);
    DomItem();
    ~DomItem();
    DomItem* child(int i);
    DomItem* parent();
    int row() const;

    DomItem* addChild(int i = -1, DomItem* item = NULL);
    void removeChild(int i);

    void removeFromParent(int row);

    QString getName();
    QString getType();
    QString getValue();

    void setName(QString n);
    void setType(QString t);
    void setValue(QString v);
    void setData(QString n, QString t, QString v);

    int childCount() const;

    DomItem* clone();

    QList<DomItem*> childItems;

    void sort_key(Qt::SortOrder order);
    void sort(int column, Qt::SortOrder order);
    static bool lessThan_key(const DomItem* item1, const DomItem* item2);

private:
    QString name;
    QString type;
    QString value;

    DomItem* parentItem;
    int rowNumber;
};

#endif
