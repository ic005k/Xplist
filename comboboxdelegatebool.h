#ifndef COMBOBOXDELEGATEBOOL_H
#define COMBOBOXDELEGATEBOOL_H

#include <QComboBox>
#include <QItemDelegate>
#include <QCheckBox>

class ComboBoxDelegateBool : public QItemDelegate
{
    Q_OBJECT

public:
    ComboBoxDelegateBool(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //QSize sizeHint ( const QStyleOptionViewItem & option,  const QModelIndex & index ) const;

signals:
    void dataChanged(const QModelIndex&, QString) const;

private slots:
    void on_chkBox_clicked();
};

#endif // COMBOBOXDELEGATEBOOL_H
