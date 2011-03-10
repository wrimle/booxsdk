#include <QDebug>

#include "onyx/data/data.h"


OData::OData(const QVariantMap & vm)
: QVariantMap(vm)
{
}

OData::~OData()
{
}

void OData::debugDump() const
{
    QString key;
    QString value;

    OData::const_iterator i = constBegin();
    qDebug() << "Dumping OData properties";
    while ( i != constEnd() )
    {
        qDebug() << i.key() << ": " << i.value().toString();
        i++;
    }
}

void clearDatas(ODatas & datas)
{
    foreach(OData *d, datas)
    {
        delete d;
    }
    datas.clear();
}

