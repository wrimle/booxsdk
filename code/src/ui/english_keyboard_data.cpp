#include "onyx/base/base.h"
#include "onyx/sys/sys.h"
#include "onyx/ui/english_keyboard_data.h"
#include "onyx/ui/onyx_keyboard_utils.h"

namespace ui
{

EnglishKeyboardData::EnglishKeyboardData()
    : KeyboardData()
{
    initTopKeyCode();
    initLeftKeyCode();
    initMiddleKeyCode();
    initRightKeyCode();
    initBottomKeyCode();

    initTopKeyShiftCode();
    initLeftKeyShiftCode();
    initMiddleKeyShiftCode();
    initRightKeyShiftCode();
    initBottomKeyShiftCode();

    initLeftKeySymbolCode();
    initMiddleKeySymbolCode();
    initRightKeySymbolCode();
}

EnglishKeyboardData::~EnglishKeyboardData()
{
}

void EnglishKeyboardData::initTopKeyCode()
{
    const QChar chs[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    for (int i = 0; i < 10; i++)
    {
        OData *dd = createData(QString(chs[i]));
        top_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initLeftKeyCode()
{
    const QChar chs[] = { 'q', 'w', 'e', 'a', 's', 'd', 'z', 'x', 'c' };
    for (int i = 0; i < 9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        left_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initMiddleKeyCode()
{
    const QChar chs[] = {'r', 't', 'y', 'f', 'g', 'h', 'v', 'b', 'n'};
    for (int i=0; i<9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        middle_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initRightKeyCode()
{
    const QChar chs[] = {'u', 'i', 'o', 'j', 'k', 'l', 'm', 'p', '.'};
    for (int i=0; i<9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        right_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initBottomKeyCode()
{
    const QChar chs[] = {'/', '+', '-', '_', ',', ' '};
    for (int i=0; i<6; i++)
    {
        OData *dd = createData(QString(chs[i]));
        bottom_codes_.push_back(dd);
    }
    OData * dd = createBackspaceData();
    bottom_codes_.push_back(dd);
    const QChar chs_next[] = {'\'', '"', ';'};
    for (int i=0; i<3; i++)
    {
        OData *dd = createData(QString(chs_next[i]));
        bottom_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initTopKeyShiftCode()
{
    const QChar chs[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')'};
    for (int i = 0; i < 10; i++)
    {
        OData *dd = createData(QString(chs[i]));
        top_shift_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initLeftKeyShiftCode()
{
    const QChar chs[] = { 'Q', 'W', 'E', 'A', 'S', 'D', 'Z', 'X', 'C' };
    for (int i = 0; i < 9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        left_shift_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initMiddleKeyShiftCode()
{
    const QChar chs[] = {'R', 'T', 'Y', 'F', 'G', 'H', 'V', 'B', 'N'};
    for (int i=0; i<9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        middle_shift_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initRightKeyShiftCode()
{
    const QChar chs[] = { 'U', 'I', 'O', 'J', 'K', 'L', 'M', 'P', ',' };
    for (int i=0; i<9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        right_shift_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initBottomKeyShiftCode()
{
    const QChar chs[] = {'{', '}', '[', ']', '=', ' '};
    for (int i=0; i<6; i++)
    {
        OData *dd = createData(QString(chs[i]));
        bottom_shift_codes_.push_back(dd);
    }
    OData * dd = createBackspaceData();
    bottom_shift_codes_.push_back(dd);
    const QChar chs_next[] = {'?', ':', '\\'};
    for (int i=0; i<3; i++)
    {
        OData *dd = createData(QString(chs_next[i]));
        bottom_shift_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initLeftKeySymbolCode()
{
    const QChar chs[] = {' ', ' ', ' ',
                         ' ', ' ', ' ',
                         ' ', ' ', ' ', };
    for (int i = 0; i < 9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        left_symbol_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initMiddleKeySymbolCode()
{
    const QChar chs[] = {'~', '<', '>',
                         '|', '`', QChar(0x00A3),
                         QChar(0x00A5), QChar(0x00A7), QChar(0x00A9), };
    for (int i = 0; i < 9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        middle_symbol_codes_.push_back(dd);
    }
}

void EnglishKeyboardData::initRightKeySymbolCode()
{
    const QChar chs[] = {QChar(0x00B1), QChar(0x00B5), ' ',
                         ' ', ' ', ' ',
                         ' ', ' ', ' ', };
    for (int i = 0; i < 9; i++)
    {
        OData *dd = createData(QString(chs[i]));
        right_symbol_codes_.push_back(dd);
    }
}

}
