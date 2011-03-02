
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

/// Return the distance between first and second widget.
int distance(QWidget * first, QWidget *second)
{
    QRect first_rc(first->mapToGlobal(QPoint()), first->size());
    QRect second_rc(second->mapToGlobal(QPoint()), second->size());
    QPoint first_pt = first_rc.center();
    QPoint second_pt = second_rc.center();
    int x = (second_pt.x() - first_pt.x()) * (second_pt.x() - first_pt.x());
    int y = (second_pt.y() - first_pt.y()) * (second_pt.y() - first_pt.y());
    return static_cast<int>(sqrt(static_cast<float>(x) + static_cast<float>(y)));
}

}


