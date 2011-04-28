
#ifndef ONYX_DATA_KEYS_H_
#define ONYX_DATA_KEYS_H_

#define MENU        0x01
#define ESC         0x02
#define PAGEUP      0x04
#define PAGEDOWN    0x08

#define ENABLE_ALL          (MENU | ESC | PAGEUP | PAGEDOWN)
#define ENABLE_MENU_ESC     (MENU | ESC)
#define ENABLE_PAGE_UP_DOWN (PAGEUP | PAGEDOWN)

#define DISABLE_MENU_ESC     ((~MENU) & (~ESC))
#define DISABLE_PAGE_UP_DOWN ((~PAGEUP) & (~PAGEDOWN))

#define DISABLE_ALL (0)



#endif

