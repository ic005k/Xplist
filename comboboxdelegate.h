#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QComboBox>
#include <QItemDelegate>
#include <QListView>
#include <QStyledItemDelegate>
#include <QHBoxLayout>

#include <domitem.h>
#include <dommodel.h>


class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    ComboBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void dataChanged(const QModelIndex&, QString) const;

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:

};


#endif // COMBOBOXDELEGATE_H
