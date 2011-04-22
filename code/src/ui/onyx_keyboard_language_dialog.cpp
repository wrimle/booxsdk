#include "onyx/ui/onyx_keyboard_language_dialog.h"
#include "onyx/ui/factory.h"
#include "onyx/ui/content_view.h"
#include "onyx/data/data.h"
#include "onyx/ui/ui_utils.h"

namespace ui
{

/// Always use UTF-8.
struct LocaleItem
{
    const char * text;
    QLocale locale;
};

// Define all supported languages.
static const LocaleItem LANGUAGES[] =
{
    {QT_TRANSLATE_NOOP("LocalePage", "English"),  QLocale(QLocale::English)},
    {QT_TRANSLATE_NOOP("LocalePage", "Russian"),  QLocale(QLocale::Russian)},
    {QT_TRANSLATE_NOOP("LocalePage", "Poland"),  QLocale(QLocale::Polish)},
    {QT_TRANSLATE_NOOP("LocalePage", "Swedish"),  QLocale(QLocale::Swedish)},
    {QT_TRANSLATE_NOOP("LocalePage", "Hungarian"),  QLocale(QLocale::Hungarian)},
};
static const int LANGUAGE_COUNT = sizeof(LANGUAGES) / sizeof(LANGUAGES[0]);

static Factory factory;

OnyxKeyboardLanguageDialog::OnyxKeyboardLanguageDialog(QLocale language,
        QWidget *parent)
    : OnyxDialog(parent)
    , layout_(&content_widget_)
    , language_group_(&factory, this)
    , language_(language)
{
    createLayout();
    resize(200, 400);
}

OnyxKeyboardLanguageDialog::~OnyxKeyboardLanguageDialog()
{
}

void OnyxKeyboardLanguageDialog::createLayout()
{
    content_widget_.setBackgroundRole(QPalette::Button);

    updateTitle(QApplication::tr("Switch Language"));
    updateTitleIcon(QPixmap(":/images/locale_small.png"));
    layout_.setContentsMargins(SPACING, 0, SPACING, 0);
    createLanguageGroup();
    layout_.addWidget(&language_group_);

    connect(&language_group_,
            SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this,
            SLOT(onItemActivated(CatalogView *, ContentView *, int)));
}

void OnyxKeyboardLanguageDialog::createLanguageGroup()
{
    language_group_.setSubItemType(CheckBoxView::type());
    language_group_.setPreferItemSize(QSize(120, defaultItemHeight()));

    ODatas ds;
    for (int i=0; i<LANGUAGE_COUNT; i++)
    {
        ODataPtr dd(new OData);
        dd->insert(TAG_TITLE, LANGUAGES[i].text);
        if (LANGUAGES[i].locale.name() == language_.name())
        {
            dd->insert(TAG_CHECKED, true);
        }
        ds.push_back(dd);
    }
    language_group_.setData(ds);

    language_group_.setFixedGrid(LANGUAGE_COUNT, 1);
    language_group_.setSearchPolicy(CatalogView::AutoVerRecycle);
}

QLocale OnyxKeyboardLanguageDialog::getLocale(const QString language_text)
{
    QLocale locale(QLocale::English);
    for (int i=0; i<LANGUAGE_COUNT; i++)
    {
        if (language_text == LANGUAGES[i].text)
        {
            locale = LANGUAGES[i].locale;
            break;
        }
    }
    return locale;
}

void OnyxKeyboardLanguageDialog::onItemActivated(CatalogView *catalog,
        ContentView *item, int user_data)
{
    OData * item_data = item->data();
    language_ = getLocale(item_data->value(TAG_TITLE).toString());
    accept();
}

}
