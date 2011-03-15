#ifndef ONYX_SCREEN_UPDATE_WATCHE_H_
#define ONYX_SCREEN_UPDATE_WATCHE_H_

#include <QWidget>
#include "screen_proxy.h"

namespace onyx
{
namespace screen
{

class ScreenUpdateWatcher : public QObject
{
    Q_OBJECT

public:
    static ScreenUpdateWatcher & instance()
    {
        static ScreenUpdateWatcher instance_;
        return instance_;
    }
    ~ScreenUpdateWatcher();

public Q_SLOTS:
    void addWatcher(QWidget *widget);
    void removeWatcher(QWidget *widget);

    void enqueue(QWidget *widget, onyx::screen::ScreenProxy::Waveform w = onyx::screen::ScreenProxy::GC, onyx::screen::ScreenCommand::WaitMode wait = ScreenCommand::WAIT_BEFORE_UPDATE);
    void enqueue(QWidget *widget, const QRect & rc, onyx::screen::ScreenProxy::Waveform w, onyx::screen::ScreenCommand::WaitMode wait = ScreenCommand::WAIT_BEFORE_UPDATE);
    void updateScreen();

public:
    bool isQueueEmpty();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    ScreenUpdateWatcher();
    ScreenUpdateWatcher(ScreenUpdateWatcher &ref);

private:
    struct UpdateItem
    {
        onyx::screen::ScreenProxy::Waveform waveform;
        onyx::screen::ScreenCommand::WaitMode wait;
        QRect rc;

        UpdateItem(){}

        UpdateItem(onyx::screen::ScreenProxy::Waveform w, onyx::screen::ScreenCommand::WaitMode wm, QRect rect= QRect())
            : waveform(w)
            , wait(wm)
            , rc(rect)
        {}
    };
    QQueue<UpdateItem> queue_;

private:
    bool enqueue(UpdateItem &, QWidget *, onyx::screen::ScreenProxy::Waveform, onyx::screen::ScreenCommand::WaitMode, const QRect & rc = QRect());

};

ScreenUpdateWatcher & watcher();

};  // namespace screen

};  // namespace onyx

#endif
