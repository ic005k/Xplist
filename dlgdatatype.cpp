#include "dlgdatatype.h"
#include "editortab.h"
#include "editortabswidget.h"
#include "ui_dlgdatatype.h"

extern EditorTabsWidget* tabWidget;

dlgDataType::dlgDataType(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::dlgDataType)
{
    ui->setupUi(this);
    this->layout()->setMargin(0);

    QStringList list;
    list << "array"
         << "dict"
         << "integer"
         << "real"
         << "string"
         << "data"
         << "bool"
         << "date";

    ui->listDataType->addItems(list);
}

dlgDataType::~dlgDataType()
{
    delete ui;
}

void dlgDataType::getDataType(QString txt)
{
    for (int i = 0; i < ui->listDataType->count(); i++) {
        if (ui->listDataType->item(i)->text() == txt) {
            ui->listDataType->setCurrentRow(i);
            break;
        }
    }
}

void dlgDataType::on_listDataType_itemClicked(QListWidgetItem* item)
{
    EditorTab* tab = tabWidget->getCurentTab();
    tab->changeDataType(item->text());
    close();
}
