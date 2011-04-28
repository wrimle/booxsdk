#ifndef KEYBOARD_CONFIG_DIALOG_H_
#define KEYBOARD_CONFIG_DIALOG_H_

#include "onyx/base/base.h"
#include "ui_utils.h"
#include "ui_global.h"
#include "onyx/data/data.h"
#include "catalog_view.h"
#include "onyx_dialog.h"

namespace ui
{

class KeyboardConfigDialog: public OnyxDialog
{
    Q_OBJECT

public:
    KeyboardConfigDialog(QWidget *parent);
    ~KeyboardConfigDialog();

    int popup();

    bool homeAndBackLocked();
    bool pageTurningLocked();

private:
    void createLayout();
    void createConfigGroup();
    void createButtonView();

    void onItemActivated(CatalogView *catalog, ContentView *item, int user_data);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout button_layout_;

    CatalogView config_group_;
    ODatas config_group_datas_;

    CatalogView button_view_;
    ODatas button_view_datas_;

    bool home_and_back_locked_;
    bool page_turning_locked_;

};

}   // namespace ui

#endif
