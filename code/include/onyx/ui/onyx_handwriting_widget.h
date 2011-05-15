#ifndef ONYX_HANDWRITING_WIDGET_H_
#define ONYX_HANDWRITING_WIDGET_H_

#include "onyx/base/base.h"
#include "ui_global.h"
#include "ui_utils.h"
#include "catalog_view.h"
#include "onyx/data/handwriting_widget.h"

namespace sketch
{
class SketchProxy;
};

using namespace ui;

namespace handwriting
{

class OnyxHandwritingWidget: public QWidget
{
    Q_OBJECT

public:
    explicit OnyxHandwritingWidget(QWidget *parent);
    ~OnyxHandwritingWidget();

Q_SIGNALS:
    void showKeyboard();

protected Q_SLOTS:
    void onItemActivated(CatalogView *catalog, ContentView *item,
            int user_data);

private Q_SLOTS:
    void onFinishCharacterTimeOut();
    void onAutoSelect();
    void onStrokeStarted();
    void onPointAdded(SketchPoint point);
    void onStrokeAdded(const Points & points);

private:
    void createLayout();
    void createMenu();
    void createCandidateCharList();
    void createSketchWidget();
    void createCharSubsetList();
    void connectWithChildren();

    void initHandwrting();

    void charSubsetClicked(int row);
    void menuClicked(int menu_type);
    void keyClicked(OData *data);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout sketch_widget_layout_;

    CatalogView menu_;
    CatalogView candidate_char_list_;
    HandwritingWidget sketch_widget_;
    CatalogView char_subset_list_;

    ODatas menu_datas_;
    ODatas candidate_char_list_datas_;
    ODatas char_subset_list_datas_;

    QStandardItemModel char_subset_model_;
    scoped_ptr<sketch::SketchProxy> sketch_proxy_;

    OnyxSingleShotTimer finish_character_timer_;
    OnyxSingleShotTimer auto_select_timer_;
};

}   // namespace handwriting

#endif
