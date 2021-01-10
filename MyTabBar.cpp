#include "MyTabBar.h"
#include "editortabswidget.h"

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

extern QString dragFileName;
extern EditorTabsWidget* tabWidget;

MyTabBar::MyTabBar(QWidget* parent)
    : QTabBar(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    //tab页签可以拖拽交换位置
    setMovable(true);
    //tab页签显示关闭按钮
    setTabsClosable(true);
}

void MyTabBar::mousePressEvent(QMouseEvent* event)
{
    QTabBar::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && currentIndex() >= 0) {
        //保存状态
        //pressPos=event->pos();
        theDragPress = true;

        dragFileName = tabWidget->getCurentTab()->getPath();
    }
}

void MyTabBar::mouseMoveEvent(QMouseEvent* event)
{
    QTabBar::mouseMoveEvent(event);

    //move的时候button为NoButton，但是button's里有
    if (theDragPress && event->buttons()) {
        //是否脱离了tabbar的范围
        if (!theDragOut && !contentsRect().contains(event->pos())) {
            theDragOut = true;
            emit beginDragOut(this->currentIndex());

            //QDrag.exec后就不会触发release了，自己手动触发
            //不过他好像还是在鼠标弹起之后才会进行动画，待解决
            QMouseEvent* e = new QMouseEvent(QEvent::MouseButtonRelease,
                this->mapFromGlobal(QCursor::pos()),
                Qt::LeftButton,
                Qt::LeftButton,
                Qt::NoModifier);
            //mouseReleaseEvent(event);
            QApplication::postEvent(this, e);
        }
    }
}

void MyTabBar::mouseReleaseEvent(QMouseEvent* event)
{
    QTabBar::mouseReleaseEvent(event);
    theDragPress = false;
    theDragOut = false;
}
