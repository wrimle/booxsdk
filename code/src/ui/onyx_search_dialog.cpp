#include "onyx/ui/onyx_search_dialog.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/ui/onyx_keyboard_utils.h"

namespace ui
{

OnyxSearchContext::OnyxSearchContext(void)
    : pattern_()
    , forward_(true)
    , case_sensitive_(false)
    , match_whole_word_(false)
    , stop_(false)
    , user_data_(0)
{
}

OnyxSearchContext::~OnyxSearchContext(void)
{
}

void OnyxSearchContext::reset()
{
    pattern_.clear();
    forward_ = true;
    case_sensitive_ = false;
    match_whole_word_ = false;
    stop_ = false;
}

void OnyxSearchContext::setPattern(const QString &pattern)
{
    pattern_ = pattern;
}

void OnyxSearchContext::setForward(bool forward)
{
    forward_ = forward;
}

void OnyxSearchContext::setCaseSensitive(bool sensitive)
{
    case_sensitive_ = sensitive;
}

void OnyxSearchContext::setMatchWholeWord(bool whole)
{
    match_whole_word_ = whole;
}

// OnyxSearchDialog
static const int CATALOG_MARGIN = 1;

// The text of "OK" item can be changed by specifying ok_button_text.
OnyxSearchDialog::OnyxSearchDialog(QWidget *parent, OnyxSearchContext & ctx)
    : OnyxDialog(parent)
    , big_layout_(&content_widget_)
    , line_edit_layout_(0)
    , line_edit_(0, this)
    , sub_menu_(0, this)
    , keyboard_(this)
    , navigate_menu_(0, this)
    , ctx_(ctx)
    , full_mode_(true)
    , update_parent_(true)
{
    createLayout();
    connectWithChildren();
}

OnyxSearchDialog::~OnyxSearchDialog()
{
}

int OnyxSearchDialog::popup(int bottom_margin)
{
    if (isHidden())
    {
        show();
    }
    resize(parentWidget()->width(), height());
    move(parentWidget()->x(), parentWidget()->height() - height());
    return exec();
}

const QString OnyxSearchDialog::inputText()
{
    LineEditView *input = static_cast<LineEditView *>(
                        line_edit_.visibleSubItems().front());
    const QString input_text = input->innerEdit()->text();
    return input_text;
}

void OnyxSearchDialog::adjustPosition()
{
    int x = 0;
    if (!keyboard_.isVisible())
    {
        x = parentWidget()->width() - width();
    }
    int y = parentWidget()->height() - height() - ui::statusBarHeight();
    move(x, y);
}

/// This function is called by parent widget to display the search widget.
void OnyxSearchDialog::ensureVisible()
{
    qDebug() << "in OnyxSearchDialog::ensureVisible begin";
    // shadows_.show(true);
    if (isHidden())
    {
        show();
    }

    updateChildrenWidgets(!full_mode_);

    if (full_mode_)
    {
        QRect parent_rect = parentWidget()->rect();
        int border = 0;
        int width = parent_rect.width() - border * 2;
        if (size().width() != width)
        {
            resize(width, height());
        }
    }

    adjustPosition();

    qDebug() << "input text is empty? " << inputText().isEmpty();
    if (inputText().isEmpty())
    {
        qDebug() << "input text is empty, set text: " << ctx_.pattern();
        LineEditView *input = static_cast<LineEditView *>(
                line_edit_.visibleSubItems().front());
        input->innerEdit()->setText(ctx_.pattern());
    }
    LineEditView *input = static_cast<LineEditView *>(
                line_edit_.visibleSubItems().front());
    input->innerEdit()->setFocus();
    updateTitle();
    qDebug() << "in OnyxSearchDialog::ensureVisible end";
}

void OnyxSearchDialog::createLineEdit()
{
    line_edit_.setSubItemType(LineEditView::type());
    line_edit_.setPreferItemSize(QSize(rect().width(), WIDGET_HEIGHT));
    ODatas ds;
    OData *dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "");
    ds.push_back(dd);
    line_edit_.setFixedGrid(1, 1);
    line_edit_.setMargin(CATALOG_MARGIN);
    line_edit_.setData(ds);
    line_edit_.setNeighbor(keyboard_.top(), CatalogView::DOWN);
    line_edit_.setNeighbor(keyboard_.menu(), CatalogView::RECYCLE_DOWN);
    line_edit_.setNeighbor(&sub_menu_, CatalogView::RIGHT);
    line_edit_.setNeighbor(&sub_menu_, CatalogView::RECYCLE_RIGHT);
}

void OnyxSearchDialog::createSubMenu()
{
    const int height = WIDGET_HEIGHT;
    sub_menu_.setPreferItemSize(QSize(height, height));
    ODatas ds;
    OData *dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "Search");
    dd->insert(MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_OK);
    ds.push_back(dd);
    dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "Clear");
    dd->insert(MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_CLEAR);
    ds.push_back(dd);
    sub_menu_.setSpacing(2);
    sub_menu_.setFixedGrid(1, 2);
    sub_menu_.setMargin(CATALOG_MARGIN);
    sub_menu_.setFixedWidth(WIDGET_HEIGHT*6);
    sub_menu_.setData(ds);
    sub_menu_.setNeighbor(&line_edit_, CatalogView::RECYCLE_LEFT);
    sub_menu_.setNeighbor(keyboard_.top(), CatalogView::DOWN);
    sub_menu_.setNeighbor(keyboard_.menu(), CatalogView::RECYCLE_DOWN);
}

void OnyxSearchDialog::createNavigateMenu()
{
    const int height = WIDGET_HEIGHT;
    navigate_menu_.setPreferItemSize(QSize(height, height));
    ODatas ds;
    OData *dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "Previous");
    // TODO add icon
//    dd->insert(ODATA_KEY_COVER, "");
    dd->insert(MENU_TYPE, SEARCH_NAV_PREVIOUS);
    ds.push_back(dd);
    dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "Next");
    // TODO add icon
//    dd->insert(ODATA_KEY_COVER, "");
    dd->insert(MENU_TYPE, SEARCH_NAV_NEXT);
    ds.push_back(dd);
    navigate_menu_.setSpacing(2);
    navigate_menu_.setFixedGrid(1, 2);
    navigate_menu_.setMargin(CATALOG_MARGIN);
    navigate_menu_.setFixedWidth(WIDGET_HEIGHT*6);
    navigate_menu_.setData(ds);
    navigate_menu_.setSearchPolicy(CatalogView::NeighborFirst
            | CatalogView::AutoHorRecycle);
}

void OnyxSearchDialog::createLayout()
{
    vbox_.setSpacing(0);
//    updateTitleIcon(QPixmap(":/images/search.png"));
    content_widget_.setBackgroundRole(QPalette::Button);
    content_widget_.setContentsMargins(0, 0, 0, 0);

    createLineEdit();
    createSubMenu();
    createNavigateMenu();

    line_edit_layout_.setContentsMargins(0, 2, 0, 0);
    line_edit_layout_.addWidget(&line_edit_);
    line_edit_layout_.setSpacing(2);
    line_edit_layout_.addWidget(&sub_menu_);

    big_layout_.addLayout(&line_edit_layout_);
    big_layout_.addWidget(&keyboard_);
    // navigate_menu_ will be hidden at initialization.
    big_layout_.addWidget(&navigate_menu_);
}

void OnyxSearchDialog::onItemActivated(CatalogView *catalog,
                                   ContentView *item,
                                   int user_data)
{
    OData * item_data = item->data();
    if (item_data->contains(MENU_TYPE))
    {
        int menu_type = item->data()->value(MENU_TYPE).toInt();
        if(OnyxKeyboard::KEYBOARD_MENU_OK == menu_type)
        {
            onSearchClicked();
        }
        else if(OnyxKeyboard::KEYBOARD_MENU_CLEAR == menu_type)
        {
            clearClicked();
            update();
            onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::DW);
        }
        else if (SEARCH_NAV_PREVIOUS == menu_type)
        {
            onSearchPrevClicked();
        }
        else if (SEARCH_NAV_NEXT == menu_type)
        {
            onSearchNextClicked();
        }
    }
}

void OnyxSearchDialog::connectWithChildren()
{
    connect(&line_edit_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&sub_menu_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&navigate_menu_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
}

void OnyxSearchDialog::clearClicked()
{
    LineEditView *input = static_cast<LineEditView *>(
            line_edit_.visibleSubItems().front());
    input->innerEdit()->clear();
}

void OnyxSearchDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (Qt::Key_Up != key
            && Qt::Key_Down != key
            && Qt::Key_Left != key
            && Qt::Key_Right != key)
    {
        QApplication::sendEvent(line_edit_.visibleSubItems().front(), event);
    }
}

void OnyxSearchDialog::keyReleaseEvent(QKeyEvent *ke)
{
    int key = ke->key();
    if (key == Qt::Key_Escape || key == ui::Device_Menu_Key)
    {
        onCloseClicked();
        return;
    }
}

void OnyxSearchDialog::readyToSearch(bool forward)
{
    ctx_.setPattern(inputText());
    ctx_.setForward(forward);
    emit search(ctx_);
}

void OnyxSearchDialog::updateTitle(const QString &message)
{
    if (message.isEmpty())
    {
        if (full_mode_)
        {
            OnyxDialog::updateTitle(tr("Search"));
        }
        else
        {
            OnyxDialog::updateTitle(tr("Search") + " " + ctx_.pattern());
        }
    }
    else
    {
        OnyxDialog::updateTitle(message);
    }
}

/// This function is called when caller can not find any matched
/// result.
void OnyxSearchDialog::noMoreMatches()
{
    // Heave ghosting when using the fast waveform.
    // Use full instead.
    updateTitle(tr("No more matches"));
    update();
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);
}

/// Update all widget according to the specified parameter.
void OnyxSearchDialog::updateChildrenWidgets(bool searching)
{
    if (searching)
    {
        navigate_menu_.show();
        line_edit_.hide();
        sub_menu_.hide();
        keyboard_.hide();
    }
    else
    {
        navigate_menu_.hide();
        line_edit_.show();
        sub_menu_.show();
        keyboard_.show();
    }
    adjustSize();
}

void OnyxSearchDialog::onSearchClicked()
{
    full_mode_ = false;
    onyx::screen::instance().enableUpdate(false);
    updateChildrenWidgets(true);
    ctx_.setPattern(inputText());
    updateTitle();
    adjustPosition();
    QApplication::processEvents();
    onyx::screen::instance().enableUpdate(true);
    update();
    onyx::screen::watcher().enqueue(parentWidget(), onyx::screen::ScreenProxy::GU);
    readyToSearch(ctx_.forward());
}

void OnyxSearchDialog::onSearchNextClicked()
{
    updateTitle();
    readyToSearch(true);
}

void OnyxSearchDialog::onSearchPrevClicked()
{
    updateTitle();
    readyToSearch(false);
}

void OnyxSearchDialog::moveEvent(QMoveEvent *e)
{
    OnyxDialog::moveEvent(e);
    update();
    onyx::screen::watcher().enqueue(parentWidget(), onyx::screen::ScreenProxy::GC);
}

void OnyxSearchDialog::resizeEvent(QResizeEvent *e)
{
    OnyxDialog::resizeEvent(e);
    adjustPosition();
    update();
    onyx::screen::watcher().enqueue(parentWidget(), onyx::screen::ScreenProxy::GC);
}

void OnyxSearchDialog::onCloseClicked()
{
    // Make sure caller stop searching.
    ctx_.stop(true);
    done(QDialog::Rejected);
    full_mode_ = true;
    emit closeClicked();
    update();
    onyx::screen::watcher().enqueue(parentWidget(), onyx::screen::ScreenProxy::GC);
}

}   // namespace ui
