

IF (WIN32)
    SET(SRC wpa_ctrl.c os_win32.c)
ELSE (WIN32)
    SET(SRC wpa_ctrl.c os_unix.c)
ENDIF (WIN32)

ADD_LIBRARY(onyx_wpa_static STATIC ${SRC})
ADD_LIBRARY(onyx_wpa SHARED ${SRC})

SET_TARGET_PROPERTIES(onyx_wpa_static PROPERTIES OUTPUT_NAME "onyx_wpa")

strict_warning(onyx_wpa)
