
#include "onyx/sys/service.h"

namespace sys
{

bool checkAndReturnBool(const QList<QVariant> & args)
{
    if (args.size() > 0)
    {
        return args.at(0).toBool();
    }
    return false;
}

}