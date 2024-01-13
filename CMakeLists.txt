cmake_minimum_required(VERSION 3.5)

project(open_ra2ob_qt6 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES open_ra2ob_qt6_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Ra2ob)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/QHotkey)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(open_ra2ob_qt6
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        ob.h ob.cpp ob.ui
        configmanager.h configmanager.cpp
        tray.h tray.cpp
        unitblock.h unitblock.cpp unitblock.ui
        playerinfo.h playerinfo.cpp playerinfo.ui
        layoutsetting.h
        messagebox.h
        globalsetting.h globalsetting.cpp
        producingblock.h producingblock.cpp producingblock.ui
        qoutlinelabel.h qoutlinelabel.cpp
        hotkeymanager.h hotkeymanager.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET open_ra2ob_qt6 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(open_ra2ob_qt6 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(open_ra2ob_qt6
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(open_ra2ob_qt6 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.open_ra2ob_qt6)
endif()
set_target_properties(open_ra2ob_qt6 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
#    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS"
)

include(GNUInstallDirs)
install(TARGETS open_ra2ob_qt6
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(open_ra2ob_qt6)
endif()
