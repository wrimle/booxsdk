
#ifndef ONYX_CONTENT_VIEW_FACTORY_H_
#define ONYX_CONTENT_VIEW_FACTORY_H_

#include "content_view.h"

namespace ui
{

class Factory
{
public:
    Factory();
    ~Factory();

public:
    ContentView * createView(QWidget *parent, const QString &type = QString());
};

};  // namespace ui

#endif      // ONYX_CONTENT_VIEW_FACTORY_H_
