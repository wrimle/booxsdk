enable_qt()

qt4_wrap_cpp(MOC_SRCS
  ${ONYXSDK_DIR}/include/onyx/data/database.h
  ${ONYXSDK_DIR}/include/onyx/data/annotation.h
  ${ONYXSDK_DIR}/include/onyx/data/annotation_agent.h
  ${ONYXSDK_DIR}/include/onyx/data/annotation_document.h
  ${ONYXSDK_DIR}/include/onyx/data/annotation_page.h
  ${ONYXSDK_DIR}/include/onyx/data/annotation_io.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_proxy.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_document.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_graphic_context.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_io.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_page.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_point.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_stroke.h
  ${ONYXSDK_DIR}/include/onyx/data/web_history.h
  ${ONYXSDK_DIR}/include/onyx/data/sketch_utils.h
  ${ONYXSDK_DIR}/include/onyx/data/handwriting_interface.h
  ${ONYXSDK_DIR}/include/onyx/data/handwriting_manager.h
  ${ONYXSDK_DIR}/include/onyx/data/handwriting_widget.h
  ${ONYXSDK_DIR}/include/onyx/data/handwriting_functions_model.h
  ${ONYXSDK_DIR}/include/onyx/data/data.h
)

SET(SRCS
  configuration.cpp
  bookmark.cpp
  reading_history.cpp
  database.cpp
  annotation.cpp
  annotation_agent.cpp
  annotation_document.cpp
  annotation_page.cpp
  annotation_io.cpp
  sketch_proxy.cpp
  sketch_document.cpp
  sketch_graphic_context.cpp
  sketch_io.cpp
  sketch_page.cpp
  sketch_point.cpp
  sketch_stroke.cpp
  web_history.cpp
  handwriting_manager.cpp
  handwriting_widget.cpp
  handwriting_functions_model.cpp
  content.cpp
  data.cpp
  search_context.cpp
  ${MOC_SRCS}
)

add_library(onyx_data_static STATIC ${SRCS})
add_library(onyx_data SHARED ${SRCS})

SET_TARGET_PROPERTIES(onyx_data_static PROPERTIES OUTPUT_NAME "onyx_data")

target_link_libraries(onyx_data onyx_cms onyx_sys)
strict_warning(onyx_data)
