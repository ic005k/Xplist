#include "comboboxdelegate.h"
#include <QDebug>
#include <QListWidget>
#include <editortabswidget.h>

extern int red;
extern EditorTabsWidget* tabWidget;

QString oldValue;
QString NewValue;
QComboBox* comboBox;

DomItem* item;

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

    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboBoxChanged(int)));

    if (red < 55) //mac = 50
    {
        comboBox->setStyleSheet("QComboBox {border:1px solid gray;background:rgba(50,50,50,255);selection-color: #277BFF;selection-background-color:#FFFFFF;}");
    } else {
        comboBox->setStyleSheet("QComboBox {border:1px solid gray;background:rgba(255,255,255,255);color:rgba(0,0,0,255);selection-color: #FFFFFF;selection-background-color:#277BFF;}");
    }

    QString value = index.data().toString();
    int n;
    n = comboBox->findText(value);
    comboBox->setCurrentIndex(n);

    oldValue = value;
    comboBox->showPopup();
}

void ComboBoxDelegate::setModelData(QWidget* editor,
    QAbstractItemModel* model, const QModelIndex& index) const
{
    Q_UNUSED(model);
    Q_UNUSED(editor);

    comboBox = static_cast<QComboBox*>(editor);
    QString val;
    val = comboBox->currentText();

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
                    qlonglong b = a;
                    CurrentValue = QString::number(b);
                }

                bool v = CurrentValue.toULongLong();
                if (v <= 0)
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
                qulonglong dec = CurrentValue.toULongLong();
                NewValue = QString("%1").arg(dec, 4, 16, QLatin1Char('0')).toUpper(); // 保留四位，不足补零
            }

            if (oldValue == "real") {
                qreal r = CurrentValue.toDouble();
                qulonglong dec = r;
                NewValue = QString("%1").arg(dec, 4, 16, QLatin1Char('0')).toUpper(); // 保留四位，不足补零
            }

            if (oldValue == "bool") {
                int dec;
                if (CurrentValue.trimmed() == "true") {
                    dec = 1;
                }
                if (CurrentValue.trimmed() == "false") {
                    dec = 0;
                }

                NewValue = QString("%1").arg(dec, 4, 16, QLatin1Char('0')).toUpper();
            }

            if (oldValue == "dict" || oldValue == "array") {
                NewValue = "";
            }

            if (oldValue == "date") {
                NewValue = CurrentValue.toUtf8().toHex().toUpper();
            }
        }

        if (val == "integer") {
            if (CurrentValue.trimmed() == "false")
                CurrentValue = "0";

            if (CurrentValue.trimmed() == "true")
                CurrentValue = "1";

            if (oldValue == "real") {
                float nv = CurrentValue.toFloat();
                qulonglong ni = nv;
                CurrentValue = QString::number(ni);
            }

            if (oldValue == "data") {
                bool ok;
                QString hex = CurrentValue;
                qulonglong dec = hex.toULongLong(&ok, 16);
                CurrentValue = QString::number(dec);
            }

            NewValue = QString::number(CurrentValue.toULongLong());
        }

        if (val == "real") {
            if (CurrentValue.trimmed() == "false")
                CurrentValue = "0";
            if (CurrentValue.trimmed() == "true")
                CurrentValue = "1";
            if (oldValue == "data") {
                bool ok;
                QString hex = CurrentValue;
                qulonglong dec = hex.toULongLong(&ok, 16);
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

        emit ComboBoxDelegate::dataChanged(QModelIndex(index), val);
    }
}

void ComboBoxDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    QRect r;
    r.setTop(option.rect.top() + 0);
    r.setBottom(option.rect.bottom() - 0);
    r.setRight(option.rect.right() - 0);
    r.setLeft(option.rect.left() + 0);

    editor->setGeometry(r);
}

void ComboBoxDelegate::OnComboBoxChanged(int index)
{
    emit comboxChanged(index);

    EditorTab* tab = tabWidget->getCurentTab();
    DomModel* mymodel = tab->getModel();
    QModelIndex in;
    item = mymodel->itemForIndex(in);

    tab->treeView->doItemsLayout();
    tab->treeView->setFocus();
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
