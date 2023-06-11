/********************************************************************************
** Form generated from reading UI file 'editortab.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORTAB_H
#define UI_EDITORTAB_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorTab
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;

    void setupUi(QWidget *EditorTab)
    {
        if (EditorTab->objectName().isEmpty())
            EditorTab->setObjectName("EditorTab");
        EditorTab->resize(400, 300);
        EditorTab->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(EditorTab);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(2, 2, 2, 2);
        frame = new QFrame(EditorTab);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);

        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(EditorTab);

        QMetaObject::connectSlotsByName(EditorTab);
    } // setupUi

    void retranslateUi(QWidget *EditorTab)
    {
        EditorTab->setWindowTitle(QCoreApplication::translate("EditorTab", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditorTab: public Ui_EditorTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORTAB_H
