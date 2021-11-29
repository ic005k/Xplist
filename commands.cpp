#include "commands.h"

#include "editortab.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

extern QCheckBox* chkBox;
extern bool chk_null;
extern MainWindow* mw_one;
extern EditorTabsWidget* tabWidget;
extern ItemState* AddMoveTemp;

AddCommand::AddCommand(DomModel* model, const QModelIndex& index,
                       QUndoCommand* parent) {
  Q_UNUSED(parent);

  m_model = model;
  m_index = QModelIndex();
  m_parent = index;

  // setText(QObject::tr("Add new item"));
  mw_one->actionUndo->setToolTip(QObject::tr("Undo") + " " +
                                 QObject::tr("Add new item"));
  mw_one->actionRedo->setToolTip(QObject::tr("Redo") + " " +
                                 QObject::tr("Add new item"));
}

void AddCommand::undo() {
  // remove item
  m_model->removeItem(m_index);
}

void AddCommand::redo() {
  /*if (m_index.isValid())
      m_model->addItem(m_parent, m_index.row() + 1);
  else
      m_index = m_model->addItem(m_parent);*/

  m_index = m_model->addItem(m_parent);
}

RemoveCommand::RemoveCommand(DomModel* model, const QModelIndex& index,
                             QUndoCommand* parent) {
  Q_UNUSED(parent);

  m_model = model;
  m_index = index;
  m_parent = index.parent();
  m_row = index.row();
  m_state = model->saveItemState(index);

  // get item name (命令列表）
  const QModelIndex nameIndex = model->index(m_row, 0, m_parent);
  QString name = model->data(nameIndex, Qt::DisplayRole).toString();
  setText(QObject::tr("Remove %1").arg(name));

  mw_one->actionUndo->setToolTip(QObject::tr("Undo") + " " +
                                 QObject::tr("Remove"));
  mw_one->actionRedo->setToolTip(QObject::tr("Redo") + " " +
                                 QObject::tr("Remove"));
}

RemoveCommand::~RemoveCommand() { delete m_state; }

void RemoveCommand::undo() {
  // copy to new item
  m_model->addItem(m_parent, m_row, m_state);
}

void RemoveCommand::redo() {
  // remove item
  m_model->removeItem(m_index);
}

EditCommand::EditCommand(QString val, DomModel* model, const QModelIndex& index,
                         QUndoCommand* parent) {
  Q_UNUSED(parent);

  m_model = model;
  m_index = index;
  m_oldVal = model->data(index, Qt::DisplayRole).toString();
  m_newVal = val;
  // model->setData(index, val);

  QString subject;

  switch (index.column()) {
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

  mw_one->actionUndo->setToolTip(QObject::tr("Undo") + " " +
                                 QObject::tr("Edit item %1").arg(subject));
  mw_one->actionRedo->setToolTip(QObject::tr("Redo") + " " +
                                 QObject::tr("Edit item %1").arg(subject));
}

void EditCommand::undo() {
  m_model->setData(m_index, m_oldVal.trimmed());

  if (!chk_null) {
    if (chkBox->isVisible()) chkBox->setVisible(false);
  }

  tabWidget->getCurentTab()->treeView->clicked(
      tabWidget->getCurentTab()->currentIndex());
}

void EditCommand::redo() {
  if (!m_index.isValid()) return;

  m_model->setData(m_index, m_newVal.trimmed());

  if (!chk_null) {
    if (chkBox->isVisible()) chkBox->setVisible(false);
  }

  tabWidget->getCurentTab()->treeView->clicked(
      tabWidget->getCurentTab()->currentIndex());
}

PasteCommand::PasteCommand(DomModel* model, const QModelIndex& index,
                           DomItem* copy_item, QUndoCommand* parent) {
  Q_UNUSED(parent);

  m_model = model;
  m_index = QModelIndex();
  m_parent = index;
  m_copy_item = copy_item;

  setText(QObject::tr("Paste entry"));

  mw_one->actionUndo->setToolTip(QObject::tr("Undo") + " " +
                                 QObject::tr("Paste entry"));
  mw_one->actionRedo->setToolTip(QObject::tr("Redo") + " " +
                                 QObject::tr("Paste entry"));
}

void PasteCommand::undo() {
  // remove item
  m_model->removeItem(m_index);
}

void PasteCommand::redo() {
  m_index =
      m_model->pasteItem(m_parent.parent(), m_parent.row(), NULL, m_copy_item);
}

AddMoveCommand::AddMoveCommand(DomModel* model, const QModelIndex& index,
                               QUndoCommand* parent) {
  Q_UNUSED(parent);

  m_model = model;
  m_index = QModelIndex();
  m_parent = index;

  setText(QObject::tr("Add new item"));

  mw_one->actionUndo->setToolTip(QObject::tr("Undo") + " " +
                                 QObject::tr("Add new item"));
  mw_one->actionRedo->setToolTip(QObject::tr("Redo") + " " +
                                 QObject::tr("Add new item"));
}

void AddMoveCommand::undo() { m_model->removeItem(m_index); }

void AddMoveCommand::redo() { m_index = m_model->addMoveItem(m_parent); }
