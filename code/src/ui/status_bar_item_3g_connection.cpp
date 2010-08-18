#include "onyx/ui/status_bar_item_3g_connection.h"
#include "onyx/sys/sys_status.h"

namespace ui
{

StatusBarItem3GConnection::StatusBarItem3GConnection(QWidget *parent)
    : StatusBarItem(CONNECTION, parent)
    , status_(-1)
{
    setConnectionStatus(0);
    connect(&sys::SysStatus::instance(),
            SIGNAL(report3GNetwork(const int, const int, const int)),
            this,
            SLOT(onSignalStrengthChanged(const int, const int, const int)));
}

StatusBarItem3GConnection::~StatusBarItem3GConnection(void)
{
}

void StatusBarItem3GConnection::setConnectionStatus(const int status)
{
    if (status_ == status)
    {
        return;
    }
    status_ = status;

    QImage & img = image();
    setFixedWidth(img.width());
}

void StatusBarItem3GConnection::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    QImage & img = image();
    QPoint point;
    point.rx() = ((rect().width() - img.width()) >> 1);
    point.ry() = ((rect().height() - img.height()) >> 1);
    painter.drawImage(point, img);
}

/// Retrieve image item according to battery value and status.
QImage & StatusBarItem3GConnection::image()
{
    int key = status_;
    if (!images_.contains(key))
    {
        images_.insert(key, QImage(resourcePath()));
    }
    return images_[key];
}

QString StatusBarItem3GConnection::resourcePath()
{
    switch (status_)
    {
    case 0:
        return ":/images/power_0.png";
    case 1:
        return ":/images/power_1.png";
    case 2:
        return ":/images/power_2.png";
    case 3:
        return ":/images/power_3.png";
    case 4:
        return ":/images/power_4.png";
    default:
        break;
    }
    return QString();
}

void StatusBarItem3GConnection::onSignalStrengthChanged(const int signal,
                                                        const int total,
                                                        const int network)
{

}

}