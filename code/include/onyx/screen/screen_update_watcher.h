#ifndef ONYX_SCREEN_UPDATE_WATCHE_H_
#define ONYX_SCREEN_UPDATE_WATCHE_H_

#include <QWidget>
#include "screen_proxy.h"

namespace onyx
{
namespace screen
{

class ScreenUpdateWatcher
{
public:
    static ScreenUpdateWatcher & instance()
    {
        static ScreenUpdateWatcher instance_;
        return instance_;
    }
    ~ScreenUpdateWatcher();

public:
    void enqueue(QWidget *widget, onyx::screen::ScreenProxy::Waveform w = onyx::screen::ScreenProxy::GC);
    void enqueue(QWidget *widget, const QRect & rc, onyx::screen::ScreenProxy::Waveform w);
    void updateScreen();
    bool isQueueEmpty();

private:
    ScreenUpdateWatcher();
    ScreenUpdateWatcher(ScreenUpdateWatcher &ref);

private:
    struct UpdateItem
    {
        QWidget *widget;
        onyx::screen::ScreenProxy::Waveform waveform;
        QRect rc;

        UpdateItem(QWidget * wnd, onyx::screen::ScreenProxy::Waveform w, QRect rect= QRect())
            : widget(wnd)
            , waveform(w)
            , rc(rect)
        {}
    };
    QQueue<UpdateItem> queue_;
};

ScreenUpdateWatcher & watcher();

};  // namespace screen

};  // namespace onyx

#endif
