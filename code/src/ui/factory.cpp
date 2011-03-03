
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
    return new CoverView(parent);
}

}
