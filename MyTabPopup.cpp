#include "MyTabPopup.h"

#include <QDebug>
#include <QMouseEvent>
#include <QVBoxLayout>

MyTabPopup::MyTabPopup(QWidget* parent)
    : QDialog(parent)
{
}

void MyTabPopup::setContentWidget(QWidget* page)
{
    if (!page)
        return;
    content = page;
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(page);
}

QWidget* MyTabPopup::getContentWidget()
{
    return content;
}

bool MyTabPopup::event(QEvent* event)
{

    if (event->type() == QEvent::MouseButtonRelease) {
    }

    if (event->type() == QEvent::NonClientAreaMouseButtonRelease) {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        if (e && e->button() == Qt::LeftButton) {
            emit dragRelease(e->globalPos());
        }
    }

    return QDialog::event(event);
}
