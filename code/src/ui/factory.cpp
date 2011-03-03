
#include "factory.h"
#include "content_view.h"

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
