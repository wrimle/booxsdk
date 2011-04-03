#ifndef ONYX_CATALOG_VIEW_H_
#define ONYX_CATALOG_VIEW_H_

#include <QGridLayout>

#include "onyx/data/data.h"

#include "content_view.h"
#include "paginator.h"
#include "factory.h"

namespace ui
{

class CatalogView : public QWidget
{
    Q_OBJECT

public:
    static const QString LEFT;
    static const QString RIGHT;
    static const QString UP;
    static const QString DOWN;
    static const QString RECYCLE_LEFT;
    static const QString RECYCLE_RIGHT;
    static const QString RECYCLE_UP;
    static const QString RECYCLE_DOWN;

     enum SearchPolicy
     {
         Normal         = 0x0,
         NeighborFirst  = 0x01,
         AutoHorRecycle = 0x02,
         AutoVerRecycle = 0x04,
     };

public:
    CatalogView(Factory * factory = 0, QWidget *parent = 0);
    ~CatalogView();

public slots:
    void setChecked(bool checked = true);
    bool isChecked() { return checked_; }

    void setSearchPolicy(int policy = NeighborFirst);
    int searchPolicy();

    bool hasNext();
    bool hasPrev();

    bool goPrev();
    bool goNext();
    bool gotoPage(const int);

    void setMargin(int left = 4, int top = 4, int right = 4, int bottom = 4);
    void margin(int *left, int *top, int *right, int *bottom);

    void setSpacing(int s = 2);
    int spacing() { return spacing_; }

    void showBorder(bool show = true) { show_border_ = show; }
    bool hasBorder() { return show_border_; }

    void setData(const ODatas &list, bool force = false);
    ODatas & data();

    void setFocusTo(const int row, const int col);
    ContentView *focusItem();

    void setFixedGrid(int rows, int cols);
    bool isFixedGrid();
    QSize preferItemSize();
    void setPreferItemSize(const QSize &size);
    int rows();
    int cols();

    void setSubItemType(const QString &type);
    void setSubItemBkColor(Qt::GlobalColor color);
    QVector<ContentView *> & visibleSubItems() { return sub_items_; }

    void associateData(bool force = false);
    void associateEmptyData();
    void arrangeAll(bool force = false);
    bool select(OData *data);
    void resetPaginator(bool sync_layout = false);
    void broadcastPositionSignal();

    void setNeighbor(CatalogView *neighbor, const QString& type);
    bool removeNeighbor(CatalogView *neighbor, const QString& type);


protected Q_SLOTS:
    virtual void onItemActivated(ContentView *item, int);
    virtual void onItemKeyRelease(ContentView *item, QKeyEvent *key);
    virtual void onMouseMoved(QPoint last, QPoint current);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *);
    void keyPressEvent(QKeyEvent *);
    void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent * event);

protected:
    Paginator & paginator();

Q_SIGNALS:
    void cursorMoved(int old, int new_pos);
    void positionChanged(const int, const int);
    void itemActivated(CatalogView *catalog, ContentView *item, int user_data);
    void keyRelease(CatalogView *view, QKeyEvent *key);

    // In some cases, the parent want to control the keyboard navigation
    // when non-catalogview exist, the following four signals can be utilized.
    void outOfLeft(CatalogView*, int, int);
    void outOfRight(CatalogView*, int, int);
    void outOfUp(CatalogView*, int, int);
    void outOfDown(CatalogView*, int, int);

private slots:
    QWidget * moveFocus(int index);
    ContentView *createSubItem();

private:
    typedef QVector<CatalogView *> CatalogViews;

private:
    void createLayout();
    void calculateLayout(int &rows, int &cols);
    void arrangeSubWidgets();

    int moveLeft(int current);
    int moveRight(int current);
    int moveUp(int current);
    int moveDown(int current);

    CatalogViews & neighbors(const QString &type);
    bool addNeighbor(const QString &type, CatalogView *neighbor);
    bool searchNeighbors(const QString &type);
    ContentView* findShortestItem(CatalogView *view, QWidget *target, int & dist, const QPoint &offset);
    QString invert(const QString &type);

    int row(int index);
    int col(int index);
    bool atDownEdge(int);
    bool atRightEdge(int);
    bool neighborFirst();
    bool verAutoRecycle();
    bool horAutoRecycle();

private:
    QGridLayout layout_;
    QVector<ContentView *> sub_items_;
    Factory * factory_;
    ODatas datas_;
    int left_margin_;
    int top_margin_;
    int right_margin_;
    int bottom_margin_;
    int spacing_;
    bool checked_;
    int policy_;
    bool show_border_;
    bool fixed_grid_;
    QSize size_;
    Paginator paginator_;
    QMap<QString, CatalogViews> neighbors_;
    QString sub_item_type_;
    Qt::GlobalColor bk_color_;
};


};  // namespace ui

#endif
