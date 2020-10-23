#include "comboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent) {}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);

    QStringList list;
    list << "array" << "dict" << "integer" << "real" << "string" << "data" << "bool" << "date";
    editor->insertItems(0, list);

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = index.data().toString();
    int n = comboBox->findText(value);
    comboBox->setCurrentIndex(n);

    /*
    if (!index.parent().isValid()) comboBox->setEnabled(false);
    else comboBox->setEnabled(true);
    */
}

void ComboBoxDelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString val = comboBox->currentText();

    // model->setData(index, val, Qt::EditRole);
    emit ComboBoxDelegate::dataChanged(QModelIndex(index), val);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
