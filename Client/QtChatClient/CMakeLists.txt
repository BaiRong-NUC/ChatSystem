cmake_minimum_required(VERSION 3.19)
project(QtChatClient LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE QTCHATCLIENT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE QTCHATCLIENT_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

# Support manual CMake runs against Qt installs from the Qt Online Installer.
if(NOT Qt6_DIR AND NOT CMAKE_PREFIX_PATH)
    file(GLOB QT_LOCAL_PREFIXES LIST_DIRECTORIES true
        "$ENV{HOME}/Qt/*/gcc_64"
        "$ENV{HOME}/Qt6/*/gcc_64"
    )
    list(SORT QT_LOCAL_PREFIXES ORDER DESCENDING)

    foreach(QT_LOCAL_PREFIX IN LISTS QT_LOCAL_PREFIXES)
        if(EXISTS "${QT_LOCAL_PREFIX}/lib/cmake/Qt6/Qt6Config.cmake")
            list(PREPEND CMAKE_PREFIX_PATH "${QT_LOCAL_PREFIX}")
            break()
        endif()
    endforeach()
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(QtChatClient
    WIN32 MACOSX_BUNDLE
    ${QTCHATCLIENT_SOURCES}
    ${QTCHATCLIENT_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/resource.qrc
)

target_include_directories(QtChatClient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(QtChatClient
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS QtChatClient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QtChatClient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
