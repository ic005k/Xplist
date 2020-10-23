#include "editortabswidget.h"
#include <QTabBar>

EditorTabsWidget::EditorTabsWidget(QWidget *parent)
{
    Q_UNUSED(parent);

    this->setMovable(true);
    this->setTabsClosable(true);
}

EditorTabsWidget::~EditorTabsWidget() {}

EditorTab *EditorTabsWidget::createTab(DomModel *model, QString filePath)
{
    // create tab
    EditorTab *editorTab = new EditorTab(model);

    if (filePath.isEmpty())
    {
        int c = this->count() + 1;
        filePath = QString(tr("NewFile %1")).arg(c);
    }
    else
    {
        editorTab->setPath(filePath);
        filePath = editorTab->getFileName();
    }

    // add tab
    this->addTab(editorTab, filePath);

    // set as active
    this->setCurrentWidget(editorTab);

    return editorTab;
}

EditorTab *EditorTabsWidget::getCurentTab()
{
    return static_cast<EditorTab*>(currentWidget());
}

EditorTab *EditorTabsWidget::getTab(int i)
{
    if (i == -1) i = currentIndex();
    return static_cast<EditorTab*>(widget(i));
}

void EditorTabsWidget::closeTab(int i)
{
    EditorTab *tab = getTab(i);
    this->removeTab(indexOf(tab));
    delete tab;
}

bool EditorTabsWidget::hasTabs()
{
    return (this->count() > 0) ? true : false;
}

void EditorTabsWidget::contextMenuEvent(QContextMenuEvent * event)
{
    if (hasTabs() && event->reason() == QContextMenuEvent::Mouse)
    {
        int i = tabBar()->tabAt(event->pos());

        if (i != -1)
        {
            setCurrentIndex(i);

            createMenu();
        }
    }
}

void EditorTabsWidget::createMenu()
{
    QMenu contextMenu;
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
    for (int i = count() - 1; i > e; --i)
    {
        emit tabCloseRequested(i);
    }
}

void EditorTabsWidget::closeOtherTabs()
{
    int e = currentIndex();
    for (int i = count() - 1; i >= 0; --i)
    {
        if (i != e) emit tabCloseRequested(i);
    }
}

void EditorTabsWidget::closeAllTabs()
{
    for (int i = count() - 1; i >= 0; --i)
    {
        emit tabCloseRequested(i);
    }
}
