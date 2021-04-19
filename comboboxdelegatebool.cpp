#include "comboboxdelegatebool.h"
#include <QDebug>

QCheckBox* chkBoxBool;

ComboBoxDelegateBool::ComboBoxDelegateBool(QObject* parent)
{
    Q_UNUSED(parent);
}

QWidget* ComboBoxDelegateBool::createEditor(QWidget* parent,
    const QStyleOptionViewItem&,
    const QModelIndex& index) const
{
    Q_UNUSED(index);

    QStringList list;
    list << "false"
         << "true";

    QCheckBox* editor = new QCheckBox(parent);

    return editor;
}

void ComboBoxDelegateBool::setEditorData(QWidget* editor,
    const QModelIndex& index) const
{

    QString value = index.data().toString();

    chkBoxBool = static_cast<QCheckBox*>(editor);
    connect(chkBoxBool, &QCheckBox::clicked, this, &ComboBoxDelegateBool::on_chkBox_clicked);
    chkBoxBool->setGeometry(editor->x() + 60, editor->y(), 100, editor->height());

    if (value == "true") {
        chkBoxBool->setChecked(true);

    } else {
        chkBoxBool->setChecked(false);
    }

    QPalette p = chkBoxBool->palette();
    p.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::white);
    chkBoxBool->setPalette(p);
}

void ComboBoxDelegateBool::setModelData(QWidget* editor,
    QAbstractItemModel* model, const QModelIndex& index) const
{
    Q_UNUSED(model);

    chkBoxBool->setGeometry(editor->x() + 60, editor->y(), 100, editor->height());
    QString val;
    if (chkBoxBool->isChecked()) {
        val = "true";

    } else {
        val = "false";
    }

    emit ComboBoxDelegateBool::dataChanged(QModelIndex(index), val);
}

void ComboBoxDelegateBool::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}

void ComboBoxDelegateBool::on_chkBox_clicked()
{

    QPalette p = chkBoxBool->palette();
    p.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::white);
    chkBoxBool->setPalette(p);

    QString val;
    if (chkBoxBool->isChecked()) {
        val = "true";

    } else {
        val = "false";
    }
}
