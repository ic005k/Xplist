#-------------------------------------------------
# Project created by QtCreator 2012-10-06T16:37:20
# 开始升级于2020.10.15
#-------------------------------------------------
QT       += core gui
QT       += xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

ICON = my.icns
RC_FILE += myapp.rc
RC_FILE +=

TARGET = PlistEDPlus
TEMPLATE = app

TRANSLATIONS = translation_ru.ts \
               translation_cn.ts

SOURCES += main.cpp\
    MyTabBar.cpp \
    MyTabPopup.cpp \
    Plist.cpp \
    PlistDate.cpp \
    autoupdatedialog.cpp \
    codeeditor.cpp \
    comboboxdelegatebool.cpp \
    filesystemwatcher.cpp \
    mainwindow.cpp \
    domparser.cpp \
    dommodel.cpp \
    domitem.cpp \
    comboboxdelegate.cpp \
    lineeditdelegate.cpp \
    editortab.cpp \
    editortabswidget.cpp \
    commands.cpp \
    itemstate.cpp \
    myapp.cpp \
    myhighlighter.cpp \
    mytreeview.cpp \
    pugixml.cpp \
    recentfiles.cpp

HEADERS  += mainwindow.h \
    MyTabBar.h \
    MyTabPopup.h \
    Plist.hpp \
    PlistDate.hpp \
    autoupdatedialog.h \
    base64.hpp \
    codeeditor.h \
    comboboxdelegatebool.h \
    domparser.h \
    dommodel.h \
    domitem.h \
    comboboxdelegate.h \
    filesystemwatcher.h \
    lineeditdelegate.h \
    editortab.h \
    editortabswidget.h \
    commands.h \
    itemstate.h \
    myapp.h \
    myhighlighter.h \
    mytreeview.h \
    pugiconfig.hpp \
    pugixml.hpp \
    recentfiles.h

FORMS    += mainwindow.ui \
    autoupdatedialog.ui \
    editortab.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    Info.plist \
    myapp.rc

CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/debug)
} else {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/release)
}

