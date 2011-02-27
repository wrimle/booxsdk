add_library(onyx_screen STATIC screen_proxy.cpp screen_update_watcher.cpp)
install(TARGETS onyx_screen DESTINATION lib)
