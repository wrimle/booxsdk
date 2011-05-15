enable_qt()

add_library(onyxapp_static STATIC application.cpp onyx_main.cpp)
add_library(onyxapp SHARED application.cpp onyx_main.cpp)

SET_TARGET_PROPERTIES(onyxapp_static PROPERTIES OUTPUT_NAME "onyxapp")

target_link_libraries(onyxapp onyx_sys onyx_screen ${QT_LIBRARIES})
strict_warning(onyxapp)

add_subdirectory(base)
add_subdirectory(cms)
add_subdirectory(data)
add_subdirectory(screen)
add_subdirectory(wpa)
add_subdirectory(sys)
add_subdirectory(ui)
add_subdirectory(wireless)
add_subdirectory(sound)
add_subdirectory(tts)
add_subdirectory(dictionary)
