
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

void ScreenUpdateWatcher::addWatcherWithGCInterval(QWidget *widget, int count)
{
    // TODO;
}

void ScreenUpdateWatcher::addWatcher(QWidget *widget)
{
    if (widget)
    {
        widget->installEventFilter(this);
    }
}

void ScreenUpdateWatcher::removeWatcher(QWidget *widget)
{
    if (widget)
    {
        widget->removeEventFilter(this);
    }
}

bool ScreenUpdateWatcher::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest)
    {
        QTimer::singleShot(0, this, SLOT(updateScreen()));
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

bool ScreenUpdateWatcher::enqueue(UpdateItem & item,
                                  QWidget *widget,
                                  onyx::screen::ScreenProxy::Waveform w,
                                  onyx::screen::ScreenCommand::WaitMode wait,
                                  const QRect & rc)
{
    if (widget == 0)
    {
        widget = qApp->desktop();
    }

    if (!widget->isVisible())
    {
        return false;
    }

    QPoint pt = widget->mapToGlobal(rc.topLeft());
    QSize s;
    if (!rc.size().isEmpty())
    {
        s = rc.size();
    }
    else
    {
        s = widget->size();
    }
    item.rc = QRect(pt, s);
    item.wait = wait;
    item.waveform = w;
    return true;
}

/// Add screen update request to queue.
/// \widget The widget to update.
/// \w Which kind of waveform to use to update screen.
void ScreenUpdateWatcher::enqueue(QWidget *widget,
                                  onyx::screen::ScreenProxy::Waveform w,
                                  onyx::screen::ScreenCommand::WaitMode wm)
{
    UpdateItem item;
    if (enqueue(item, widget, w, wm))
    {
        queue_.enqueue(item);
    }
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
    UpdateItem item;
    if (enqueue(item, widget, w, wait, rc))
    {
        queue_.enqueue(item);
    }
}

/// Get item from queue and decide which waveform to use.
void ScreenUpdateWatcher::updateScreen()
{
    updateScreenInternal(true);
}

// Forward method call to screen proxy.
void ScreenUpdateWatcher::setGCInterval(const int interval)
{
    onyx::screen::instance().setGCInterval(interval);
}

void ScreenUpdateWatcher::resetGUCount()
{
    onyx::screen::instance().resetGUCount();
}

void ScreenUpdateWatcher::updateScreenInternal(bool automatic,
                                               onyx::screen::ScreenProxy::Waveform waveform)
{
    onyx::screen::ScreenProxy::Waveform w = onyx::screen::ScreenProxy::DW;
    onyx::screen::ScreenCommand::WaitMode wait = onyx::screen::ScreenCommand::WAIT_NONE;
    QRect rc;
    while (!queue_.isEmpty())
    {
        UpdateItem i = queue_.dequeue();
        rc = rc.united(i.rc);
        if (automatic)
        {
            if (i.waveform > w)
            {
                w = i.waveform;
            }
        }
        else
        {
            w = waveform;
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

void ScreenUpdateWatcher::updateScreenWithGCInterval()
{
}

bool ScreenUpdateWatcher::isQueueEmpty()
{
    return queue_.isEmpty();
}

};

};
