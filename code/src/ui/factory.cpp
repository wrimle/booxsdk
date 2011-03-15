
#include "onyx/ui/factory.h"
#include "onyx/ui/content_view.h"

namespace ui
{

Factory::Factory()
{
}

Factory::~Factory()
{
}

ContentView * Factory::createView(QWidget *parent, const QString &type)
{
    ContentView *target;
    if (CoverView::type() == type)
    {
        target = new CoverView(parent);
    }
    else if (CheckBoxView::type() == type)
    {
        target = new CheckBoxView(parent);
    }
    else if (LineEditView::type() == type)
    {
        target = new LineEditView(parent);
    }
    return target;
}

}
