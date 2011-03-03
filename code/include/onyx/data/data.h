
#ifndef ONYX_DATA_INTERFACE_H_
#define ONYX_DATA_INTERFACE_H_

#include <QVariantMap>

/// Onyx content. It can be an ebook or a catalog.
class OData : public QVariantMap
{
public:
    OData(const QVariantMap & vm = QVariantMap());
    virtual ~OData();

public:
    void debugDump();

};

typedef QVector<OData *> ODatas;


#endif

