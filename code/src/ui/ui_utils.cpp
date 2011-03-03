
#include "onyx/ui/ui_utils.h"
#include "onyx/sys/sys_status.h"
#include "onyx/sys/platform.h"

namespace ui
{

QRect screenGeometry()
{
    QRect rc = QApplication::desktop()->geometry();
    int def_rotation = sys::defaultRotation();
    int r1 = (def_rotation + 90) % 360;
    int r2 = (def_rotation + 270) % 360;
    if (sys::SysStatus::instance().screenTransformation() == r1 ||
        sys::SysStatus::instance().screenTransformation() == r2)
    {
        int w = rc.width();
        rc.setWidth(rc.height());
        rc.setHeight(w);
    }
    return rc;
}

bool dockWidget(QWidget *target, QWidget * container, Qt::Alignment align)
{
    return true;
}

int statusBarHeight()
{
    return 35;
}

QPoint globalTopLeft(QWidget *wnd)
{
    return wnd->mapToGlobal(QPoint());
}

QPoint globalCenter(QWidget *wnd)
{
    return QRect(wnd->mapToGlobal(QPoint()), wnd->size()).center();
}

/// Return the distance between first and second widget.
int distance(QWidget * first, QWidget *second)
{
    QPoint first_pt = globalCenter(first);
    QPoint second_pt = globalCenter(second);
    return distance(first_pt, second_pt);
}

int distance(QPoint first_pt, QPoint second_pt)
{
    int x = (second_pt.x() - first_pt.x()) * (second_pt.x() - first_pt.x());
    int y = (second_pt.y() - first_pt.y()) * (second_pt.y() - first_pt.y());
    return static_cast<int>(sqrt(static_cast<float>(x) + static_cast<float>(y)));
}

}


