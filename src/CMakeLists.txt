add_executable(
    ${CMAKE_PROJECT_NAME}
    main.cpp
    TreeMap.cpp
)

include(FetchContent)
set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    raylib
)

