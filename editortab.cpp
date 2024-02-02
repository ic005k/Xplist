#include "editortab.h"

#include "mainwindow.h"
#include "ui_editortab.h"
#include "ui_mainwindow.h"

extern EditorTabsWidget* tabWidget;

QVector<DomItem*> copy_item;

extern QUndoGroup* undoGroup;
extern MainWindow* mw_one;

QCheckBox* chkBox;
bool chk_null = true;
int childCount = 0;
int currentRow = 0;
bool loading = false;

EditorTab::EditorTab(DomModel* m, QWidget* parent)
    : QWidget(parent), ui(new Ui::EditorTab) {
  ui->setupUi(this);

  loading = true;

  chkBox = new QCheckBox(this);
  chkBox->setVisible(false);

  QSizePolicy policy;
  policy.setHorizontalPolicy(QSizePolicy::Expanding);
  policy.setVerticalPolicy(QSizePolicy::Expanding);

  treeView = new MyTreeView(this);
  treeView->setFont(mw_one->getFont());
  treeView->header()->setDefaultAlignment(Qt::AlignBottom | Qt::AlignLeft);

  lblTips = new QTextBrowser(this);
  lblTips->setLineWrapMode(QTextEdit::NoWrap);
  lblTips->setStyleSheet(
      "QTextEdit{background-color:rgb(250,250,224);color:rgb(5,5,5);}");
  lblTips->setFixedHeight(0);
  lblTips->setHidden(true);

  ui->gridLayout_2->addWidget(treeView);
  ui->gridLayout_2->addWidget(lblTips);

  treeView->setEditTriggers(QAbstractItemView::DoubleClicked |
                            QAbstractItemView::EditKeyPressed);

  treeExpanded = false;

  model = NULL;
  setModel(m);

#ifdef Q_OS_WIN32
  treeView->setColumnWidth(0, 370);
#endif

#ifdef Q_OS_LINUX
  treeView->setColumnWidth(0, 500);
#endif

#ifdef Q_OS_MAC
  treeView->setColumnWidth(0, 420);
#endif

  undoStack = new QUndoStack();
  undoView = new QUndoView(undoStack);
  undoView->setWindowTitle("Command List");
  // undoView->show();
  undoView->setAttribute(Qt::WA_QuitOnClose, false);

  delegate1 = new LineEditDelegate(treeView);

  delegate2 = new ComboBoxDelegate(treeView);

  delegate_bool = new ComboBoxDelegateBool(treeView);

  treeView->expandToDepth(0);

  treeView->setItemDelegateForColumn(0, delegate1);
  treeView->setItemDelegateForColumn(1, delegate2);
  treeView->setItemDelegateForColumn(2, delegate1);
  //不选中一行，分单元格选择
  // treeView->setSelectionBehavior(QAbstractItemView::SelectItems);

  treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);  //选择多行

  connect(model, SIGNAL(itemAdded(const QModelIndex&)), this,
          SLOT(onItemAdded(const QModelIndex&)));

  connect(delegate1, SIGNAL(dataChanged(const QModelIndex&, QString)), this,
          SLOT(editorDataAboutToBeSet(const QModelIndex&, QString)));
  connect(delegate2, SIGNAL(dataChanged(const QModelIndex&, QString)), this,
          SLOT(editorDataAboutToBeSet(const QModelIndex&, QString)));
  connect(delegate_bool, SIGNAL(dataChanged(const QModelIndex&, QString)), this,
          SLOT(editorDataAboutToBeSet(const QModelIndex&, QString)));

  connect(treeView, &QTreeView::clicked, this, &EditorTab::treeView_clicked);
  connect(treeView, &QTreeView::expanded, this, &EditorTab::treeView_expanded);
  connect(treeView, &QTreeView::collapsed, this,
          &EditorTab::treeView_collapsed);
  // connect(treeView, &QTreeView::doubleClicked, this,
  // &EditorTab::treeView_doubleClicked);
  connect(treeView->selectionModel(), &QItemSelectionModel::currentRowChanged,
          this, &EditorTab::slotCurrentRowChanged);
}

EditorTab::~EditorTab() {
  clearModel();
  delete undoStack;
  delete ui;
}

#ifndef QT_NO_CONTEXTMENU
void EditorTab::contextMenuEvent(QContextMenuEvent* event) {
  QMenu menu(treeView);

  QAction* expandAction = new QAction(tr("Expand"), this);
  menu.addAction(expandAction);

  QAction* collapseAction = new QAction(tr("Collapse"), this);
  menu.addAction(collapseAction);

  menu.addSeparator();

  menu.addAction(mw_one->ui->actionCopy);

  menu.addAction(mw_one->ui->actionCut);

  menu.addSeparator();

  menu.addAction(mw_one->ui->actionPaste);

  menu.addAction(mw_one->ui->actionPaste_as_child);

  menu.addSeparator();

  menu.addAction(mw_one->ui->actionNew_Sibling);
  menu.addAction(mw_one->ui->actionNew_Child);
  menu.addAction(mw_one->ui->actionRemove_2);

  menu.addSeparator();

  menu.addAction(mw_one->ui->actionSort);

  connect(expandAction, SIGNAL(triggered()), this, SLOT(on_expandAction()));
  connect(collapseAction, SIGNAL(triggered()), this, SLOT(on_collapseAction()));

  menu.exec(event->globalPos());
}
#endif  // QT_NO_CONTEXTMENU

void EditorTab::onItemAdded(const QModelIndex& index) {
  treeView->resizeColumnToContents(0);

  treeView->expand(index);
  treeView->doItemsLayout();  //重要：刷新数据的显示
}

bool EditorTab::isExpanded() { return treeExpanded; }

void EditorTab::expand() {
  loading = true;

  if (treeExpanded == false) {
    treeExpanded = true;
    // ui->treeView->expandAll(); //性能差

    DomModel* model = this->getModel();
    QModelIndex index;
    index = model->index(0, 0);

    if (index.isValid()) {
      view_expand(index, model);  //性能好
    }

  } else  // if(treeExpanded)
  {
    treeExpanded = false;
    treeView->expandToDepth(0);  //性能好些

    DomModel* model = this->getModel();
    QModelIndex index;
    index = model->index(0, 0);

    if (index.isValid()) {
      // view_collapse(index, model); //性能略差
    }
  }

  treeView->resizeColumnToContents(0);

  loading = false;
}
QModelIndex EditorTab::currentIndex() const {
  QModelIndex i = treeView->currentIndex();
  return QModelIndex(i);
}

DomModel* EditorTab::getModel() { return model; }

void EditorTab::setModel(DomModel* m) {
  if (model != NULL) clearModel();
  this->model = m;

  // proxyModel = new QSortFilterProxyModel(this);
  // proxyModel->setSourceModel(m);

  // m->supportedDragActions();
  treeView->setModel(m);

  // ui->treeView->setModel(proxyModel);

  // ui->treeView->setSortingEnabled(true);
  // ui->treeView->sortByColumn(0, Qt::AscendingOrder);
}

void EditorTab::clearModel() {
  treeView->setModel(NULL);
  delete model;
  model = NULL;
}

void EditorTab::treeView_expanded() {
  if (!loading) treeView->resizeColumnToContents(0);
}

void EditorTab::treeView_collapsed() {
  if (!loading) treeView->resizeColumnToContents(0);
}

QUndoStack* EditorTab::getUndoStack() { return undoStack; }

QString EditorTab::getPath() { return fileInfo.filePath(); }

void EditorTab::setPath(QString p) { fileInfo.setFile(p); }

QString EditorTab::getFileName() {
  // return path.section("/", -1, -1);
  return fileInfo.fileName();
}

void EditorTab::editorDataAboutToBeSet(const QModelIndex& index, QString val) {
  if (model->itemNotPlist(QModelIndex(index))) {
    QUndoCommand* editCommand = new EditCommand(val, model, QModelIndex(index));
    undoStack->push(editCommand);
  }
}

void EditorTab::treeView_doubleClicked(const QModelIndex& index) {
  Q_UNUSED(index);
}

QByteArray EditorTab::HexStrToByte(QString value) {
  QByteArray ba;
  QVector<QString> byte;
  int len = value.length();
  int k = 0;
  ba.resize(len / 2);
  for (int i = 0; i < len / 2; i++) {
    byte.push_back(value.mid(k, 2));
    ba[k / 2] = byte[k / 2].toUInt(nullptr, 16);
    k = k + 2;
  }

  return ba;
}

int EditorTab::hex_to_ascii(QString str) {
  int ch = str.toInt(0, 16);
  if ((ch >= '0') && (ch <= '9')) {
    return ch - 0x30;
  } else if ((ch >= 'A') && (ch <= 'F')) {
    return ch - 'A' + 10;
  } else if ((ch >= 'a') && (ch <= 'f')) {
    return ch - 'a' + 10;
  } else {
    return (-1);
  }
}

void EditorTab::changeDataType(QString txt) {
  DomItem* item;
  item = this->model->itemForIndex(this->currentIndex());
  item->setType(txt);
}

void EditorTab::treeView_clicked(const QModelIndex& index) {
  if (!index.isValid()) return;

  mw_one->clearTreeIndexWidget();

  int c_count = 0;
  DomItem* item;
  DomItem* parentItem;
  DomItem* topParentItem;
  item = this->model->itemForIndex(index);
  parentItem = this->model->itemForIndex(index.parent());
  topParentItem = this->model->itemForIndex(index.parent().parent());

  if (item->getName().contains("Item ") && parentItem->getName() == "Tools" &&
      topParentItem->getName() == "Misc") {
    QModelIndex indexNew = this->model->index(index.row(), 0, index.parent());
    QString str1, str2;

    item = this->model->itemForIndex(indexNew);
    c_count = item->childCount();

    for (int i = 0; i < c_count; i++) {
      QModelIndex myIndex = this->model->index(i, 0, indexNew);
      item = this->model->itemForIndex(myIndex);
      if (item->getName() == "Path") str1 = item->getValue();

      if (item->getName() == "Enabled")
        str2 = "    Enabled: " + item->getValue();
    }

    lblTips->setText(str1 + str2);
    setTipsFixedHeight();
    lblTips->setHidden(false);
  } else

      if (item->getName().contains("Item ") && parentItem->getName() == "Add" &&
          topParentItem->getName() == "Kernel") {
    QModelIndex indexNew = this->model->index(index.row(), 0, index.parent());
    QString str1, str2;

    item = this->model->itemForIndex(indexNew);
    c_count = item->childCount();

    for (int i = 0; i < c_count; i++) {
      QModelIndex myIndex = this->model->index(i, 0, indexNew);
      item = this->model->itemForIndex(myIndex);
      if (item->getName() == "BundlePath") str1 = item->getValue();

      if (item->getName() == "Enabled")
        str2 = "    Enabled: " + item->getValue();
    }

    lblTips->setText(str1 + str2);
    setTipsFixedHeight();
    lblTips->setHidden(false);
  } else

      if (item->getName().contains("Item ") && parentItem->getName() == "Add" &&
          topParentItem->getName() == "ACPI") {
    QModelIndex indexNew = this->model->index(index.row(), 0, index.parent());
    QString str1, str2;

    item = this->model->itemForIndex(indexNew);
    c_count = item->childCount();

    for (int i = 0; i < c_count; i++) {
      QModelIndex myIndex = this->model->index(i, 0, indexNew);
      item = this->model->itemForIndex(myIndex);
      if (item->getName() == "Path") str1 = item->getValue();

      if (item->getName() == "Enabled")
        str2 = "    Enabled: " + item->getValue();
    }

    lblTips->setText(str1 + str2);
    setTipsFixedHeight();
    lblTips->setHidden(false);
  } else

      if (item->getName() == "Add" && item->getType() == "array" &&
          parentItem->getName() == "ACPI") {
    lblTips->setText(tr(
        "Drag and drop one or more aml files to the window to add this file."));
    setTipsFixedHeight();
    lblTips->setHidden(false);
  } else

      if (item->getName() == "Add" && item->getType() == "array" &&
          parentItem->getName() == "Kernel") {
    lblTips->setText(
        tr("Drag and drop one or more kext files to the window to add this "
           "file."));
    setTipsFixedHeight();
    lblTips->setHidden(false);

  } else

      if (item->getName() == "Tools" && item->getType() == "array" &&
          parentItem->getName() == "Misc") {
    lblTips->setText(tr(
        "Drag and drop one or more efi files to the window to add this file."));
    setTipsFixedHeight();
    lblTips->setHidden(false);

  } else

      if (item->getName() == "Drivers" && item->getType() == "array" &&
          parentItem->getName() == "UEFI") {
    lblTips->setText(tr(
        "Drag and drop one or more efi files to the window to add this file."));
    setTipsFixedHeight();
    lblTips->setHidden(false);

  } else if (item->getType() != "data") {
    lblTips->setText("");
    lblTips->setHidden(true);
    mw_one->ui->frameData->setHidden(true);
  }

  mw_one->ui->actionSort->setEnabled(true);

  initBoolWidget(index);

  mw_one->showMsg();

  if (mw_one->ui->actionShowPlistText->isChecked()) mw_one->goPlistText();

  if (item->getType() == "data") {
#if QT_VERSION_MAJOR >= 6
    QString str =
        item->getValue().remove(QRegularExpression("\\s"));  // 16进制去除所有空格

    mw_one->ui->lblBytes->setText(QString::number(str.length() / 2) + " " +
                                  tr("bytes"));
#else
    QString str =
        item->getValue().remove(QRegExp("\\s"));  // 16进制去除所有空格

    mw_one->ui->lblBytes->setText(QString::number(str.count() / 2) + " " +
                                  tr("bytes"));
#endif
        
    mw_one->ui->editHex->setText(str);
    mw_one->ui->editASCII->setText(HexStrToByte(str));
    mw_one->ui->editBase64->setText(HexStrToByte(str).toBase64());
    setTipsFixedHeight();
    lblTips->setHidden(true);
    if (!mw_one->blListItemClick)
      mw_one->ui->frameData->setHidden(false);
    else
      mw_one->ui->frameData->setHidden(true);
  }
}

void EditorTab::setTipsFixedHeight() {
  // 文本高度
  QTextDocument* document = lblTips->document();
  document->setTextWidth(this->width());
  QTextOption op;
  op.setWrapMode(QTextOption::NoWrap);
  document->setDefaultTextOption(op);
  document->adjustSize();
  QAbstractTextDocumentLayout* layout = document->documentLayout();

  int newHeight = layout->documentSize().height();
  lblTips->setFixedHeight(newHeight * 1.0 + 2);
}

void EditorTab::slotCurrentRowChanged(const QModelIndex index,
                                      const QModelIndex& previous) {
  Q_UNUSED(previous);
  if (!loading) treeView_clicked(index);
}

void EditorTab::initBoolWidget(QModelIndex index) {
  DomModel* model = this->model;
  DomItem* item = model->itemForIndex(index);
  if (index != index_bool_bak) {
    if (index_bool_bak.isValid()) {
      item_bool = model->itemForIndex(index_bool_bak);

      if (item_bool->getType() == "bool")
        item_bool->setValue(val_bool.trimmed());

      QModelIndex col3Index =
          model->index(index_bool_bak.row(), 2, index_bool_bak.parent());
      treeView->setIndexWidget(col3Index, NULL);

      chk_null = true;
      index_bool_bak = QModelIndex();
    }
  }

  if (item->getType() == "bool") {
    chkBox = new QCheckBox(this);
    chk_null = false;
    treeView->setFocus();

    // chkBox->setStyleSheet("QCheckBox{Color:rgb(255,255,255);
    // Background-color:rgb(0,0,255); }");

    connect(chkBox, &QCheckBox::clicked, this, &EditorTab::on_chkBox);
    QModelIndex index_m = model->index(index.row(), 2, index.parent());

    treeView->setIndexWidget(index_m, chkBox);

    val_bool = item->getValue();

    if (item->getValue() == "false") {
      chkBox->setChecked(false);
      item->setValue("     " + val_bool);

    } else if (item->getValue() == "true") {
      chkBox->setChecked(true);
      item->setValue("     " + val_bool);
    }

    chkBox->setText("          ");

    index_bool_bak = index_m;

    if (item->getValue().trimmed() == "true" && index.column() == 2) {
      chkBox->setChecked(true);
    }
  }
}

QStandardItem* EditorTab::getTopParent(QStandardItem* item) {
  QStandardItem* secondItem = item;
  while (item->parent() != 0) {
    secondItem = item->parent();
    item = secondItem;
  }
  if (secondItem->index().column() != 0) {
    QStandardItemModel* model =
        static_cast<QStandardItemModel*>(treeView->model());
    secondItem = model->itemFromIndex(
        secondItem->index().sibling(secondItem->index().row(), 0));
  }
  return secondItem;
}

QModelIndex EditorTab::getTopParent(QModelIndex itemIndex) {
  QModelIndex secondItem = itemIndex;
  while (itemIndex.parent().isValid()) {
    secondItem = itemIndex.parent();
    itemIndex = secondItem;
  }
  if (secondItem.column() != 0) {
    secondItem = secondItem.sibling(secondItem.row(), 0);
  }
  return secondItem;
}

void EditorTab::on_copyAction() {
  DomModel* model;
  EditorTab* tab = tabWidget->getCurentTab();
  model = tab->getModel();
  DomItem* item;

  QItemSelectionModel* selections = tab->treeView->selectionModel();
  QModelIndexList selectedsList = selections->selectedRows();

  copy_item.clear();
#if QT_VERSION_MAJOR >= 6
  std::sort(selectedsList.begin(), selectedsList.end());
#else
  std::sort(selectedsList.begin(), selectedsList.end(),
            qGreater<QModelIndex>());
#endif

  foreach (QModelIndex index, selectedsList) {
    item = model->itemForIndex(index);
    if (item->getName() != "plist") {
      if (index.isValid()) {
        copy_item.append(model->copyItem(index));
      }
    }
  }
}

void EditorTab::on_cutAction() {
  QItemSelectionModel* selections = this->treeView->selectionModel();
  QModelIndexList selectedsList = selections->selectedRows();

  copy_item.clear();
#if QT_VERSION_MAJOR >= 6
  std::sort(selectedsList.begin(), selectedsList.end());
#else
  std::sort(selectedsList.begin(), selectedsList.end(),
            qGreater<QModelIndex>());
#endif
  foreach (QModelIndex index, selectedsList) {
    if (index.parent().data().toString() != "")  //最顶层不允许剪切
    {
      if (index.isValid()) {
        copy_item.append(
            model->copyItem(index));  //必须要有克隆的过程，否则粘贴出错);
      }
    }
  }

  mw_one->actionRemove_activated();
}

void EditorTab::on_pasteAction() {
  if (copy_item.at(0) == NULL) return;

  DomModel* model;
  QModelIndex index;
  EditorTab* tab = tabWidget->getCurentTab();
  index = tab->currentIndex();
  model = tab->getModel();

  for (int i = 0; i < copy_item.count(); i++) {
    if (index.isValid()) {
      QUndoCommand* pasteCommand =
          new PasteCommand(model, index, copy_item.at(i));
      undoGroup->activeStack()->push(pasteCommand);
    }
  }

  index = tab->currentIndex();
  treeView_clicked(index);
}

void EditorTab::on_pasteAsChildAction() {
  if (copy_item.at(0) == NULL) return;

  DomModel* model;
  QModelIndex index;
  EditorTab* tab = tabWidget->getCurentTab();
  index = tab->currentIndex();
  model = tab->getModel();

  if (index.isValid()) {
    DomItem* item = model->itemForIndex(index);
    if (item->getType() != "array" && item->getType() != "dict") return;

    QModelIndex childIndex = model->index(0, 0, index);

    for (int i = 0; i < copy_item.count(); i++) {
      if (childIndex.isValid()) {
        QUndoCommand* pasteCommand =
            new PasteCommand(model, childIndex, copy_item.at(i));
        undoGroup->activeStack()->push(pasteCommand);
      } else {
        on_actionNewChild();
        childIndex = model->index(0, 0, index);
        QUndoCommand* pasteCommand =
            new PasteCommand(model, childIndex, copy_item.at(i));
        undoGroup->activeStack()->push(pasteCommand);
        treeView->setCurrentIndex(model->index(1, 0, index));
        mw_one->actionRemove_activated();
      }
    }
  }
}

void EditorTab::view_collapse(const QModelIndex index, DomModel* model) {
  int childCount = model->rowCount(index);

  for (int childNo = 0; childNo < childCount; childNo++) {
    // QModelIndex childIndex = index.child(childNo, 0);
    QModelIndex childIndex = model->index(childNo, 0, index);

    if (model->rowCount(childIndex) > 0) {
      if (treeView->isExpanded(childIndex)) {
        treeView->setExpanded(childIndex, false);

        view_collapse(childIndex, model);
      }
    }
  }
}

void EditorTab::view_expand(const QModelIndex index, DomModel* model) {
  int childCount = model->rowCount(index);

  for (int childNo = 0; childNo < childCount; childNo++) {
    // QModelIndex childIndex = index.child(childNo, 0);
    QModelIndex childIndex = model->index(childNo, 0, index);
    if (model->rowCount(childIndex) > 0) {
      if (!treeView->isExpanded(childIndex)) {
        treeView->setExpanded(childIndex, true);
        // ui->treetreeView->expand(childIndex);

        view_expand(childIndex, model);
      }
    }
  }
}

void EditorTab::on_expandAction() {
  loading = true;

  QModelIndex index = this->currentIndex();
  DomModel* model = this->getModel();
  treeView->expand(index);
  view_expand(index, model);

  treeView->resizeColumnToContents(0);

  loading = false;
}

void EditorTab::on_collapseAction() {
  loading = true;

  QModelIndex index = this->currentIndex();
  DomModel* model = this->getModel();
  view_collapse(index.parent(), model);

  treeView->resizeColumnToContents(0);

  loading = false;
}

void EditorTab::on_actionNewSibling() {
  //增加同级项

  EditorTab* tab = tabWidget->getCurentTab();
  QModelIndex index = tab->currentIndex();
  DomItem* item = tab->model->itemForIndex(index.parent());

  if (index.parent().data().toString() == "") return;

  if (index.isValid()) {
    QUndoCommand* addMoveCommand =
        new AddMoveCommand(tab->getModel(), index.parent());
    undoGroup->activeStack()->push(addMoveCommand);

    // 转到该项
    QModelIndex newIndex =
        tab->model->index(item->childCount() - 1, 0, index.parent());
    treeView->setCurrentIndex(newIndex);
  }
}

void EditorTab::on_actionNewChild() { mw_one->actionAdd_activated(); }

void EditorTab::setIcon() { treeView->setIconSize(QSize(6, 6)); }

void EditorTab::on_chkBox() {
  if (chkBox->isChecked()) {
    val_bool = "true";

  } else  // if(!chkBox->isChecked())
  {
    val_bool = "false";
  }

  chkBox->setText("          ");

  DomModel* model = this->model;
  QModelIndex index = this->currentIndex();

  QModelIndex index_m = model->index(index.row(), 2, index.parent());
  DomItem* item = model->itemForIndex(index_m);

  editorDataAboutToBeSet(index_m, val_bool);

  item->setValue("     " + val_bool);

  treeView->doItemsLayout();
  treeView->setFocus();
}

int EditorTab::getCurrentRow() { return currentRow; }
