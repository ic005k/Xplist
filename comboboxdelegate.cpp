#include "comboboxdelegate.h"
#include <QDebug>
#include <editortabswidget.h>

extern int red;
extern EditorTabsWidget* tabWidget;

QString oldValue;
QString NewValue;
QComboBox* comboBox;
DomItem* item;
QTreeView* treeView;

ComboBoxDelegate::ComboBoxDelegate(QObject* parent)
{
    Q_UNUSED(parent);
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem& /*option*/,
    const QModelIndex& index) const
{
    Q_UNUSED(index);

    QComboBox* editor = new QComboBox(parent);
    editor = new QComboBox(parent);

    QStringList list;
    list << "array"
         << "dict"
         << "integer"
         << "real"
         << "string"
         << "data"
         << "bool"
         << "date";
    editor->insertItems(0, list);

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget* editor,
    const QModelIndex& index) const
{

    comboBox = static_cast<QComboBox*>(editor);

    //connect(comboBox, SIGNAL(currentIndexChanged(int)), this, &ComboBoxDelegate::on_comboBox_currentIndexChanged);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboBoxChanged(int)));

    //QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    //comboBox->setItemDelegate(itemDelegate);

    if (red < 55) //mac = 50
    {
        comboBox->setStyleSheet("QComboBox {border:none;background:rgba(50,50,50,255);}");

    } else {
        //comboBox->setStyleSheet("QComboBox {border:none;background:rgba(255,255,255,255);color:rgba(0,0,0,255);}");
        //comboBox->setStyleSheet("QComboBox {border:none;}");
    }

    QString value = index.data().toString();
    int n = comboBox->findText(value);
    comboBox->setCurrentIndex(n);
    oldValue = value;
    comboBox->showPopup();
}

void ComboBoxDelegate::setModelData(QWidget* editor,
    QAbstractItemModel* model, const QModelIndex& index) const
{
    Q_UNUSED(model);
    Q_UNUSED(editor);

    //comboBox = static_cast<QComboBox*>(editor);
    QString val = comboBox->currentText();

    if (oldValue != val) {

        EditorTab* tab = tabWidget->getCurentTab();
        DomModel* mymodel = tab->getModel();
        item = mymodel->itemForIndex(index);

        QString CurrentValue = item->getValue();

        QModelIndex index_m = model->index(index.row(), 2, index.parent());
        tab->editorDataAboutToBeSet(index_m, NewValue);

        if (item->getName() != "plist") {

            if (val == "bool") {

                if (oldValue == "real") {
                    double a = CurrentValue.toDouble();
                    int b = a;
                    CurrentValue = QString::number(b);
                }

                bool v = CurrentValue.toInt();
                if (v == 0)
                    NewValue = "false";
                else
                    NewValue = "true";
            }
        }

        if (val == "string") {

            if (oldValue == "data") {
                NewValue = CurrentValue.remove(QRegExp("\\s")); //16进制去除所有空格
                NewValue = HexStrToByte(CurrentValue);

            } else if (oldValue == "bool")
                NewValue = CurrentValue.trimmed();
            else
                NewValue = CurrentValue;
        }

        if (val == "array" || val == "dict") {
            NewValue = "";
        }

        if (val == "data") {
            if (oldValue == "string")
                NewValue = CurrentValue.toUtf8().toHex().toUpper();

            if (oldValue == "integer") {
                int dec = CurrentValue.toInt();
                NewValue = QString("%1").arg(dec, 4, 16, QLatin1Char('0')).toUpper(); // 保留四位，不足补零
            }

            if (oldValue == "real") {
                double r = CurrentValue.toDouble();
                int dec = r;
                NewValue = QString("%1").arg(dec, 4, 16, QLatin1Char('0')).toUpper(); // 保留四位，不足补零
            }
        }

        if (val == "integer") {
            if (CurrentValue.trimmed() == "false")
                CurrentValue = "0";

            if (CurrentValue.trimmed() == "true")
                CurrentValue = "1";

            if (oldValue == "real") {
                double nv = CurrentValue.toDouble();
                qDebug() << nv;
                int ni = nv;
                CurrentValue = QString::number(ni);
            }

            if (oldValue == "data") {
                bool ok;
                QString hex = CurrentValue;
                int dec = hex.toInt(&ok, 16);
                CurrentValue = QString::number(dec);
            }

            NewValue = QString::number(CurrentValue.toInt());
        }

        if (val == "real") {
            if (CurrentValue.trimmed() == "false")
                CurrentValue = "0";
            if (CurrentValue.trimmed() == "true")
                CurrentValue = "1";
            if (oldValue == "data") {
                bool ok;
                QString hex = CurrentValue;
                int dec = hex.toInt(&ok, 16);
                CurrentValue = QString::number(dec);
            }

            double a = CurrentValue.toDouble();

            NewValue = QString::number(a);
        }

        if (val == "date") {
            NewValue = QDate::currentDate().toString("yyyy-MM-ddT") + QTime::currentTime().toString("hh:mm:ssZ");
        }

        item->setValue(NewValue);

        comboBox->clear();

        // model->setData(index, val, Qt::EditRole);
        emit ComboBoxDelegate::dataChanged(QModelIndex(index), val);
    }
}

void ComboBoxDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    QRect r;
    r.setTop(option.rect.top() + 0);
    r.setBottom(option.rect.bottom() + 0);
    r.setRight(option.rect.right());
    r.setLeft(option.rect.left());

    editor->setGeometry(r);
}

void ComboBoxDelegate::OnComboBoxChanged(int index)
{
    emit comboxChanged(index);

    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* mymodel = tab->getModel();
    QModelIndex in;
    item = mymodel->itemForIndex(in);
    treeView = new QTreeView;
    treeView = (QTreeView*)tab->children().at(1);

    QString val = comboBox->currentText();

    treeView->doItemsLayout();
    treeView->setFocus();
}

QByteArray ComboBoxDelegate::HexStrToByte(QString value) const
{
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
