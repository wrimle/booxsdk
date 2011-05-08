#include "onyx/ui/onyx_handwriting_widget.h"
#include "onyx/data/data_tags.h"
#include "onyx/ui/keyboard_data.h"
#include "onyx/ui/keyboard_key_view_factory.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/data/handwriting_functions_model.h"
#include "onyx/data/handwriting_manager.h"

namespace handwriting
{

enum HandwritingMenuType
{
    HANDWRITING_MENU_TYPE = 11,
};

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
    , vertical_layout_(0)
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

    connectWithChildren();
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

    vertical_layout_.addWidget(&menu_, 0, Qt::AlignTop);
    vertical_layout_.addWidget(&candidate_char_list_, 0, Qt::AlignTop);
    vertical_layout_.addWidget(&sketch_widget_, 0);

    big_layout_.addLayout(&vertical_layout_, 1);
    big_layout_.addWidget(&char_subset_list_, 0, Qt::AlignRight);
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
    back_to_keyboard->insert(TAG_MENU_TYPE, HANDWRITING_MENU_TYPE);

    menu_datas_.push_back(space);
    menu_datas_.push_back(backspace);
    menu_datas_.push_back(enter);
    menu_datas_.push_back(back_to_keyboard);

    menu_.setData(menu_datas_);
    menu_.setFixedGrid(1, menu_datas_.size());
    menu_.setNeighbor(&char_subset_list_, CatalogView::RIGHT);
    menu_.setNeighbor(&char_subset_list_, CatalogView::RECYCLE_RIGHT);
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
    candidate_char_list_.setNeighbor(&menu_, CatalogView::RECYCLE_UP);
    candidate_char_list_.setNeighbor(&char_subset_list_, CatalogView::RIGHT);
    candidate_char_list_.setNeighbor(&char_subset_list_, CatalogView::RECYCLE_RIGHT);
}

void OnyxHandwritingWidget::createSketchWidget()
{
    sketch_widget_.setFixedHeight(300);
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
}

void OnyxHandwritingWidget::connectWithChildren()
{
    connect(&menu_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&candidate_char_list_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&char_subset_list_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
}

void OnyxHandwritingWidget::charSubsetClicked(int row)
{
    int row_count = char_subset_model_.rowCount();
    if (row < 0 || row > row_count-1)
    {
        return;
    }

    QStandardItem *standard_item = char_subset_model_.item(row);
    qDebug() << standard_item->text() << " clicked.";
    QModelIndex index = char_subset_model_.indexFromItem(standard_item);

    int item = index.data(Qt::UserRole + 1).toInt();
    HandwritingFunctionsModel::instance().onItemClicked(index, char_subset_model_);

    // switch locale or other settings
    if (item > 0)
    {
        HandwritingManager::instance().setLocale(QLocale(static_cast<QLocale::Language>(item)));
    }
    else
    {
        HandwritingManager::instance().setSpecialRecognizeRange(static_cast<SpecialRecognizeRange>(item));
    }

    // TODO: rearrange the layout of subsets according to use frequency.
}

void OnyxHandwritingWidget::menuClicked(int menu_type)
{
    if (menu_type == HANDWRITING_MENU_TYPE)
    {
        emit showKeyboard();
    }
}

void OnyxHandwritingWidget::keyClicked(OData *data)
{
    int key_code;
    QString key_text;
    if (data->contains(TAG_SPECIAL_KEY))
    {
        key_text = data->value(TAG_SPECIAL_KEY_TEXT).toString();
        key_code = data->value(TAG_SPECIAL_KEY).toInt();
    }
    else
    {
        key_text = data->value(TAG_TITLE).toString();
        key_code = key_text.at(0).unicode();
    }
    qDebug() << "key: " << key_text << "clicked.";
    // TODO send event to parent
//    QKeyEvent * key_event = new QKeyEvent(QEvent::KeyPress, key_code,
//            Qt::NoModifier, key_text);
//    QApplication::sendEvent(parentWidget(), key_event);
}

void OnyxHandwritingWidget::onItemActivated(CatalogView *catalog,
                                   ContentView *item,
                                   int user_data)
{
    if (!item && !item->data())
    {
        return;
    }

    OData *data = item->data();
    if (data->contains(TAG_ROW))
    {
        bool ok;
        int row = data->value(TAG_ROW).toInt(&ok);
        if (ok)
        {
            charSubsetClicked(row);
        }
    }
    else if (data->contains(TAG_MENU_TYPE))
    {
        bool ok;
        int menu_type = data->value(TAG_MENU_TYPE).toInt(&ok);
        if (ok)
        {
            menuClicked(menu_type);
        }
    }
    else
    {
        keyClicked(data);
    }
}

}   // namespace handwriting
