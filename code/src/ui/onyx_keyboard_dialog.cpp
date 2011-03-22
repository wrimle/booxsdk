#include "onyx/ui/onyx_keyboard_dialog.h"
#include "onyx/ui/onyx_keyboard_utils.h"
#include "onyx/screen/screen_update_watcher.h"

namespace ui
{

static const int CATALOG_MARGIN = 1;

// The text of "OK" item can be changed by specifying ok_button_text.
OnyxKeyboardDialog::OnyxKeyboardDialog(QWidget *parent,
        const QString ok_button_text)
    : OnyxDialog(parent)
    , big_layout_(&content_widget_)
    , line_edit_layout_(0)
    , line_edit_(0, this)
    , sub_menu_(0, this)
    , keyboard_(this)
    , ok_button_text_(ok_button_text)
{
    createLayout();
    connectWithChildren();
}

OnyxKeyboardDialog::~OnyxKeyboardDialog()
{
}

int OnyxKeyboardDialog::popup(int bottom_margin)
{
    if (isHidden())
    {
        show();
    }
    resize(parentWidget()->width(), height());
    move(parentWidget()->x(), parentWidget()->height() - height());
    return exec();
}

void OnyxKeyboardDialog::createLineEdit()
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

void OnyxKeyboardDialog::createSubMenu()
{
    const int height = WIDGET_HEIGHT;
    sub_menu_.setPreferItemSize(QSize(height, height));
    ODatas ds;
    OData *dd = new OData;
    dd->insert(ODATA_KEY_TITLE, ok_button_text_);
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

void OnyxKeyboardDialog::createLayout()
{
    vbox_.setSpacing(0);
    content_widget_.setBackgroundRole(QPalette::Button);
    content_widget_.setContentsMargins(0, 0, 0, 0);

    createLineEdit();
    createSubMenu();

    line_edit_layout_.setContentsMargins(0, 2, 0, 0);
    line_edit_layout_.addWidget(&line_edit_);
    line_edit_layout_.setSpacing(2);
    line_edit_layout_.addWidget(&sub_menu_);

    big_layout_.addLayout(&line_edit_layout_);
    big_layout_.addWidget(&keyboard_);
}

void OnyxKeyboardDialog::onItemActivated(CatalogView *catalog,
                                   ContentView *item,
                                   int user_data)
{
    // TODO
    qDebug() << "item activated: " << item->data()->value(ODATA_KEY_TITLE);
}

void OnyxKeyboardDialog::connectWithChildren()
{
    connect(&line_edit_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&sub_menu_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
}

void OnyxKeyboardDialog::clearClicked()
{
    LineEditView *input = static_cast<LineEditView *>(
            line_edit_.visibleSubItems().front());
    input->innerEdit()->clear();
}

void OnyxKeyboardDialog::keyPressEvent(QKeyEvent *event)
{
    QApplication::sendEvent(line_edit_.visibleSubItems().front(), event);
}

}   // namespace ui
