
#include <QPainter>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/content_view.h"

namespace ui
{

static QPoint s_mouse;
static const int MARGIN = 4;

ContentView::ContentView(QWidget *parent)
        : QWidget(parent)
        , data_(0)
        , pressed_(false)
        , pen_width_(3)
{
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(false);
}

ContentView::~ContentView()
{
}

bool ContentView::updateData(OData* data, bool force)
{
    if (data_ == data && !force)
    {
        return false;
    }
    data_ = data;
    updateView();
    update();
    return true;
}

OData * ContentView::data()
{
    return data_;
}

bool ContentView::isPressed()
{
    return pressed_;
}

void ContentView::setPressed(bool p)
{
    pressed_ = p;
}

void ContentView::activate()
{
    if (data())
    {
        emit activated(this);
    }
}

void ContentView::repaintAndRefreshScreen()
{
    update();
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::DW);
}

void ContentView::mousePressEvent(QMouseEvent *event)
{
    s_mouse = event->globalPos();
    if (data())
    {
        setPressed(true);
        repaintAndRefreshScreen();
    }
    QWidget::mousePressEvent(event);
}

void ContentView::mouseReleaseEvent(QMouseEvent *event)
{
    if (isPressed())
    {
        activate();
    }
    else
    {
        emit mouse(s_mouse, event->globalPos());
    }
    setPressed(false);
    if (data())
    {
        repaintAndRefreshScreen();
    }
    QWidget::mouseReleaseEvent(event);
}

void ContentView::mouseMoveEvent(QMouseEvent * e)
{
    if (isPressed() && !rect().contains(e->pos()) && data())
    {
        setPressed(false);
        repaintAndRefreshScreen();
    }
}

void ContentView::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return)
    {
        activate();
        e->accept();
        return;
    }
    e->ignore();
    emit keyRelease(this, e);
}

void ContentView::changeEvent(QEvent *event)
{
}

void ContentView::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
}

bool ContentView::event(QEvent * e)
{
    switch (e->type())
    {
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
        e->accept();
        return true;
    default:
        break;
    }
    return QWidget::event(e);
}

void ContentView::focusInEvent(QFocusEvent * e)
{
    QWidget::focusInEvent(e);
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::DW);
}

void ContentView::focusOutEvent(QFocusEvent * e)
{
    QWidget::focusOutEvent(e);
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::DW);
}

void ContentView::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    if (data())
    {
        if (isPressed())
        {
            painter.fillRect(rect(), Qt::darkGray);
        }
        if (hasFocus())
        {
            QPen pen;
            pen.setWidth(penWidth());
            painter.setPen(pen);
            painter.drawRoundedRect(rect().adjusted(0, 0, -penWidth() , -penWidth()), 5, 5);
        }
    }
}




CoverView::CoverView(QWidget *parent)
: ContentView(parent)
{
}

CoverView::~CoverView()
{
}

void CoverView::updateView()
{
    update();
}

void CoverView::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    if (data())
    {
        if (isPressed())
        {
            painter.fillRect(rect(), Qt::darkGray);
        }
        if (hasFocus())
        {
            QPen pen;
            pen.setWidth(penWidth());
            painter.setPen(pen);
            painter.drawRoundedRect(rect().adjusted(0, 0, -penWidth() , -penWidth()), 5, 5);
        }

        drawCover(painter, rect());
        drawTitle(painter, rect());
    }
}

void CoverView::drawCover(QPainter & painter, QRect & rect)
{
    if (data() && data()->contains("cover"))
    {
        QPixmap pixmap(qVariantValue<QPixmap>(data()->value("cover")));
        int x = (rect.width() - pixmap.width()) / 2;
        painter.drawPixmap(x, MARGIN, pixmap);
    }
}

void CoverView::drawTitle(QPainter & painter, QRect & rect)
{
    if (data() && data()->contains("title"))
    {
        painter.drawText(rect, Qt::AlignCenter, data()->value("title").toString());
    }
}

}
