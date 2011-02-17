#include <algorithm>
#include "onyx/ui/tab_button.h"

static const int SPACING = 5;

TabButton::TabButton(QWidget *parent, const int id)
    : QWidget(parent)
    , button_id_(id)
    , checked_(false)
    , pressed_(false)
    , layout_(this)
{
    setAutoFillBackground(false);
    createLayout();
}

TabButton::~TabButton()
{
}

bool TabButton::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::MouseMove:
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

void TabButton::createLayout()
{
    layout_.setContentsMargins(0, 0, 0, 0);
    layout_.setSpacing(5);

    text_label_.setAlignment(Qt::AlignCenter);
    pixmap_label_.setAlignment(Qt::AlignCenter);

    layout_.addWidget(&text_label_);
    layout_.addWidget(&pixmap_label_);
}

void TabButton::setText(const QString &title)
{
    text_label_.setText(title);
}

void TabButton::setPixmap(const QPixmap & pixmap)
{
    pixmap_label_.setPixmap(pixmap);
}

void TabButton::setChecked(bool checked)
{
    if (checked_ != checked)
    {
        checked_ = checked;
        update();
    }
}

/// TODO, change it to better rendering.
void TabButton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QRect border = rect().adjusted(1, 1, -1, -1);
    if (isChecked())
    {
        p.fillRect(rect(), Qt::white);
    }
    else
    {
        p.fillRect(rect(), QBrush(QColor(174, 174, 174)));
        QPen pen(QColor(97, 99, 98));
        pen.setWidth(2);
        p.setPen(pen);
        p.drawRect(border);
    }

    if (isPressed())
    {
        p.fillRect(rect(), Qt::darkGray);
    }
}

bool TabButton::isPressed()
{
    return pressed_;
}

void TabButton::setPressed(bool p)
{
    pressed_ = p;
}

void TabButton::activate()
{
    emit clicked(this);
}

void TabButton::mousePressEvent(QMouseEvent *event)
{
    setPressed(true);
    QWidget::mousePressEvent(event);
    repaint();
}

void TabButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (isPressed())
    {
        activate();
    }
    setPressed(false);
    QWidget::mouseReleaseEvent(event);
    repaint();
}

void TabButton::mouseMoveEvent(QMouseEvent * e)
{
    if (isPressed() && !rect().contains(e->pos()))
    {
        setPressed(false);
        repaint();
    }
}

void TabButton::click()
{
    setChecked(true);
    setFocus();
    activate();
}

