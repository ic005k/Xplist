#include "filesystemwatcher.h"
#include "mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMainWindow>

extern MainWindow* mw_one;
extern QVector<QString> openFileList;

FileSystemWatcher* FileSystemWatcher::m_pInstance = NULL;

FileSystemWatcher::FileSystemWatcher(QObject* parent)
    : QObject(parent)
{
}

// 监控文件或目录
void FileSystemWatcher::addWatchPath(QString path)
{
    //qDebug() << QString("Add to watch: %1").arg(path);

    if (m_pInstance == NULL) {
        m_pInstance = new FileSystemWatcher();
        m_pInstance->m_pSystemWatcher = new QFileSystemWatcher();

        // 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(directoryChanged(QString)), m_pInstance, SLOT(directoryUpdated(QString)));
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(fileChanged(QString)), m_pInstance, SLOT(fileUpdated(QString)));
    }

    // 添加监控路径
    m_pInstance->m_pSystemWatcher->addPath(path);

    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir()) {
        const QDir dirw(path);
        m_pInstance->m_currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    }
}

void FileSystemWatcher::removeWatchPath(QString path)
{
    m_pInstance->m_pSystemWatcher->removePath(path);
}

// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void FileSystemWatcher::directoryUpdated(const QString& path)
{
    qDebug() << QString("Directory updated: %1").arg(path);

    // 比较最新的内容和保存的内容找出区别(变化)
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);

    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList); //旧
    //QSet<QString> newDirSet = QSet<QString>(newEntryList.begin(), newEntryList.end());//新

    QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList); //旧
    //QSet<QString> currentDirSet = QSet<QString>(currEntryList.begin(), currEntryList.end());//新

    // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.values();

    // 文件已被移除
    QSet<QString> deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.values();

    // 更新当前设置
    m_currentContentsMap[path] = newEntryList;

    if (!newFile.isEmpty() && !deleteFile.isEmpty()) {
        // 文件/目录重命名
        if ((newFile.count() == 1) && (deleteFile.count() == 1)) {
            qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
        }
    } else {
        // 添加新文件/目录至Dir
        if (!newFile.isEmpty()) {
            qDebug() << "New Files/Dirs added: " << newFile;

            foreach (QString file, newFile) {
                // 处理操作每个新文件....
            }
        }

        // 从Dir中删除文件/目录
        if (!deleteFile.isEmpty()) {
            qDebug() << "Files/Dirs deleted: " << deleteFile;

            foreach (QString file, deleteFile) {
                // 处理操作每个被删除的文件....
            }
        }
    }
}

// 文件修改时调用
void FileSystemWatcher::fileUpdated(const QString& path)
{
    QFileInfo file(path);
    QString strPath = file.absolutePath();
    QString strName = file.fileName();

    //qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);

    if (msgClose) {

        msgClose = false;

        QMessageBox message(QMessageBox::Warning, "", tr("The file has been modified by another program. Do you want to reload?") + "\n\n" + QString("%1").arg(strName));
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setButtonText(QMessageBox::Yes, QString(tr("Yes")));
        message.setButtonText(QMessageBox::No, QString(tr("No")));
        message.setDefaultButton(QMessageBox::Yes);
        int choice = message.exec();
        switch (choice) {
        case QMessageBox::Yes:
            //ReLoad = true;
            mw_one->openPlist(path); //重新装入文件
            msgClose = true;
            break;
        case QMessageBox::No:
            msgClose = true;
            break;
        }
    }

    mw_one->watchFileModification();
}
