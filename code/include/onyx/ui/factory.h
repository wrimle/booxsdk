
#ifndef ONYX_CONTENT_VIEW_FACTORY_H_
#define ONYX_CONTENT_VIEW_FACTORY_H_

#include "content_view.h"

class Factory
{
public:
    Factory();
    ~Factory();

public:
    ContentView * createView(QWidget *parent, const QString &type = QString());
};

#endif      // ONYX_PAGINATOR_H_
