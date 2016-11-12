file(GLOB_RECURSE APPS_SRC_FILES "${PROJECT_SOURCE_DIR}/src/calculator/*.cpp")
file(GLOB_RECURSE TEST_SRC_FILES "${PROJECT_SOURCE_DIR}/test/calculator/*.cpp")

include_directories(${COMMON_INCLUDES} ${CATCH_INCLUDE_DIR})

add_executable(xxcalc ${APPS_SRC_FILES} "${PROJECT_SOURCE_DIR}/src/apps/xxcalc.cpp")
add_executable(xxcalc-debug ${APPS_SRC_FILES} "${PROJECT_SOURCE_DIR}/src/apps/xxcalc.cpp")
add_executable(xxcalc-test ${APPS_SRC_FILES} ${TEST_SRC_FILES} "${PROJECT_SOURCE_DIR}/src/apps/test.cpp")

install(TARGETS xxcalc DESTINATION bin)
install(TARGETS xxcalc-debug DESTINATION bin)
install(TARGETS xxcalc-test DESTINATION bin)

target_compile_definitions(xxcalc PUBLIC -DNODEBUG)
target_compile_definitions(xxcalc-debug PUBLIC -DDEBUG)
target_compile_definitions(xxcalc-test PUBLIC -DDEBUG)

