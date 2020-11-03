#include "comboboxdelegate.h"
#include <editortabswidget.h>
#include <QDebug>

QComboBox *comboBox;
QString oldValue;
extern int red;
extern EditorTabsWidget *tabWidget;

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
{
    Q_UNUSED(parent);

}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const
{
    Q_UNUSED(index);

    QComboBox *editor = new QComboBox(parent);
    editor = new QComboBox(parent);

    QStringList list;
    list << "array" << "dict" << "integer" << "real" << "string" << "data" << "bool" << "date";
    editor->insertItems(0, list);

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const
{

    comboBox = static_cast<QComboBox*>(editor);

    //QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    //comboBox->setItemDelegate(itemDelegate);

    if(red < 55)  //mac = 50
    {
        comboBox->setStyleSheet("QComboBox {border:none;background:rgba(50,50,50,255);}");

    }
    else
    {
        //comboBox->setStyleSheet("QComboBox {border:none;background:rgba(255,255,255,255);color:rgba(0,0,0,255);}");
        //comboBox->setStyleSheet("QComboBox {border:none;}");
    }

    //connect(comboBox, &QComboBox::currentIndexChanged, this, &ComboBoxDelegate::on_comboBox_currentIndexChanged);
    QString value = index.data().toString();
    int n = comboBox->findText(value);
    comboBox->setCurrentIndex(n);
    oldValue = value;
    comboBox->showPopup();

}

void ComboBoxDelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);
    Q_UNUSED(editor);

    //comboBox = static_cast<QComboBox*>(editor);
    QString val = comboBox->currentText();

    if(oldValue != val)
    {

        DomItem *item;
        EditorTab *tab = tabWidget->getCurentTab();
        DomModel *mymodel = tab->getModel();
        item = mymodel->itemForIndex(index);
        QTreeView *treeView = new QTreeView;
        treeView = (QTreeView*)tab->children().at(1);

        if(item->getName() != "plist")
        {

            if(val == "bool")
            {

                if(item->getValue() == "" || (item->getValue() != "true" && item->getValue() != "false"))
                {

                    QString value = "false";
                    item->setValue(value);

                }

            }

            if(val == "string" || val == "array" || val == "dict" || val == "data")
            {
                QString value = "";
                item->setValue(value);


            }


            if(val == "integer" || val == "real")
            {
                QString value = "0";
                item->setValue(value);
            }


            if(val == "date")
            {
                QString value = QDate::currentDate().toString("yyyy-MM-ddT")+ QTime::currentTime().toString("hh:mm:ssZ");
                item->setValue(value);

            }

            treeView->doItemsLayout();
            treeView->setFocus();

        }

    }

    comboBox->clear();

    // model->setData(index, val, Qt::EditRole);
    if(oldValue != val)
        emit ComboBoxDelegate::dataChanged(QModelIndex(index), val);

}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    QRect r;
    r.setTop(option.rect.top() + 0);
    r.setBottom(option.rect.bottom() + 0);
    r.setRight(option.rect.right());
    r.setLeft(option.rect.left());

    editor->setGeometry(r);
}

void ComboBoxDelegate::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    qDebug() << "test";
}

