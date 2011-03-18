#ifndef ONYX_KEYBOARD_UTILS_H_
#define ONYX_KEYBOARD_UTILS_H_

#include <QString>
#include "onyx/base/base.h"
#include "onyx/ui/ui_global.h"

namespace ui
{

static const QString ODATA_KEY_TITLE = "title";
static const QString ODATA_KEY_COVER = "cover";
static const QString ODATA_KEY_CHECKED = "checked";
static const QString ODATA_KEY_FONT = "font";

static const QString ODATA_KEY_CODE = "code";
static const QString ODATA_KEY_ICON = "icon";
static const QString ODATA_KEY_SPECIAL = "special";
// This key in OData is just for debug purpose.
static const QString ODATA_KEY_SPECIAL_TEXT = "special_text";

static const QString MENU_TYPE = "menu_type";

}   // namespace ui

#endif  // ONYX_KEYBOARD_UTILS_H_
