#ifndef ONYX_HANDWRITING_WIDGET_H_
#define ONYX_HANDWRITING_WIDGET_H_

#include "onyx/base/base.h"
#include "ui_global.h"
#include "ui_utils.h"
#include "catalog_view.h"
#include "onyx/data/handwriting_widget.h"

using namespace ui;

namespace handwriting
{

class OnyxHandwritingWidget: public QWidget
{
    Q_OBJECT

public:
    explicit OnyxHandwritingWidget(QWidget *parent);
    ~OnyxHandwritingWidget();

private:
    void createLayout();
    void createMenu();
    void createCandidateCharList();
    void createSketchWidget();
    void createCharSubsetList();

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
};

}   // namespace handwriting

#endif
