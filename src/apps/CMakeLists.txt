file(GLOB_RECURSE APPS_SRC_FILES "${PROJECT_SOURCE_DIR}/src/calculator/*.cpp")

include_directories(${COMMON_INCLUDES})

add_executable(xxcalc ${APPS_SRC_FILES} "${PROJECT_SOURCE_DIR}/src/apps/xxcalc.cpp")
add_executable(xxcalc-debug ${APPS_SRC_FILES} "${PROJECT_SOURCE_DIR}/src/apps/xxcalc.cpp")

install(TARGETS xxcalc DESTINATION bin)
install(TARGETS xxcalc-debug DESTINATION bin)

target_compile_definitions(xxcalc PUBLIC -DNODEBUG)
target_compile_definitions(xxcalc-debug PUBLIC -DDEBUG)

