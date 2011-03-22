#ifndef ONYX_KEYBOARD_DIALOG_H_
#define ONYX_KEYBOARD_DIALOG_H_

#include "onyx/base/base.h"
#include "ui_global.h"
#include "onyx_dialog.h"
#include "catalog_view.h"
#include "onyx_keyboard.h"

namespace ui
{

// Dialog for getting input with soft keyboard support.
class OnyxKeyboardDialog: public OnyxDialog
{
    Q_OBJECT

public:
    OnyxKeyboardDialog(QWidget *parent = 0);
    ~OnyxKeyboardDialog();

public:
    int popup(int bottom_margin);

private:
    void createLayout();
    void createLineEdit();
    void createSubMenu();

    void clearClicked();

private:
    QVBoxLayout big_layout_;
    QHBoxLayout line_edit_layout_;
    CatalogView line_edit_;
    // contains OK and Clear menu items
    CatalogView sub_menu_;

    OnyxKeyboard keyboard_;
};

}   // namespace ui

#endif
