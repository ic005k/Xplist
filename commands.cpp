#include "commands.h"

AddCommand::AddCommand(DomModel *model, const QModelIndex &index, QUndoCommand *parent)
{

    Q_UNUSED(parent);

    m_model = model;
    m_index = QModelIndex();
    m_parent = index;

    // maybe we should create permanent index?
    setText(QObject::tr("Add new item"));
}

void AddCommand::undo()
{
    // remove item
    m_model->removeItem(m_index);
}

void AddCommand::redo()
{

    //if (m_index.isValid()) m_model->addItem(m_parent, m_index.row());
    //    else m_index = m_model->addItem(m_parent);

    m_index = m_model->addItem(m_parent);


}

RemoveCommand::RemoveCommand(DomModel *model, const QModelIndex &index, QUndoCommand *parent)
{
    Q_UNUSED(parent);

    m_model = model;
    m_index = index;
    m_parent = index.parent();
    m_row = index.row();
    m_state = model->saveItemState(index);

    // get item name
    const QModelIndex nameIndex = model->index(m_row, 0, m_parent);
    QString name = model->data(nameIndex, Qt::DisplayRole).toString();

    setText(QObject::tr("Remove %1").arg(name));
}

RemoveCommand::~RemoveCommand()
{
    delete m_state;
}

void RemoveCommand::undo()
{
    // copy to new item
    m_model->addItem(m_parent, m_row, m_state);

}

void RemoveCommand::redo()
{
    // remove item
    m_model->removeItem(m_index);

}

EditCommand::EditCommand(QString val, DomModel *model, const QModelIndex &index, QUndoCommand *parent)
{
    m_model = model;
    m_index = index;
    m_oldVal = model->data(index, Qt::DisplayRole).toString();
    m_newVal = val;
    //model->setData(index, val);

    QString subject;

    switch (index.column())
    {
        case 0:
            subject = QObject::tr("key");
        break;

        case 1:
            subject = QObject::tr("type");
        break;

        case 2:
            subject = QObject::tr("value");
    };

    setText(QObject::tr("Edit item %1").arg(subject));

}

void EditCommand::undo()
{
    m_model->setData(m_index, m_oldVal);
}

void EditCommand::redo()
{
    if(!m_index.isValid())
        return;

    m_model->setData(m_index, m_newVal);
}

PasteCommand::PasteCommand(DomModel *model, const QModelIndex &index, QUndoCommand *parent)
{

    Q_UNUSED(parent);

    m_model = model;
    m_index = QModelIndex();
    m_parent = index;

    // maybe we should create permanent index?
    setText(QObject::tr("Paste entry"));

}

void PasteCommand::undo()
{
    // remove item
    m_model->removeItem(m_index);
}

void PasteCommand::redo()
{

    m_index = m_model->pasteItem(m_parent.parent(), -1, NULL);

}
