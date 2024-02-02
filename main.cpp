#include <QLibraryInfo>
#include <QTranslator>
#include <QtWidgets/QApplication>

#include "mainwindow.h"
#include "myapp.h"

extern QString fileName, CurVersion;
MainWindow* mw_one;

int main(int argc, char* argv[]) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  {
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#if QT_VERSION_MAJOR < 6 
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
  }
#endif

  // QApplication a(argc, argv);
  MyApplication* a = new MyApplication(argc, argv);
#ifdef Q_OS_MAC
  MainWindow::init_MacVerInfo(CurVersion);
#endif

  static QTranslator translator;  //注意：使translator一直生效
  static QTranslator translator0;
  static QTranslator translator1;
  QLocale locale;
  if (locale.language() == QLocale::English)  //获取系统语言环境
  {
  } else if (locale.language() == QLocale::Chinese) {
    bool tr = false;
    tr = translator.load(":/new/toolbar/res/translation_cn.qm");
    if (tr) {
      qApp->installTranslator(&translator);
    }

    if (translator0.load(":/new/toolbar/res/qt_zh_CN.qm")) {
      qApp->installTranslator(&translator0);
    }

    if (translator1.load(":/new/toolbar/res/widgets_zh_cn.qm")) {
      qApp->installTranslator(&translator1);
    }

  } else if (locale.language() == QLocale::Russian) {
    bool tr = false;
    tr = translator.load(":/new/toolbar/res/translation_ru.qm");
    if (tr) {
      qApp->installTranslator(&translator);
    }
  }

  QFont f;

#ifdef Q_OS_WIN32

  fileName = QString::fromLocal8Bit(argv[1]);  //解决乱码

  f.setFamily("Microsoft YaHei UI");

#endif

#ifdef Q_OS_LINUX

#endif

#ifdef Q_OS_MAC

#endif

  if (!fileName.isEmpty()) {
    a->new_win();
  } else {
    mw_one = new MainWindow();

    mw_one->show();
  }
  f.setPixelSize(12);
  a->setFont(f);
  return a->exec();
}
