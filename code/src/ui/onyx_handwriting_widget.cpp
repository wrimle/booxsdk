#include "onyx/ui/onyx_handwriting_widget.h"
#include "onyx/data/data_tags.h"
#include "onyx/ui/keyboard_data.h"
#include "onyx/ui/keyboard_key_view_factory.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/data/handwriting_functions_model.h"

namespace handwriting
{

static const QString KEYBOARD_KEY_VIEW_TYPE = "KeyboardKeyView";
static const QString TAG_ROW = "row";
static const int SUBSET_ITEM_HEIGHT = 36;

ODataPtr createBackspaceData()
{
    QPixmap backspace_pixmap(":/images/back_spac.png");
    OData * dd = new OData;
    dd->insert(TAG_COVER, backspace_pixmap);
    dd->insert(TAG_SPECIAL_KEY, Qt::Key_Backspace);
    dd->insert(TAG_SPECIAL_KEY_TEXT, KeyboardData::BACKSPACE_TEXT);
    return dd;
}

ODataPtr createEnterData()
{
    QPixmap enter_pixmap(":/images/enter_key.png");
    OData * dd = new OData;
    dd->insert(TAG_COVER, enter_pixmap);
    dd->insert(TAG_SPECIAL_KEY, Qt::Key_Enter);
    dd->insert(TAG_SPECIAL_KEY_TEXT, KeyboardData::ENTER_TEXT);
    return dd;
}

static KeyBoardKeyViewFactory keyboard_key_view_factory;

OnyxHandwritingWidget::OnyxHandwritingWidget(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , big_layout_(this)
    , sketch_widget_layout_(0)
    , menu_(&keyboard_key_view_factory, this)
    , candidate_char_list_(0, this)
    , sketch_widget_(this)
    , char_subset_list_(0, this)
{
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Button);

    createLayout();

    QWidget * widget = safeParentWidget(parentWidget());
    resize(widget->width(), 400);

    onyx::screen::watcher().addWatcher(this);
}

OnyxHandwritingWidget::~OnyxHandwritingWidget()
{
    clearDatas(menu_datas_);
    clearDatas(candidate_char_list_datas_);
    clearDatas(char_subset_list_datas_);
}

void OnyxHandwritingWidget::createLayout()
{
    createMenu();
    createCandidateCharList();
    createSketchWidget();

    HandwritingFunctionsModel::instance().getModel(char_subset_model_);

    createCharSubsetList();

    big_layout_.setContentsMargins(0, 2, 0, 2);
    big_layout_.setSpacing(2);
    big_layout_.addWidget(&menu_, 0, Qt::AlignTop);
    big_layout_.addWidget(&candidate_char_list_, 0, Qt::AlignTop);

    sketch_widget_layout_.addWidget(&sketch_widget_, 1,
            Qt::AlignLeft | Qt::AlignVCenter);
    sketch_widget_layout_.addSpacing(2);
    sketch_widget_layout_.addWidget(&char_subset_list_, 0, Qt::AlignRight);

    big_layout_.addLayout(&sketch_widget_layout_);
}

void OnyxHandwritingWidget::createMenu()
{
    menu_.setSubItemType(KEYBOARD_KEY_VIEW_TYPE);
    menu_.setFixedHeight(keyboardKeyHeight());
    menu_.setSearchPolicy(CatalogView::NeighborFirst
            | CatalogView::AutoHorRecycle);

    ODataPtr space(new OData);
    space->insert(TAG_TITLE, " ");

    ODataPtr backspace = createBackspaceData();
    ODataPtr enter = createEnterData();

    ODataPtr back_to_keyboard(new OData);
    back_to_keyboard->insert(TAG_TITLE, tr("Keyboard"));
    back_to_keyboard->insert(TAG_FONT_SIZE, 20);

    menu_datas_.push_back(space);
    menu_datas_.push_back(backspace);
    menu_datas_.push_back(enter);
    menu_datas_.push_back(back_to_keyboard);

    menu_.setData(menu_datas_);
    menu_.setFixedGrid(1, menu_datas_.size());
    menu_.setNeighbor(&char_subset_list_, CatalogView::RECYCLE_DOWN);
}

void OnyxHandwritingWidget::createCandidateCharList()
{
    candidate_char_list_.setFixedHeight(keyboardKeyHeight());
    candidate_char_list_.setSearchPolicy(CatalogView::NeighborFirst
            | CatalogView::AutoHorRecycle);

    int size = 9;
    for (int i=0; i<size; i++)
    {
        ODataPtr number(new OData);
        number->insert(TAG_TITLE, QString::number(i));
        candidate_char_list_datas_.push_back(number);
    }

    candidate_char_list_.setData(candidate_char_list_datas_);
    candidate_char_list_.setFixedGrid(1, candidate_char_list_datas_.size());
    candidate_char_list_.setNeighbor(&menu_, CatalogView::UP);
}

void OnyxHandwritingWidget::createSketchWidget()
{
    sketch_widget_.setFixedSize(QSize(400-2, 300));
}

void OnyxHandwritingWidget::createCharSubsetList()
{
    int rows = char_subset_model_.rowCount();
    char_subset_list_datas_.clear();
    for (int i = 0; i < rows; i++)
    {
        QStandardItem *item = char_subset_model_.item(i);
        OData *dd = new OData;
        dd->insert(TAG_TITLE, item->text());
        dd->insert(TAG_FONT_SIZE, 22);
        int alignment = Qt::AlignHCenter | Qt::AlignVCenter;
        dd->insert(TAG_ALIGN, alignment);
        dd->insert(TAG_ROW, i);
        char_subset_list_datas_.push_back(dd);
    }

    char_subset_list_.setPreferItemSize(QSize(-1, SUBSET_ITEM_HEIGHT));
    char_subset_list_.setFixedWidth(200);
    char_subset_list_.setSearchPolicy(CatalogView::NeighborFirst
            | CatalogView::AutoVerRecycle);
    char_subset_list_.setData(char_subset_list_datas_);
    char_subset_list_.setFixedGrid(char_subset_list_datas_.size(), 1);
    char_subset_list_.setNeighbor(&candidate_char_list_, CatalogView::UP);
}

}   // namespace handwriting
