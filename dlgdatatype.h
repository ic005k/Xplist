#ifndef DLGDATATYPE_H
#define DLGDATATYPE_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class dlgDataType;
}

class dlgDataType : public QDialog {
    Q_OBJECT

public:
    explicit dlgDataType(QWidget* parent = nullptr);
    ~dlgDataType();

    void getDataType(QString txt);

private slots:
    void on_listDataType_itemClicked(QListWidgetItem* item);

private:
    Ui::dlgDataType* ui;
};

#endif // DLGDATATYPE_H
