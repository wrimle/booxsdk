#include "onyx/ui/keyboard_data.h"
#include "onyx/ui/onyx_keyboard.h"

namespace ui
{

KeyboardData::KeyboardData()
{
    initMenuKeyCode();
}

KeyboardData::~KeyboardData()
{
}

ODatas & KeyboardData::topCodes(bool shift)
{
    return (!shift? top_codes_: top_shift_codes_);
}

ODatas & KeyboardData::leftCodes(bool shift)
{
    return (!shift? left_codes_: left_shift_codes_);
}

ODatas & KeyboardData::middleCodes(bool shift)
{
    return (!shift? middle_codes_: middle_shift_codes_);
}

ODatas & KeyboardData::rightCodes(bool shift)
{
    return (!shift? right_codes_: right_shift_codes_);
}

ODatas & KeyboardData::bottomCodes(bool shift)
{
    return (!shift? bottom_codes_: bottom_shift_codes_);
}

ODatas & KeyboardData::menuCodes()
{
    return menu_codes_;
}

ODatas & KeyboardData::leftSymbolCodes()
{
    return left_symbol_codes_;
}

ODatas & KeyboardData::middleSymbolCodes()
{
    return middle_symbol_codes_;
}

ODatas & KeyboardData::rightSymbolCodes()
{
    return right_symbol_codes_;
}

OData * KeyboardData::createData(const QString & value)
{
    OData * data = new OData;
    data->insert(ODATA_KEY_CODE, value);
    return data;
}

OData * KeyboardData::createBackspaceData()
{
    QPixmap backspace_pixmap(":/images/back_spac.png");
    OData * dd = new OData;
    dd->insert(ODATA_KEY_ICON, backspace_pixmap);
    dd->insert(ODATA_KEY_SPECIAL, Qt::Key_Backspace);
    dd->insert(ODATA_KEY_SPECIAL_TEXT, "backspace");
    return dd;
}

void KeyboardData::initMenuKeyCode()
{
    OData *dd = createData(QApplication::tr("Shift"));
    static const int MENU_FONT_SIZE = 20;
    dd->insert(MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_SHIFT);
    dd->insert(ODATA_KEY_FONT, MENU_FONT_SIZE);
    menu_codes_.push_back(dd);
    dd = createData(QApplication::tr("Symbol"));
    dd->insert(MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_SYMBOL);
    dd->insert(ODATA_KEY_FONT, MENU_FONT_SIZE);
    menu_codes_.push_back(dd);
    dd = createData(QApplication::tr("Language"));
    dd->insert(MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_LANGUAGE);
    dd->insert(ODATA_KEY_FONT, MENU_FONT_SIZE);
    menu_codes_.push_back(dd);
    dd = createData(QApplication::tr("Write"));
    dd->insert(MENU_TYPE, OnyxKeyboard::KEYBOARD_MENU_WRITE);
    dd->insert(ODATA_KEY_FONT, MENU_FONT_SIZE);
    menu_codes_.push_back(dd);
}


}   // namespace ui
