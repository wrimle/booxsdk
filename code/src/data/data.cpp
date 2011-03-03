#include <QDebug>

#include "onyx/data/data.h"


OData::OData()
: QVariantMap()
{
}

OData::~OData()
{
}

void OData::debugDump()
{
// #ifdef QT_DEBUG
    QString key;
    QString value;

    OData::const_iterator i = constBegin();
    qDebug() << "Dumping OData properties";
    while ( i != constEnd() )
    {
        qDebug() << i.key() << ": " << i.value().toString();
        i++;
    }
// #endif
}



