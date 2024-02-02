#include "autoupdatedialog.h"

#include "mainwindow.h"
#include "ui_autoupdatedialog.h"

extern MainWindow* mw_one;
extern QString appName;

AutoUpdateDialog::AutoUpdateDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::AutoUpdateDialog) {
  ui->setupUi(this);

  QFileInfo fi(qAppName());
  strLinuxTargetFile = fi.absoluteFilePath();

  setWindowTitle("");
  ui->progressBar->setTextVisible(false);
  Init();
  tempDir = QDir::homePath() + "/tempplist/";
  mw_one->deleteDirfile(tempDir);
  ui->label->setVisible(false);
}

AutoUpdateDialog::~AutoUpdateDialog() { delete ui; }

void AutoUpdateDialog::Init() {
  //"https://raw.fastgit.org/andyvand/"  //HK
  strWinUrl =
      "https://ghproxy.com/https://raw.githubusercontent.com/andyvand/"
      "QtOpenCoreConfigDatabase/main/win.zip";

  strMacUrl =
      "https://ghproxy.com/https://raw.githubusercontent.com/andyvand/"
      "QtOpenCoreConfigDatabase/main/Contents.zip";

  strMacClassicalUrl =
      "https://ghproxy.com/https://github.com/andyvand/QtOpenCoreConfigDatabase/"
      "releases/download/1.0.0/Contents.zip";

  strLinuxUrl =
      "https://ghproxy.com/https://github.com/andyvand/QtOpenCoreConfigDatabase/"
      "releases/download/1.0.0/OCAuxiliaryTools-Linux-x86_64.AppImage";

  strDatabaseUrl =
      "https://ghproxy.com/https://raw.githubusercontent.com/andyvand/"
      "QtOpenCoreConfigDatabase/main/Database.zip";

  manager = new QNetworkAccessManager(this);
  myfile = new QFile(this);
}

void AutoUpdateDialog::doProcessReadyRead()  //读取并写入
{
  while (!reply->atEnd()) {
    QByteArray ba = reply->readAll();
    myfile->write(ba);
  }
}

void AutoUpdateDialog::doProcessFinished() { myfile->close(); }

void AutoUpdateDialog::doProcessDownloadProgress(qint64 recv_total,
                                                 qint64 all_total)  //显示
{
  ui->progressBar->setMaximum(all_total);
  ui->progressBar->setValue(recv_total);
  setWindowTitle(tr("Download Progress") + " : " + GetFileSize(recv_total, 2) +
                 " -> " + GetFileSize(all_total, 2));

  if (recv_total == all_total) {
    if (recv_total < 10000) {
      return;
    }
    ui->btnStartUpdate->setEnabled(true);

    this->repaint();
    if ((mw_one->win) && ui->btnStartUpdate->isVisible()) {
      ui->label->setVisible(false);
    }

    if (mw_one->linuxOS) {
      QProcess* p = new QProcess;
      p->start("chmod", QStringList() << "+x" << tempDir + filename);
    }
  }
}

void AutoUpdateDialog::doProcessError(QNetworkReply::NetworkError code) {
  qDebug() << code;
}

void AutoUpdateDialog::on_btnStartUpdate_clicked() {
  ui->btnStartUpdate->setEnabled(false);
  startUpdate();
}

void AutoUpdateDialog::startUpdate() {
  ui->btnStartUpdate->setEnabled(false);
  this->repaint();

  QString strZip, strPath, strExec, strUnzip;
  QFileInfo appInfo(qApp->applicationDirPath());
  strZip = tempDir + filename;

  QDir dir;
  dir.setCurrent(tempDir);

  qApp->exit();

  if (mw_one->mac || mw_one->osx1012) {
    QString fileName = tempDir + "up.sh";
    QTextEdit* txtEdit = new QTextEdit();
    QString strTarget = appInfo.path().replace("Contents", "");
    strTarget = strTarget + ".";
    strTarget = "\"" + strTarget + "\"";
    if (mw_one->mac) {
      txtEdit->append("hdiutil mount -mountpoint /Volumes/plist " + strZip);
      txtEdit->append(
          "cp -R -p -f "
          "/Volumes/plist/" +
          appName + ".app/. " + strTarget);

      txtEdit->append("hdiutil eject /Volumes/plist");
    }
    if (mw_one->osx1012) {
      txtEdit->append("hdiutil mount -mountpoint /Volumes/plist1012 " + strZip);
      txtEdit->append(
          "cp -R -p -f "
          "/Volumes/plist1012/" +
          appName + ".app/. " + strTarget);

      txtEdit->append("hdiutil eject /Volumes/plist1012");
    }

    strPath = appInfo.path().replace("Contents", "");
    strExec = strPath.mid(0, strPath.length() - 1);
    strExec = "\"" + strExec + "\"";
    txtEdit->append("open " + strExec);

    TextEditToFile(txtEdit, fileName);

    QProcess::startDetached("bash", QStringList() << fileName);
  }

  if (mw_one->win) {
    QString fileName = tempDir + "up.bat";
    strPath = appInfo.filePath();

    QTextEdit* txtEdit = new QTextEdit();
    strUnzip = strPath + "/unzip.exe";
    strUnzip = "\"" + strUnzip + "\"";
    strZip = "\"" + strZip + "\"";
    strPath = "\"" + strPath + "\"";
    strExec = qApp->applicationFilePath();
    strExec = "\"" + strExec + "\"";
    QString strCommand1, strCommand2;
    QString strx = "\"" + tempDir + "\"";
    strCommand1 = strUnzip + " -o " + strZip + " -d " + strx;
    QString stry = tempDir + QFileInfo(filename).baseName();
    stry = "\"" + stry + "\"";
    strCommand2 = "xcopy " + stry + " " + strPath + " /s/y";
    txtEdit->append(strCommand1 + " && " + strCommand2 + " && " + strExec);

    TextEditToFile(txtEdit, fileName);

    QProcess::startDetached("cmd.exe", QStringList() << "/c" << fileName);
  }

  if (mw_one->linuxOS) {
    QString fileName = tempDir + "up.sh";
    QTextEdit* txtEdit = new QTextEdit();
    strZip = "\"" + strZip + "\"";
    strLinuxTargetFile = "\"" + strLinuxTargetFile + "\"";
    txtEdit->append("cp -f " + strZip + " " + strLinuxTargetFile);
    txtEdit->append(strLinuxTargetFile);

    TextEditToFile(txtEdit, fileName);

    QProcess::execute("chmod", QStringList() << "+x" << fileName);
    QProcess::startDetached("bash", QStringList() << fileName);
  }
}

void AutoUpdateDialog::startDownload(bool Database) {
  setWindowTitle("");
  ui->label->setVisible(false);
  ui->btnStartUpdate->setEnabled(false);

  this->repaint();

  if (!Database) {
    ui->btnStartUpdate->setVisible(true);

  } else {
    strUrl = strDatabaseUrl;
    ui->btnStartUpdate->setVisible(false);
  }

  QString str0, str1;
  str0 = "https://download.fastgit.org/";            // 日本东京
  str1 = "https://ghproxy.com/https://github.com/";  // 韩国首尔

  str0 = mw_one->getProxy().trimmed();
  str1 = strUrl;
  str1.replace("https://github.com/", str0);

  QNetworkRequest request;
  request.setUrl(QUrl(str1));

  // github redirects the request, so this attribute must be set to true,
  // otherwise returns nothing from qt5.6
#if QT_VERSION_MAJOR >= 6
  request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, true);
#else
  request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif

  reply = manager->get(request);  //发送请求
  connect(reply, &QNetworkReply::readyRead, this,
          &AutoUpdateDialog::doProcessReadyRead);  //可读
  connect(reply, &QNetworkReply::finished, this,
          &AutoUpdateDialog::doProcessFinished);  //结束
  connect(reply, &QNetworkReply::downloadProgress, this,
          &AutoUpdateDialog::doProcessDownloadProgress);  //大小
  // connect(reply,
  //        QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
  //        this, &AutoUpdateDialog::doProcessError);  //异常
  QStringList list = strUrl.split("/");
  filename = list.at(list.length() - 1);
  QDir dir;
  if (dir.mkpath(tempDir)) {
  }
  QString file = tempDir + filename;

  myfile->setFileName(file);
  bool ret =
      myfile->open(QIODevice::WriteOnly | QIODevice::Truncate);  //创建文件
  if (!ret) {
    QMessageBox::warning(this, "warning", "Failed to open.");
    return;
  }
  ui->progressBar->setValue(0);
  ui->progressBar->setMinimum(0);
}

void AutoUpdateDialog::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event);
  reply->close();
}

QString AutoUpdateDialog::GetFileSize(qint64 size) {
  if (size < 0) return "0";
  if (!size) {
    return "0 Bytes";
  }
  static QStringList SizeNames;
  if (SizeNames.empty()) {
    SizeNames << " Bytes"
              << " KB"
              << " MB"
              << " GB"
              << " TB"
              << " PB"
              << " EB"
              << " ZB"
              << " YB";
  }
  int i = qFloor(qLn(size) / qLn(1024));
  return QString::number(size * 1.0 / qPow(1000, qFloor(i)), 'f',
                         (i > 1) ? 2 : 0) +
         SizeNames.at(i);
}

QString AutoUpdateDialog::GetFileSize(const qint64& size, int precision) {
  double sizeAsDouble = size;
  static QStringList measures;
  if (measures.isEmpty())
    measures << QCoreApplication::translate("QInstaller", "bytes")
             << QCoreApplication::translate("QInstaller", "KiB")
             << QCoreApplication::translate("QInstaller", "MiB")
             << QCoreApplication::translate("QInstaller", "GiB")
             << QCoreApplication::translate("QInstaller", "TiB")
             << QCoreApplication::translate("QInstaller", "PiB")
             << QCoreApplication::translate("QInstaller", "EiB")
             << QCoreApplication::translate("QInstaller", "ZiB")
             << QCoreApplication::translate("QInstaller", "YiB");
  QStringListIterator it(measures);
  QString measure(it.next());
  while (sizeAsDouble >= 1024.0 && it.hasNext()) {
    measure = it.next();
    sizeAsDouble /= 1024.0;
  }
  return QString::fromLatin1("%1 %2")
      .arg(sizeAsDouble, 0, 'f', precision)
      .arg(measure);
}

void AutoUpdateDialog::TextEditToFile(QTextEdit* txtEdit, QString fileName) {
  QFile* file;
  file = new QFile;
  file->setFileName(fileName);
  bool ok = file->open(QIODevice::WriteOnly);
  if (ok) {
    QTextStream out(file);
    out << txtEdit->toPlainText();
    file->close();
    delete file;
  }
}

void AutoUpdateDialog::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      // reply->close();
      // close();
      break;

    case Qt::Key_Return:

      break;

    case Qt::Key_Backspace:

      break;

    case Qt::Key_Space:

      break;

    case Qt::Key_F1:

      break;
  }

  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_M) {
      this->setWindowState(Qt::WindowMaximized);
    }
  }
}
