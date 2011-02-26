#ifndef UI_LIB_TAB_BAR_H_
#define UI_LIB_TAB_BAR_H_

#include <vector>
#include "tab_button.h"

namespace ui
{

class TabBar : public QWidget
{
    Q_OBJECT
public:
    TabBar(QWidget *parent);
    ~TabBar(void);

public:
    bool addButton(const int id, const QString & title, const QPixmap & pixmap);
    bool removeButton(const int id);
    bool clickButton(const int id);
    int  selectedButton();
    bool setButtonText(const int id, const QString & title);

    bool setOrientation(const Qt::Orientation orientation);
    Qt::Orientation orientation() const { return orientation_; }

Q_SIGNALS:
    void buttonClicked(TabButton *button);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);

private Q_SLOTS:
    void onClicked(TabButton *button);

private:
    void createLayout();
    void clear();
    void setFocusNextPrevChild(bool next);
    void clickSelectedChild();

private:
    QBoxLayout layout_;
    Qt::Orientation orientation_;
    typedef std::vector<TabButton *> Buttons;
    typedef Buttons::iterator ButtonIter;
    Buttons buttons_;
};

};

#endif
