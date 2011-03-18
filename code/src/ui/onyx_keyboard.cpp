#include "onyx/ui/ui_utils.h"
#include "onyx/ui/onyx_keyboard_utils.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/onyx_keyboard.h"
#include "onyx/ui/keyboard_key_view.h"
#include "onyx/ui/factory.h"
#include "onyx/ui/keyboard_key_view_factory.h"
#include "onyx/ui/onyx_keyboard_language_dialog.h"

namespace ui
{

static const QSize s_size(keyboardKeyHeight(), keyboardKeyHeight());

static const int LETTER_ROWS = 3;
static const int LEFT_COLS = 3;
static const int MIDDLE_COLS = 3;
static const int RIGHT_COLS = 3;
static const int CATALOG_MARGIN = 1;

static KeyBoardKeyViewFactory keyboard_key_view_factory;

OnyxKeyboard::OnyxKeyboard(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , big_layout_(this)
    , line_edit_layout_(0)
    , layout_(0)
    , line_edit_(new Factory(), this)
    , sub_menu_(new Factory(), this)
    , top_(&keyboard_key_view_factory, this)
    , left_(&keyboard_key_view_factory, this)
    , middle_(&keyboard_key_view_factory, this)
    , right_(&keyboard_key_view_factory, this)
    , bottom_(&keyboard_key_view_factory, this)
    , menu_(&keyboard_key_view_factory, this)
    , keyboard_data_(0)
    , shift_(false)
    , symbol_(false)
    , language_(QLocale::system())
    , is_handwriting_(false)
{
    if (!parentWidget())
    {
        setFixedWidth(screenGeometry().width());
    }
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Button);
    init(language_);
    createLayout();
}

OnyxKeyboard::~OnyxKeyboard()
{
}

void OnyxKeyboard::init(const QLocale & locale)
{
    keyboard_data_ = keyboard_data_factory_.getKeyboardData(locale);
}

void OnyxKeyboard::createLayout()
{
    createLineEdit();
    createSubMenu();
    createTop();
    createLeft();
    createMiddle();
    createRight();
    createBottom();
    createMenu();

    line_edit_layout_.setContentsMargins(0, 2, 0, 0);
    line_edit_layout_.addWidget(&line_edit_);
    line_edit_layout_.addSpacing(4);
    line_edit_layout_.addWidget(&sub_menu_);
    big_layout_.addLayout(&line_edit_layout_);
    big_layout_.addWidget(&top_);
    layout_.addWidget(&left_);
    layout_.addWidget(&middle_);
    layout_.addWidget(&right_);

    big_layout_.setContentsMargins(1, 1, 1, 1);
    big_layout_.setSpacing(1);
    big_layout_.addLayout(&layout_);
    big_layout_.addWidget(&bottom_);

    big_layout_.addWidget(&menu_);

    connectWithChildren();
}

void OnyxKeyboard::connectWithChildren()
{
    connect(&sub_menu_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
                this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&top_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&left_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&middle_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&right_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&bottom_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&menu_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
}

void OnyxKeyboard::createLineEdit()
{
    line_edit_.setSubItemType(LineEditView::type());
    line_edit_.setPreferItemSize(
            QSize(rect().width()-4*keyboardKeyHeight(), 36));
    ODatas ds;
    OData *dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "");
    ds.push_back(dd);
    line_edit_.setFixedGrid(1, 1);
    line_edit_.setMargin(CATALOG_MARGIN);
    line_edit_.setData(ds);
    line_edit_.setNeighbor(&top_, CatalogView::DOWN);
    line_edit_.setNeighbor(&menu_, CatalogView::RECYCLE_DOWN);
    line_edit_.setNeighbor(&sub_menu_, CatalogView::RIGHT);
    line_edit_.setNeighbor(&sub_menu_, CatalogView::RECYCLE_RIGHT);
}

void OnyxKeyboard::createSubMenu()
{
    sub_menu_.setPreferItemSize(s_size);
    ODatas ds;
    OData *dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "OK");
    dd->insert(MENU_TYPE, KEYBOARD_MENU_OK);
    ds.push_back(dd);
    dd = new OData;
    dd->insert(ODATA_KEY_TITLE, "Clear");
    dd->insert(MENU_TYPE, KEYBOARD_MENU_CLEAR);
    ds.push_back(dd);
    sub_menu_.setFixedGrid(1, 2);
    sub_menu_.setMargin(CATALOG_MARGIN);
    sub_menu_.setFixedWidth(keyboardKeyHeight()*4);
    sub_menu_.setData(ds);
    sub_menu_.setNeighbor(&line_edit_, CatalogView::RECYCLE_LEFT);
    sub_menu_.setNeighbor(&top_, CatalogView::DOWN);
    sub_menu_.setNeighbor(&menu_, CatalogView::RECYCLE_DOWN);
}

void OnyxKeyboard::createTop()
{
    top_.data().clear();
    top_.setSubItemType(KeyboardKeyView::type());
    top_.setFixedHeight(keyboardKeyHeight());
    top_.setPreferItemSize(s_size);
    top_.setData(keyboard_data_->topCodes());
    top_.setFixedGrid(1, 10);
    top_.setMargin(CATALOG_MARGIN);
    top_.setHorSelfRecycle(true);
    top_.setNeighbor(&left_, CatalogView::DOWN);
    top_.setNeighbor(&middle_, CatalogView::DOWN);
    top_.setNeighbor(&right_, CatalogView::DOWN);
}

void OnyxKeyboard::createBottom()
{
    bottom_.data().clear();
    bottom_.setSubItemType(KeyboardKeyView::type());
    bottom_.setFixedHeight(keyboardKeyHeight());
    bottom_.setHorSelfRecycle(true);
    bottom_.setPreferItemSize(s_size);
    bottom_.setData(keyboard_data_->bottomCodes());
    bottom_.setFixedGrid(1, 10);
    bottom_.setMargin(CATALOG_MARGIN);
    bottom_.setNeighbor(&menu_, CatalogView::DOWN);
}

void OnyxKeyboard::createMenu()
{
    menu_.data().clear();
    menu_.setSubItemType(KeyboardKeyView::type());
    menu_.setFixedHeight(keyboardKeyHeight());
    menu_.setHorSelfRecycle(true);
    menu_.setPreferItemSize(s_size);
    menu_.setData(keyboard_data_->menuCodes());
    menu_.setFixedGrid(1, 4);
    menu_.setMargin(CATALOG_MARGIN);
    menu_.setNeighbor(&line_edit_, CatalogView::DOWN);
}

void OnyxKeyboard::createLeft()
{
    left_.data().clear();
    left_.setSubItemType(KeyboardKeyView::type());
    left_.setFixedHeight(keyboardKeyHeight()*3);
    left_.setPreferItemSize(s_size);
    left_.setData(keyboard_data_->leftCodes());
    left_.setFixedGrid(3, 3);
    left_.setMargin(CATALOG_MARGIN);
    left_.setNeighbor(&middle_, CatalogView::RIGHT);
    left_.setNeighbor(&right_, CatalogView::RECYCLE_RIGHT);
    left_.setNeighbor(&bottom_, CatalogView::DOWN);
}

void OnyxKeyboard::createMiddle()
{
    middle_.data().clear();
    middle_.setSubItemType(KeyboardKeyView::type());
    middle_.setFixedHeight(keyboardKeyHeight()*3);
    middle_.setPreferItemSize(s_size);
    middle_.setData(keyboard_data_->middleCodes());
    middle_.setFixedGrid(LETTER_ROWS, MIDDLE_COLS);
    middle_.setMargin(CATALOG_MARGIN);
    middle_.setNeighbor(&left_, CatalogView::LEFT);
    middle_.setNeighbor(&right_, CatalogView::RIGHT);
    middle_.setNeighbor(&bottom_, CatalogView::DOWN);
}

void OnyxKeyboard::createRight()
{
    right_.data().clear();
    right_.setSubItemType(KeyboardKeyView::type());
    right_.setFixedHeight(keyboardKeyHeight()*3);
    right_.setPreferItemSize(s_size);
    right_.setData(keyboard_data_->rightCodes());
    right_.setFixedGrid(LETTER_ROWS, RIGHT_COLS);
    right_.setMargin(CATALOG_MARGIN);
    right_.setNeighbor(&middle_, CatalogView::LEFT);
    right_.setNeighbor(&left_, CatalogView::RECYCLE_LEFT);
    right_.setNeighbor(&bottom_, CatalogView::DOWN);
}

bool OnyxKeyboard::event(QEvent * event)
{
    bool ret = QWidget::event(event);
    if (event->type() == QEvent::UpdateRequest)
    {
        onyx::screen::watcher().updateScreen();
    }
    return ret;
}

void OnyxKeyboard::onItemActivated(CatalogView *catalog,
                                   ContentView *item,
                                   int user_data)
{
    OData * item_data = item->data();
    if (item_data->contains("menu_type"))
    {
        menuItemActivated(item, user_data);
        return;
    }
    else
    {
        int key_code;
        QString key_text;
        if (item_data->contains(ODATA_KEY_SPECIAL))
        {
            key_text = item_data->value(ODATA_KEY_SPECIAL_TEXT).toString();
            key_code = item_data->value(ODATA_KEY_SPECIAL).toInt();
        }
        else
        {
            key_text = item_data->value(ODATA_KEY_CODE).toString();
            key_code = key_text.at(0).unicode();
        }
        QKeyEvent * key_event = new QKeyEvent(QEvent::KeyPress, key_code,
                Qt::NoModifier, key_text);
        QApplication::sendEvent(line_edit_.visibleSubItems().front(),
                key_event);
    }
}

void OnyxKeyboard::menuItemActivated(ContentView *item, int user_data)
{
    int menu_type = item->data()->value(MENU_TYPE).toInt();
    if (KEYBOARD_MENU_SHIFT == menu_type)
    {
        shiftClicked();
        update();
        onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);
    }
    else if(KEYBOARD_MENU_SYMBOL == menu_type)
    {
        symbolClicked();
        update();
        onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);
    }
    else if(KEYBOARD_MENU_LANGUAGE == menu_type)
    {
        languageClicked();
    }
    else if(KEYBOARD_MENU_WRITE == menu_type)
    {
        // TODO
    }
    else if(KEYBOARD_MENU_OK == menu_type)
    {
        LineEditView *input = static_cast<LineEditView *>(
                line_edit_.visibleSubItems().front());
        QString input_text = input->innerEdit()->text();
        emit okClicked(input_text);
    }
    else if(KEYBOARD_MENU_CLEAR == menu_type)
    {
        clearClicked();
        update();
        onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::DW);
    }

}

void OnyxKeyboard::resetData(bool shift)
{
    top_.setData(keyboard_data_->topCodes(shift));
    left_.setData(keyboard_data_->leftCodes(shift));
    middle_.setData(keyboard_data_->middleCodes(shift));
    right_.setData(keyboard_data_->rightCodes(shift));
    bottom_.setData(keyboard_data_->bottomCodes(shift));
}

void OnyxKeyboard::shiftClicked()
{
    shift_ = !shift_;
    // reset the symbol menu item
    symbol_ = false;
    resetData(shift_);
}

void OnyxKeyboard::symbolClicked()
{
    symbol_ = !symbol_;
    if (!symbol_)
    {
        shift_ = true;
        shiftClicked();
    }
    else
    {
        top_.setData(keyboard_data_->topCodes(true));
        left_.setData(keyboard_data_->leftSymbolCodes());
        middle_.setData(keyboard_data_->middleSymbolCodes());
        right_.setData(keyboard_data_->rightSymbolCodes());
        bottom_.setData(keyboard_data_->bottomCodes(true));
    }
}

void OnyxKeyboard::languageClicked()
{
    OnyxKeyboardLanguageDialog dialog(language_, 0);
    onyx::screen::watcher().addWatcher(&dialog);
    int ret = dialog.exec();

    if (ret != QDialog::Rejected)
    {
        QLocale selected = dialog.selectedLocale();
        if (selected.name() != language_.name())
        {
            language_ = selected;
            init(language_);
            resetData(false);
        }
    }
}

void OnyxKeyboard::clearClicked()
{
    LineEditView *input = static_cast<LineEditView *>(
            line_edit_.visibleSubItems().front());
    input->innerEdit()->clear();
}

}   // namespace ui
