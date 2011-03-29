#ifndef ONYX_PASSWORD_DIALOG_H_
#define ONYX_PASSWORD_DIALOG_H_

#include "onyx/base/base.h"
#include "ui_global.h"
#include "onyx_dialog.h"
#include "onyx_keyboard.h"
#include "line_edit_view_group.h"

namespace ui
{

class OnyxPasswordDialog: public OnyxDialog
{
    Q_OBJECT

public:
    OnyxPasswordDialog(QWidget *parent = 0, const ODatas &ds = ODatas(),
            const QString &title = QString(),
            const QString &default_passwd_label = tr("Password: "));
    ~OnyxPasswordDialog();

public:
    bool popup();
    QString value(OData * d_index = 0);

protected Q_SLOTS:
    void onItemActivated(CatalogView *catalog, ContentView *item,
            int user_data);

private:
    void appendDefaultPasswordEdit();
    void addLineEditsToGroup();

    void createLayout();
    void createLineEdits();
    void createSubMenu();
    void createShowPlainText();
    void connectWithChildren();

    CatalogView * createEditItem(OData *data, int index);

    void clearClicked();
    void setEditEchoMode(QLineEdit::EchoMode mode);
    void showPlainTextClicked(bool target_value);

    void keyPressEvent(QKeyEvent *event);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout *line_edit_layout_;

    CatalogView sub_menu_;
    CatalogView show_plain_text_;
    QVector<CatalogView *> edit_view_list_;
    LineEditViewGroup edit_view_group_;

    OnyxKeyboard keyboard_;
    QString title_;
    QString default_passwd_label_;
    ODatas edit_list_;

};

}   // namespace ui

#endif
