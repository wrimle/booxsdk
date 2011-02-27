#ifndef TAB_BUTTON_H_
#define TAB_BUTTON_H_

#include "onyx/ui/ui_global.h"

class TabButton : public QWidget
{
    Q_OBJECT

public:
    TabButton(QWidget *parent, const int id);
    ~TabButton(void);

public:
    const int id() { return button_id_; }

    void setText(const QString &title);
    void setPixmap(const QPixmap & pixmap);

    void setChecked(bool check = true);
    bool isChecked();

public Q_SLOTS:
    void click();

Q_SIGNALS:
    void clicked(TabButton *button);

protected:
    virtual bool event(QEvent *e);
    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    void createLayout();
    bool isPressed();
    void setPressed(bool p = true);
    void activate();
    void repaintAndRefreshScreen();

private:
    int button_id_;
    bool checked_;
    bool pressed_;

    QVBoxLayout layout_;
    QLabel text_label_;
    QLabel pixmap_label_;
};

inline bool TabButton::isChecked()
{
    return checked_;
}

#endif
