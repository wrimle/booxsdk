#ifndef CENTENT_VIEW_H_
#define CENTENT_VIEW_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "data.h"

class ContentView : public QWidget
{
    Q_OBJECT

public:
    ContentView(QWidget *parent);
    virtual ~ContentView();

public:
    bool updateData(OData* data, bool force_update = false);
    OData * data();
    virtual void updateView() = 0;
    void repaintAndRefreshScreen();

    int penWidth() { return pen_width_; }
    void setPenWidth(int w) { pen_width_ = w; }

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *);
    void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent * event);
    bool event(QEvent * event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);

Q_SIGNALS:
    void activated(ContentView *item);
    void keyRelease(ContentView *item, QKeyEvent *key);
    void mouse(QPoint last, QPoint current);

protected:
    bool isPressed();
    void setPressed(bool p = true);
    void activate();

private:
    OData* data_;
    bool pressed_;
    int pen_width_;
};


/// Cover view provides a cover and title support.
class CoverView : public ContentView
{
    Q_OBJECT

public:
    CoverView(QWidget *parent);
    virtual ~CoverView();

public:
    virtual void updateView();

protected:
    void paintEvent(QPaintEvent * event);
    void drawCover(QPainter & painter, QRect & rect);
    void drawTitle(QPainter & painter, QRect & rect);

};


#endif
