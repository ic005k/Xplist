#-------------------------------------------------
#
# Project created by QtCreator 2012-10-06T16:37:20
# 开始升级于2020.10.15
#-------------------------------------------------

QT += core gui xml
QT += widgets

ICON = my.icns
RC_FILE += myapp.rc

TARGET = PlistEDPlus
TEMPLATE = app

TRANSLATIONS = translation_ru.ts \
               translation_cn.ts

SOURCES += main.cpp\
    comboboxdelegatebool.cpp \
    mainwindow.cpp \
    domparser.cpp \
    dommodel.cpp \
    domitem.cpp \
    comboboxdelegate.cpp \
    lineeditdelegate.cpp \
    editortab.cpp \
    editortabswidget.cpp \
    commands.cpp \
    itemstate.cpp

HEADERS  += mainwindow.h \
    comboboxdelegatebool.h \
    domparser.h \
    dommodel.h \
    domitem.h \
    comboboxdelegate.h \
    lineeditdelegate.h \
    editortab.h \
    editortabswidget.h \
    commands.h \
    itemstate.h

FORMS    += mainwindow.ui \
    editortab.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    myapp.rc
