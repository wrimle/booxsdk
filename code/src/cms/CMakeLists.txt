enable_qt()

set(SRCS
  content_category.cpp
  content_manager.cpp
  content_node.cpp
  content_thumbnail.cpp
  content_options.cpp
  content_bookmarks.cpp
  content_shortcut.cpp
  cms_version.cpp
  notes_manager.cpp
  cms_utils.cpp
  user_db.cpp
  download_db.cpp)

add_library(onyx_cms_static STATIC ${SRCS})
add_library(onyx_cms SHARED ${SRCS})

SET_TARGET_PROPERTIES(onyx_cms_static PROPERTIES OUTPUT_NAME "onyx_cms")

strict_warning(onyx_cms)
