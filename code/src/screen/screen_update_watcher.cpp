
#include <QtGui/QtGui>
#ifdef ENABLE_EINK_SCREEN
#include <QtGui/qscreen_qws.h>
#endif

#include "onyx/screen/screen_update_watcher.h"

namespace onyx
{

namespace screen
{

ScreenUpdateWatcher & watcher()
{
    return ScreenUpdateWatcher::instance();
}


ScreenUpdateWatcher::ScreenUpdateWatcher()
{
}

ScreenUpdateWatcher::ScreenUpdateWatcher(ScreenUpdateWatcher &ref)
{
}

ScreenUpdateWatcher::~ScreenUpdateWatcher()
{
}

void ScreenUpdateWatcher::addWatcher(QWidget *widget)
{
    widget->installEventFilter(this);
}

bool ScreenUpdateWatcher::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest)
    {
        obj->event(event);
        static int count = 0;
        updateScreen();
        qDebug("event filter update request %d.\n", count++);
        return true;
    }
    else if (event->type() == QEvent::WindowActivate)
    {
        if (obj->isWidgetType())
        {
            QWidget * wnd = static_cast<QWidget *>(obj);
            wnd->update();
            enqueue(wnd, onyx::screen::ScreenProxy::GC);
        }
    }
    return QObject::eventFilter(obj, event);
}

/// Add screen update request to queue.
/// \widget The widget to update.
/// \w Which kind of waveform to use to update screen.
void ScreenUpdateWatcher::enqueue(QWidget *widget, onyx::screen::ScreenProxy::Waveform w, onyx::screen::ScreenCommand::WaitMode wm)
{
    queue_.enqueue(UpdateItem(widget, w, wm));
}

/// Add screen update request to queue.
/// \widget The widget to update.
/// \rc The rectangle of widget needs to update. If it's empty, whole widget will be updated.
/// \w Which kind of waveform to use to update screen.
void ScreenUpdateWatcher::enqueue(QWidget *widget,
                                  const QRect & rc,
                                  onyx::screen::ScreenProxy::Waveform w,
                                  onyx::screen::ScreenCommand::WaitMode wait)
{
    queue_.enqueue(UpdateItem(widget, w, wait, rc));
}

/// Get item from queue and decide which waveform to use.
void ScreenUpdateWatcher::updateScreen()
{
    QRect rc;
    onyx::screen::ScreenProxy::Waveform w = onyx::screen::ScreenProxy::DW;
    onyx::screen::ScreenCommand::WaitMode wait = onyx::screen::ScreenCommand::WAIT_NONE;
    while (!queue_.isEmpty())
    {
        UpdateItem i = queue_.dequeue();
        QWidget *p = i.widget;
        if (p == 0)
        {
            p = qApp->desktop();
        }

        if (!p->isVisible())
        {
            continue;
        }

        QPoint pt = p->mapToGlobal(i.rc.topLeft());
        QSize s;
        if (!i.rc.size().isEmpty())
        {
            s = i.rc.size();
        }
        else
        {
            s = p->size();
        }
        QRect t(pt, s);
        rc = rc.united(t);
        if (i.waveform > w)
        {
            w = i.waveform;
        }

        if (i.wait > wait)
        {
            wait = i.wait;
        }
    }
    if (!rc.isEmpty())
    {
        qDebug() << "update screen " << rc << "Waveform " << w;
        onyx::screen::instance().updateWidgetRegion(0, rc, w, wait);
    }
}

bool ScreenUpdateWatcher::isQueueEmpty()
{
    return queue_.isEmpty();
}

};

};
