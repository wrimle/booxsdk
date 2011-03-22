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
    OnyxKeyboardDialog(QWidget *parent = 0,
            const QString ok_button_text = "OK");
    ~OnyxKeyboardDialog();

public:
    int popup(int bottom_margin);

Q_SIGNALS:
    void okClicked(const QString text);

protected Q_SLOTS:
    void onItemActivated(CatalogView *catalog, ContentView *item,
            int user_data);

private:
    void createLayout();
    void createLineEdit();
    void createSubMenu();
    void connectWithChildren();

    void clearClicked();

    // handling key press event
    void keyPressEvent(QKeyEvent *event);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout line_edit_layout_;
    CatalogView line_edit_;
    // contains OK and Clear menu items
    CatalogView sub_menu_;

    OnyxKeyboard keyboard_;
    QString ok_button_text_;
};

}   // namespace ui

#endif
