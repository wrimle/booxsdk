enable_qt()

qt4_wrap_cpp(MOC_SRCS
  ${ONYXSDK_DIR}/include/onyx/base/base_model.h
  ${ONYXSDK_DIR}/include/onyx/base/tasks_handler.h
)

add_library(onyx_base_static STATIC ${MOC_SRCS} base_model.cpp tasks_handler.cpp)
add_library(onyx_base SHARED ${MOC_SRCS} base_model.cpp tasks_handler.cpp)

SET_TARGET_PROPERTIES(onyx_base_static PROPERTIES OUTPUT_NAME "onyx_base")

target_link_libraries(onyx_base
  onyx_data # This is added for base_model. Perhaps base_model should be in onyx_data
)
strict_warning(onyx_base)
