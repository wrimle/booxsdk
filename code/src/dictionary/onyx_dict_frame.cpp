#include "onyx/dictionary/onyx_dict_frame.h"
#include "onyx/data/data_tags.h"
#include "onyx/screen/screen_update_watcher.h"

enum OnyxDictionaryMenuType
{
    MENU_DICTIONARIES = 11,
    MENU_SIMULAR_WORDS = 12,
    MENU_EXPLANATION = 13,
    MENU_LOOKUP = 14,
};

static const int DICT_MENU_FONT_SIZE = 20;

OnyxDictFrame::OnyxDictFrame(QWidget *parent, DictionaryManager & dict,
        tts::TTS *tts)
    : OnyxDialog(parent)
    , big_layout_(&content_widget_)
    , line_edit_layout_(0)
    , dict_menu_layout_(0)
    , line_edit_(0, this)
    , explanation_(0)
    , words_list_widget_(0, 0)
    , dictionary_menu_(0, this)
    , tts_button_view_(0, this)
    , keyboard_(this)
    , status_bar_(this, MENU | MESSAGE | BATTERY | CLOCK | SCREEN_REFRESH | INPUT_TEXT)
    , dict_mgr_(dict)
    , tts_engine_(tts)
    , internal_state_(-1)
{
#ifndef Q_WS_QWS
    resize(600, 800);
#else
    resize(qApp->desktop()->screenGeometry().size());
#endif

    createLayout();
}

OnyxDictFrame::~OnyxDictFrame()
{

}

void OnyxDictFrame::createLineEdit()
{
    line_edit_.setSubItemType(LineEditView::type());
    line_edit_.setPreferItemSize(QSize(rect().width(), defaultItemHeight()));

    ODatas ds;
    OData *dd = new OData;
    dd->insert(TAG_TITLE, "");
    ds.push_back(dd);

    line_edit_.setFixedGrid(1, 1);
    line_edit_.setFixedHeight(defaultItemHeight()+2*SPACING);
    line_edit_.setMargin(OnyxKeyboard::CATALOG_MARGIN);
    line_edit_.setData(ds);
    line_edit_.setNeighbor(&dictionary_menu_, CatalogView::DOWN);
    line_edit_.setNeighbor(keyboard_.menu(), CatalogView::RECYCLE_DOWN);
    line_edit_.setNeighbor(&sub_menu_, CatalogView::RIGHT);
    line_edit_.setNeighbor(&sub_menu_, CatalogView::RECYCLE_RIGHT);
}

void OnyxDictFrame::createSubMenu()
{
    const int height = defaultItemHeight();
    sub_menu_.setPreferItemSize(QSize(height, height));
    ODatas ds;

    OData *dd = new OData;
    dd->insert(TAG_TITLE, tr("Clear"));
    dd->insert(TAG_MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_CLEAR);
    ds.push_back(dd);

    sub_menu_.setFixedGrid(1, 1);
    sub_menu_.setMargin(OnyxKeyboard::CATALOG_MARGIN);
    sub_menu_.setFixedHeight(defaultItemHeight()+2*SPACING);
    sub_menu_.setFixedWidth(defaultItemHeight()*3);
    sub_menu_.setData(ds);
    sub_menu_.setNeighbor(&line_edit_, CatalogView::RECYCLE_LEFT);
    sub_menu_.setNeighbor(&dictionary_menu_, CatalogView::DOWN);
    sub_menu_.setNeighbor(keyboard_.menu(), CatalogView::RECYCLE_DOWN);
}

void OnyxDictFrame::createDictionaryMenu()
{
    const int height = defaultItemHeight();
    dictionary_menu_.setPreferItemSize(QSize(height, height));
    ODatas ds;

    OData *dd = new OData;
    dd->insert(TAG_TITLE, tr("Dictionaries"));
    dd->insert(TAG_MENU_TYPE, MENU_DICTIONARIES);
    dd->insert(TAG_FONT_SIZE, DICT_MENU_FONT_SIZE);
    ds.push_back(dd);

    dd = new OData;
    dd->insert(TAG_TITLE, tr("Similar Words"));
    dd->insert(TAG_MENU_TYPE, MENU_SIMULAR_WORDS);
    dd->insert(TAG_FONT_SIZE, DICT_MENU_FONT_SIZE);
    ds.push_back(dd);

    dd = new OData;
    dd->insert(TAG_TITLE, tr("Explanation"));
    dd->insert(TAG_MENU_TYPE, MENU_EXPLANATION);
    dd->insert(TAG_FONT_SIZE, DICT_MENU_FONT_SIZE);
    ds.push_back(dd);

    dd = new OData;
    dd->insert(TAG_TITLE, tr("Lookup"));
    dd->insert(TAG_MENU_TYPE, MENU_LOOKUP);
    dd->insert(TAG_FONT_SIZE, DICT_MENU_FONT_SIZE);
    ds.push_back(dd);

    dictionary_menu_.setFixedGrid(1, 4);
    dictionary_menu_.setMargin(OnyxKeyboard::CATALOG_MARGIN);
    dictionary_menu_.setFixedHeight(defaultItemHeight()+5*SPACING);
    dictionary_menu_.setData(ds);
    dictionary_menu_.setNeighbor(&line_edit_, CatalogView::UP);
    dictionary_menu_.setNeighbor(&sub_menu_, CatalogView::UP);
    dictionary_menu_.setNeighbor(keyboard_.top(), CatalogView::DOWN);
    dictionary_menu_.setNeighbor(&tts_button_view_, CatalogView::RIGHT);
    dictionary_menu_.setNeighbor(&tts_button_view_, CatalogView::RECYCLE_RIGHT);
}

void OnyxDictFrame::createTtsButtonView()
{
    const int height = defaultItemHeight();
    tts_button_view_.setPreferItemSize(QSize(height, height));
    ODatas ds;

    OData *dd = new OData;
    QPixmap tts_pixmap(":/images/tts_menu.png");
    dd->insert(TAG_COVER, tts_pixmap);
    dd->insert(TAG_MENU_TYPE, MENU_DICTIONARIES);
    ds.push_back(dd);

    tts_button_view_.setFixedGrid(1, 1);
    tts_button_view_.setMargin(OnyxKeyboard::CATALOG_MARGIN);
    tts_button_view_.setFixedHeight(defaultItemHeight()+5*SPACING);
    tts_button_view_.setFixedWidth(defaultItemHeight()+7*SPACING);
    tts_button_view_.setData(ds);
    tts_button_view_.setNeighbor(&sub_menu_, CatalogView::UP);
    tts_button_view_.setNeighbor(keyboard_.top(), CatalogView::DOWN);
    tts_button_view_.setNeighbor(&dictionary_menu_, CatalogView::LEFT);
    tts_button_view_.setNeighbor(&dictionary_menu_, CatalogView::RECYCLE_LEFT);
}

void OnyxDictFrame::createLayout()
{
    OnyxDialog::updateTitle(tr("Dictionary"));
    updateTitleIcon(QPixmap(":/images/dictionary_title.png"));
    content_widget_.setBackgroundRole(QPalette::Button);
    content_widget_.setContentsMargins(0, 0, 0, 0);

    createLineEdit();
    createSubMenu();
    createDictionaryMenu();
    createTtsButtonView();

    line_edit_layout_.setContentsMargins(0, 2, 0, 0);
    line_edit_layout_.setSpacing(2);
    line_edit_layout_.addWidget(&line_edit_);
    line_edit_layout_.addWidget(&sub_menu_);

    big_layout_.setContentsMargins(2, 2, 2, 2);
    big_layout_.setSpacing(0);
    big_layout_.addLayout(&line_edit_layout_, 0);

    // for explanation and similar words list
    big_layout_.addWidget(&explanation_);
    big_layout_.addWidget(&words_list_widget_);
    words_list_widget_.setVisible(false);

    dict_menu_layout_.setContentsMargins(0, 2, 0, 0);
    dict_menu_layout_.setSpacing(2);
    dict_menu_layout_.addWidget(&dictionary_menu_);
    dict_menu_layout_.addWidget(&tts_button_view_);

    big_layout_.addLayout(&dict_menu_layout_, 0);
    big_layout_.addWidget(&keyboard_);
    big_layout_.addWidget(&status_bar_);
}

bool OnyxDictFrame::lookup(const QString &word)
{
    // TODO
    return false;
}

void OnyxDictFrame::setDefaultFocus()
{
    // TODO default item not determined
    keyboard_.top()->visibleSubItems().front()->setFocus();
}

void OnyxDictFrame::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    update();
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);
}

void OnyxDictFrame::onItemActivated(CatalogView *catalog, ContentView *item,
                                   int user_data)
{
    OData * item_data = item->data();
    if (item_data->contains(TAG_MENU_TYPE))
    {
        // TODO
    }
}
