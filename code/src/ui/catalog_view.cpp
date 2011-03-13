#include <QKeyEvent>
#include <QPainter>
#include <QVector>
#include "onyx/sys/sys.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/catalog_view.h"

namespace ui
{

const QString CatalogView::LEFT         = "left";
const QString CatalogView::RIGHT        = "right";
const QString CatalogView::UP           = "up";
const QString CatalogView::DOWN         = "down";
const QString CatalogView::RECYCLE_LEFT = "r-left";
const QString CatalogView::RECYCLE_RIGHT= "r-right";
const QString CatalogView::RECYCLE_UP   = "r-up";
const QString CatalogView::RECYCLE_DOWN = "r-down";


CatalogView::CatalogView(Factory * factory, QWidget *parent)
        : QWidget(parent)
        , layout_(this)
        , factory_(factory)
        , margin_(0)
        , checked_(true)
        , self_hor_recycle_(false)
        , self_ver_recycle_(false)
        , show_border_(false)
        , fixed_grid_(false)
        , size_(200, 150)
{
    createLayout();
}

CatalogView::~CatalogView()
{
}

void CatalogView::setHorSelfRecycle(bool r)
{
    self_hor_recycle_ = r;
}

bool CatalogView::isHorSelfRecycle()
{
    return self_hor_recycle_;
}

void CatalogView::setVerSelfRecycle(bool r)
{
    self_ver_recycle_ = r;
}

bool CatalogView::isVerSelfRecycle()
{
    return self_ver_recycle_;
}

void CatalogView::createLayout()
{
    layout_.setContentsMargins(margin(), margin(), margin(), margin());
    layout_.setSpacing(0);
    layout_.setVerticalSpacing(0);
}

void CatalogView::calculateLayout(int &rows, int &cols)
{
    QSize s = preferItemSize();
    if (s.height() <= 0)
    {
        rows = 1;
    }
    else
    {
        rows = rect().height() / s.height();
    }

    if (s.width() <= 0)
    {
        cols = 1;
    }
    else
    {
        cols = rect().width() / s.width();
    }
}

void CatalogView::setMargin(int m)
{
    margin_ = m;
    layout_.setContentsMargins(m, m, m, m);
}

/// Arrange all sub widgets according to current widget size.
/// Ensure we fill the grid layout. Widget level, different from associateData,
/// associateData will update all associated data.
void CatalogView::arrangeSubWidgets()
{
    // If rows or columns changed.
    int rows = 0;
    int cols = 0;
    calculateLayout(rows, cols);

    bool add = false;
    ContentView * p = 0;
    int index = 0;
    if (paginator().rows() != rows || paginator().cols() != cols || isFixedGrid())
    {
        // Remove all widgets from layout.
        for (int i = 0; i < sub_items_.size(); ++i)
        {
            p = sub_items_.at(i);
            layout_.removeWidget(p);
            p->hide();
        }
        add = true;
        if (isFixedGrid())
        {
            rows = paginator().rows();
            cols = paginator().cols();
        }
        paginator().setGrid(rows, cols);
        paginator().resize(rows * cols);
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (index <  sub_items_.size())
            {
                p = sub_items_.at(index);
                if (add)
                {
                    layout_.addWidget(p, i, j);
                }
                p->show();
            }
            else
            {
                p = createSubItem();
                layout_.addWidget(p, i, j);
            }
            ++index;
        }
    }
}

/// Associate data with sub widget to display content.
void CatalogView::associateData(bool force)
{
    // find the last item in the currentInvisibleParent() that will be shown in the last grid
    int count = data().size() - paginator().first_visible();
    count = std::min(count, sub_items_.size());
    for (int i = 0; i < count; ++i)
    {
        sub_items_.at(i)->updateData(data().at(paginator().first_visible() + i), force);
    }
    for (int i = count; i < sub_items_.size(); ++i)
    {
        sub_items_.at(i)->updateData(0);
    }
}

void CatalogView::associateEmptyData()
{
    for (int i = 0; i < sub_items_.size(); ++i)
    {
        sub_items_.at(i)->updateData(0);
    }
}

/// Arrange everything, including sub widgets, associate data, focus and position.
/// It does not reset paginator.
void CatalogView::arrangeAll(bool force)
{
    arrangeSubWidgets();
    associateData(force);

    // TODO: need a better way to manage focus.
    if (sub_items_.size() > 0 && sub_items_.front()->data())
    {
        sub_items_.front()->setFocus();
    }
    broadcastPositionSignal();
    onyx::screen::watcher().enqueue(parentWidget(), onyx::screen::ScreenProxy::GC);
}

int CatalogView::moveLeft(int current)
{
    if (col(current) == 0)
    {
        if (!isHorSelfRecycle())
        {
            if (searchNeighbors(LEFT) || searchNeighbors(RECYCLE_RIGHT))
            {
                return -1;
            }
        }
        else
        {
            return current = paginator().last_visible();
        }
    }
    return --current;
}

int CatalogView::moveRight(int current)
{
    if (col(current) == paginator().cols() - 1)
    {
        if (!isHorSelfRecycle())
        {
            if (searchNeighbors(RIGHT) ||searchNeighbors(RECYCLE_LEFT))
            {
                return -1;
            }
        }
        else
        {
            return current = paginator().first_visible();
        }
    }
    return ++current;
}

int CatalogView::moveUp(int current)
{
    if (row(current) == 0)
    {
        if (!isVerSelfRecycle())
        {
            if (searchNeighbors(UP) || searchNeighbors(RECYCLE_DOWN))
            {
                return -1;
            }
        }
        else
        {
            return current = paginator().last_visible();
        }
    }
    return current - paginator().cols();
}

int CatalogView::moveDown(int current)
{
    if (row(current) == paginator().rows() - 1)
    {
        if (!isVerSelfRecycle())
        {
            if (searchNeighbors(DOWN) || searchNeighbors(RECYCLE_UP))
            {
                return -1;
            }
        }
        else
        {
            return current = paginator().first_visible();
        }
    }
    return current + paginator().cols();
}

int CatalogView::row(int index)
{
    return index / paginator().cols();
}

int CatalogView::col(int index)
{
    return index % paginator().cols();
}

Paginator & CatalogView::paginator()
{
    return paginator_;
}

void CatalogView::gotoPage(const int p)
{
    if (paginator().jump(p-1))
    {
        arrangeAll();
    }
}

void CatalogView::setData(const ODatas &list, bool force)
{
    datas_ = list;
    resetPaginator(true);
    arrangeAll(force);
}

ODatas & CatalogView::data()
{
    return datas_;
}

/// \index The absolute index in data list.
void CatalogView::setFocusTo(const int row, const int col)
{
    int index = row * paginator().cols() + col;
    if (index >= 0 && index < sub_items_.size())
    {
        setChecked();
        sub_items_.at(index)->setFocus();
    }
}

ContentView* CatalogView::focusItem()
{
    QWidget *wnd = focusWidget();
    foreach(ContentView *item, sub_items_)
    {
        if (item == wnd)
        {
            return item;
        }
    }
    return 0;
}

void CatalogView::mousePressEvent ( QMouseEvent *event )
{
}

void CatalogView::mouseReleaseEvent ( QMouseEvent *event )
{
}

void CatalogView::setChecked(bool checked)
{
    if (isChecked() != checked)
    {
        checked_ = checked;
        update();
    }
}

void CatalogView::goNext()
{
    if (paginator().next())
    {
        arrangeAll();
    }
}

bool CatalogView::hasNext()
{
    return paginator().isNextEnable();
}

bool CatalogView::hasPrev()
{
    return paginator().isPrevEnable();
}

void CatalogView::goPrev()
{
    if (paginator().prev())
    {
        arrangeAll();
    }
}

void CatalogView::keyReleaseEvent ( QKeyEvent *ke )
{
    ke->accept();
    switch ( ke->key())
    {
    case Qt::Key_PageDown:
        {
            goNext();
        }
        break;
    case Qt::Key_PageUp:
        {
            goPrev();
        }
        break;
    default:
        QWidget::keyReleaseEvent(ke);
        break;
    }
}

void CatalogView::keyPressEvent(QKeyEvent*e )
{
    QWidget::keyPressEvent(e);
}

/// This function returns widget that has new focus.
/// If it returns itself, we need to refresh whole screen.
QWidget * CatalogView::moveFocus(int index)
{
    if (index > data().size() )
    {
        return 0;
    }

    if (index >= paginator().items_per_page())
    {
        goNext();
        return 0;
    }
    else if ( index < 0)
    {
        goPrev();
        return 0;
    }
    else
    {
        ContentView * p = sub_items_.at(index);
        if (p->data())
        {
            p->setFocus();
            return p;
        }
    }
    return 0;
}

void CatalogView::changeEvent ( QEvent *event )
{
}

void CatalogView::resizeEvent ( QResizeEvent * event )
{
    QWidget::resizeEvent ( event );
    arrangeSubWidgets();
    arrangeAll();
}

void CatalogView::paintEvent ( QPaintEvent * event )
{
    QPainter painter(this);
    if (isChecked() && hasBorder())
    {
        int pen_width = margin() - 2;
        QPen pen;
        pen.setWidth(pen_width);
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawRoundedRect(rect().adjusted(0, 0, -pen_width , -pen_width), 5, 5);
    }
}

ContentView* CatalogView::createSubItem()
{
    ContentView * instance = 0;
    if (!factory_)
    {
        qWarning("Content view factory is empty, use default factory.");
        instance = new CoverView(this);
    }
    else
    {
        instance = factory_->createView(this, sub_item_type_);
    }
    connect(instance, SIGNAL(activated(ContentView*,int)), this, SLOT(onItemActivated(ContentView *,int)));
    connect(instance, SIGNAL(keyRelease(ContentView*, QKeyEvent*)), this, SLOT(onItemKeyRelease(ContentView *, QKeyEvent*)));
    connect(instance, SIGNAL(mouse(QPoint, QPoint)), this, SLOT(onMouseMoved(QPoint, QPoint)));
    sub_items_.push_back(instance);

    QSize s = preferItemSize();
    if (s.height() <= 0)
    {
        // instance->setFixedWidth(s.width());
    }
    else if (s.width() <= 0)
    {
        // instance->setFixedHeight(s.height());
    }
    else
    {
        // instance->setFixedSize(s);
    }
    return instance;
}

void CatalogView::setFixedGrid(int rows, int cols)
{
    fixed_grid_ = true;
    paginator().setGrid(rows, cols);
}

bool CatalogView::isFixedGrid()
{
    return fixed_grid_;
}

QSize CatalogView::preferItemSize()
{
    if (isFixedGrid())
    {
        return QSize(width() / paginator().cols(), height() / paginator().rows());
    }
    return size_;
}

void CatalogView::setPreferItemSize(const QSize &size)
{
    if (size_ != size)
    {
        size_ = size;
        arrangeSubWidgets();
        arrangeAll();
    }
}

int CatalogView::rows()
{
    return paginator().rows();
}

int CatalogView::cols()
{
    return paginator().cols();
}

void CatalogView::setSubItemType(const QString &type)
{
    sub_item_type_ = type;
}

void CatalogView::broadcastPositionSignal()
{
    qDebug("position changed %d %d", paginator().currentPage(), paginator().pages());
    emit positionChanged(paginator().currentPage(), paginator().pages());
}

void CatalogView::resetPaginator(bool sync_layout)
{
    if (sync_layout && !isFixedGrid())
    {
        int rows = 0;
        int cols = 0;
        calculateLayout(rows, cols);
        paginator().reset(0, rows * cols, data().size());
        paginator().setGrid(rows, cols);
    }
    else
    {
        paginator().reset(0, paginator().rows() * paginator().cols(), data().size());
    }
}

void CatalogView::onItemActivated(ContentView *item, int user_data)
{
    if (item == 0 || item->data() == 0)
    {
        return;
    }
    setChecked();
    emit itemActivated(this, item, user_data);
}

void CatalogView::onItemKeyRelease(ContentView *item, QKeyEvent *key)
{
    QWidget * p = 0;
    int index = visibleSubItems().indexOf(item);

    // check index at first.

    switch (key->key())
    {
    case Qt::Key_Left:
        {
            index = moveLeft(index);
        }
        break;
    case Qt::Key_Right:
        {
            index = moveRight(index);
        }
        break;
    case Qt::Key_Up:
        {
            index = moveUp(index);
        }
        break;
    case Qt::Key_Down:
        {
            index = moveDown(index);
        }
        break;
    default:
        return;
    }
    p = moveFocus(index);
    if (p)
    {
        onyx::screen::watcher().enqueue(item, onyx::screen::ScreenProxy::DW);
        onyx::screen::watcher().enqueue(p, onyx::screen::ScreenProxy::DW);
    }
}

void CatalogView::onMouseMoved(QPoint last, QPoint current)
{
    int direction = sys::SystemConfig::direction(last, current);

    if (direction > 0)
    {
        goNext();
    }
    else if (direction < 0)
    {
        goPrev();
    }
}

QString CatalogView::invert(const QString &type)
{
    if (type == LEFT)
    {
        return RIGHT;
    }
    else if (type == RIGHT)
    {
        return LEFT;
    }
    else if (type == UP)
    {
        return DOWN;
    }
    else if (type == DOWN)
    {
        return UP;
    }
    else if (type == RECYCLE_RIGHT)
    {
        return RECYCLE_LEFT;
    }
    else if (type == RECYCLE_LEFT)
    {
        return RECYCLE_RIGHT;
    }
    else if (type == RECYCLE_UP)
    {
        return RECYCLE_DOWN;
    }
    else if (type == RECYCLE_DOWN)
    {
        return RECYCLE_UP;
    }
    return QString();
}

void CatalogView::setNeighbor(CatalogView *neighbor, const QString &type)
{
    // caller should check.
    addNeighbor(type, neighbor);
    neighbor->addNeighbor(invert(type), this);
}

bool CatalogView::removeNeighbor(CatalogView *neighbor, const QString& type)
{
    CatalogViews & views = neighbors(type);
    int index = views.indexOf(neighbor);
    if (index < 0)
    {
        return false;
    }
    views.erase(views.begin() + index);
    return neighbor->removeNeighbor(this, invert(type));
}

ContentView* CatalogView::findShortestItem(CatalogView *view,
                                           QWidget *target,
                                           int & dist,
                                           const QPoint &offset)
{
    // visit all and check the shortest item.
    int tmp = INT_MAX;
    ContentView *ret = view->sub_items_.first();
    foreach(ContentView * item, view->sub_items_)
    {
        int d = 0;
        QPoint target_center = ui::globalCenter(target);
        QPoint item_center = ui::globalCenter(item);
        target_center += offset;
        d = ui::distance(target_center, item_center);

        if (d < tmp && item->data())
        {
            tmp = d;
            ret = item;
        }
    }
    dist = tmp;
    return ret;
}

CatalogView::CatalogViews & CatalogView::neighbors(const QString &type)
{
    return neighbors_[type];
}

bool CatalogView::addNeighbor(const QString &type, CatalogView *neighbor)
{
    CatalogViews & views = neighbors(type);
    if (views.indexOf(neighbor) >= 0)
    {
        return false;
    }
    views.push_back(neighbor);
    return true;
}

bool CatalogView::searchNeighbors(const QString &type)
{
    // set focus to shortest item.
    CatalogViews & views = neighbors(type);
    if (views.isEmpty())
    {
        return false;
    }
    QPoint offset;
    int dist = INT_MAX;
    ContentView * ret = 0;

    QWidget * wnd = focusItem();
    foreach(CatalogView *view, views)
    {
        // Adjust offset if necessary.
        if (type == RECYCLE_LEFT)
        {
            offset.setX(view->mapToGlobal(QPoint()).x() - wnd->mapToGlobal(QPoint()).x());
        }
        else if (type == RECYCLE_RIGHT)
        {
            offset.setX(view->mapToGlobal(QPoint()).x() + view->width());
        }
        else if (type == RECYCLE_UP)
        {
            offset.setY(wnd->mapToGlobal(QPoint()).y() - view->mapToGlobal(QPoint()).y());
        }
        else if (type == RECYCLE_DOWN)
        {
            offset.setY(view->mapToGlobal(QPoint()).y() + view->height());
        }

        int tmp = 0;
        ContentView * item = findShortestItem(view, wnd, tmp, offset);
        if (dist > tmp)
        {
            dist = tmp;
            ret = item;
        }
    }
    if (ret)
    {
        ret->setFocus();
    }
    return true;
}

}
