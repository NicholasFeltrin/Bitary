cmake_minimum_required(VERSION 3.5)

project(Bitary VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(SQLite3 REQUIRED)

set(QT_SOURCES
        src/ui/window.cpp
        src/ui/mainTable.cpp
        src/ui/buttons.cpp
        src/ui/tests.cpp
        src/ui/window.h
        src/ui/mainTable.h
        src/ui/window.ui
)

set(TEST_SOURCES
        src/tests/tests.c
        src/tests/tests.h
)

set(LIBRARY_SOURCES
        src/library/library.c
        src/library/database.c
        src/library/library.h
        src/library/database.h
        src/library/helper.h
        src/library/defs.h
)

set(PROJECT_SOURCES
        src/main.cpp
        ${LIBRARY_SOURCES}
        ${TEST_SOURCES}
        ${QT_SOURCES}
)

# Default to Release build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    add_definitions(-DDEBUG)  # DEBUG macro
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# Release configuration
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

add_executable(Bitary ${PROJECT_SOURCES})

target_link_libraries(Bitary PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core SQLite::SQLite3)

set_target_properties(Bitary PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Bitary
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Bitary)
endif()
