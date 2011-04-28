#include "onyx/ui/keyboard_config_dialog.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/data/data_tags.h"

namespace ui
{

enum ButtonType
{
    BUTTON_TYPE_OK,
    BUTTON_TYPE_CANCEL,
};

KeyboardConfigDialog::KeyboardConfigDialog(QWidget *parent)
    : OnyxDialog(parent)
    , big_layout_(&content_widget_)
    , button_layout_(0)
    , config_group_(0, this)
    , home_and_back_locked_(true)
    , page_turning_locked_(false)
{
    createLayout();
}

KeyboardConfigDialog::~KeyboardConfigDialog()
{
    clearDatas(config_group_datas_);
}

void KeyboardConfigDialog::createConfigGroup()
{
    config_group_.setSubItemType(CheckBoxView::type());
    config_group_.setPreferItemSize(QSize(-1, 60));

    ODataPtr first(new OData);
    QPixmap home_and_back(":/images/lock_home_and_back.png");
    first->insert(TAG_COVER, home_and_back);
    first->insert(TAG_CHECKED, true);
    config_group_datas_.push_back(first);

    ODataPtr second(new OData);
    QPixmap page_turning(":/images/lock_page_turning.png");
    second->insert(TAG_COVER, page_turning);
    second->insert(TAG_CHECKED, false);
    config_group_datas_.push_back(second);

    config_group_.setData(config_group_datas_);

    config_group_.setFixedGrid(2, 1);
    config_group_.setFixedHeight(120);
    config_group_.setNeighbor(&button_view_, CatalogView::DOWN);
    config_group_.setNeighbor(&button_view_, CatalogView::RECYCLE_DOWN);
}

void KeyboardConfigDialog::createButtonView()
{
    const int height = defaultItemHeight();
    button_view_.setPreferItemSize(QSize(height, height));

    ODataPtr dd(new OData);
    dd->insert(TAG_TITLE, tr("OK"));
    dd->insert(TAG_MENU_TYPE, BUTTON_TYPE_OK);
    button_view_datas_.push_back(dd);

    ODataPtr b(new OData);
    b->insert(TAG_TITLE, tr("Cancel"));
    b->insert(TAG_MENU_TYPE, BUTTON_TYPE_CANCEL);
    button_view_datas_.push_back(b);

    button_view_.setSpacing(2);
    button_view_.setFixedGrid(1, 2);
    button_view_.setFixedHeight(defaultItemHeight()+2*SPACING);
    button_view_.setFixedWidth(defaultItemHeight()*6);
    button_view_.setData(button_view_datas_);
}

void KeyboardConfigDialog::createLayout()
{
    vbox_.setSpacing(0);
    content_widget_.setBackgroundRole(QPalette::Button);
    content_widget_.setContentsMargins(0, 0, 0, 0);

    createConfigGroup();
    createButtonView();

    big_layout_.addWidget(&config_group_, 0, Qt::AlignTop);

    button_layout_.addWidget(&button_view_, 0, Qt::AlignRight);

    big_layout_.addLayout(&button_layout_);
}

int KeyboardConfigDialog::popup()
{
    if (isHidden())
    {
        show();
    }
    resize(300, 300);

    onyx::screen::watcher().addWatcher(this);
    int ret = this->exec();
    onyx::screen::watcher().removeWatcher(this);
    return ret;
}

bool KeyboardConfigDialog::homeAndBackLocked()
{
    return home_and_back_locked_;
}

bool KeyboardConfigDialog::pageTurningLocked()
{
    return page_turning_locked_;
}

void KeyboardConfigDialog::onItemActivated(CatalogView *catalog,
                                   ContentView *item,
                                   int user_data)
{
    OData * item_data = item->data();
    if (item_data->contains(TAG_MENU_TYPE))
    {
        int menu_type = item->data()->value(TAG_MENU_TYPE).toInt();
        if(BUTTON_TYPE_OK == menu_type)
        {
            // TODO
        }
        else if(BUTTON_TYPE_CANCEL == menu_type)
        {
            onCloseClicked();
        }
    }
}

}
