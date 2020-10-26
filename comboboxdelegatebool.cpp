#include "comboboxdelegatebool.h"
#include <QDebug>

QCheckBox *chkBox;

ComboBoxDelegateBool::ComboBoxDelegateBool(QObject *parent)
{
    Q_UNUSED(parent);
}

QWidget *ComboBoxDelegateBool::createEditor(QWidget *parent,
    const QStyleOptionViewItem &,
    const QModelIndex &index) const
{
    Q_UNUSED(index);

    //QComboBox *editor = new QComboBox(parent);
    QStringList list;
    list << "false" << "true";
    //editor->insertItems(0, list);

    QCheckBox *editor = new QCheckBox(parent);

    return editor;
}

void ComboBoxDelegateBool::setEditorData(QWidget *editor,
    const QModelIndex &index) const
{
    //QComboBox *comboBox = static_cast<QComboBox*>(editor);
      QString value = index.data().toString();
    //int n = comboBox->findText(value);
    //comboBox->setCurrentIndex(n);

    chkBox = static_cast<QCheckBox*>(editor);
    connect(chkBox, &QCheckBox::clicked, this, &ComboBoxDelegateBool::on_chkBox_clicked);
    chkBox->setGeometry(editor->x() + 60, editor->y(), 100, editor->height());

    if(value == "true")
    {
        chkBox->setChecked(true);

    }
    else
    {
        chkBox->setChecked(false);

    }

    QPalette p = chkBox->palette();
    p.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::white);
    chkBox->setPalette(p);

    if(value == "")
        chkBox->setText("false");
    else
        chkBox->setText(value);

}

void ComboBoxDelegateBool::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);

    //QComboBox *comboBox = static_cast<QComboBox*>(editor);
    //QString val = comboBox->currentText();

    //QCheckBox *chkBox = static_cast<QCheckBox*>(editor);
    chkBox->setGeometry(editor->x() + 60, editor->y(), 100, editor->height());
    QString val;
    if(chkBox->isChecked())
    {
        val = "true";

    }
    else
    {
        val = "false";

    }

    //model->setData(index, val, Qt::EditRole);
    emit ComboBoxDelegateBool::dataChanged(QModelIndex(index), val);
}

void ComboBoxDelegateBool::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}

/*QSize ComboBoxDelegateBool::sizeHint ( const QStyleOptionViewItem & option,  const QModelIndex & index ) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    size.setHeight( size.height() + 4 );
    return size;
}*/

void ComboBoxDelegateBool::on_chkBox_clicked()
{

    QPalette p = chkBox->palette();
    p.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::white);
    chkBox->setPalette(p);

    QString val;
    if(chkBox->isChecked())
    {
        val = "true";

    }
    else
    {
        val = "false";

    }

    chkBox->setText(val);


}

