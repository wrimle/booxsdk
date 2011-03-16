
#include <QPainter>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/ui/content_view.h"

namespace ui
{

static QPoint s_mouse;
static const int MARGIN = 4;

ContentView::ContentView(QWidget *parent)
        : QWidget(parent)
        , data_(0)
        , pressed_(false)
        , checked_(false)
        , pen_width_(3)
        , bk_color_(Qt::white)
{
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(false);
}

ContentView::~ContentView()
{
}

void ContentView::setChecked(bool checked)
{
    checked_ = checked;
}

bool ContentView::isChecked()
{
    return checked_;
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

void ContentView::activate(int user_data)
{
    if (data())
    {
        emit activated(this, user_data);
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
    bool broadcast = false;
    if (isPressed())
    {
        broadcast = true;
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
    if (broadcast)
    {
        activate();
    }
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
    painter.fillRect(rect(), bkColor());

    if (data())
    {
        if (isPressed())
        {
            //painter.fillRect(rect(), Qt::darkGray);
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

const QString CoverView::type()
{
    return "CoverView";
}

void CoverView::updateView()
{
    update();
}

void CoverView::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(rect(), bkColor());

    if (data())
    {
        if (isPressed() || isChecked())
        {
            painter.fillRect(rect().adjusted(penWidth(), penWidth(), -penWidth() - 1, -penWidth() - 1), Qt::gray);
        }
        if (hasFocus())
        {
            QPen pen;
            pen.setWidth(penWidth());
            painter.setPen(pen);
            painter.drawRoundedRect(rect().adjusted(0, 0, -penWidth() , -penWidth()), 5, 5);
        }

        drawCover(painter, rect());
        if (isPressed() || isChecked())
        {
            painter.setPen(Qt::white);
        }
        drawTitle(painter, rect());
    }
}

void CoverView::drawCover(QPainter & painter, QRect rect)
{
    if (data() && data()->contains("cover"))
    {
        QPixmap pixmap(qVariantValue<QPixmap>(data()->value("cover")));
        int x = (rect.width() - pixmap.width()) / 2;
        painter.drawPixmap(x, MARGIN, pixmap);
    }
}

void CoverView::drawTitle(QPainter & painter, QRect rect)
{
    if (data() && data()->contains("title"))
    {
        QFont font;
        font.setPointSize(ui::defaultFontPointSize());
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignCenter, data()->value("title").toString());
    }
}


CheckBoxView::CheckBoxView(QWidget *parent)
: ContentView(parent)
{

}

CheckBoxView::~CheckBoxView()
{
}

const QString CheckBoxView::type()
{
    return "CheckBoxView";
}

void CheckBoxView::updateView()
{
    update();
}

void CheckBoxView::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(rect(), bkColor());

    if (data())
    {
        static const QString CHECKED = "checked";
        if (data()->contains(CHECKED))
        {
            setChecked(qVariantValue<bool> (data()->value(CHECKED)));
            qDebug() << "contains checked data: " << qVariantValue<bool> (
                    data()->value(CHECKED));
        }
        if (isPressed() || isChecked())
        {
            painter.fillRect(rect().adjusted(penWidth(), penWidth(), -penWidth() - 1, -penWidth() - 1), Qt::gray);
        }
        if (hasFocus())
        {
            QPen pen;
            pen.setWidth(penWidth());
            painter.setPen(pen);
            painter.drawRoundedRect(rect().adjusted(0, 0, -penWidth() , -penWidth()), 5, 5);
        }

        QRect check_box_r = drawCheckBox(painter, rect());

        int icon_x = check_box_r.right() + MARGIN;
        QRect icon_r = drawCover(painter, QRect(icon_x, rect().y(),
                rect().width()-icon_x, rect().height()));

        if (isPressed() || isChecked())
        {
            painter.setPen(Qt::white);
        }
        int title_x = icon_r.right() + MARGIN;
        drawTitle(painter, QRect(title_x, rect().y(),
                rect().width()-title_x, rect().height()));
    }
}

QRect CheckBoxView::drawCheckBox(QPainter & painter, QRect rect)
{
    int width = checkBoxViewWidth();
    int height = width;
    int x = rect.x() + 20;
    int y = (rect.height() - height)/2;
    QRect check_box_rect(x, y, width, height);
    if (isChecked())
    {
        painter.setPen(QPen(Qt::white, 2));
        painter.fillRect(check_box_rect, Qt::black);
    }
    else
    {
        painter.setPen(QPen(Qt::black, 2));
        painter.fillRect(check_box_rect, Qt::white);
    }
    painter.drawRect(check_box_rect);
    return check_box_rect;
}

QRect CheckBoxView::drawCover(QPainter & painter, QRect rect)
{
    QRect icon_rect(rect.topLeft(), rect.topLeft());
    if (data() && data()->contains("cover"))
    {
        QPixmap pixmap(qVariantValue<QPixmap>(data()->value("cover")));
        painter.drawPixmap(MARGIN, (rect.height() - pixmap.height()) / 2, pixmap);
        icon_rect.setRight(pixmap.width());
    }
    return icon_rect;
}

void CheckBoxView::drawTitle(QPainter & painter, QRect rect)
{
    if (data() && data()->contains("title"))
    {
        rect.adjust(MARGIN, 0, 0, 0);
        QFont font;
        font.setPointSize(ui::defaultFontPointSize());
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignLeft|Qt::AlignVCenter,
                data()->value("title").toString());
    }
}




LineEditView::LineEditView(QWidget *parent)
: ContentView(parent)
//, inner_edit_(this)
{
}

LineEditView::~LineEditView()
{
}

const QString LineEditView::type()
{
    return "LineEditView";
}

void LineEditView::updateView()
{
}

void LineEditView::mousePressEvent(QMouseEvent *event)
{
}

void LineEditView::mouseMoveEvent(QMouseEvent * event)
{
}

void LineEditView::mouseReleaseEvent(QMouseEvent *event)
{
}

void LineEditView::keyReleaseEvent(QKeyEvent *)
{
}

void LineEditView::paintEvent(QPaintEvent * event)
{
}

bool LineEditView::event(QEvent * event)
{
    return true;
}


void LineEditView::focusInEvent(QFocusEvent * event)
{
}

void LineEditView::focusOutEvent(QFocusEvent * event)
{
}


}




