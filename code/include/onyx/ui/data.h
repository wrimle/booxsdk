
#ifndef ONYX_DATA_INTERFACE_H_
#define ONYX_DATA_INTERFACE_H_

#include <QtCore/QtCore>
#include <QPixmap>

namespace ui
{

/// Onyx content. It can be an ebook or a catalog.
class OData : public QVariantMap
{
public:
    OData();
    virtual ~OData();

public:
    void debugDump();

};

typedef QVector<OData *> ODatas;

};

#endif

