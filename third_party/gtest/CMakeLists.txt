include_directories(.)

add_library(gtest_static STATIC src/gtest-all.cc)
add_library(gtest SHARED src/gtest-all.cc)

SET_TARGET_PROPERTIES(gtest_static PROPERTIES OUTPUT_NAME "gtest")

set_target_properties(gtest
  PROPERTIES
  COMPILE_FLAGS "-DPATH_MAX=4096")

add_library(gtest_main_static STATIC src/gtest_main.cc)
add_library(gtest_main SHARED src/gtest_main.cc)

SET_TARGET_PROPERTIES(gtest_main_static PROPERTIES OUTPUT_NAME "gtest_main")

target_link_libraries(gtest_main gtest)
