#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QTranslator qtTranslator1;
    QTranslator qtTranslator2;

    if (!qtTranslator1.load("qt_" + QLocale::system().name(),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    qtTranslator1.load("qt_ru.qm");


    if (!qtTranslator2.load(":/translation_ru"))
    qtTranslator2.load(":/new/toolbar/res/translation_ru.qm");


    a.installTranslator(&qtTranslator1);
    a.installTranslator(&qtTranslator2);*/


    static QTranslator translator;  //注意：使translator一直生效
    static QTranslator translator0;
    QLocale locale;
    if( locale.language() == QLocale::English )  //获取系统语言环境
    {



    }
    else if( locale.language() == QLocale::Chinese )
    {
        bool tr = false;
        tr = translator.load(":/new/toolbar/res/translation_cn.qm");
        if(tr)
        {
            qApp->installTranslator(&translator);

        }

        if(translator0.load(":/new/toolbar/res/qt_zh_CN.qm"))
        {
            qApp->installTranslator(&translator0);


        }


    }
    else if(locale.language() == QLocale::Russian)
    {
        bool tr = false;
        tr = translator.load(":/new/toolbar/res/translation_ru.qm");
        if(tr)
        {
            qApp->installTranslator(&translator);

        }
    }

    MainWindow w;
    w.show();
    
    return a.exec();
}
