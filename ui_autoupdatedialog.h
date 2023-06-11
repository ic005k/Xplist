/********************************************************************************
** Form generated from reading UI file 'autoupdatedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOUPDATEDIALOG_H
#define UI_AUTOUPDATEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AutoUpdateDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *btnStartUpdate;
    QProgressBar *progressBar;
    QLabel *label;

    void setupUi(QDialog *AutoUpdateDialog)
    {
        if (AutoUpdateDialog->objectName().isEmpty())
            AutoUpdateDialog->setObjectName("AutoUpdateDialog");
        AutoUpdateDialog->resize(508, 150);
        gridLayout = new QGridLayout(AutoUpdateDialog);
        gridLayout->setObjectName("gridLayout");
        btnStartUpdate = new QPushButton(AutoUpdateDialog);
        btnStartUpdate->setObjectName("btnStartUpdate");

        gridLayout->addWidget(btnStartUpdate, 2, 0, 1, 1);

        progressBar = new QProgressBar(AutoUpdateDialog);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 0, 0, 1, 1);

        label = new QLabel(AutoUpdateDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);


        retranslateUi(AutoUpdateDialog);

        QMetaObject::connectSlotsByName(AutoUpdateDialog);
    } // setupUi

    void retranslateUi(QDialog *AutoUpdateDialog)
    {
        AutoUpdateDialog->setWindowTitle(QCoreApplication::translate("AutoUpdateDialog", "Dialog", nullptr));
        btnStartUpdate->setText(QCoreApplication::translate("AutoUpdateDialog", "Close and start upgrade", nullptr));
        label->setText(QCoreApplication::translate("AutoUpdateDialog", "The upgrade will start automatically after closing, \n"
" please turn it on again later.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AutoUpdateDialog: public Ui_AutoUpdateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOUPDATEDIALOG_H
