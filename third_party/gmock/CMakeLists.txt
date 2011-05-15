include_directories(. include)

add_library(gmock_static STATIC src/gmock-all.cc)
add_library(gmock SHARED src/gmock-all.cc)

SET_TARGET_PROPERTIES(gmock_static PROPERTIES OUTPUT_NAME "gmock")

set_target_properties(gmock
  PROPERTIES
  COMPILE_FLAGS "-DPATH_MAX=4096")
