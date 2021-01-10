#include "editortabswidget.h"
#include "MyTabBar.h"
#include "MyTabPopup.h"
#include "filesystemwatcher.h"
#include "mainwindow.h"

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QSettings>
#include <QTabBar>

QString dragFileName;

extern MainWindow* mw_one;
extern EditorTabsWidget* tabWidget;
extern QVector<QString> openFileList;

EditorTabsWidget::EditorTabsWidget(QWidget* parent)
{
    Q_UNUSED(parent);

    this->setMovable(true);
    this->setTabsClosable(true);

    setAttribute(Qt::WA_StyledBackground);

    initTabBar();

    QString tabBarStyle0 = "QTabBar::tab {min-width:100px;border: 0px solid;border-top-left-radius: 5px;border-top-right-radius: 5px;padding:2px;}\
            QTabBar::tab:!selected {margin-top: 2px;}\
            QTabWidget::pane{border:none;}\
            QTabBar::tab{background:transparent;color:black;}\
            QTabBar::tab:hover{background:rgba(0, 0, 255, 80);}\
            QTabBar::tab:selected{border-color: white;background:rgba(0, 0, 255, 255);color:white;}";

    QString tabBarStyle1 = "QTabBar::tab {min-width:100px;color: rgba(150,150,150);border: 2px solid;border-top-left-radius: 10px;border-top-right-radius: 10px;padding:5px;}\
            QTabBar::tab:!selected {margin-top: 5px;} \
            QTabBar::tab:selected {color: rgba(255,255,255);}";

    QString tabBarStyle2 = "QTabBar::tab {min-width:100px;color: black;background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #eeeeee, stop: 1 gray);border: 0px solid;border-top-left-radius: 10px;border-top-right-radius: 10px;padding:5px;}\
            QTabBar::tab:!selected {margin-top: 5px;} \
            QTabBar::tab:selected {color: blue;}";

    QString tabBarStyle3 = "QTabWidget::pane{border:none;}\
            QTabWidget::tab-bar{alignment:left;}\
            QTabBar::tab{background:transparent;color:black;}\
            QTabBar::tab:hover{background:rgba(0, 0, 255, 80);}\
            QTabBar::tab:selected{border-color: white;background:rgba(0, 0, 255, 255);color:white;}";

    QString tabBarStyle4 = "QTabBar::tab:selected{background:rgba(125, 125, 125, 255);color:white;}";

    //this->setStyleSheet(tabBarStyle4);
    this->setDocumentMode(true);
    this->setTabBarAutoHide(true);
}

EditorTabsWidget::~EditorTabsWidget() { }

EditorTab* EditorTabsWidget::createTab(DomModel* model, QString filePath)
{
    // create tab
    EditorTab* editorTab = new EditorTab(model);

    if (filePath.isEmpty()) {
        int c = this->count() + 1;
        filePath = QString(tr("NewFile %1")).arg(c);
    } else {
        editorTab->setPath(filePath);
        filePath = editorTab->getFileName();
    }

    // add tab
    this->addTab(editorTab, filePath);

    // set as active
    this->setCurrentWidget(editorTab);

    editorTab->setWindowTitle("        " + filePath); //为拖拽显示窗口标题做准备

    return editorTab;
}

EditorTab* EditorTabsWidget::getCurentTab()
{
    return static_cast<EditorTab*>(currentWidget());
}

EditorTab* EditorTabsWidget::getTab(int i)
{
    if (i == -1)
        i = currentIndex();
    return static_cast<EditorTab*>(widget(i));
}

void EditorTabsWidget::closeTab(int i)
{
    EditorTab* tab = getTab(i);
    this->removeTab(indexOf(tab));
    delete tab;
}

bool EditorTabsWidget::hasTabs()
{
    return (this->count() > 0) ? true : false;
}

void EditorTabsWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (hasTabs() && event->reason() == QContextMenuEvent::Mouse) {
        int i = tabBar()->tabAt(event->pos());

        if (i != -1) {
            setCurrentIndex(i);

            //createMenu();
            QMenu contextMenu(this);
            contextMenu.addAction(tr("Close tab"), this, SLOT(closeTab()));
            contextMenu.addAction(tr("Close tabs to the right"), this, SLOT(closeTabsToRight()));
            contextMenu.addAction(tr("Close other tabs"), this, SLOT(closeOtherTabs()));
            contextMenu.addAction(tr("Close all tabs"), this, SLOT(closeAllTabs()));
            contextMenu.popup(event->globalPos());
            //contextMenu.exec();
        }
    }
}

void EditorTabsWidget::createMenu()
{
    QMenu contextMenu(this);
    contextMenu.addAction(tr("Close tab"), this, SLOT(closeTab()));
    contextMenu.addAction(tr("Close tabs to the right"), this, SLOT(closeTabsToRight()));
    contextMenu.addAction(tr("Close other tabs"), this, SLOT(closeOtherTabs()));
    contextMenu.addAction(tr("Close all tabs"), this, SLOT(closeAllTabs()));
    contextMenu.popup(QCursor::pos());
    contextMenu.exec();
}

void EditorTabsWidget::closeTabsToRight()
{
    int e = currentIndex();
    for (int i = count() - 1; i > e; --i) {
        emit tabCloseRequested(i);
    }
}

void EditorTabsWidget::closeOtherTabs()
{
    int e = currentIndex();
    for (int i = count() - 1; i >= 0; --i) {
        if (i != e)
            emit tabCloseRequested(i);
    }
}

void EditorTabsWidget::closeAllTabs()
{
    for (int i = count() - 1; i >= 0; --i) {
        emit tabCloseRequested(i);
    }
}

void EditorTabsWidget::initTabBar()
{
    MyTabBar* bar = new MyTabBar(this);
    //setTabBar是protected成员函数，要使用就得继承
    setTabBar(bar);

    //点击页签上的关闭按钮时，触发信号
    //connect(bar, &QTabBar::tabCloseRequested, this, &MyTabWidget::removeNormalIndex);

    //拖拽到外部-还未释放鼠标
    connect(bar, &MyTabBar::beginDragOut, this, [this, bar](int index) {
        if (!indexValid(index))
            return;
        QWidget* drag_tab = this->widget(index);
        //固定tab就不让拖出
        if (!drag_tab || fixedPage.contains(drag_tab))
            return;
        //把当前页作为快照拖拽
        //尺寸加了标题栏和边框
        QPixmap pixmap(drag_tab->size() + QSize(2, 31));
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        if (painter.isActive()) {
            //这里想做标题栏贴在内容之上
            //但是没法获取默认标题栏的图像啊，就随便画一个矩形框
            //如果设置了外部主题颜色，需要改下
            QRect title_rect { 0, 0, pixmap.width(), 30 };
            painter.fillRect(title_rect, Qt::white);
            painter.drawText(title_rect, Qt::AlignLeft | Qt::AlignVCenter, "  " + drag_tab->windowTitle());
            painter.drawRect(pixmap.rect().adjusted(0, 0, -1, -1));
        }
        painter.end();
        drag_tab->render(&pixmap, QPoint(1, 30));

        QMimeData* mime = new QMimeData;
        QDrag* drag = new QDrag(bar);
        drag->setMimeData(mime);
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(10, 0));

        //鼠标弹起后drag就释放了，这时候去判断是否拖拽到了外部
        connect(drag, &QDrag::destroyed, this, [=] {
            QPoint bar_point = bar->mapFromGlobal(QCursor::pos());
            //不在范围，拖出
            if (!bar->contentsRect().contains(bar_point)) {
                popPage(drag_tab);
            }
        });

        drag->exec(Qt::MoveAction);
    });
}

void EditorTabsWidget::popPage(QWidget* page)
{

    if (!QFileInfo(dragFileName).exists())
        return;

    takeNormalPage(page);
    //这里套一个自定义标题栏的窗口给page
    MyTabPopup* pop = new MyTabPopup(this);

    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setContentWidget(page);
    page->setWindowTitle(QFileInfo(dragFileName).baseName());
    pop->setWindowTitle(page->windowTitle());

    pop->resize(page->size());
    //拖出来的位置有点偏移
    pop->move(QCursor::pos() - QPoint(10, 10));

    //判断独立窗口是否拖回tab
    connect(pop, &MyTabPopup::dragRelease, this, [=](const QPoint& pos) {
        const QPoint bar_pos = tabBar()->mapFromGlobal(pos);
        //如果又拖回了tabbar范围内，就把widget取出来放回tab
        if (tabBar()->contentsRect().contains(bar_pos)) {
            QWidget* content = pop->getContentWidget();
            this->appendNormalPage(content);
            pop->disconnect();
            //关闭的时候会在原来的位置闪一下？
            pop->close();
            //this->activateWindow();
        }
    });

    //pop->show();
    //page->show();
    //pop->activateWindow();
    //pop->setFocus();

    QString qfile = QDir::homePath() + "/.config/PlistEDPlus/PlistEDPlus.ini";
    QFile file(qfile);
    QSettings Reg(qfile, QSettings::IniFormat);
    Reg.setValue("restore", true);
    Reg.setValue("count", 1);

    Reg.setValue(QString::number(0) + "/" + "file", dragFileName);

    FileSystemWatcher::removeWatchPath(dragFileName);
    for (int i = 0; i < openFileList.count(); i++) {
        if (dragFileName == openFileList.at(i)) {
            openFileList.remove(i);
            break;
        }
    }

    mw_one->on_NewWindow();
}

void EditorTabsWidget::takeNormalPage(QWidget* page)
{
    if (!page)
        return;
    removeTab(indexOf(page));
}

int EditorTabsWidget::appendNormalPage(QWidget* page)
{
    if (!page)
        return -1;
    //设置为调用close时释放
    page->setAttribute(Qt::WA_DeleteOnClose);
    //最后是添加到stackedWidget去了
    const int index = addTab(page, page->windowTitle());
    //切换为当前新增页
    setCurrentIndex(index);
    return index;
}

bool EditorTabsWidget::indexValid(int index) const
{
    if (index < 0 || index >= this->count())
        return false;
    return true;
}
