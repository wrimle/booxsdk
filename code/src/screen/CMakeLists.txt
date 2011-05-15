QT4_WRAP_CPP(MOC_SRCS ${ONYXSDK_DIR}/include/onyx/screen/screen_update_watcher.h)

add_library(onyx_screen_static STATIC screen_proxy.cpp screen_update_watcher.cpp ${MOC_SRCS})
add_library(onyx_screen SHARED screen_proxy.cpp screen_update_watcher.cpp ${MOC_SRCS})

SET_TARGET_PROPERTIES(onyx_screen_static PROPERTIES OUTPUT_NAME "onyx_screen")

install(TARGETS onyx_screen DESTINATION lib)
