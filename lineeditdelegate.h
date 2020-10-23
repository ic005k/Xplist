#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QLineEdit>
#include <QItemDelegate>

class LineEditDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    LineEditDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    void dataChanged(const QModelIndex&, QString) const;
private:
    static bool checkInput(const QString &type, const QString &val, int col);
};

#endif // LINEEDITDELEGATE_H
