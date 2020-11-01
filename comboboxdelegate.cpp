#include "comboboxdelegate.h"
#include <editortabswidget.h>
#include <QDebug>

QComboBox *comboBox;
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

    comboBox->showPopup();


    //if (!index.parent().isValid()) comboBox->setEnabled(false);
    //else comboBox->setEnabled(true);

}

void ComboBoxDelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);
    Q_UNUSED(editor);

    //comboBox = static_cast<QComboBox*>(editor);
    QString val = comboBox->currentText();

    if(val == "bool")
    {
        DomItem *item;
        DomModel *mymodel = tabWidget->getCurentTab()->getModel();
        item = mymodel->itemForIndex(index);
        if(item->getValue() == "" || (item->getValue() != "true" && item->getValue() != "false"))
        {
            item->setValue("false");
            QTreeView *treeView = new QTreeView;
            treeView = (QTreeView*)tabWidget->getCurentTab()->children().at(1);
            treeView->doItemsLayout();
        }

    }

    comboBox->clear();

    // model->setData(index, val, Qt::EditRole);
    emit ComboBoxDelegate::dataChanged(QModelIndex(index), val);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    QRect r;
    r.setTop(option.rect.top() + 0);
    r.setBottom(option.rect.bottom() - 0);
    r.setRight(option.rect.right());
    r.setLeft(option.rect.left());

    editor->setGeometry(r);
}

/*QSize ComboBoxDelegate::sizeHint ( const QStyleOptionViewItem & option,  const QModelIndex & index ) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    size.setHeight( size.height() + 5 );
    eturn size;
}*/

void ComboBoxDelegate::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    qDebug() << "test";
}

