#include "lineeditdelegate.h"
#include "domitem.h"
#include "dommodel.h"
#include "editortab.h"
#include "editortabswidget.h"
#include <QCompleter>
#include <QTreeView>

extern EditorTabsWidget* tabWidget;
QLineEdit* lineEdit;
void setTextCompleter(QLineEdit* editor);

LineEditDelegate::LineEditDelegate(QObject* parent)
{
    Q_UNUSED(parent);
}

QWidget* LineEditDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem& /*option*/,
    const QModelIndex& /*index*/) const
{

    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* model = tab->getModel();
    QModelIndex index = tab->currentIndex();
    DomItem* item = model->itemForIndex(index);

    if (index.column() == 1) {
        return 0;
    }

    //父节点的数组和字典值，不允许编辑
    if (index.column() == 2 && (item->getType() == "array" || item->getType() == "dict"))
        return 0;

    if (index.column() == 2 && item->getType() == "bool")
        return 0;

    if (index.data().toString() == "plist")
        return 0;

    QLineEdit* editor = new QLineEdit(parent);

    return editor;
}

void LineEditDelegate::setEditorData(QWidget* editor,
    const QModelIndex& index) const
{
    lineEdit = static_cast<QLineEdit*>(editor);
    connect(lineEdit, &QLineEdit::editingFinished, this, &LineEditDelegate::on_setText);
    connect(lineEdit, &QLineEdit::textChanged, this, &LineEditDelegate::on_textChanged);

    QString value = index.data().toString();

    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* model = tab->getModel();
    DomItem* item = model->itemForIndex(index);

    if (item->getType() == "string")
        setTextCompleter(lineEdit);

    if (item->getType() == "data") {
        QRegExp regx("[A-Fa-f0-9]{2,1024}");
        QValidator* validator = new QRegExpValidator(regx, lineEdit);
        lineEdit->setValidator(validator);
        //lineEdit->setPlaceholderText(tr("Hexadecimal"));
    }

    if (item->getType() == "date" && index.column() == 2) {
        if (value == "")
            value = QDate::currentDate().toString("yyyy-MM-ddT")
                + QTime::currentTime().toString("hh:mm:ssZ");
    }

    lineEdit->setText(value);
}

void LineEditDelegate::setModelData(QWidget* editor,
    QAbstractItemModel* model, const QModelIndex& index) const
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
    if (newVal != oldVal)
        emit dataChanged(index, val);
}

void LineEditDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool LineEditDelegate::checkInput(const QString& type, const QString& val, int col)
{
    bool ok;
    if (col != 2)
        ok = true;
    else {
        if (type == "integer") {
            val.toULongLong(&ok);
        } else {
            if (type == "real") {
                val.toFloat(&ok);
            } else {
                // we shouldn`t edit values of a dict or array
                ok = ((type == "array" || type == "dict") && col == 2) ? false : true;
            }
        }
    }

    //数组元素数据检查
    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* model = tab->getModel();
    const QModelIndex index = tab->currentIndex();
    DomItem* item = model->itemForIndex(index.parent());
    if (item->getType() == "array" && col == 0) {

        QString str = val.mid(4, val.length() - 4);

        if (val.mid(0, 5) != "Item " || !str.toInt() || str.toInt() == 0)
            return 0;
    }

    if (type == "date") {
        QDateTime date = QDateTime::fromString(val);
        if (!date.isValid()) {
            //return 0;
        }
    }

    if (val.trimmed() == "plist")
        return 0;

    if (val.trimmed().mid(0, 4) == "Item" && item->getType() != "array")
        return 0;

    return ok;
}

//行高(以此为准)
QSize LineEditDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QItemDelegate::sizeHint(option, index);

#ifdef Q_OS_WIN32
    size.setHeight(size.height() + 4);

#endif

#ifdef Q_OS_LINUX
    size.setHeight(size.height() + 4);

#endif

#ifdef Q_OS_MAC
    size.setHeight(size.height() + 6);

#endif

    return size;
}

void setTextCompleter(QLineEdit* editor)
{
    QStringList textList;

    textList.append("Any");
    textList.append("i386");
    textList.append("x86_64");
    textList.append("layout-id");
    textList.append("Auto");
    textList.append("i386-user32");
    textList.append("Cacheless");
    textList.append("Mkext");
    textList.append("Prelinked");
    textList.append("None");
    textList.append("RTC");
    textList.append("NVRAM");
    textList.append("Disabled");
    textList.append("Full");
    textList.append("Short");
    textList.append("Default");
    textList.append("Builtin");
    textList.append("External");
    textList.append("Apple");
    textList.append("Old");
    textList.append("Modern");
    textList.append("Signed");
    textList.append("Secure");
    textList.append("Optional");
    textList.append("Basic");
    textList.append("Create");
    textList.append("TryOverwrite");
    textList.append("Overwrite");
    textList.append("Custom");
    textList.append("Upgradable");
    textList.append("Soldered");
    textList.append("Enabled");
    textList.append("V1");
    textList.append("V2");
    textList.append("AMI");
    textList.append("ASUS");
    textList.append("Max");
    textList.append("BuiltinGraphics");
    textList.append("SystemGraphics");
    textList.append("SystemText");
    textList.append("SystemGeneric");
    textList.append("System");
    textList.append("OEM");

    QCompleter* editFindCompleter = new QCompleter(textList);
    editFindCompleter->setCaseSensitivity(Qt::CaseSensitive);
    editFindCompleter->setCompletionMode(QCompleter::PopupCompletion);
    editor->setCompleter(editFindCompleter);
}

void LineEditDelegate::on_setText()
{
    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* model = tab->getModel();
    QModelIndex index = tab->currentIndex();
    DomItem* item = model->itemForIndex(index);

    if (item->getType() == "data") {
        QString str = lineEdit->text();
        lineEdit->setText(str.toUpper());
    }
}

void LineEditDelegate::on_textChanged() { }
