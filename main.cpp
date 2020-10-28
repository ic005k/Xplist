#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "mainwindow.h"
#include "myapp.h"

extern QString fileName;
MainWindow * mw_one;


int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    MyApplication *a = new MyApplication(argc, argv);

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

#ifdef Q_OS_WIN32

   fileName = QString::fromLocal8Bit(argv[1]);//解决乱码

#endif

#ifdef Q_OS_LINUX

#endif

#ifdef Q_OS_MAC

#endif

    if(!fileName.isEmpty())
    {
        a->new_win();
    }

    else
    {

        mw_one = new MainWindow();
        mw_one->show();

    }

    //MainWindow w;
    //w.show();
    
    return a->exec();
}
