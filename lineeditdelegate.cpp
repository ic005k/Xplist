#include <QTreeView>
#include "lineeditdelegate.h"
#include "dommodel.h"
#include "editortab.h"
#include "editortabswidget.h"
#include "domitem.h"

extern EditorTabsWidget *tabWidget;
QLineEdit *lineEdit;

LineEditDelegate::LineEditDelegate(QObject *parent)
{
    Q_UNUSED(parent);

}

QWidget *LineEditDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const
{

    EditorTab *tab = tabWidget->getCurentTab();
    DomModel *model = tab->getModel();
    const QModelIndex index = tab->currentIndex();
    DomItem *item = model->itemForIndex(index);
    //数组编号不允许编辑
    //if(index.column() == 0 && item->parent()->getType() == "array" && item->getName().mid(0, 4) == "Item")
    //    return 0;

    //父节点的数组和字典值，不允许编辑
    if(index.column() == 2 && (item->getType() == "array" || item->getType() == "dict"))
        return 0;

    if(index.column() == 2 && item->getType() == "bool")
        return 0;

    if(index.column() == 1)
        return 0;

    if(index.data().toString() == "plist")
        return 0;

    QLineEdit *editor = new QLineEdit(parent);

    return editor;

}

void LineEditDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const
{
    lineEdit = static_cast<QLineEdit*>(editor);
    QString value = index.data().toString();

    EditorTab *tab = tabWidget->getCurentTab();
    DomModel *model = tab->getModel();
    //const QModelIndex index = tab->currentIndex();
    DomItem *item = model->itemForIndex(index);
    if(item->getType() == "date" && index.column() == 2)
    {
        if(value == "")//目前格式待定
        //value = QDateTime::currentDateTime().toString("MMM dd,  yyyy at hh:mm:ss");
        value = QDate::currentDate().toString("yyyy-MM-ddT")+ QTime::currentTime().toString("hh:mm:ssZ");

    }

    lineEdit->setText(value);



    /*
    if (!index.parent().isValid()) lineEdit->setEnabled(false);
    else lineEdit->setEnabled(true);
    */
}

void LineEditDelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    // get editor
    //QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    // get old val form the model
    QString oldVal = model->data(index).toString();

    // get new val from editor
    QString newVal = lineEdit->text();

    // get index of type item
    QModelIndex typeIndex = model->index(index.row(), 1, index.parent());

    // get type val
    QString type = model->data(typeIndex).toString();

    // check new value
    QString val = (checkInput(type, newVal, index.column())) ? newVal : oldVal;

    // set data
    //model->setData(index, val, Qt::EditRole);
    emit dataChanged(index, val);
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool LineEditDelegate::checkInput(const QString &type, const QString &val, int col)
{
    bool ok;
    if (col != 2) ok = true;
    else
    {
        if (type == "integer")
        {
            val.toInt(&ok);
        }
        else
        {
            if(type == "real")
            {
                val.toFloat(&ok);
            }
            else
            {
                // we shouldn`t edit values of a dict or array
                ok = ((type == "array" || type == "dict") && col == 2) ? false : true;

            }
        }
    }

    //数组元素数据检查
    EditorTab *tab = tabWidget->getCurentTab();
    DomModel *model = tab->getModel();
    const QModelIndex index = tab->currentIndex();
    DomItem *item = model->itemForIndex(index.parent());
    if(item->getType() == "array" && col == 0)
    {

        QString str = val.mid(4, val.length() - 4);
        //qDebug() << str;
        if(val.mid(0, 5) != "Item " || !str.toInt() || str.toInt() == 0)
            return 0;

    }

    if(type == "date")
    {
        QDateTime date = QDateTime::fromString(val);
        if(!date.isValid())
        {
              //return 0;
        }

    }


    return ok;
}

//行高(以此为准)
QSize LineEditDelegate::sizeHint ( const QStyleOptionViewItem & option,  const QModelIndex & index ) const
{
    QSize size = QItemDelegate::sizeHint(option, index);

#ifdef Q_OS_WIN32
   size.setHeight( size.height() + 4);

#endif

#ifdef Q_OS_LINUX
   size.setHeight( size.height() + 4);

#endif

#ifdef Q_OS_MAC
   size.setHeight( size.height() + 5);

#endif

    return size;
}

